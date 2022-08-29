<div align=center>
    <img src="../src/logo.png"/>
    <h1>VuiLib[VML] 快速上手：使用 VML 快速构建软件布局</h1>
</div>

> 该部分将会通过 VuiLib 中的 VML 快速构建一个软件的布局
> 
> 本教程的完整 VML 源码可以在 doc/code/vml-start 找到

## 第一步：认识 VML
VML 是基于 XML 改造而来的语言，可以让你快速的构造软件的布局，一个基础的 VML 节点如下定义：
```XML
<!--
	type   : 控件类型 例如: pushbutton
	x, y, 
	width, 
	height : 控件几何信息 （必须为数字或富属性内容）
-->
<tag type="control-type" x="x" y="y" width="width" height="height">
</tag>
```
type、x、y、width、height 这五个属性是构成一个控件的五要素，这五个属性中最重要的是 type 属性，指定控件的类型。

而对于 VML 的显示，可以在 C++ 层使用 VML::VMLWidget 来调用显示，如下所示：
```C++
#include "./include/core/render/vpainter.h"
#include "./include/core/widget/vwidget.h"

#include "./include/vml/vmlwidget.h"

int main() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;

    VMLWidget VMLWidget(0, 0, &Application, true);
    auto	  Result = VMLWidget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);

    return Application.Exec();
}
```
而如果是我们在开发过程中，则可以使用仓库目录里 toolchain 下的 vmlviewer_x64.exe 使用如下方法来预览你的 VML：
```
vmlviewer.exe ${vmlfilepath}
```
## 第二步：开始使用 VML 构建第一个窗口
你已经了解了基本语法，接下来，开始使用 VML 来构造第一个窗口吧，VMainWindow 的 VMLType 是 mainwindow，并且接受 title 属性设置标题，sizable 属性设置是否拖拽缩放大小，所以我们可以这么写 VML 来构造窗口：
```XML
<vml-window type="mainwindow" title="Hello VML!" width="680" height="320">
</vml-window>
```
接下来，使用 vmlviewer.exe 打开这个 VML 文件，如果工作正常，那么会出现一个宽为 680，高为 320 的窗口，如果是这样，恭喜你，成功使用 VML 创建了第一个你的窗口！
## 第三步：添加文本框控件
既然这个窗口还什么都没有，我们不妨给它加入一个文本框，让它显示 Hello World，VTextLabel 的 VMLType 是 textlabel，支持 text 属性输入文本，那么我们就可以这样子定义我们的 Hello World 文本：
```XML
<hello-world-text type="textlabel" text="Hello World!" width="80" height="20">
</hello-world-text>
```
接下来，在使用 vmlviewer_x64.exe 打开预览这个 VML，您将会在左上角看到一行灰色的 Hello World! 如果无误的话，恭喜你成功使用 VML 做出了一个 Hello World
## 第四步：使用富属性创建相对位置布局
但是我们发现我们的 Hello World! 在左上角，并不明显，如何让他居中，这里提供一个方法：富属性，在 VML 中，富属性就是为了相对布局而生，其提供了一个非常优雅的方式来让用户可以快速进行布局操作，例如把 Hello World! 文本居中，只需要这么写：
```XML
<hello-world-text type="textlabel" text="Hello World!" width="80" height="20" x="@center" y="@center">
</hello-world-text>
```
再次使用 vmlviewer_x64.exe 预览，就会发现 Hello World! 居中显示，富属性的相对布局，是相对于该节点的父节点而言（窗口无法使用富布局）除此以外，还可以使用例如 @scale @fill 的富属性控制长宽，@relative 控制相对 x、y，@middleoffset 控制中心位移 x、y，这里不多做赘述。