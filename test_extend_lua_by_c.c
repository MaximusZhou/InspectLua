/*
	
	Example:
	$gcc -Wall test_extend_lua_by_c.c util.c  c_function_to_extend_lua.c c_module_to_extend_lua.c c_userdata_to_extend_lua.c -llua -lm -ldl -o  test_extend_lua_by_c
	$./test_extend_lua_by_c
	--extend lua by c function--
	libsin: -0.98803162409286
	mysin:  -0.98803162409286
	--extend lua by c module--
	mysininmodule:  -0.98803162409286
	i:      2
	i:      4
	i:      6
	--test upvalue in c function--
	10
	hi
	table: 0x1c153f0
	10      hi      table: 0x1c153f0        3
	--test userdata--
	userdata: 0x1c15af8
	1000
	true
	false

	Analyse:

*/
#include <stdio.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "util.h"
#include "c_function_to_extend_lua.h"

int luaopen_mylib(lua_State *L);
int luaopen_arraylib(lua_State *L);

void open_c_func_to_extend_lua(lua_State *L)
{
	lua_pushcfunction(L,l_sin);
	lua_setglobal(L,"mysin");

	lua_pushcfunction(L,l_sin);

	/*open c module*/
	luaopen_mylib(L);
	luaopen_arraylib(L);
}

int main(void)
{
	char *fname = "TestLuaExtendedbyC.lua";
	lua_State *L = luaL_newstate();

	luaL_openlibs(L);
	open_c_func_to_extend_lua(L);

	load(L,fname); /*load confile file*/

	return 0;
}
