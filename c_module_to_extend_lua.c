/*
	Example:
	gcc -Wall -fpic -shared -llua -lm -ldl c_module_to_extend_lua.c -o mylib.so

	Analyse:
	Use luaL_register to register c module  is old way,new way is use the function luaL_newlib,And we can refer to loslib.c in lua Source Code¡£

*/
#include <math.h>

#include "lua.h"
#include "lauxlib.h"

static int l_sin(lua_State *L)
{
	double d = luaL_checknumber(L,1);
	lua_pushnumber(L,sin(d));

	return 1;
}

static int l_map(lua_State *L)
{
	int i,n;
	
	/*chceck parameter type*/
	luaL_checktype(L,1,LUA_TTABLE);
	luaL_checktype(L,2,LUA_TFUNCTION);

	/*get array size*/
	n = lua_objlen(L,1);

	for(i = 1; i <= n; i ++)
	{
		lua_pushvalue(L,2); /*push function*/
		lua_rawgeti(L,1,i);  /*push t[i]*/
		lua_call(L,1,1);   /*call f(t[i]), Notice use lua_call,Not lua_pcall*/
		lua_rawseti(L,1,i);
	}

	return 0; /*no return value be pushed in stack*/
}


static int t_tuple(lua_State *L)
{
	int default_value = 0;
	int op = luaL_optint(L,1,default_value);
	if(op == 0) /*no parameters*/
	{
		int i;
		for(i = 1; !lua_isnone(L,lua_upvalueindex(i)); i++)
			lua_pushvalue(L,lua_upvalueindex(i));
		return (i - 1);
	}
	else
	{
		luaL_argcheck(L,0 < op,1,"index out of range");
		if(lua_isnone(L,lua_upvalueindex(op)))
			return 0;

		lua_pushvalue(L,lua_upvalueindex(op));
		return 1;
	}
}

static int t_new(lua_State *L)
{
	lua_pushcclosure(L,t_tuple,lua_gettop(L));
	return 1;
}

static const struct luaL_Reg mytestlib[] = {
	{"sin",l_sin},
	{"map",l_map},
	{"tuplenew",t_new},
	{NULL,NULL},
};

int luaopen_mylib(lua_State *L)
{
	/*#define luaL_register(L,n,l)    (luaL_openlib(L,(n),(l),0)) in lauxlib.h */
	luaL_register(L,"mytestlib",mytestlib);
	return 1;
}
