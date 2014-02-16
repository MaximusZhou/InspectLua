/*
	
	Example:
	$gcc -Wall simple_lua_interpreter.c -llua -lm -ldl -o simple_lua_interpreter

	Analyse:
	http://stackoverflow.com/questions/4743233/is-usr-local-lib-searched-for-shared-libraries: About /usr/local/lib
	http://fangmenghu.blog.163.com/blog/static/124582020104171451565/: About /lib,/usr/lib and /usr/local/lib
	https://erex.sinaapp.com/?p=126: About compile search path
*/

#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h" //for luaL_openlibs

int main(void)
{
	char buf[256];
	int error;
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	while(fgets(buf,sizeof(buf),stdin) != NULL)
	{
		error = luaL_loadbuffer(L,buf,strlen(buf),"line") || lua_pcall(L,0,0,0);
		
		if(error)
		{
			fprintf(stderr,"%s",lua_tostring(L,-1));
			lua_pop(L,1);
		}
	}
	lua_close(L);

	return 0;
}
