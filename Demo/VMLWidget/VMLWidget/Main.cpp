/////////////////////////////////////////////////
//            VMLWidget
//    描述 : 这是一个 VML 构建 VML 描述界面以实现前端
//			后端分离的 Demo
#include "../../../Library/VUILib"

void VMLButtonOnClicked() {
	MessageBox(GetHWnd(), L"VMLButton Pushed!", L"VMLButton Pushed!", MB_OK);
}

int main() {
	VApplication* Application = new VApplication();

	// 创建一个 VML 界面
	VVMLWidget*   VMLWidget   = new VVMLWidget(0, 0, Application);

	// 从文件导入一个 VML 界面
	VVMLParser    Parser(L"./LayoutVML.xml", VVMLParserParseMode::FromFile);

	// 解析 VML 后显示 VML 界面
	VMLWidget->LoadVML(Parser.ParseVML());

	// 链接 VML 层的 Button 的信号
	((VPushButton*)((VUIObject*)(*VMLWidget)[L"MainWindow"][L"PushButton"]))->ButtonPushed.Connect(VMLButtonOnClicked);

	return Application->Exec();
}