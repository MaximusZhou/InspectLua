/*
	
	Example:
	$gcc -Wall test_general_call_lua_in_c.c util.c -llua -lm -ldl -o test_general_call_lua_in_c 
	$./test_general_call_lua_in_c 
	result=0.239713

	Analyse:

*/
#include <stdio.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "util.h"

int main(void)
{
	char *fname = "TestConfig.lua";
	lua_State *L = luaL_newstate();
	load(L,fname); /*load confile file*/

	luaL_openlibs(L);

	double x = 0.5, y = 0.5;
	double result;
	call_va(L,"f","dd>d",x,y,&result);
	printf("result=%g\n",result); /*result=0.239713*/

	return 0;
}
