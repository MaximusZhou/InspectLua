/*
	define c++ class for lua
*/

#ifndef TEST_BINDING_CPP_WITH_LUA
#define TEST_BINDING_CPP_WITH_LUA

#include <stdlib.h>
#include "binding_cpp_with_lunafive.hpp"

class Foo {
	public:
		Foo(lua_State*L) {
			printf("in constructor\n");	
		}

		int foo(lua_State *L) {
			printf("in foo\n");
			return 0;
		}

		int set_member(lua_State *L){
			int value = luaL_checkint(L,2);
			member = value;
			return 0;
		}

		int get_member(lua_State *L) {
			lua_pushnumber(L,member);
			return 1;
		}

		
		int getmember(lua_State *L) {
			lua_pushnumber(L,member);
			return 1;
		}

		int setmember(lua_State *L){
			int value = luaL_checkint(L,1);
			member = value;
			return 0;
		}

		
		~Foo() {
			printf("in destructor\n");
		}

		static const char className[];
		static const Luna<Foo>::FunctionType methods[];
		static const Luna<Foo>::PropertyType properties[];

	private:
		int member;
};

const char Foo::className[] = "Foo";

#define method(class, funcname) {#funcname, &class::funcname}
const Luna<Foo>::FunctionType Foo::methods[] = {
	method(Foo,foo),
	method(Foo,set_member),
	method(Foo,get_member),
	{0,0},
};

#define property(class,propertyname,getfuncname,setfuncname) \
					{#propertyname,&class::getfuncname,&class::setfuncname}

const Luna<Foo>::PropertyType Foo::properties[] = {
	property(Foo,member,getmember,setmember),
	{0,0,0},
};

#endif
