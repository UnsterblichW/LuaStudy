
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

        // 宿主语言C语言，申请一块空间给 lightuserdata 使用，注意 lightuserdata 的空间需要宿主语言管理，所以下面需要free
        int *pcnt = (int *)malloc(sizeof(int));
        *pcnt = 0;

        lua_pushlightuserdata(L, pcnt);
        // LUA_REGISTRYINDEX是有效伪索引，"mark.reg.counter"这个表名必须全局唯一（任何 C 库都可以在这张表里保存数据，为了防止冲突）
        // 而且"mark.reg.counter"并不在虚拟栈上面，也就是说lua_pushlightuserdata和lua_setfield执行完后，虚拟栈的状态会和原来一样
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
