/////////////////////////////////////////////////
//            VMLWidget
//    ���� : ����һ��ʹ�� VML �� VSS �����Ľ���
//          ��Ҫʵ��һ����������
#include "../../../Library/VUILib"

VVMLWidget* VMLWidget;

VPushButton* DemoButton;
VTextLabel*  TextLabel;
VTextLabel*  CommentLabel;

void SetBlackTheme() {
	VSSParser Parser(L"./UI/LayoutSurfaceBlack.css", VVVSParserParseMode::FromFile);

	VMLWidget->SetStyleSheet(Parser.ParseVSS());

	DemoButton->SetPlaneText(L"��ɫ����İ�ť");
	TextLabel->SetPlaneText(L"��ɫ������ı���");
	CommentLabel->SetPlaneText(L"�� ��ɫ����������");
}
void SetWhiteTheme() {
	VSSParser Parser(L"./UI/LayoutSurfaceWhite.css", VVVSParserParseMode::FromFile);

	VMLWidget->SetStyleSheet(Parser.ParseVSS());

	DemoButton->SetPlaneText(L"��ɫ����İ�ť");
	TextLabel->SetPlaneText(L"��ɫ������ı���");
	CommentLabel->SetPlaneText(L"�� ��ɫ����������");
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

	DemoButton   = VMLWidget->operator[](L"MainWindow")[L"PushButtonDemo"].Get<VPushButton>();
	TextLabel    = VMLWidget->operator[](L"MainWindow")[L"PlaneTextDemo"].Get<VTextLabel>();
	CommentLabel = VMLWidget->operator[](L"MainWindow")[L"CommentText"].Get<VTextLabel>();

	return Application->Exec();
}