/////////////////////////////////////////////////
//            VMLWidget
//    描述 : 这是一个使用 VML 和 VSS 构建的界面
//          主要实现一键换肤功能
#include "../../../Library/VUILib"

VVMLWidget* VMLWidget;

VPushButton* DemoButton;
VTextLabel*  TextLabel;
VTextLabel*  CommentLabel;

void SetBlackTheme() {
	VSSParser Parser(L"./UI/LayoutSurfaceBlack.css", VVVSParserParseMode::FromFile);

	VMLWidget->SetStyleSheet(Parser.ParseVSS());

	DemoButton->SetPlaneText(L"黑色主题的按钮");
	TextLabel->SetPlaneText(L"黑色主题的文本框");
	CommentLabel->SetPlaneText(L"↑ 黑色主题的输入框");
}
void SetWhiteTheme() {
	VSSParser Parser(L"./UI/LayoutSurfaceWhite.css", VVVSParserParseMode::FromFile);

	VMLWidget->SetStyleSheet(Parser.ParseVSS());

	DemoButton->SetPlaneText(L"白色主题的按钮");
	TextLabel->SetPlaneText(L"白色主题的文本框");
	CommentLabel->SetPlaneText(L"↑ 白色主题的输入框");
}

int main() {
	VApplication* Application = new VApplication();

	// 创建一个 VML 界面
	              VMLWidget = new VVMLWidget(0, 0, Application);

	// 从文件导入一个 VML 界面
	VVMLParser    Parser(L"./UI/LayoutUI.xml", VVMLParserParseMode::FromFile);

	// 解析 VML 后显示 VML 界面
	auto Result = VMLWidget->LoadVML(Parser.ParseVML());

	// 检查 VML 导入情况
	if (Result.Status != VVMLWidgetVMLLoadStats::Ok) {
		MessageBox(GetHWnd(), Result.FailedMessage.c_str(), L"We are Sorry that VML Loaded Error", MB_OK + 16);

		return -1;
	}

	VMLWidget->operator[](L"MainWindow")[L"BlackThemeButton"].Get<VPushButton>()->
		ButtonPushed.Connect(SetBlackTheme);
	VMLWidget->operator[](L"MainWindow")[L"WhiteThemeButton"].Get<VPushButton>()->
		ButtonPushed.Connect(SetWhiteTheme);

	DemoButton   = VMLWidget->operator[](L"MainWindow")[L"PushButtonDemo"].Get<VPushButton>();
	TextLabel    = VMLWidget->operator[](L"MainWindow")[L"PlaneTextDemo"].Get<VTextLabel>();
	CommentLabel = VMLWidget->operator[](L"MainWindow")[L"CommentText"].Get<VTextLabel>();

	return Application->Exec();
}