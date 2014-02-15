/*
	
	Example:
	$gcc -Wall extend_c_app_by_lua.c -llua -lm -ldl -o extend_c_app_by_lua
	$./extend_c_app_by_lua 
	w=10,h=20

	Analyse:

*/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"


void error(lua_State *L, const char *fmt, ...)
{
	va_list argp;
	va_start(argp,fmt);

	vfprintf(stderr,fmt,argp);

	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

void load(lua_State *L, const char *fname,int *w,int *h)
{
	if ((luaL_loadfile(L,fname)) || (lua_pcall(L,0,0,0)))
		error(L,"cannot run config. file:%s\n",lua_tostring(L,-1));

	/*push global var in stack*/
	lua_getglobal(L,"width");
	lua_getglobal(L,"height");

	if (!lua_isnumber(L,-2))
		error(L,"width should be a number\n");

	if (!lua_isnumber(L,-1))
		error(L,"height should be a number\n");

	*w = lua_tonumber(L,-2);
	*h = lua_tonumber(L,-1);
}

int main(void)
{
	lua_State *L = luaL_newstate();
	int w = 0,h = 0;
	char *fname = "TestConfig.lua";

	load(L,fname,&w,&h);

	printf("w=%d,h=%d\n",w,h);

	return 0;
}
