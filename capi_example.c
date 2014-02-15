/*
	
	Example:
	$gcc -Wall capi_example.c -llua -lm -ldl -o capi_example
	$./capi_example 
	true 10 nil 'hello' 
	true 10 nil 'hello' true 
	true 10 true 'hello' 
	true 10 true 'hello' nil nil 
	true 10 true nil nil 
	true 

	Analyse:

*/
#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"

static void stack_dump(lua_State *L)
{
	int i;
	int top = lua_gettop(L);
	for(i = 1; i <= top; i ++)
	{
		int t = lua_type(L,i);
		switch(t){
			case LUA_TSTRING:
			{
				printf("'%s'",lua_tostring(L,i));
				break;
			}

			case LUA_TBOOLEAN:
			{
				printf(lua_toboolean(L,i) ? "true":"false");
				break;
			}

			case LUA_TNUMBER:
			{
				printf("%g",lua_tonumber(L,i));
				break;
			}

			default:
			{
				printf("%s",lua_typename(L,t));
				break;
			}
		}
		printf(" ");
	}
	printf("\n");
}

int main(void)
{
	lua_State *L = luaL_newstate();

	lua_pushboolean(L,1);
	lua_pushnumber(L,10);
	lua_pushnil(L);
	lua_pushstring(L,"hello");
	stack_dump(L);

	lua_pushvalue(L,-4);
	stack_dump(L);

	lua_replace(L,3);
	stack_dump(L);

	lua_settop(L,6);
	stack_dump(L);

	lua_remove(L,-3);
	stack_dump(L);

	lua_settop(L,-5);
	stack_dump(L);

	lua_close(L);

	return 0;

}
