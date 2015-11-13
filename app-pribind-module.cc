#include<iostream>
#include "ns3/object.h"
#include "ns3/ptr.h"
#include <string>
/***********************************************************************************
绑定的App和相应模块的更复杂实现版本，为了数据安全，模块中的APP指针是私有的，App中的模块指针也是私有的。
对于模块类：
每个模块通过自己的GetModule模板函数访问获取所在App中其他模块的指针，函数中设置了访问权限检查。该函数私有，只能由模块类的成员函数来调用
测试思路：
App的FuncApp函数调用ModuleB的funcb函数，funcb调用同一个App中的ModuleA的funca函数，funca函数输出模块A所在App的ID
v2g最后的思路，拟采用此方法
***********************************************************************************/
using namespace ns3;
using namespace std;
class App; //foward declaration

class ModuleBase:public Object
{
 int m_data;
};
class ModuleA:public ModuleBase
{
friend App;
public:
    void funca();
    ~ModuleA(){std::cout<<"~ModuleA"<<std::endl;}
private: 
friend Ptr<ModuleA> CreateObject<ModuleA>(App *app);
template<typename MoType>
 Ptr<MoType> GetModule(const string &name);
 ModuleA(){std::cout<<"ModuleA object is created"<<std::endl;}
 ModuleA(App *app);
 Ptr<App> m_ptrApp;
};


class ModuleB:public ModuleBase
{
friend App;
public:
 void funcb();
private:
 friend Ptr<ModuleB> CreateObject<ModuleB>(App *app);
template<typename MoType>
 Ptr<MoType> GetModule(const string &name);
 ModuleB();  
 ModuleB(App *app);
 Ptr<App> m_ptrApp;
};

class App:public Object 
{
friend ModuleA;
friend ModuleB;
public:
 App();
 App(int id);
 void FuncApp();
 void createA(){ModuleA ma;}
private:
 Ptr<ModuleA> m_ptrMa;
 Ptr<ModuleB> m_ptrMb;
 int m_appId;
};


ModuleA::ModuleA(App *app):m_ptrApp(app){}

/*输出所在App的id*/
void ModuleA::funca() {
        std::cout<<"Id of ModuleA's owner application is:"<<m_ptrApp->m_appId<<std::endl;
}


template<typename MoType>
Ptr<MoType> ModuleA::GetModule(const string &name)
{
 if(name == "ModuleB"){
  return DynamicCast<MoType>(m_ptrApp->m_ptrMb);
 }
 else{
 std::cout << "type error"<<std::endl;   
 Ptr<MoType> ptr;
 return ptr;
 } 
}

ModuleB::ModuleB(App *app):m_ptrApp(app){}

/*调用同一个App中的ModuleA的funca函数*/
void ModuleB::funcb() {
    if(m_ptrApp) { // make sure not null
      Ptr<ModuleA>  pma = GetModule<ModuleA>("ModuleA");
        pma->funca();
        return ;
    }
}

template<typename MoType>
Ptr<MoType> ModuleB::GetModule(const string &name)
{
 if(name == "ModuleA"){
  return DynamicCast<MoType>(m_ptrApp->m_ptrMa);
 }
 else{
 std::cout << "type error"<<std::endl;   
 Ptr<MoType> ptr;
 return ptr;
 } 
}


App::App(int m):m_appId(m)
{
  m_ptrMa = CreateObject<ModuleA>(this);
  m_ptrMb = CreateObject<ModuleB>(this);
}

App::App():m_appId(0)
{
  m_ptrMa = CreateObject<ModuleA>(this);
  m_ptrMb = CreateObject<ModuleB>(this);
}

//调用ModuleB的funcb函数。
void App::FuncApp()
{
  m_ptrMb->funcb();
}

int main()
{
    App app(100);
    app.FuncApp();
    return 0;
}
