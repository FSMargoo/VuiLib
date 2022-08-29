<div align=center>
    <img src="../src/logo.png"/>
    <h1>VuiLib 快速上手：使用 VWidget 创造一个简单的应用</h1>
</div>

> 该部分将会通过 VuiLib 中的 VWidget 来创造一个简单的 Demo 应用（ 即不使用任何 VML 和 VSS，纯 C++ 开发 ）
> 
> 在开始教程以前，请先使用您任意受使用支持的编译器和 API 版本的 IDE 创建一个 .cpp 文件
> 
> 本教程的完整源码可以在 doc/code/vwidget-start 找到


## 第一步：引用 VuiLib 的头文件
1. 打开你的 cpp 文件，如果你已经配置好了 VuiLib 环境，并且已经连接好了 VuiLib 的静态库，那么你可以开始像下面的代码一样引用 VuiLib 的头文件
```C++
// 按照您的配置地址而定具体头文件地址
// demo 中的源码是直接引用了当前版本的 vuilib 源码
#include <vuilib/core/widget/vwidget.h>
#include <vuilib/core/control/vpushbutton.h>
```

## 第二步：创建一个 VApplication 类
VApplicaiton 类是整个 VuiLib 的基石，没有它，整个 VuiLib 不能够正常运作，它负责处理消息循环并通过对象树消息发送给每一个对象，创建一个 VApplicaiton 类需要修改 main 的返回值为 ${YourApplicaitonClassName}.Exec(); 就像下面一样:
```C++
int main() {
    Core::VApplication MyDemo;

	return MyDemo.Exec();
}
```
恭喜，至此你已经创建了一个 VuiLib 的框架，接下来，开始创建我们的第一个 VuiLib 窗口吧。
## 第三步：创建一个 VMainWindow 窗口并设置标题
1. 创建窗口：在 VuiLib 的纯 C++ 开发中，使用 VMainWindow 作为我们唯一的创建窗口方法，我们可以用如下方式创建一个窗口:
```C++
// 第三个参数需要传入一个 VApplicaiton 的指针
Core::VMainWindow MyWindow(640, 480, &MyDemo);
```
2. 设置标题：在 VMainWindow 中提供了一个方法 SetTitle 来设置窗口的标题，我们可以通过调用 SetTitle 即可设置窗口在标题栏上展示的文字，如下所示:
```C++
MyWindow.SetTitle(L"Hello VuiLib!");
```
接下来，将你所写的代码编译出来，如果这段代码正常工作，那么它将会弹出一个 640px 宽 480px 高，标题为 Hello VuiLib! 的黑色背景窗口，如果是这样，那恭喜你，你成功地创建了一个 VuiLib 的窗口！接下来，为你的窗口添加一些能和用户交互的东西吧！
## 第四步：创建一个 VPushButton 按钮控件
还记得我们一开始引用的 vpushbutton.h 吗？关于按钮控件的东西就在这个小文件里头，我们现在可以直接开始创建一个按钮控件了，接下来，我们可以使用这段代码来创建一个按钮控件：
```C++
Core::VPushButton MyPushButton(100, 40, L"Push me!", &MyWindow);
```
接下来，再把你所写的代码编译运行，如果你看到左上角有一个按钮出现并且写着 Push me! 字样，那么恭喜你，你已经成功地创建了一个按钮，可是这个按钮并不会相应用户的点击，让我们来让它能处理用户的点击吧！
## 第五步：绑定 VPushButton 的信号
VPushButton 中有一个公有成员，叫 ButtonPushed，可以通过这个成员的 Connect 方法来链接按钮按下的信号，就像下面一样
```C++
// 此处使用 λ 表达式来代替函数
MyPushButton.ButtonPushed.Connect([]()->void { MessageBox(GetHWnd(), L"You pushed me uwu!", L"uwu", MB_OK); });
```
再次编译运行，并且点击按钮，你应该会发现，在点击以后弹出了一个消息提示框，如果是这样，恭喜你成功给按钮的 ButtonPushed 信号绑定了一个 lambda 对象！

VuiLib 中的信号功能非常强大，也是 VuiLib 中的基石之一，整个 VuiLib 界面依托信号进行驱动，在日后我们在对它进行慢慢的介绍。
## 结语
至此，本篇到此结束，你也成功使用 VuiLib 创建了一个简单的应用程序，因为全部代码非常的简短，这里放出本篇的完整代码（可以直接复制到你的 IDE 或编辑器里编译运行）：
```C++
#include "../../../include/core/widget/vwidget.h"
#include "../../../include/core/control/vpushbutton.h"

int main() {
	Core::VApplication MyDemo;
	Core::VMainWindow  MyWindow(640, 480, &MyDemo);

	Core::VPushButton MyPushButton(100, 40, L"Push me!", &MyWindow);

	MyPushButton.ButtonPushed.Connect([]()->void { MessageBox(GetHWnd(), L"You pushed me uwu!", L"uwu", MB_OK); });
	MyWindow.SetTitle(L"Hello VuiLib!");

	return MyDemo.Exec();
}
```