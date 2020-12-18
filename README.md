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
3. 感觉完成一个阶段的人物了就git commit 一下
4. 结束工作了就push一下
5. 在群里说一下

其他(具体使用GOOGLE)：
1. git checkout是用来反悔的
2. git log+git reset HEAD可以查看以前的代码。