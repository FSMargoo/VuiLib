/////////////////////////////////////////////////
//            VMLAnimation
//    ���� : ����һ����ʾͨ�� VML ʵ�ֿؼ������� Demo
//
#include "../../../Library/VUILib"

// ӳ�� VML ���� C++ ��
VPushButton*        PositionAnimationButton;
VPushButton*        AlphaAnimationButton;
VPushButton*        GeomteryAnimationButton;
VPositionAnimation* PositionAnimation;
VAlphaAnimation*    AlphaAnimation;
VGeomteryAnimation* GeomteryAnimation;

void PlayPositionAnimation() {
	PositionAnimationButton->Move(80, 80);
	PositionAnimation->Start();
}
void PlayGeomteryAnimation() {
	GeomteryAnimationButton->Resize(0, 0);
	GeomteryAnimation->Start();
}
void PlayAlphaAnimation() {
	AlphaAnimationButton->SetTransparency(0);
	AlphaAnimation->Start();
}

int main() {
	// ���� VML �� VMLWidget ��
	VApplication* VMLAnimationDemo = new VApplication();
	VVMLWidget    VMLWidget(0, 0, VMLAnimationDemo);
	VVMLParser    VMLParser(L"./AnimationVML.xml", VVMLParserParseMode::FromFile);

	auto VMLParserResult = VMLWidget.LoadVML(VMLParser.ParseVML());

	if (VMLParserResult.Status != VVMLWidgetVMLLoadStats::Ok) {
		MessageBox(VMLWidget.GetWinID(), VMLParserResult.FailedMessage.c_str(), 
			L"Sorry but Something Error Happend", MB_OK + 16);

		return -1;
	}

	// ӳ�� VML ���� C++ ��
	PositionAnimationButton = VMLWidget[L"MainWindow"][L"PositionAnimationButton"].Get<VPushButton>();
	AlphaAnimationButton    = VMLWidget[L"MainWindow"][L"AlphaAnimationButton"].Get<VPushButton>();
	GeomteryAnimationButton = VMLWidget[L"MainWindow"][L"GeomteryAnimationButton"].Get<VPushButton>();

	PositionAnimation       = VMLWidget[L"MainWindow"][L"PositionAnimationButton"][L"Animation"].Get<VPositionAnimation>();
	AlphaAnimation          = VMLWidget[L"MainWindow"][L"AlphaAnimationButton"][L"Animation"].Get<VAlphaAnimation>();
	GeomteryAnimation       = VMLWidget[L"MainWindow"][L"GeomteryAnimationButton"][L"Animation"].Get<VGeomteryAnimation>();

	// ���źŲ�
	PositionAnimationButton->ButtonPushed.Connect(PlayPositionAnimation);
	AlphaAnimationButton->ButtonPushed.Connect(PlayAlphaAnimation);
	GeomteryAnimationButton->ButtonPushed.Connect(PlayGeomteryAnimation);

	return VMLAnimationDemo->Exec();
}