#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>

extern int Counter(lua_State *);

static int
lecho(lua_State *L) {
    int gcnt = Counter(L);
    lua_Integer cnt = lua_tointeger(L, lua_upvalueindex(1));
    cnt++;
    const char * str = lua_tostring(L, -1);
    printf("[cnt=%lld, gcnt=%d] %s\n", cnt, gcnt, str);
    lua_pushinteger(L, cnt);
    lua_replace(L, lua_upvalueindex(1));
    return 0;
}

int luaopen_closure_reg(lua_State *L) {
    lua_createtable(L, 0, 1);
    lua_pushinteger(L, 0);
    lua_pushcclosure(L, lecho, 1);
    lua_setfield(L, -2, "echo");
    return 1;
}