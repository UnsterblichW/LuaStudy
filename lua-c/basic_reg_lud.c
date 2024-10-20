
#include <stdio.h>

#include <stdlib.h>

#include <lua.h>
#include <lualib.h>

#include <lauxlib.h>

static void
call_func_0(lua_State *L, const char * funcname) {
    lua_getglobal(L, funcname);
    lua_call(L, 0, 0);
}

static void
call_func_1(lua_State *L, const char * funcname, int params) {
    lua_getglobal(L, funcname);
    lua_pushnumber(L, params);
    lua_call(L, 1, 0);
}

int Counter(lua_State *L) {
    lua_getfield(L, LUA_REGISTRYINDEX, "mark.reg.counter");
    int *pcnt = (int*)lua_touserdata(L, -1);
    int n = ++(*pcnt);
    lua_pop(L, 1);
    return n;
}

int main(int argc, char** argv) {

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if (argc > 1) {

        if (luaL_dofile(L, argv[1]) != LUA_OK) {
            const char *err = lua_tostring(L, -1);
            fprintf(stderr, "err:\t %s \n", err);
            return 1;
        }

        int *pcnt = (int *)malloc(sizeof(int));
        *pcnt = 0;
        lua_pushlightuserdata(L, pcnt);
        lua_setfield(L, LUA_REGISTRYINDEX, "mark.reg.counter");

        call_func_0(L, "Init");
        call_func_1(L, "Loop", 1000);
        call_func_0(L, "DeInit");

        free(pcnt);
        pcnt = NULL;
    }

    lua_close(L);
    
    return 0;
}
