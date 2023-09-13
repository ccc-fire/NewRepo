//
//#include<string>
//#include <iostream>
//#include <string_view>
//
//static uint32_t s_AlloCount = 0;
//#define STRING_VIEW 1
//void* operator new(size_t size)
//{
//	s_AlloCount++;
//	std::cout << "malloc " << size << " bytes\n";
//	return malloc(size);
//}
//
//#if STRING_VIEW
//void PrintName(std::string_view name)
//{
//	std::cout << name << std::endl;
//}
//#else
//void PrintName(const std::string& name)
//{
//	std::cout << name << std::endl;
//}
//#endif
//
//
//int main()
//{
//	std::string name = "Yanyan Zhu";
//#if STRING_VIEW 
//	std::string_view first(name.c_str(), 6);
//	std::string_view last(name.c_str()+7, 3);
//#else
//	std::string first(name.substr(0, 6));
//	std::string last(name.substr(7, 3));
//#endif
//	PrintName(first);
//
//	PrintName(last);
//
//	std::cout << s_AlloCount << " allocations" << std::endl;
//	
//}