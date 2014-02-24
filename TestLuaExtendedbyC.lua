--
print("--extend lua by c function--")
--
local angle = 30
print("libsin:",math.sin(angle))
print("mysin:",mysin(angle))

--
print("--extend lua by c module--")
--
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

--
print("--test upvalue in c function--")
--
x = mytestlib.tuplenew(10,"hi",{},3)
print(x(1))
print(x(2))
print(x(3))
print(x())

--
print("--test userdata--")
--
local SIZE = 1000
a = array.new(SIZE)
print(a)
print(array.size(a))
for i = 1,SIZE do
	array.set(a,i,i%5==0)
end

print(array.get(a,10))
print(array.get(a,9))
--print(array.get(io.stdin,10)) --cannot run config. file:TestLuaExtendedbyC.lua:46: bad argument #1 to 'get' (MyArray expected, got userdata)
