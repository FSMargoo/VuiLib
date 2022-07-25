/////////////////////////////////////////////////
//            VMLWidget
//    描述 : 这是一个使用 VML 和 VSS 构建的界面
//          主要实现一键换肤功能
#include "../../../Library/VUILib"

VVMLWidget* VMLWidget;

void SetBlackTheme() {
	VSSParser Parser(L"./UI/LayoutSurfaceBlack.css", VVVSParserParseMode::FromFile);

	VMLWidget->SetStyleSheet(Parser.ParseVSS());
}
void SetWhiteTheme() {
	VSSParser Parser(L"./UI/LayoutSurfaceWhite.css", VVVSParserParseMode::FromFile);

	VMLWidget->SetStyleSheet(Parser.ParseVSS());
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

	return Application->Exec();
}