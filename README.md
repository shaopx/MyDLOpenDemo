# MyDLOpenDemo
1.  演示了一种通过读取/proc/self/maps文件, 来加载so文件, 并解析函数符号, 最终定位目标函数调用地址的方法. 
2.  演示了通过调用art::Thread::DumpJavaStack()方法, 来打印(收集)堆栈信息的方法, 同样的思路也可以实现对native堆栈的打印
3.  演示了如何获取当前线程对应的art::Thread的方法

## 具体介绍: https://www.jianshu.com/p/226e243a579b
