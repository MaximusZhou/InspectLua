InspectLua
==========

主要深入学习Lua，包括Lua与C等其他语言的交互、Lua源码阅读等。下面是各个源文件一个简单的说明：

学习在C中调用lua的一般全局变量、table以及函数相应的文件有：
TestConfig.lua 

util.h
c_function_to_extend_lua.h

util.c                                --主要定义了打印lua栈、C中调用Lua的函数一个通用接口
capi_example.c                        --主要用来熟悉lua与c交互的一些API
c_function_to_extend_lua.c
extend_c_app_by_lua.c
test_general_call_lua_in_c.c
