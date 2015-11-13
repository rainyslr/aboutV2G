#include<iostream>
#include "ns3/object.h"
#include "ns3/ptr.h"
/***********************************************************************************
这个脚本用于测试函数形参分别为智能指针，传入子类的数据时能否访问子类中新增的数据成员
***********************************************************************************/
using namespace ns3;
class MyBase:public Object
{
public:
  MyBase(int ti):m_baseData(ti){}
  int m_baseData;
};

class ModuleA:public MyBase
{
public:
  ModuleA(int ti,float tf):MyBase(ti),m_subData(tf){}
  float m_subData;
};


class Test
{
public:
    void testPtr(Ptr<MyBase> ptr);
    
};
/*
这个函数版本不同通过，主函数中的调用语句是合法的，但是函数执行时报错说MyBase中没有m_subData成员。和普通的指针相似。即使调用函数时传进的实参为子类指针，
仍旧不能通过该指针访问子类的数据成员（但是能通过该指针实现虚函数的动态绑定）。
void Test::testPtr(Ptr<MyBase> ptr)
{
 std::cout << "m_baseData is:"<< ptr->m_baseData << std::endl;
 std::cout << "m_subData is:"<< ptr->m_m_subData << std::endl;
 return;
}*/

//testPtr函数的改进版本，没有什么使用性，只是为了说明只有将基类指针强制转换成子类的智能指针之后才能够通过指针访问子类的数据成员
void Test::testPtr(Ptr<MyBase> ptr)
{
 Ptr<ModuleA> ptra= DynamicCast<ModuleA>(ptr);//将实参强制转换成Ptr<ModuleA>
 std::cout << "m_baseData is:"<< ptra->m_baseData << std::endl;
 std::cout << "m_subData is:"<< ptra->m_subData << std::endl;
 return;
}

int main()
{
 
 Ptr<ModuleA> ma = CreateObject<ModuleA>(100,99.9);
 Test mt;
 mt.testPtr(ma);
 return 0;
}
