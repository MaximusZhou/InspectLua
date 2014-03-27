/*
	study luabridge to bind cpp
	
	
	Example:
	$g++ study_lua_bridge_main.cpp -llua -lm -ldl -o study_lua_bridge_main

	Analyse:
*/

#include <vector>
#include <string>

extern "C" {
		#include "lua.h"
		#include "lualib.h"
		#include "lauxlib.h"
}

//#include "../LuaBridge/Source/LuaBridge/LuaBridge.h"
#include "./LuaBridge/LuaBridge.h"

using namespace luabridge;

void load(lua_State *L, const char *file_name)
{
	if ((luaL_loadfile(L,file_name)) || (lua_pcall(L,0,0,0)))
		printf("cannot run config file:%s\n",lua_tostring(L,-1));
}

int globalVar;
static float staticVar;

/*
std::string stringProperty;
std::string getString () { return stringProperty; }
void setString (std::string s) { stringProperty = s; }
*/

int foo () { return 42; }
void bar (char const* var) {printf("in function bar,var=%s\n",var); }
int cFunc (lua_State* L) { return 0; }


class A {
	public:
		A() { printf("A constructor\n");}
		~A() { printf("A destructor\n");}

		static int staticData;
		static int getStaticData() {return staticData;}

		static float staticProperty;
		static float getStaticProperty () { return staticProperty; }
		static void setStaticProperty (float f) { staticProperty = f; }


		static int staticCFunc (lua_State *L) { return 0; }

		std::string dataMember;

		int dataMember1;

		char dataProperty;
		char getProperty () const { return dataProperty; }
		void setProperty (char v) { dataProperty = v; }

		void func1 () {printf("func1 In Class A\n"); }
		virtual void virtualFunc () {printf("virtualFunc In Class A\n");  }

		int cfunc (const char *var1,int var2,lua_State* L) 
		{ printf("cfunc In Class A:%s,%d,%d\n",var1,var2,lua_gettop(L));  return 0; }
};

class B : public A {
	public:
		B(const char *s,int t) { printf("B constructor:%s,%d\n",s,t);}
		~B() { printf("B destructor\n");}

		double dataMember2;

		void func1 () {printf("func1 In Class B\n"); }
		void func2 () { printf("func2 In Class B\n"); }
		void virtualFunc () {printf("virtualFunc In Class B\n");  }
};

int A::staticData = 3;
float A::staticProperty = 0.5;


void register_by_luabridge(lua_State*L)
{
	/*test Namespaces
	luabridge::getGlobalNamespace (L)
	  .beginNamespace ("test")
	      .beginNamespace ("detail")
		  .endNamespace ()
		  .beginNamespace ("utility")
		  .endNamespace ()
	  .endNamespace ();
	 */
	/*test Data, Properties, Functions, and CFunctions
	luabridge::getGlobalNamespace (L)
		.beginNamespace ("test")
			.addVariable ("var1", &globalVar)
			.addVariable ("var2", &staticVar, false)     // read-only
			//.addProperty ("prop1", getString, setString)
			//.addProperty ("prop2", getString)            // read-only
			.addFunction ("foo", foo)
			.addFunction ("bar", bar)
			.addCFunction ("cfunc", cFunc)
		.endNamespace ();
	*/
	
	getGlobalNamespace (L)
		.beginNamespace ("test")
			.beginClass<A>("A")
            	.addConstructor <void (*) (void)> ()
				.addStaticData ("staticData", &A::staticData)
				.addStaticProperty ("staticProperty", &A::getStaticData)
				.addStaticFunction ("getStaticProperty", &A::getStaticProperty) //read-only
				.addStaticCFunction ("staticCFunc", &A::staticCFunc)
				.addData ("data", &A::dataMember)
				.addData ("data1", &A::dataMember1)
				.addProperty ("prop", &A::getProperty, &A::setProperty)
				.addFunction ("func1", &A::func1)
				.addFunction ("virtualFunc", &A::virtualFunc)
				.addFunction ("cfunc", &A::cfunc)
			.endClass ()
			.deriveClass<B, A>("B")
            	.addConstructor <void (*) (const char*, int)> ()
				.addData ("data", &B::dataMember2)
				.addFunction ("func1", &B::func1)
				.addFunction ("func2", &B::func2)
			.endClass ()
		.endNamespace ();


	//C++ lifetime
	A a;                       
	a.dataMember1 = 42;

	push (L, &a);             // pointer to 'a', C++ lifetime
	lua_setglobal (L, "a");

	push (L, (A const*)&a);   // pointer to 'a const', C++ lifetime
	lua_setglobal (L, "ac");

	push <A const*> (L, &a);  // equivalent to push (L, (A const*)&a)
	lua_setglobal (L, "ac2");

	//push (L, new A);          // compiles, but will leak memory, aslo C++ lifttiime
	//lua_setglobal (L, "ap");

	B b("helloB",42);

	push(L,b);                // Copy of b passed, Lua lifetime
	lua_setglobal(L,"b");

	const char *fname = "study_lua_bridge.lua";
	load(L,fname); /*load confile file*/
}

int main(void)
{

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	register_by_luabridge(L);

	lua_close(L);
	return 0;
}
