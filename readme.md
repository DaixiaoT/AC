# 版本说明

**个人测试**

此版本是在版本**2.1**的基础上所做修改，其中改动的地方有以下几处：

## 主要改动


1. 源程序中 部分注释、未使用到的函数、以前的打印信息全部被删除
2. 将maintenance.cpp 文件拆分成ptu_monitor.cpp、ptu_parameter.cpp、ptu_error_run_log.cpp三个文件---------------------2023-08-15 10:44:12
3. 删除所有后期代码，仅保留可运行框架。-------------------------------------------------------------------------------2023-08-16 16:34:51

> 当前此版本作为最初始版本，今后所有测试都在此版本基础上

4. 将函数valve1_timeout()移到类中，才可以正常通过编译，明天再根据膨胀阀485通讯协议，修改一下485文件中变量的命名。--------2023-08-17 17:21:52
5. 更改485Module中的变量命名，使用更明了的单词进行变量命名。下一步将完成膨胀阀和变频器的数据读取。-----------------------2023-08-18 17:08:10
6. 完成485通信模块编写，且调试后可以正常通信。明天继续完善ac_control()中的逻辑。-----2023-08-21 18:05:49







