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