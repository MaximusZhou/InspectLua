_G.LoadModule = _G.LoadModule or {}
local LoadModule = _G.LoadModule

local function ReplaceTbl(Dest, Src)
	local function RealFun(Dest, Src, Depth)
		assert(type(Dest)=="table" and type(Src)=="table",
			"error data type")

		if not Depth then
			Depth = 0
		end
		if Depth>=20 then
			error("too long Depth to replace")
			return
		end

		for k,v in pairs(Dest) do
			if type(v) == "table" then
				if type(Src[k]) == "table" and
					k ~="__index" then
					RealFun(v, Src[k], Depth+1)
				else
					Dest[k] = Src[k]
				end

			else
				Dest[k] = Src[k]
			end
		end

		--add new data or function
		for k,v in pairs(Src) do
			if rawget(Dest, k) == nil then
				Dest[k] = v
			end
		end
		
		
		setmetatable(Dest, getmetatable(Src))
	end
	RealFun(Dest, Src)
end

local function Update(PathFile, Reload)

	local Old = LoadModule[PathFile]
	if Old and not Reload then
		return Old
	end

	local func, err = loadfile(PathFile)
	if not func then
		return func, err
	end

	--first update
	if not Old then
		LoadModule[PathFile] = {}
		local New = LoadModule[PathFile]
		setmetatable(New, {__index = _G})
		setfenv(func, New)()
		return New
	end

	local OldCache = {}
	for k,v in pairs(Old) do
		OldCache[k] = v
		Old[k] = nil
	end

	--change in modue inplace!!
	setfenv(func, Old)()

	for k,v in pairs(OldCache) do
		local TmpNewData = Old[k]
		
		Old[k] = v  --default value is old value
		if TmpNewData then
			if type(v) == "table" then
				if type(TmpNewData) == "table" then
					if rawget(v,"__ClassType") then 	--if table is classtype, Update!
						local mt = getmetatable(v)
						local old_mt = v.mt
						local index = old_mt.__index
						ReplaceTbl(v, TmpNewData)   --Notice: change directly table v to ensure reference v  rightly for others var
						v.mt = old_mt
						old_mt.__index = index
					end
					local mt = getmetatable(TmpNewData)
					if mt then setmetatable(v, mt) end
				end
			--update function
			elseif type(v) == "function" then
				Old[k] = TmpNewData
			end
		end
	end

	return Old
end
