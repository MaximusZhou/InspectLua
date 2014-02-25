/*
	Example:

	Analyse:
*/

#include <dirent.h>
#include <errno.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define DIRUDATA_MTABLE_NAME "MYDIR"

static int dir_iter(lua_State*L)
{
	DIR *d = *(DIR**)lua_touserdata(L,lua_upvalueindex(1));
	struct dirent *entry;

	if((entry = readdir(d)) != NULL)
	{
		lua_pushstring(L,entry->d_name);
		return 1;
	}

	return 0;
}

static int l_dir(lua_State *L)
{
	const char *path = luaL_checkstring(L,1);

	size_t nbytes = sizeof(DIR*);
	DIR **d = (DIR**)lua_newuserdata(L,nbytes);

	luaL_getmetatable(L,DIRUDATA_MTABLE_NAME);
	lua_setmetatable(L,-2);

	*d = opendir(path);
	if (*d == NULL)
		luaL_error(L,"cannot open %s,%s",path,strerror(errno));

	lua_pushcclosure(L,dir_iter,1);

	return 1;
}

static int dir_gc(lua_State *L)
{
	DIR *d = *(DIR**)lua_touserdata(L,lua_upvalueindex(1));
	if (d)
		closedir(d);
	
	return 0;
}

int luaopen_dir(lua_State *L)
{
	luaL_newmetatable(L,DIRUDATA_MTABLE_NAME);

	/*set __gc value*/
	lua_pushstring(L,"__gc");
	lua_pushcfunction(L,dir_gc);
	lua_settable(L,-3);

	/*register c function*/
	lua_pushcfunction(L,l_dir);
	lua_setglobal(L,"dir");

	return 0;
}
