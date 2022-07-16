/////////////////////////////////////////////////
//        LayoutDemo
// 描述 : 此 Demo 演示 VLib 中的 VLayout 控件实现自动
//		 布局的功能
//
#include "../../../Library/VUILib"

class LayoutWidget : public VMainWindow {
private:
	// 控件
	VPushButton* PushButton;
	VTextLabel*  TextLabel;

	VLayout*     PushButtonLayout;
	VLayout*     TextLabelLayout;

private:
	// 初始化 UI
	void InitUI() {
		PushButton = new VPushButton(this, L"Layout 按钮");
		TextLabel  = new VTextLabel(this, L"欢迎使用 VUILib");

		PushButton->Resize(140, 40);
		TextLabel->Resize(140, 20);
		TextLabel->SetAlignment(VStringAlignment::AlignmentCenter);
	}

public:
	LayoutWidget(VApplication* App, int Width, int Height) : VMainWindow(Width, Height, App, true) {
		InitUI();

		// 初始化 Layout 布局控件
		PushButtonLayout = new VLayout(PushButton, this);
		TextLabelLayout  = new VLayout(TextLabel, this);

		PushButtonLayout->SetVerticalLayoutMode(VLayoutMode::LayoutModePercent);
		PushButtonLayout->SetVerticalLayoutPercent(0.6);

		TextLabelLayout->SetVerticalLayoutMode(VLayoutMode::LayoutModePercent);
		TextLabelLayout->SetVerticalLayoutPercent(0.5);
	}
};

int main() {
	VApplication* LayoutDemo = new VApplication();
	LayoutWidget* Layout     = new LayoutWidget(LayoutDemo, 400, 400);

	return LayoutDemo->Exec();
}