# APLE
Advanced Programming in the Linux Environment from Vienct Wei's class.

记录参加魏老师 《高级 LINUX 环境编程》的笔记和测试练习代码

## 第一讲作业：
- 阅读开源代码：
    - Glib 中 g_slice 分配器: https://github.com/GNOME/glib
    - WTF 中的 bmalloc 分配器和 libpas: https://github.com/WebKit/WebKit
    - PurC 中的变体管理及 move heap: https://github.com/HVML/PurC
- 用 Buddy 算法实现一个私有堆的管理模块，并和 C 库的 malloc/free 对比性能
    - 要求使用线程局部存储来管理线程私有堆。
    - 衡量私有堆的内存使用效率。


## 第二讲作业
1. 阅读开源 档及代码：
    Glib 中的 件操作和 件系统接 封装
    https://docs.gtk.org/glib
    https://github.com/GNOME/glib
2. 改写方同学的程序，使之很容易重现写混乱；进一步增强上述程序，使之彻底避免写混乱。 [我的实现](https://github.com/edgar1470/APLE/blob/main/src/lecture01_memory_management/multi-process-concurent-write-to-one-same-file.c)
3. 编写一个程序，启动 /bin/bc 并通过伪终端驱动 /bin/bc 完成任意精度的四则表达式计算。[我的实现](https://github.com/edgar1470/APLE/blob/main/src/lecture02_filesystem/fake-bc.c)
4. 编写一个 Shell 脚本和一个 C 程序用于遍历文件系统，给出指定路径下所有文件和目录项的总和. [我的实现](https://github.com/edgar1470/APLE/blob/main/src/lecture02_filesystem/recursive-iterator-directory.sh) [我的实现](https://github.com/edgar1470/APLE/blob/main/src/lecture02_filesystem/recursive-iterator-directory.c)
