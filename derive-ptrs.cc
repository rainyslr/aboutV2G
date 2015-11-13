#include<iostream>
#include "ns3/object.h"
#include "ns3/ptr.h"
/***********************************************************************************
这个脚本用于测试函数形参分别为智能指针，智能指针引用，智能指针的地址时，传入子类的智能指针相关类型数据时函数能否运行。
***********************************************************************************/
using namespace ns3;
class MyBase:public Object
{
public:
  virtual void fun()= 0;
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
    void testPtrRef(Ptr<MyBase> &rptr);
    void testPtrAdd(Ptr<MyBase> *pptr);
    void testPtr(Ptr<MyBase> ptr);
    
};


void ModuleA::fun() {
        std::cout<<"funca"<<std::endl;
    }
void ModuleB::fun() {
        std::cout<<"funcb"<<std::endl;
    }

void Test::testPtrRef(Ptr<MyBase> &rptr)
{
 rptr->fun();
 return;
}
void Test::testPtrAdd(Ptr<MyBase> *pptr)
{
 (*pptr)->fun();
 return;
}
void Test::testPtr(Ptr<MyBase> ptr)
{
 ptr->fun();
 return;
}

int main()
{
 Ptr<ModuleA> ma = CreateObject<ModuleA>();
 Ptr<ModuleB> mb = CreateObject<ModuleB>();
 Test mt;
 //*这两句不能运行，无法实现基类智能指针引用和子类智能指针引用的自动转换*//
 //mt.testPtrRef(ma);
 //mt.testPtrRef(mb);
 //Ptr<ModuleA> *pma = &ma;
 //Ptr<ModuleB> *pmb = &mb;
 //*这两句无法运行，无法将子类智能指针的指针转换成子类智能指针的指针*//
 //mt.testPtrAdd(pma);
// mt.testPtrAdd(pmb);
 mt.testPtr(ma);
 mt.testPtr(mb);
 return 0;
}
