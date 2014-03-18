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
		}
		
		~Foo() {
			printf("in destructor\n");
		}

		static const char className[];
		static const Luna<Foo>::RegTpe Register[];
};

const char Foo::className[] = "Foo";
const Luna<Foo>::RegTpe Foo::Register[] = {
	{"foo",&Foo::foo},
	{0}
};

#endif
