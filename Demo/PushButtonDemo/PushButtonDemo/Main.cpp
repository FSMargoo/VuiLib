/////////////////////////////////////////////////
//            PushButtonDemo
//    描述 : 这是一个 VPushButton 的实例 Demo
//           主要演示 VPushButton 的部分功能
#include "../../../Library/VUILib"

// 按钮被点击响应函数
void ButtonOnClicked() {
	// 可以使用 EasyX 的 GetHWnd() 直接获取 MainWindow 的窗口句柄
	MessageBox(GetHWnd(), L"按钮被点击！", L"按钮被点击！", MB_OK);
}

int main() {
	VApplication* PushButtonDemo = new VApplication();
	
	// 创建一个不可调整大小的 MainWindow
	VMainWindow*  MainWindow     = new VMainWindow(640, 480, PushButtonDemo, false);

	// 创建按钮
	VPushButton*  PushButton     = new VPushButton(MainWindow, L"PushButton 测试");

	// 按钮布局
	PushButton->Resize(180, 60);
	PushButton->Move(640 / 2 - 180 / 2, 480 / 2 - 60 / 2);

	// 信号链接
	PushButton->ButtonPushed.Connect(ButtonOnClicked);

	// 进入事件循环
	return PushButtonDemo->Exec();
}