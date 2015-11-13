#include <iostream>
using namespace std;
/***********************************************************************************
这个脚本用于测试函数形参分别为基类，基类引用，基类指针，基类指针的指针时，传入子类的相关类型数据时函数能否运行。
/***********************************************************************************/
class MyBase
{
	public:
		virtual void fun() = 0;
};

class ModuleA:public MyBase
{
	public:
		virtual void fun();
};
class ModuleB:public MyBase
{
	public:
		virtual void fun();
};
class Test
{
	public:
	//test 函数不能声明，因为base是抽象类，不能定义成形参	
//		void test(MyBase base);
		void testptr(MyBase* ptr);
		void testref(MyBase &rbase);
		void testpptr(MyBase** pptr);
};
/*void Test::test(MyBase base)
{
  base.fun();
}*/
void Test::testptr(MyBase* ptr)
{
	ptr->fun();
}
void Test::testref(MyBase &rbase)
{
	rbase.fun();
}
void Test::testpptr(MyBase **pptr)
{
	(*pptr)->fun();
}
void ModuleA::fun()
{
	cout <<"funca"<<endl;
}
void ModuleB::fun()
{
	cout << "funcb"<<endl;
}
int main()
{
	ModuleA ma;
	ModuleB mb;
	Test mt;
	mt.testptr(&ma);
	mt.testptr(&mb);
	mt.testref(ma);
	mt.testref(mb);
	ModuleA* pma = &ma;
	ModuleB* pmb = &mb;
//下面这两行函数调用不能正常运行，因为无法将子类指针的指针转化成基类指针的指针。
//	mt.testpptr(&pma);
//	mt.testpptr(&pmb);
	return 0;
}
