/////////////////////////////////////////////////
//            VMLWidget
//    ���� : ����һ��ʹ�� VML �� VSS �����Ľ���
//          ��Ҫʵ��һ����������
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

	// ����һ�� VML ����
	              VMLWidget = new VVMLWidget(0, 0, Application);

	// ���ļ�����һ�� VML ����
	VVMLParser    Parser(L"./UI/LayoutUI.xml", VVMLParserParseMode::FromFile);

	// ���� VML ����ʾ VML ����
	auto Result = VMLWidget->LoadVML(Parser.ParseVML());

	// ��� VML �������
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