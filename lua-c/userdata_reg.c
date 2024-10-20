
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>
/**
 *  1. 打印功能
 *  2. 记录打印次数
 *  3. 提供一个接口，输入最近打印 10 条记录
 */

struct log {
    int count;
};
/*
q->count % 10   
0,1,2,...9
1,2,3,...,10

(q->count -1) % 10 + 1

*/

extern int Counter(lua_State *);

static int lecho (lua_State *L) {
    int gcnt = Counter(L);
    struct log *q = (struct log *)luaL_checkudata(L, 1, "mk.userdata.log");
    q->count++;
    const char *str = lua_tostring(L, -1);
    printf("[cnt=%d,gcnt=%d] %s\n", q->count, gcnt, str);
    lua_getuservalue(L, 1);
    lua_pushvalue(L, 2);
    lua_seti(L, -2, ((q->count -1) % 10 + 1));
    return 0;
}

static int lhistory (lua_State *L) {
    int gcnt = Counter(L);
    struct log *q = (struct log *)luaL_checkudata(L, 1, "mk.userdata.log");
    lua_getuservalue(L, 1);
    if (q->count <= 10) {
        for (int i=q->count; i >= 1; i--) {
            lua_geti(L, -1, i);
            printf("H:[cnt=%d,gcnt=%d] %s\n", i, gcnt, lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    } else {
        for (int i=0; i<10; i++) {
            int cnt = q->count - i;
            int idx = (cnt-1) %10 + 1;
            lua_geti(L, -1, idx);
            printf("H:[cnt=%d,gcnt=%d] %s\n", cnt, gcnt, lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    return 0;
}

static int lgc(lua_State *L) {
    struct log *q = (struct log *)luaL_checkudata(L, 1, "mk.userdata.log");
    printf("userdata[%p] has gc\n", q);
    return 0;
}

static int lnew (lua_State *L) {
    struct log *q = (struct log *)lua_newuserdata(L, sizeof(struct log));
    q->count = 0;
    lua_createtable(L, 10, 0);
    lua_setuservalue(L, -2);
    if (luaL_newmetatable(L, "mk.userdata.log")) {
        luaL_Reg m[] = {
            {"echo", lecho},
            {"history", lhistory},
            {NULL, NULL},
        };
        luaL_newlib(L, m);
        lua_setfield(L, -2, "__index");
        lua_pushcfunction(L, lgc);
        lua_setfield(L, -2, "__gc");
        /*
        {
            [echo] = lecho,
            [history] = lhistory,
        }
        */
    }
    lua_setmetatable(L, -2);
    return 1;
}

static const luaL_Reg l[] = {
    {"new", lnew},
    {NULL, NULL},
};

int luaopen_ud(lua_State *L) {
    luaL_newlib(L, l);
    return 1;
}
