# 各个文件的学习内容

userdata.c 和 userdata.lua 主要在研究 Lua 和 C 之间交互的 UserData 这个机制如何使用

-----
basic_reg.c 会提供一个注册表函数 Counter ，然后把它编译成可执行程序，会在 lua-c/userdata_reg.c 和 lua-c/closure_reg.c 里面用到；
所以说在编译 basic_reg.c 的时候，需要引入liblua库 libdl动态库相关的库(提供了对动态加载库的支持，通常用于运行时加载和卸载共享库) libm数学库(包含了标准数学函数的实现)

`gcc basic_reg.c -o basic_reg -llua -ldl -lm`

然后在编译lua-c/userdata_reg.c 和 lua-c/closure_reg.c的时候，就只用编译成 .so 就可以，因为他们俩是lua要调用的，不需要可执行

`gcc userdata_reg.c -o userdata_reg.so -fPIC --shared`

`gcc closure_reg.c -o closure_reg.so -fPIC --shared`

lua-c/basic_reg.lua 会调用上面两个so库里面的函数

当执行 `./basic_reg ./basic_reg.lua` 的时候会报错
```shell
$ ./basic_reg ./basic_reg.lua 
err:     error loading module 'userdata_reg' from file './userdata_reg.so':
        ./userdata_reg.so: undefined symbol: lua_settop 
```
lua_settop明显是 liblua库里面的函数，问题出在 basic_reg 这个可执行文件里面没有包含userdata_reg.so和closure_reg.so想要的符号表，
所以说要这样改一下

`gcc basic_reg.c -o basic_reg -llua -ldl -lm -Wl,-E`

- -Wl,-E：这是一个传递给链接器的选项。
- -Wl是一个传递选项给链接器的通用前缀，而-E是传递给链接器的具体选项。
- -E通常与--export-dynamic选项相对应，这意味着它告诉链接器将所有的全局符号都导出到动态符号表中。这样做可以让动态链接的共享库能够引用主程序中的符号。

需要注意的是， basic_reg.lua 里面调用了两个c模块， userdata_reg 和 closure_reg ，**而这两个模块，都可以访问到 basic_reg.c 里面创建的注册表**，这就是注册表的作用

-----
lua-c/basic_reg_lud.c 这里是研究 lightuserdata 的使用方式
