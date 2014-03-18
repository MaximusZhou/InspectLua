/*
	test a c++ template to bind cpp with lua
	
	Example:
	$g++ binding_cpp_with_lua.cpp -llua -lm -ldl -o binding_cpp_with_lua
	$./binding_cpp_with_lua
	in constructor
	in foo
	object member value:    8
	in destructor

	Analyse:

*/


#include "test_binding_cpp_with_lua.hpp"

void load(lua_State *L, const char *file_name)
{
	if ((luaL_loadfile(L,file_name)) || (lua_pcall(L,0,0,0)))
		printf("cannot run config file:%s\n",lua_tostring(L,-1));
}

int main(void)
{
	const char *fname = "TestBindingCppWithLua.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	Luna<Foo>::Register(L);

	load(L,fname); /*load confile file*/

	lua_close(L);
	return 0;
}
