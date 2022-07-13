////////////////////////////////////////////
//           ImageLabelDemo
//  描述 : 该 Demo 主要展示 VImageLabel 控件
//         的部分功能
#include "../../../Library/VUILib"

int main() {
	VApplication* ImageLabelDemo = new VApplication();
	VMainWindow*  MainWindow     = new VMainWindow(680, 480, ImageLabelDemo);

	VImage        SrcImage(L"./Src/Image.png");
	VImageLabel*  ImageLabel     = new VImageLabel(&SrcImage, MainWindow);
	VTextLabel*   Description    = new VTextLabel(MainWindow, L"古镇");

	Description->Resize(120, 40);
	Description->SetTextColor(VColor(255, 255, 255));
	Description->SetTextSize(20);
	Description->SetLineAlignment(VStringAlignment::AlignmentNear);
	Description->SetAlignment(VStringAlignment::AlignmentNear);

	ImageLabel->Resize(SrcImage.GetWidth(), SrcImage.GetHeight());

	return ImageLabelDemo->Exec();
}