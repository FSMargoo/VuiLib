////////////////////////////////////////////////////
//            AnimationDemo
// 介绍 : 这是 VUILib 中使用 VAnimation 相关类实现控件
//		 动画的实例

#include "../../../Library/VUILib"

VPushButton* AnimationPlay1;
VPushButton* AnimationPlay2;
VPushButton* AnimationPlay3;

VTextLabel*  AnimationObject;

VGeomteryAnimation* GeomteryAnimation;
VAlphaAnimation*    AlphaAnimation;
VPositionAnimation* PositionAnimation;

void PlayAnimation1() {
	AnimationObject->Move(400, 400);
	PositionAnimation->Start({ 280, 0 });
}
void PlayAnimation2() {
	AnimationObject->SetTransparency(0);
	AlphaAnimation->Start(255);
}
void PlayAnimation3() {
	AnimationObject->Resize(200, 400);
	GeomteryAnimation->Start({ 200, 20 });
}

int main() {
	VApplication* AnimationDemo = new VApplication();
	VMainWindow* MainWindow = new VMainWindow(640, 480, AnimationDemo);
	
	AnimationPlay1  = new VPushButton(MainWindow, L"播放动画（位移）");
	AnimationPlay2  = new VPushButton(MainWindow, L"播放动画（alpha 值）");
	AnimationPlay3  = new VPushButton(MainWindow, L"播放动画（几何大小）");
	AnimationObject = new VTextLabel(MainWindow, L"这是动画展示对象");

	GeomteryAnimation = new VGeomteryAnimation(AnimationObject, 1800, VInterpolatorType::AccelerateInterpolator);
	AlphaAnimation    = new VAlphaAnimation(AnimationObject, 800, VInterpolatorType::AccelerateInterpolator);
	PositionAnimation = new VPositionAnimation(AnimationObject, 800, VInterpolatorType::AccelerateInterpolator);

	AnimationPlay1->Resize(200, 40);
	AnimationPlay2->Resize(200, 40);
	AnimationPlay3->Resize(200, 40);

	AnimationPlay1->ButtonPushed.Connect(PlayAnimation1);
	AnimationPlay2->ButtonPushed.Connect(PlayAnimation2);
	AnimationPlay3->ButtonPushed.Connect(PlayAnimation3);

	AnimationObject->Resize(200, 20);
	AnimationObject->SetAlignment(VStringAlignment::AlignmentCenter);
	AnimationObject->SetLineAlignment(VStringAlignment::AlignmentCenter);

	AnimationObject->Move(280, 0);

	AnimationPlay2->Move(0, 60);
	AnimationPlay3->Move(0, 120);

	return AnimationDemo->Exec();
}