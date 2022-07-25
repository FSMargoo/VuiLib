/////////////////////////////////////////////////
//            VMLWidget
//    描述 : 这是一个使用 VML 和 VSS 构建的界面
//          主要实现一键换肤功能
#include "../../../Library/VUILib"

VVMLWidget*  VMLWidget;

VImageLabel*     BlurImage;
VAlphaAnimation* AlphaAnimation;

VViewLabel*      AnimationMessageBox;
VAlphaAnimation* MessageBoxAnimation;

void StartAnimation() {
	BlurImage->SetTransparency(0);
	AnimationMessageBox->SetTransparency(0);

	AlphaAnimation->Start();
	MessageBoxAnimation->Start();
}

int main() {
	VApplication* Application = new VApplication();

	// 创建一个 VML 界面
	VMLWidget = new VVMLWidget(0, 0, Application);

	// 从文件导入一个 VML 界面
	VVMLParser    Parser(L"./UISurface.xml", VVMLParserParseMode::FromFile);

	// 解析 VML 后显示 VML 界面
	auto Result = VMLWidget->LoadVML(Parser.ParseVML());

	// 检查 VML 导入情况
	if (Result.Status != VVMLWidgetVMLLoadStats::Ok) {
		MessageBox(GetHWnd(), Result.FailedMessage.c_str(), L"We are Sorry that VML Loaded Error", MB_OK + 16);

		return -1;
	}

	BlurImage           = VMLWidget->operator[](L"MainWindow")[L"BlurImage"].Get<VImageLabel>();
	AlphaAnimation      = VMLWidget->operator[](L"MainWindow")[L"BlurImage"][L"AlphaAnimation"].Get<VAlphaAnimation>();
	MessageBoxAnimation = VMLWidget->operator[](L"MainWindow")[L"MessageViewlabel"][L"AlphaAnimation"].Get<VAlphaAnimation>();
	AnimationMessageBox = VMLWidget->operator[](L"MainWindow")[L"MessageViewlabel"].Get<VViewLabel>();

	VMLWidget->operator[](L"MainWindow")[L"MessageViewlabel"][L"ContinueButton"].Get<VPushButton>()->ButtonPushed.Connect(StartAnimation);
	VMLWidget->operator[](L"MainWindow")[L"MessageViewlabel"][L"ExitButton"].Get<VPushButton>()->ButtonPushed.Connect([]() -> void { exit(0); });

	VSmartTimer* AnimationClock = new VSmartTimer(Application);

	AnimationClock->Start(500);
	AnimationClock->TimerOnTime.Connect(StartAnimation);

	return Application->Exec();
}