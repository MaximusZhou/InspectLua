#ifndef util_h
#define util_h

void error(lua_State *L, const char *fmt, ...);

void stack_dump(lua_State *L);

void load(lua_State *L, const char *file_name);

/*a general interface can be userd to call lua function in c*/
void call_va(lua_State *L,const char *funcname,const char *sig, ...);

#endif
