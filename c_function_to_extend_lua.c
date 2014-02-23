/*
	define some c function to extend lua script
*/

#include <math.h>

#include "lua.h"
#include "lauxlib.h"
#include "c_function_to_extend_lua.h"

int l_sin(lua_State *L)
{
	double d = luaL_checknumber(L,1);
	lua_pushnumber(L,sin(d));

	return 1;
}

int l_dir(lua_State *L)
{
	return 1;
}
