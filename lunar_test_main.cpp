extern "C" {
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include "lunar.hpp"

class Account {
	public:
		Account(double balance=0)    : m_balance(balance) { }
		void deposit(double amount)  { m_balance += amount; }
		void withdraw(double amount) { m_balance -= amount; }
		double balance(void)         { return m_balance; }
		~Account() { printf("deleted Account (%p)\n", this); }

		// Lua interface
		Account(lua_State *L) :     m_balance(luaL_checknumber(L, 1)) { }
		int deposit (lua_State *L) { deposit (luaL_checknumber(L, 1)); return 0; }
		int withdraw(lua_State *L) { withdraw(luaL_checknumber(L, 1)); return 0; }
		int balance (lua_State *L) { lua_pushnumber(L, balance()); return 1; }

		static const char className[];
		static Lunar<Account>::RegType methods[];

	private:
		lua_Number m_balance;
};

const char Account::className[] = "Account";

#define method(class, name) {#name, &class::name}

Lunar<Account>::RegType Account::methods[] = {
	method(Account, deposit),
	method(Account, withdraw),
	method(Account, balance),
	{0,0}
};

static int report (lua_State *L, int status)
{
	if (status) {
		const char *msg = lua_tostring(L, -1);
		if (msg == NULL) msg = "(error with no message)";
		fprintf(stderr, "ERROR: %s\n", msg);
		lua_pop(L, 1);
	}
	return status;
}

static int application (lua_State *L)
{
	lua_settop(L, 0);
	lua_pushliteral(L, "_TRACEBACK");
	lua_gettable(L, LUA_GLOBALSINDEX);   // get traceback function
	int tb = lua_gettop(L);

	Account a;
	Account *b = new Account(30);

	int A = Lunar<Account>::push(L, &a);
	int B = Lunar<Account>::push(L, b, true);

	lua_pushliteral(L, "a");
	lua_pushvalue(L, A);
	lua_settable(L, LUA_GLOBALSINDEX);

	lua_pushliteral(L, "b");
	lua_pushvalue(L, B);
	lua_settable(L, LUA_GLOBALSINDEX);

	lua_pushvalue(L, A);
	lua_pushnumber(L, 100.00);
	report(L, Lunar<Account>::call(L, "deposit", 1, 0, tb) < 0);
	lua_pushvalue(L, A);
	report(L, Lunar<Account>::call(L, "show",    0, 0, tb) < 0);
	lua_pushvalue(L, B);
	report(L, Lunar<Account>::call(L, "show",    0, 0, tb) < 0);

	lua_pushliteral(L, "main");
	lua_gettable(L, LUA_GLOBALSINDEX);
	report(L, lua_pcall(L, 0, 0, tb));

	return 0;
}

int main (int argc, char *argv[])
{
	lua_State *L = luaL_newstate();

	luaL_openlibs(L);

	Lunar<Account>::Register(L);

	if (argc>1) {
		printf("loading '%s'\n", argv[1]);
		if (report(L, luaL_loadfile(L, argv[1]) || lua_pcall(L, 0, 0, 0)) == 0) {
			printf("running application\n");
			if (report(L, lua_cpcall(L, &application, 0)) == 0) {
				printf("okay\n");
			}
		}
	}

	lua_gc(L,LUA_GCRESTART,0);
	printf("close\n");
	lua_close(L);
	printf("done\n");
	return 0;
}
