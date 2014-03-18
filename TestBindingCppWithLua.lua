
local foo = Foo()

foo:foo()

foo:set_member(8)
local member = foo:get_member()

print("object member value:",member) --8
