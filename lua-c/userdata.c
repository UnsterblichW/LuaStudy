
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
static int lecho (lua_State *L) {
    // 检查函数的第 arg 个参数是否是一个类型为 tname 的用户数据 （参见 luaL_newmetatable )。 它会返回该用户数据的地址 （参见 lua_touserdata）。
    struct log *q = (struct log *)luaL_checkudata(L, 1, "mk.userdata.log");
    q->count++; // 累加打印的次数
    const char *str = lua_tostring(L, -1); // 接受lua那边传来的字符串
    printf("[cnt=%d] %s\n", q->count, str);
    lua_getuservalue(L, 1);
    lua_pushvalue(L, 2);
    lua_seti(L, -2, ((q->count -1) % 10 + 1));
    return 0;
}

static int lhistory (lua_State *L) {
    struct log *q = (struct log *)luaL_checkudata(L, 1, "mk.userdata.log");
    lua_getuservalue(L, 1);
    if (q->count <= 10) {
        for (int i=q->count; i >= 1; i--) {
            lua_geti(L, -1, i);
            printf("H:[cnt=%d] %s\n", i, lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    } else {
        for (int i=0; i<10; i++) {
            int cnt = q->count - i;
            int idx = (cnt-1) %10 + 1;
            lua_geti(L, -1, idx);
            printf("H:[cnt=%d] %s\n", cnt, lua_tostring(L, -1));
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
    // 此时，虚拟栈中只有一个元素，就是这个创建出来的 userdata ，后面会往栈里面进进出出一系列元素，但是这个userdata一直都在栈里面
    struct log *q = (struct log *)lua_newuserdata(L, sizeof(struct log));
    q->count = 0;

    //
    lua_createtable(L, 10, 0);
    lua_setuservalue(L, -2);

    // 如果注册表中已存在键 tname，返回 0 。
    // 否则，为用户数据的元表创建一张新表。向这张表加入 __name = tname 键值对，并将 [tname] = new table 添加到注册表中，返回 1 。
    if (luaL_newmetatable(L, "mk.userdata.log")) {
        luaL_Reg m[] = {
            {"echo", lecho},
            {"history", lhistory},
            {NULL, NULL},
        };
        // 把上面这个m表，放入栈顶，也就是 m 的位置是 -1
        luaL_newlib(L, m);
        // 做一个等价于 t[k] = v 的操作， 这里 t 是给出的索引处的值， 而 v 是栈顶的那个值。 这个函数将把这个值弹出栈。 
        // 此处把名为 "mk.userdata.log" 的metatable里面，加入了 ["__index"] = m 
        lua_setfield(L, -2, "__index");
        // 这个新增gc元方法和上面的新增__index元方法的原理是一样的
        lua_pushcfunction(L, lgc);
        lua_setfield(L, -2, "__gc");
        /*
        {
            [echo] = lecho,
            [history] = lhistory,
        }
        */
    }

    // 上面的一通操作过后，走到这里的时候，虚拟栈里面只有 2个元素了，
    // 栈底是函数刚刚开始的时候创建的 userdata，
    // 栈顶是上面的if里面创建的 metatable ，就是名字被我们叫做 "mk.userdata.log" 的这个
    // "mk.userdata.log" 的位置是 -1 ，把它弹出，设置给 userdata 位置是 -2
    lua_setmetatable(L, -2); // 把一张表弹出栈，并将其设为给定索引处的值的元表。
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



// 通过 这下面这条命令 把 userdata.c 编成 动态库 .so ，然后在 userdata.lua 里面就可以通过package.cpath来找到这个模块了
// gcc userdata.c -o ud.so -fPIC --shared