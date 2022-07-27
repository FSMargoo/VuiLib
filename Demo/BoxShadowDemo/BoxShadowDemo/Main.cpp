/////////////////////////////////////////////////////////
//            BoxShaowDemo
//    ���� : ����һ�� VuiLib �е� BoxShadow ��ʽ�� Demo
#include "../../../Library/VUILib"

int main() {
	VApplication* Application = new VApplication();

	// ����һ�� VML ����
	VVMLWidget*  VMLWidget = new VVMLWidget(0, 0, Application);

	// ���ļ�����һ�� VML ����
	VVMLParser    Parser(L"./LayoutVML.xml", VVMLParserParseMode::FromFile);

	// ���� VML ����ʾ VML ����
	auto Result = VMLWidget->LoadVML(Parser.ParseVML());

	// ��� VML �������
	if (Result.Status != VVMLWidgetVMLLoadStats::Ok) {
		MessageBox(GetHWnd(), Result.FailedMessage.c_str(), L"We are Sorry that VML Loaded Error", MB_OK + 16);

		return -1;
	}

	return Application->Exec();
}