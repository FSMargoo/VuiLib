/////////////////////////////////////////////////
//            VMLAnimation
//    描述 : 这是一个演示通过 VML 实现控件动画的 Demo
//
#include "../../../Library/VUILib"

// 映射 VML 对象到 C++ 层
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
	// 加载 VML 到 VMLWidget 上
	VApplication* VMLAnimationDemo = new VApplication();
	VVMLWidget    VMLWidget(0, 0, VMLAnimationDemo);
	VVMLParser    VMLParser(L"./AnimationVML.xml", VVMLParserParseMode::FromFile);

	auto VMLParserResult = VMLWidget.LoadVML(VMLParser.ParseVML());

	if (VMLParserResult.Status != VVMLWidgetVMLLoadStats::Ok) {
		MessageBox(VMLWidget.GetWinID(), VMLParserResult.FailedMessage.c_str(), 
			L"Sorry but Something Error Happend", MB_OK + 16);

		return -1;
	}

	// 映射 VML 对象到 C++ 中
	PositionAnimationButton = VMLWidget[L"MainWindow"][L"PositionAnimationButton"].Get<VPushButton>();
	AlphaAnimationButton    = VMLWidget[L"MainWindow"][L"AlphaAnimationButton"].Get<VPushButton>();
	GeomteryAnimationButton = VMLWidget[L"MainWindow"][L"GeomteryAnimationButton"].Get<VPushButton>();

	PositionAnimation       = VMLWidget[L"MainWindow"][L"PositionAnimationButton"][L"Animation"].Get<VPositionAnimation>();
	AlphaAnimation          = VMLWidget[L"MainWindow"][L"AlphaAnimationButton"][L"Animation"].Get<VAlphaAnimation>();
	GeomteryAnimation       = VMLWidget[L"MainWindow"][L"GeomteryAnimationButton"][L"Animation"].Get<VGeomteryAnimation>();

	// 绑定信号槽
	PositionAnimationButton->ButtonPushed.Connect(PlayPositionAnimation);
	AlphaAnimationButton->ButtonPushed.Connect(PlayAlphaAnimation);
	GeomteryAnimationButton->ButtonPushed.Connect(PlayGeomteryAnimation);

	return VMLAnimationDemo->Exec();
}