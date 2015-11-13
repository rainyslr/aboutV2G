#include<iostream>
#include "ns3/object.h"
#include "ns3/ptr.h"
/***********************************************************************************
App中包含模块A和模块B，三者是绑定在一起的，能找到彼此。为了测试简便，所有成员都设置成公共的，并且未进行权限检查
测试思路：
通过app,调用模块B的funcb函数，该函数通过调用同一个App中的模块a的funca函数，输出App的id
***********************************************************************************/
using namespace ns3;

class App; //foward declaration

class ModuleA:public Object
{
friend App;

public:
 void funca();
 ~ModuleA(){std::cout<<"~ModuleA"<<std::endl;}
private: 
 friend Ptr<ModuleA> CreateObject<ModuleA>(App *app);
 ModuleA(){std::cout<<"ModuleA object is created"<<std::endl;}
 ModuleA(App *app);
 Ptr<App> m_ptrApp;
};


class ModuleB:public Object
{
friend App;
public:
 void funcb();
 ~ModuleB(){std::cout<<"~ModuleA"<<std::endl;}
private:
 friend Ptr<ModuleB> CreateObject<ModuleB>(App *app);
 ModuleB(App *app);
 ModuleB();
 Ptr<App> m_ptrApp;
};

class App:public Object 
{
friend void ModuleA::funca();
public:
 App();
 App(int id);
 Ptr<ModuleA> m_ptrMa;
 Ptr<ModuleB> m_ptrMb;
 void createA(){ModuleA ma;}
private:
int m_id;
};


ModuleA::ModuleA(App *app):m_ptrApp(app){}

//输出所在App的ID
void ModuleA::funca() {
        std::cout<<"Id of ModuleA's owner application is:"<<m_ptrApp->m_id<<std::endl;
    }


ModuleB::ModuleB():m_ptrApp(){}

//调用所在App的模块A的函数
ModuleB::ModuleB(App *app):m_ptrApp(app){}
void ModuleB::funcb() {
    if(m_ptrApp) { // make sure not null
        m_ptrApp->m_ptrMa->funca();
        return ;
    }
}

App::App(int m):m_id(m)
{
  m_ptrMa = CreateObject<ModuleA>(this);
  m_ptrMb = CreateObject<ModuleB>(this);
}

App::App():m_id(0)
{
  m_ptrMa = CreateObject<ModuleA>(this);
  m_ptrMb = CreateObject<ModuleB>(this);
}

int main()
{
    
    App app(100);
    app.m_ptrMb->funcb();
    return 0;
}
