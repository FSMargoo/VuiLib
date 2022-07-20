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
	auto Result = VMLWidget->LoadVML(Parser.ParseVML());

	// 检查 VML 导入情况
	if (Result.Status != VVMLWidgetVMLLoadStats::Ok) {
		MessageBox(GetHWnd(), Result.FailedMessage.c_str(), L"We are Sorry that VML Loaded Error", MB_OK + 16);

		return -1;
	}

	// 链接 VML 层的 Button 的信号
	((VPushButton*)((VUIObject*)(*VMLWidget)[L"MainWindow"][L"PushButton"]))->ButtonPushed.Connect(VMLButtonOnClicked);

	return Application->Exec();
}