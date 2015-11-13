#智能指针
@(NS3相关)[Ptr][SimpleRefCount]
##特别注意
即使ptr指针为空（也就是传统意义上的等于0），还是能通过该指针调用类的某个函数(只要不设计类的数据的访问就不会报错，如果函数设计类对象数据的访问，提示段错误)
**因此良好的编程习惯应该是检查所得到的Ptr是否为空，再通过其调用类对象的函数**
```c++
 Ptr<ExamAlmc> pAlmc;
 if(pAlmc == 0)
{
 cout << "ptr Null"<<endl;
 pAlmc->funAlmc();
 cout << "test data of Almc is:"<<pAlmc->m_testdata<<endl;
}
```
示例程序提示段错误，但是funAlmc函数被调用了

##SimpleRefCount
递归模板类，自带计数功能
- Ref函数
使引用计数值增加1.用户在引用该类对象的时候应该调用该函数
- Unref函数
使引用计数值减1.在计数值达到0的时候delete该类对象。
- 构造函数
定义了默认构造函数和接受一个SimpleRefCount引用的构造函数，创建对象时均将计数值初始化为1.
###Ptr类
####Acquire函数
调用内置指针的Ref函数，即增加计数值
####构造函数
 - 默认构造函数：内置指针初始化为NULL指针
 - Ptr (T *ptr):使内置指针指向ptr所指位置并调用Acquire()
 - Ptr (T *ptr, bool ref):使内置指针指向ptr所指位置,如果ref为True则调用Acquire()
 - Ptr (Ptr const&o) ：构建新Ptr，使其指向o所指内存
####析构函数
调用内置指针的Unref(),即减少引用计数值
###Ptr类相关的模板函数
####PeekPointer
- 只返回内置Ptr中的内置指针而不调用Accqire函数。
- 在需要实现指针的值行为的时候调用该函数
- 比如`Copy (Ptr<T> object)`函数模板。
- 两种使用模式
 - 和new T()配套使用，取得指针只是为了构造一个和指针指向的值相同的T对象副本
 - 和Accquire函数一起使用，多见与Ptr的构造函数，使得新指针指向传入指针所指的内容
```c++
U * PeekPointer (const Ptr<U> &p){
  return p.m_ptr;
}
```
####GetPointer
注释中说是永久获得指针，要求该函数的调用者在合适的时机调用p.m_ptr的Unref函数（在不再引用该指针后）。
```c++
U * GetPointer (const Ptr<U> &p){
  p.Acquire ();
  return p.m_ptr;
}
```
####Copy函数
只是创建一个新的`Ptr<T>`，使得指针指向的值和形参指针指向的值相同（而非新指针和和形参指针指向相同地址）
```c++
Ptr<T> Copy (Ptr<T> object){
  Ptr<T> p = Ptr<T> (new T (*PeekPointer (object)), false);
  return p;
}
```
####Create函数
根据传入的形参构建新的T对象，并以此新对象创建一个智能指针
`return Ptr<T> (new T (), false);`
