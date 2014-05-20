-------------------------------------------------
--Generate all permutation by coroutine
-------------------------------------------------

function print_array(t)
	for _,value in ipairs( t or {}) do
		io.write(value," ")
	end
	io.write("\n")
end

function genpermu(t,n)
	n = n or #t
	if n <= 1 then
		coroutine.yield(t)
	else
		for i =n,1,-1 do
			t[i],t[n] = t[n],t[i]
			genpermu(t,n-1)
			t[i],t[n] = t[n],t[i]
		end
	end
end

function permutations(a)
	local co = coroutine.create(function() genpermu(a) end)
	return function()
		local code,res = coroutine.resume(co)
		return res
	end
end

for p in permutations({"a","b","c"}) do
	print_array(p)
end
