/////////////////////////////////////////////////
//        LayoutDemo
// 描述 : 这是利用 VMainWindow 的 Size 信号
//		 来实现 layout 自动布局的 Demo 演示
//
#include "../../../Library/VUILib"

class LayoutWidget : public VMainWindow {
private:
	// 控件
	VPushButton* PushButton;
	VTextLabel*  TextLabel;

private:
	// 初始化 UI
	void InitUI() {
		PushButton = new VPushButton(this, L"Layout 按钮");
		TextLabel  = new VTextLabel(this, L"欢迎使用 VUILib");

		PushButton->Resize(140, 40);
		TextLabel->Resize(140, 20);
		TextLabel->SetAlignment(VStringAlignment::AlignmentCenter);
	}

	// Layout 布局
	void Layout(int, int) {
		PushButton->Move(GetWidth() / 2 - PushButton->GetWidth() / 2, GetHeight() / 2 - PushButton->GetHeight() / 2 + 80);
		TextLabel->Move(GetWidth() / 2 - TextLabel->GetWidth() / 2, GetHeight() / 2 - TextLabel->GetHeight() / 2 - 40);
	}

public:
	LayoutWidget(VApplication* App, int Width, int Height) : VMainWindow(Width, Height, App, true) {
		InitUI();
		Layout(0, 0);

		// 绑定信号，实现自动布局
		SizeOnChange.Connect(this, &LayoutWidget::Layout);
	}
};

int main() {
	VApplication* LayoutDemo = new VApplication();
	LayoutWidget* Layout     = new LayoutWidget(LayoutDemo, 400, 400);

	return LayoutDemo->Exec();
}