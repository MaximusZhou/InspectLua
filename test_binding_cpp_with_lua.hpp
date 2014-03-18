/*
	define c++ class for lua
*/

#ifndef TEST_BINDING_CPP_WITH_LUA
#define TEST_BINDING_CPP_WITH_LUA

#include <stdlib.h>
#include "binding_cpp_with_lua.hpp"

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
		
		~Foo() {
			printf("in destructor\n");
		}

		static const char className[];
		static const Luna<Foo>::RegTpe Register[];

	private:
		int member;
};

const char Foo::className[] = "Foo";
const Luna<Foo>::RegTpe Foo::Register[] = {
	{"foo",&Foo::foo},
	{"set_member",&Foo::set_member},
	{"get_member",&Foo::get_member},
	{0}
};

#endif
