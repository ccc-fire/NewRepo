//
//
//#include<future>
//#include<vector>
//
//static std::mutex s; //互斥锁
//
//
//std::lock_guard<std::mutex> lock(s); //上锁
//
//void F()
//{
//
//}
//
//std::vector<std::future<void>> m;
//
///*
//1、为什么不能传引用？
//线程函数的参数按值移动或复制。如果引用参数需要传递给线程函数，它必须被包装（例如使用std :: ref或std :: cref）
//
//2、std::async为什么一定要返回值？
//如果没有返回值，那么在一次for循环之后，临时对象会被析构，而析构函数中需要等待线程结束，所以就和顺序执行一样，一个个的等下去
//如果将返回值赋值给外部变量，那么生存期就在for循环之外，那么对象不会被析构，也就不需要等待线程结束。
//*/
//
//#define AS 1
//
//#if AS
//
//#else
//
//#endif