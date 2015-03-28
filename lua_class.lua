
--Get Parent Class 
function Super(Class)
	return getmetatable(Class).__index
end

--if clsSub is clsAncestor subcalss
function IsSub(clsSub, clsAncestor)
	local Temp = clsSub
	while  1 do
		local mt = getmetatable(Temp)
		if mt then
			Temp = mt.__index
			if Temp == clsAncestor then
				return true
			end
		else
			return false
		end
	end
end

clsObject = {
	__ClassType = "class type" --identifying table is class, Not object
	}
	
function clsObject:Inherit(o)	
	o = o or {}
	o.__ClassType = "class type"
	o.mt = { __index = o}
	setmetatable(o, {__index = self})
	return o
end


function clsObject:New(...)
	local o = {}

	setmetatable(o, self.mt)

	if o.__init__ then
		o:__init__(...)
	end
	return o
end

function clsObject:__init__()
end

function clsObject:Destroy()
end

function clsObject:GetType()
	return "BaseClass"
end
