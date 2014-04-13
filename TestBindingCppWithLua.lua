
local foo = Foo()

foo:foo() -- in foo

foo:set_member(8)
local member = foo:get_member()

print("object member value:",member) --8

--property test
print("object member value:",foo.member) --8

foo.member = 9
print("object member value:",foo.member) --9

foo1 = Foo()

print(tostring(foo1)) --Foo (0x97d9e0)

print(foo == foo)  --true
print(foo1 == foo) --false
