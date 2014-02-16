/*
	some useful interface
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "util.h"

void error(lua_State *L, const char *fmt, ...)
{
	va_list argp;
	va_start(argp,fmt);

	vfprintf(stderr,fmt,argp);

	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

void stack_dump(lua_State *L)
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

void load(lua_State *L, const char *file_name)
{
	if ((luaL_loadfile(L,file_name)) || (lua_pcall(L,0,0,0)))
		error(L,"cannot run config. file:%s\n",lua_tostring(L,-1));
}

void call_va(lua_State *L,const char *funcname,const char *sig, ...)
{
	int narg,nres; /*arg number and result number*/
	bool isbreak;

	va_list vl;
	va_start(vl,sig);

	/*push function*/
	lua_getglobal(L,funcname);

	/*push parameteres*/

	isbreak = false;
	for(narg = 0; *sig ; narg ++)
	{
		/*check stack is full*/
		lua_checkstack(L,1);

		switch(*sig)
		{
			case 'd': /*double type parametere*/
			{
				lua_pushnumber(L,va_arg(vl,double));
				break;
			}

			case 'i': /*int type parametere*/
			{
				lua_pushinteger(L,va_arg(vl,int));
				break;
			}
			case 's': /*string type parametere*/
			{
				lua_pushstring(L,va_arg(vl,char*));
				break;
			}

			case '>': /* parametere end*/
				isbreak = true;
				break;
			default:
				error(L,"invalid option (%c)",*sig);
		}

		sig++;
		if(isbreak)
			break;
	}

	nres = strlen(sig);  /*result number*/

	if(lua_pcall(L,narg,nres,0) != 0)
		error(L,"error calling '%s':%s\n",funcname,lua_tostring(L,-1));

	/*get all result*/
	nres = -nres;
	while(*sig)
	{
		switch(*sig)
		{
			case 'd':
			{
				if(!lua_isnumber(L,nres))
					error(L,"wrong result type,%c",*sig);
				*va_arg(vl,double *) = lua_tonumber(L,nres);
				break;
			}

			case 'i':
			{
				if(!lua_isnumber(L,nres))
					error(L,"wrong result type,%c",*sig);
				*va_arg(vl,int *) = lua_tonumber(L,nres);
				break;
			}

			case 's':
			{
				if(!lua_isstring(L,nres))
					error(L,"wrong result type");
				*va_arg(vl,const char **) = lua_tostring(L,nres);
				break;
			}

			default:
				error(L,"invalid option (%c)",*sig);
		}
		sig++;
		nres++;
	}
	
	va_end(vl);
}
