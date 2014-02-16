/*
	
	Example:
	$gcc -Wall extend_c_app_by_lua.c -llua -lm -ldl -o extend_c_app_by_lua
	$./extend_c_app_by_lua 
	stack size = 0
	w=10,h=20
	r=76,g=25,b=0
	result=0.239713

	Analyse:

*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define MAX_COLOR  255

static char *fname = "TestConfig.lua";

struct ColorTable{
	char *name;
	unsigned char red,green,blue;
}colortable[] = {
	{"WHITE",MAX_COLOR,MAX_COLOR,MAX_COLOR},
	{"RED",MAX_COLOR,0,0},
	{"GREEN",0,MAX_COLOR,0},
	{"BLUE",0,0,MAX_COLOR},
	{NULL,0,0,0},
};


void error(lua_State *L, const char *fmt, ...)
{
	va_list argp;
	va_start(argp,fmt);

	vfprintf(stderr,fmt,argp);

	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

void load(lua_State *L, const char *file_name)
{
	if ((luaL_loadfile(L,file_name)) || (lua_pcall(L,0,0,0)))
		error(L,"cannot run config. file:%s\n",lua_tostring(L,-1));
}

void get_globar_var(lua_State *L, int *w,int *h)
{
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

/*use global var in c*/
void test_global_var(lua_State *L)
{
	int w = 0,h = 0;

	get_globar_var(L,&w,&h);

	printf("w=%d,h=%d\n",w,h); /*w=10,h=20*/

}

/*get field from table in stack top*/
static int getfield(lua_State *L, const char *key)
{
	int result;

	if(!lua_istable(L,-1))
		error(L,"the element in stack top is not a table in get key %s\n",key);
	
	/*next two lines can be replaced by lua_getfield(L,-1,key)*/
	lua_pushstring(L,key);
	lua_gettable(L,-2);

	if(!lua_isnumber(L,-1))
		error(L,"invalid component in background color");

	result = (int)(lua_tonumber(L,-1) * MAX_COLOR);
	lua_pop(L,1);  /* lua_pop(L,n) == lua_settop(L, -(n)-1)*/

	return result;
}

/*use table in c*/
void test_table(lua_State *L)
{
	int red,green,blue;
	lua_getglobal(L,"background");
	if (lua_isstring(L,-1))
	{
		const char *colorname = lua_tostring(L,-1);
		int i ;
		for(i = 0; colortable[i].name != NULL; i++)
			if (strcmp(colortable[i].name,colorname) == 0)
				break;

			if (colortable[i].name == NULL)
				error(L,"invaild color name (%s)",colorname);
			else
			{
				red = colortable[i].red;
				green = colortable[i].green;
				blue = colortable[i].blue;
			}
	}
	else if(lua_istable(L,-1))
	{
		red = getfield(L,"r");
		green = getfield(L,"g");
		blue = getfield(L,"b");
	}
	else
		error(L,"invaild value for 'background'");

	printf("r=%d,g=%d,b=%d\n",red,green,blue);
}


/*use lua function in c*/
void test_lua_function(lua_State *L)
{
	double x = 0.5, y = 0.5;
	double result;

	lua_getglobal(L,"f");  /*push f function in stack*/

	lua_pushnumber(L,x);  /*push two parameter */
	lua_pushnumber(L,y);

	if(lua_pcall(L,2,1,0) != 0)
		error(L,"error running function 'f':%s\n",lua_tostring(L,-1));

	if (!lua_isnumber(L,-1))
		error(L,"function 'f' must be return number\n");
	
	result = lua_tonumber(L,-1);
	printf("result=%g\n",result); /*result=0.239713*/
}
int main(void)
{
	lua_State *L = luaL_newstate();
	load(L,fname); /*load confile file*/
	printf("stack size = %d\n",lua_gettop(L)); /*stack size = 0*/

	lua_settop(L,0); /*clear stack*/
	test_global_var(L);  /*test TestConfig.lua*/

	lua_settop(L,0); /*clear stack*/
	test_table(L);

	lua_settop(L,0); /*clear stack*/
	luaL_openlibs(L);
	test_lua_function(L);

	return 0;
}
