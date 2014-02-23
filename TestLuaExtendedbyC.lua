--extend lua by c function
local angle = 30

print("libsin:",math.sin(angle))
print("mysin:",mysin(angle))

--extend lua by c module
--require "mylib"
print("mysininmodule:",mytestlib.sin(angle))

local s = {1,2,3}
function f(i)
	return 2*i
end

mytestlib.map(s,f)
for _,v in ipairs(s) do
	print("i:",v)
end

x = mytestlib.tuplenew(10,"hi",{},3)
print(x(1))
print(x(2))
print(x(3))
print(x())
