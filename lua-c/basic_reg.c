
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
    // 通过LUA_REGISTRYINDEX有效伪索引注册的表，依然只能通过它重新获取到
    // 此时压栈了一个值，也就是下面这个 n = lua_tointeger 要转化的值
    lua_getfield(L, LUA_REGISTRYINDEX, "mark.reg.counter");
    int n = lua_tointeger(L, -1);
    n++;
    lua_pushinteger(L, n); // 这个时候又压栈了一个值，压入的是自增之后的n，此时栈中已经有两个值了
    lua_setfield(L, LUA_REGISTRYINDEX, "mark.reg.counter"); // 自增之后的n弹出栈，被设置到伪索引的注册表里面去了
    lua_pop(L, 1); //还需要再弹出 n = lua_tointeger
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

        // LUA_REGISTRYINDEX是有效伪索引，"mark.reg.counter"这个表名必须全局唯一（任何 C 库都可以在这张表里保存数据，为了防止冲突）
        // 而且"mark.reg.counter"并不在虚拟栈上面，也就是说lua_pushlightuserdata和lua_setfield执行完后，虚拟栈的状态会和原来一样
        // 
        lua_pushinteger(L, 0);
        lua_setfield(L, LUA_REGISTRYINDEX, "mark.reg.counter");

        call_func_0(L, "Init");
        call_func_1(L, "Loop", 1000);
        call_func_0(L, "DeInit");

    }

    lua_close(L);
    
    return 0;
}
