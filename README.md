# oslab
1. 递交代码的时候commit的内容要详细，直接push到主分支`git push origin`或`git push origin master`即可。
2. 代码最好加上详细的注释，说明函数的功能，一些需要特别关注的地方，用到的知识点。
3. 每次开始写代码前先pull一遍

## lab2
### 汇编标签命名规范：
|命名范式|例子|
|-|-|
|usage_mode | sti_handler_m|
|funtionname_retaddr|_start_retaddr|
|异常处理函数命名exceptionname_handler_mode|sti_handler_m|

随便写了几个，不遵守也不要紧。
现在调试的时候用符号设置断点，会设在虚拟地址上。如果在物理寻址模式下是无法断住的，用绝对地址才行。
如果要调试的话，可以加个-DDEBUGTEST选项，然后可以输出帮助调试的信息，最后上交的时候删掉这个-DDEBUGTEST。

## 经验总结
大家在看别人写的代码的时候，带着**批判的眼光**。大家都是自己人，别客气，**直接无情的指出**你觉得程序**可能**存在的问题。
上传代码流程：
1. git status查看修改情况。
2. git add 需要的代码，一般`git add .`
3. git commit -m "尽可能详细的注释，中英文均可"
4. git push origin master

写代码建议：
1. 先git pull origin，合并前人的工作
2. 一边写，写了一会感觉要备份一下就可以git add了
3. 感觉完成一个阶段的任务了就git commit 一下
4. 结束工作了就push一下
5. 在群里说一下

其他(具体使用GOOGLE)：
1. git checkout是用来反悔的
2. git log+git reset HEAD可以查看以前的代码。

---

## 代码规范
文件统一用UTF-8编码
注释：
    每一个函数体前要有注释说明
    ```
    /* input：函数的输入
     * output：函数的输出
     * usage: 函数是干什么的
     * other: 比如哪里还可以改进，什么需要注意的
     */
    ```
C语言缩进：
    统一用4个空格
    所有的花括号独立一行{

    }

**头文件**规范：
    对外提供的函数接口，绝对不能出现static
    宏
    typedef
    extern 别的变量，增加程序的耦合度，如果没有必要的话就不用

命名规范：
    对外提供的函数接口命名： _下划线命名
    
    变量命名： 

**Makefile**规范：
    ？？

## Git和Github
.gitignore  忽略文件
**git**分支管理

## 改进方向
我们有什么内容是妥协的操作，比如第一个进程的创建。，
1. shell
2. exit,wait,exec
3. file system，从而更好的实现page fault的处理
4. 第一个进程
5. loader
6. 添加更多的syscall，之前的syscall有了文件系统也要做修改
7. user部分的物理地址管理，真的使用buddy吗 kmalloc, malloc?
8. 解决一下现在的好多warning
9. **RISCV深入了解**，不只是指令
PS：回顾之前的lab，把上下文关系搞清楚，给之前的代码添加必要的注释，删除不必要的注释，把之前的代码整个重构一下。

## 问题
板子的写，debug
怎么更好的管理大的工程呢？ IDE？
怎么样系统的测试呢？
SRTP 做什么，怎么准备，有什么指导建议
重构

## 学习
RISCV
操作系统和Linux 
git
Makefile
工程管理    