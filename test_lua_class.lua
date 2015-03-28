require "lua_class"

clsParent = clsObject:Inherit()

function clsParent:Foo()
	print("ParentFoo!")
end

local ParentObj = clsParent:New()
ParentObj:Foo()


clsSon = clsParent:Inherit()
function clsSon:Foo()
	Super(clsSon).Foo(self)
	print("SonFoo")
end

local SonObj = clsSon:Inherit()
SonObj:Foo()

print(IsSub(clsSon, clsParent))
print(IsSub(clsSon, clsObject))
