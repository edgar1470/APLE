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



