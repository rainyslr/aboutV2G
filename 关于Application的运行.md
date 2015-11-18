#Application类运行机制
@(V2G项目相关)
###Application添加事件方式
- `Application::DoInitialize` 
	初始化函数，该函数在已设定好的Application的开始运行时间调用StartApplication函数（在对应的时间点Schedule该函数的调用，相当于添加第一个事件）
-  `Application::StartApplication`
进行一些配置工作，并且Schedule一些事件，比如Schedule周期性执行的函数的第一次执行
###Application初始化的调用链
- `UdpEchoServerHelper::install(node)` 
调用`UdpEchoServerHelper::InstallPriv(node)`函数，为Application关联与之对应的Node对象指针。
- `Node::AddApplication(Application app)` 
调用app的Initialize，对app进行配置
-`Application::Initialize`
调用`Application::Initialize`函数实现初始化
###first.cc中Application示例解析
####udp-echo-client类
- udp-echo-client的StartApplication函数
```c++
1.绑定套接字m_socket
2.连接Server
3. m_socket->SetRecvCallback (MakeCallback (&UdpEchoClient::HandleRead, this));//为m_socket设置接收报文回调函数
4.ScheduleTransmit (Seconds (0.));//在时刻0Schedule一个发送事件
```
 - ScheduleTransmit函数进一步Schedule函数UdpEchoClient::Send（）的执行，添加发送事件。
- UdpEchoClient::Send（）
```c++
UdpEchoClient::Send (void)
{
Ptr<Packet> p;
p = Create<Packet> (m_data, m_dataSize);
m_txTrace (p);
m_socket->Send (p);
++m_sent;
if (m_sent < m_count) 
    {
      ScheduleTransmit (m_interval);//每间隔固定时间就Schedule自己的调用，实现添加发送报文事件，从而使得仿真列表中出现新事件推动仿真的进行。
    }
}
```
###项目各个App运行机制设计思路
App的运行机制上也仿照这样来写。大家的模块里面，如果需要在周期性执行的，就在函数内部Schedule自身。然后每个模块写一个DoInitialize函数，用于本模块的初始化工作。而App的DoInitialize函数则调用各个模块的DoInitialize函数，以及完成其他初始化工作。
