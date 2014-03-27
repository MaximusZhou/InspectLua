
local foo = Foo()

foo:foo()

local temp = foo.foo
foo.foo = function(self)
	temp(self)
	print("haha,extend")
end

--[[output:
in foo
haha,extend
--]]
foo:foo() 

foo:set_member(8)
local member = foo:get_member()

print("object member value:",member) --8
