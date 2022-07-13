#pragma once

#include "../../../Library/VUIlib"

#include <comutil.h>

#include <map>
#include <io.h>

#ifdef _DEBUG
#	pragma comment(lib, "comsuppwd.lib")
#else
#	pragma comment(lib, "comsuppw.lib")
#endif

#include "PVSoftware.hpp"

enum class PVLocalUISurface {
	StartupUI, MainUI
};

class PVImageLabel : public VImageLabel {
public:
	VSignal<> DragStart;
	VSignal<> DragEnd;

public:
	PVImageLabel(VImage* Image, VUIObject* Parent)
		: VImageLabel(Image, Parent) {
	}

	void MouseLeftClicked(VMouseClickedFlag ClickedFlag) override {
		if (ClickedFlag == VMouseClickedFlag::Down) {
			DragStart.Emit();
		}
		else {
			DragEnd.Emit();
		}
	}
};

class PVMainWindow : public VMainWindow {
public:
	static std::wstring CodeConvert(const char* String) {
		_bstr_t Temp(String);

		return Temp.operator const wchar_t* ();
	}
	static std::string  CodeConvert(const wchar_t* String) {
		_bstr_t Temp(String);

		return Temp.operator const char* ();
	}

private:
	void InitWorkingPath() {
		wchar_t LocalPath[MAX_PATH + 1] = { 0 };
		GetModuleFileName(NULL, LocalPath, MAX_PATH);

		std::wstring PathString = LocalPath;

		PathString = PathString.substr(0, PathString.find_last_of(L"\\"));

		_wchdir(PathString.c_str());
	}

private:
	VControlGroup StartupSurface;
	VControlGroup MainSurface;

	VImage*       InViewImage;
	VImage*       ZoomedImage = nullptr;

	int           LocalContainerPosition = 0;

	double        ZoomedSize             = 0;

	std::vector<std::wstring> PicturesContainer;

	bool          InDrag                 = false;

private:
	void MouseDragStart() {
		InDrag = true;

		ImageOffsetPoint = { 0, 0 };
	}
	void MouseDragEnd() {
		InDrag        = false;
		FirstTimeDrag = true;
	}

private:
	void ZoomImage(int Width, int Height) {
		if (ZoomedImage != nullptr &&
			ZoomedImage != InViewImage) {
			delete ZoomedImage;

			ZoomedImage = nullptr;
		}

		ZoomedImage = new VImage(Width, Height);

		VPainterDevice Painter(ZoomedImage);

		Painter.DrawImage(InViewImage, { 0, 0, Width, Height });
	}

private:
	std::wstring PictureFilePath;
	std::wstring PictureFileName;
	std::wstring PictureLocalPath;

private:
	PVLocalUISurface LocalSurface;

private:
	void ScanPicturePath(std::wstring Path, std::vector<std::wstring>& Files) {
		intptr_t    FileHandle = NULL;
		_finddata_t FileInfo;

		std::string  FindPath;
		std::wstring TempPath;
		FindPath  = FindPath.assign(CodeConvert(Path.c_str())).append("*");

		if ((FileHandle = _findfirst(FindPath.c_str(), &FileInfo)) != -1) {
			do {
				if (!(FileInfo.attrib & _A_SUBDIR)) {
					TempPath = Path + CodeConvert(FileInfo.name);

					if (TempPath.find(L".jpg")  == TempPath.size() - 4 ||
						TempPath.find(L".png")  == TempPath.size() - 4 ||
						TempPath.find(L".gif")  == TempPath.size() - 4 ||
						TempPath.find(L".jpge") == TempPath.size() - 5 ||
						TempPath.find(L".jpg")  == TempPath.size() - 4) {
						Files.push_back(TempPath);
					}
				}
			} while (_findnext(FileHandle, &FileInfo) == 0);
		}
	}

	std::wstring GetPercentString(double Percent) {
		if (Percent >= 1) {
			return std::to_wstring(Percent * 100).substr(0, 3) + L"%";
		}
		if (Percent <= 1 && Percent >= 0.1) {
			return std::to_wstring(Percent * 100).substr(0, 2) + L"%";
		}
		if (Percent <= 0.1) {
			return std::to_wstring(Percent * 100).substr(0, 1) + L"%";
		}

		return L"Error%";
	}

private:
	VTextLabel*  SoftwareTextLabel;
	VTextLabel*  SoftwareWelcomeText;
	VPushButton* OpenPictureFile;

	void ConfigSoftwareStartupUI() {
		SoftwareTextLabel->Move(0, -1);
		SoftwareTextLabel->Resize(GetWidth(), 83);
		SoftwareWelcomeText->Resize(GetWidth(), 47);

		SoftwareWelcomeText->Move(0, GetHeight() / 2 - SoftwareWelcomeText->GetHeight() / 2 - 47);

		OpenPictureFile->Resize(203, 52);
		OpenPictureFile->Move(GetWidth() / 2 - OpenPictureFile->GetWidth() / 2,
			GetHeight() / 2 - OpenPictureFile->GetHeight() / 2 + 20);
	}
	void InitSoftwareStartupUI() {
		LocalSurface = PVLocalUISurface::StartupUI;

		SoftwareTextLabel   = new VTextLabel(this, PVVersionTitle);
		SoftwareWelcomeText = new VTextLabel(this, L"欢迎来到 PhotoViewer");
		OpenPictureFile     = new VPushButton(this, L"打开您的图片文件");

		SoftwareTextLabel->SetAlignment(VStringAlignment::AlignmentCenter);
		SoftwareTextLabel->SetLineAlignment(VStringAlignment::AlignmentCenter);
		SoftwareTextLabel->SetTextSize(20);
		SoftwareTextLabel->SetBackgroundColor(VColor(68, 68, 68));

		SoftwareWelcomeText->SetAlignment(VStringAlignment::AlignmentCenter);
		SoftwareWelcomeText->SetLineAlignment(VStringAlignment::AlignmentCenter);
		SoftwareWelcomeText->SetTextSize(34);
		SoftwareWelcomeText->SetTextColor(VColor(255, 255, 255));

		ConfigSoftwareStartupUI();

		OpenPictureFile->ButtonPushed.Connect(this, &PVMainWindow::OpenPictureButtonOnClicked);

		StartupSurface.AddObject(SoftwareTextLabel);
		StartupSurface.AddObject(SoftwareWelcomeText);
		StartupSurface.AddObject(OpenPictureFile);
	}

	void InitPicture() {
		InViewImage = new VImage(PictureFilePath);

		if (InViewImage->GetWidth() > GetWidth() ||
			InViewImage->GetHeight() > GetHeight()) {
			if (InViewImage->GetWidth() > GetWidth()) {
				ZoomedSize = double(GetWidth()) / InViewImage->GetWidth();
			}
			else {
				ZoomedSize = double(GetHeight()) / InViewImage->GetHeight();
			}

			ImageViewLabel->Resize(InViewImage->GetWidth() * ZoomedSize, InViewImage->GetHeight() * ZoomedSize);

			ZoomImage(ImageViewLabel->GetWidth(), ImageViewLabel->GetHeight());

			ZoomPercentText->SetPlaneText(GetPercentString(ZoomedSize));
		}
		else {
			ZoomedImage = InViewImage;

			ZoomedSize = 1.f;

			ImageViewLabel->Resize(InViewImage->GetWidth(), InViewImage->GetHeight());

			ZoomPercentText->SetPlaneText(L"100%");
		}
	}

	void OpenPictureButtonOnClicked() {
		if (OpenFileSelector() == true) {
			StartupSurface.Hide();
			MainSurface.Show();

			LocalSurface = PVLocalUISurface::MainUI;

			InViewImage = new VImage(PictureFilePath);

			InitPicture();

			ConfigMainUI();
			PlayAnimation();
		}
	}

	bool OpenFileSelector() {
		OPENFILENAME* FileSelector = new OPENFILENAME{ 0 };
		TCHAR         FilePath[MAX_PATH];

		FileSelector->lStructSize = sizeof(OPENFILENAME);

		FileSelector->hwndOwner = GetHWnd();
		FileSelector->lpstrFile = FilePath;
		FileSelector->lpstrFile[0] = '\0';

		FileSelector->nMaxFile = sizeof(FilePath);

		FileSelector->lpstrFilter = L"All(*.*)\0*.*\0";
		FileSelector->nFilterIndex = 1;
		FileSelector->lpstrFileTitle = NULL;
		FileSelector->nMaxFileTitle = 0;
		FileSelector->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(FileSelector)) {
			PictureFilePath = FilePath;

			int SliptPosition = static_cast<int>(PictureFilePath.find_last_of(L"\\")) + 1;

			PictureFileName  = PictureFilePath.substr(SliptPosition, PictureFilePath.size() - SliptPosition);
			PictureLocalPath = PictureFilePath.substr(0, SliptPosition);

			ScanPicturePath(PictureLocalPath, PicturesContainer);

			for (auto& Path : PicturesContainer) {
				if (Path == PictureFilePath) {
					break;
				}

				++LocalContainerPosition;
			}

			return true;
		}

		return false;
	}

private:
	VPoint MouseDragPoint;
	VPoint ImageOffsetPoint;

	bool   FirstTimeDrag = true;

	time_t DealyClock = 0;

private:
	void DealyMessage(VMessage* Message) override {
		if (clock() - DealyClock >= 100) {
			DealyClock = clock();

			if (Message->GetType() == VMessageType::KeyClickedMessage) {
				VKeyClickedMessage* KeyMessage = static_cast<VKeyClickedMessage*>(Message);

				if (KeyMessage->KeyPrevDown == true) {
					if (KeyMessage->KeyVKCode == VK_LEFT &&
						LocalContainerPosition - 1 >= 0) {
						if (ZoomedImage == InViewImage) {
							ZoomedImage = nullptr;
						}

						delete InViewImage;
						InViewImage = nullptr;

						if (ZoomedImage != nullptr) {
							delete ZoomedImage;
							ZoomedImage = nullptr;
						}

						--LocalContainerPosition;

						PictureFilePath = PicturesContainer[LocalContainerPosition];

						int SliptPosition = static_cast<int>(PictureFilePath.find_last_of(L"\\")) + 1;

						PictureFileName = PictureFilePath.substr(SliptPosition, PictureFilePath.size() - SliptPosition);

						InitPicture();
						ConfigMainUI();
					}
					if (KeyMessage->KeyVKCode == VK_RIGHT &&
						LocalContainerPosition + 1 < PicturesContainer.size()) {
						if (ZoomedImage == InViewImage) {
							ZoomedImage = nullptr;
						}

						delete InViewImage;
						InViewImage = nullptr;

						if (ZoomedImage != nullptr) {
							delete ZoomedImage;
							ZoomedImage = nullptr;
						}

						++LocalContainerPosition;

						PictureFilePath = PicturesContainer[LocalContainerPosition];

						int SliptPosition = static_cast<int>(PictureFilePath.find_last_of(L"\\")) + 1;

						PictureFileName = PictureFilePath.substr(SliptPosition, PictureFilePath.size() - SliptPosition);

						InitPicture();
						ConfigMainUI();
					}
				}
			}
			if (Message->GetType() == VMessageType::MouseMoveMessage) {
				if (InDrag == true) {
					VMouseMoveMessage* MouseMessage = static_cast<VMouseMoveMessage*>(Message);

					if (FirstTimeDrag == true) {
						MouseDragPoint = MouseMessage->MousePosition;

						FirstTimeDrag = false;
					}
					else {
						ImageOffsetPoint.x = -(MouseDragPoint.x - MouseMessage->MousePosition.x);
						ImageOffsetPoint.y = -(MouseDragPoint.y - MouseMessage->MousePosition.y);

						ConfigMainUI();
					}
				}
			}
			if (Message->GetType() == VMessageType::MouseWheelMessage) {
				VMouseWheelMessage* WheelMessage = static_cast<VMouseWheelMessage*>(Message);

				if (WheelMessage->WheelValue > 0) {
					ZoomUp();
				}
				else {
					ZoomDown();
				}
			}
		}
	}

private:
	void ZoomUp() {
		if (ZoomedSize + 0.2 <= 8) {
			ZoomedSize += 0.2;
		}

		ImageViewLabel->Resize(InViewImage->GetWidth() * ZoomedSize, InViewImage->GetHeight() * ZoomedSize);

		ZoomImage(ImageViewLabel->GetWidth(), ImageViewLabel->GetHeight());

		ZoomPercentText->SetPlaneText(GetPercentString(ZoomedSize));

		ConfigMainUI();
	}
	void ZoomDown() {
		if (ZoomedSize - 0.2 >= 0.05) {
			ZoomedSize -= 0.2;
		}

		ImageViewLabel->Resize(InViewImage->GetWidth() * ZoomedSize, InViewImage->GetHeight() * ZoomedSize);

		ZoomImage(ImageViewLabel->GetWidth(), ImageViewLabel->GetHeight());

		ZoomPercentText->SetPlaneText(GetPercentString(ZoomedSize));

		ConfigMainUI();
	}
	void ZoomReset() {
		ImageOffsetPoint = { 0, 0 };

		ZoomedSize = 1;

		ImageViewLabel->Resize(InViewImage->GetWidth() * ZoomedSize, InViewImage->GetHeight() * ZoomedSize);

		ZoomImage(ImageViewLabel->GetWidth(), ImageViewLabel->GetHeight());

		ZoomPercentText->SetPlaneText(GetPercentString(ZoomedSize));

		ConfigMainUI();
	}

private:
	VTextLabel*   ImageFileName;
	VTextLabel*   ZoomPercentText;
	PVImageLabel* ImageViewLabel;
	
	VIconButton*  ZoomUpButton;
	VIconButton*  ZoomDownButton;
	VIconButton*  ZoomResetButton;

	VAlphaAnimation* ZoomUpButtonAlpha;
	VAlphaAnimation* ZoomDownButtonAlpha;
	VAlphaAnimation* ZoomResetButtonAlpha;
	VPositionAnimation* ZoomUpButtonPosition;
	VPositionAnimation* ZoomDownButtonPosition;
	VPositionAnimation* ZoomResetButtonPosition;

	void PlayAnimation() {
		ZoomUpButton->SetTransparency(0);
		ZoomDownButton->SetTransparency(0);
		ZoomResetButton->SetTransparency(0);

		ZoomUpButton->Move(ZoomUpButton->GetX(), ZoomUpButton->GetY() - 40);
		ZoomDownButton->Move(ZoomDownButton->GetX(), ZoomDownButton->GetY() - 40);
		ZoomResetButton->Move(ZoomResetButton->GetX(), ZoomResetButton->GetY() - 40);

		ZoomUpButtonAlpha->Start(255);
		ZoomDownButtonAlpha->Start(255);
		ZoomResetButtonAlpha->Start(255);

		ZoomUpButtonPosition->Start({ ZoomUpButton->GetX(), ZoomUpButton->GetY() + 40 });
		ZoomDownButtonPosition->Start({ ZoomDownButton->GetX(), ZoomDownButton->GetY() + 40 });
		ZoomResetButtonPosition->Start({ ZoomResetButton->GetX(), ZoomResetButton->GetY() + 40 });
	}
	void ConfigMainUI() {
		ImageFileName->SetPlaneText(PictureFileName);
		ImageFileName->Resize(873, 83);
		ImageFileName->Move(GetWidth() / 2 - ImageFileName->GetWidth() / 2, 38);

		ZoomUpButton->Move(ImageFileName->GetX() + 58, 61);
		ZoomDownButton->Move(ImageFileName->GetX() + 123, 61);
		ZoomResetButton->Move(ImageFileName->GetX() + 188, 61);

		ImageViewLabel->SetImage(ZoomedImage);

		ZoomPercentText->Move(ImageFileName->GetX() + ImageFileName->GetWidth() - 128,
			ImageFileName->GetY() + (ImageFileName->GetHeight() / 2 - ZoomPercentText->GetHeight() / 2));

		ImageViewLabel->Move(GetWidth() / 2 - ImageViewLabel->GetWidth() / 2 + ImageOffsetPoint.x,
			GetHeight() / 2 - ImageViewLabel->GetHeight() / 2 + ImageOffsetPoint.y);
	}
	void InitMainUI() {
		ImageViewLabel  = new PVImageLabel(nullptr, this);
		ImageFileName   = new VTextLabel(this, PictureFileName);
		ZoomPercentText = new VTextLabel(this, L"Error");

		ZoomUpButton    = new VIconButton(this);
		ZoomDownButton  = new VIconButton(this);
		ZoomResetButton = new VIconButton(this);

		ZoomUpButtonAlpha    = new VAlphaAnimation(ZoomUpButton, 300, VInterpolatorType::OvershootInterpolator);
		ZoomDownButtonAlpha  = new VAlphaAnimation(ZoomDownButton, 300, VInterpolatorType::OvershootInterpolator);
		ZoomResetButtonAlpha = new VAlphaAnimation(ZoomResetButton, 300, VInterpolatorType::OvershootInterpolator);

		ZoomUpButtonPosition = new VPositionAnimation(ZoomUpButton, 300, VInterpolatorType::OvershootInterpolator);
		ZoomDownButtonPosition = new VPositionAnimation(ZoomDownButton, 300, VInterpolatorType::OvershootInterpolator);
		ZoomResetButtonPosition = new VPositionAnimation(ZoomResetButton, 300, VInterpolatorType::OvershootInterpolator);

		ZoomUpButton->ButtonPushed.Connect(this, &PVMainWindow::ZoomUp);
		ZoomDownButton->ButtonPushed.Connect(this, &PVMainWindow::ZoomDown);
		ZoomResetButton->ButtonPushed.Connect(this, &PVMainWindow::ZoomReset);

		ImageViewLabel->DragStart.Connect(this, &PVMainWindow::MouseDragStart);
		ImageViewLabel->DragEnd.Connect(this, &PVMainWindow::MouseDragEnd);

		VImage* ZoomUpIcon    = new VImage(L"./pv/ZoomUp.png");
		VImage* ZoomDownIcon  = new VImage(L"./pv/ZoomDown.png");
		VImage* ZoomResetIcon = new VImage(L"./pv/ZoomReset.png");

		ZoomUpButton->Resize(45, 42);
		ZoomUpButton->SetIcon(ZoomUpIcon);
		
		ZoomDownButton->Resize(45, 42);
		ZoomDownButton->SetIcon(ZoomDownIcon);

		ZoomResetButton->Resize(45, 42);
		ZoomResetButton->SetIcon(ZoomResetIcon);

		ImageFileName->SetBackgroundColor(VColor(57, 57, 57, 153));
		ImageFileName->SetRadius({ 12, 12 });
		ImageFileName->SetTextColor(VColor(255, 255, 255));
		ImageFileName->SetTextSize(20);

		ZoomPercentText->SetTextSize(18);
		ZoomPercentText->SetTextColor(VColor(255, 255, 255));
		ZoomPercentText->Resize(100, 18);
		ZoomPercentText->SetAlignment(VStringAlignment::AlignmentFar);

		MainSurface.AddObject(ImageFileName);
		MainSurface.AddObject(ImageViewLabel);
		MainSurface.AddObject(ZoomPercentText);
		MainSurface.AddObject(ZoomUpButton);
		MainSurface.AddObject(ZoomDownButton);
		MainSurface.AddObject(ZoomResetButton);

		MainSurface.Hide();
	}

private:
	void WindowSized(int, int) {
		if (LocalSurface == PVLocalUISurface::StartupUI) {
			ConfigSoftwareStartupUI();
		}
		else {
			ConfigMainUI();
		}
	}

public:
	PVMainWindow(int Width, int Height, VApplication* Parent)
		: VMainWindow(Width, Height, Parent) {
		SetBackgroundColor(VColor(41, 41, 41));

		SizeOnChange.Connect(this, &PVMainWindow::WindowSized);

		InitMainUI();
		InitSoftwareStartupUI();
	}
	PVMainWindow(int Width, int Height, VApplication* Parent, std::wstring FilePath)
		: VMainWindow(Width, Height, Parent) {
		InitWorkingPath();

		SetBackgroundColor(VColor(41, 41, 41));

		SizeOnChange.Connect(this, &PVMainWindow::WindowSized);

		InitMainUI();
		InitSoftwareStartupUI();

		PictureFilePath = FilePath;

		int SliptPosition = static_cast<int>(PictureFilePath.find_last_of(L"\\")) + 1;

		PictureFileName = PictureFilePath.substr(SliptPosition, PictureFilePath.size() - SliptPosition);
		PictureLocalPath = PictureFilePath.substr(0, SliptPosition);

		ScanPicturePath(PictureLocalPath, PicturesContainer);

		for (auto& Path : PicturesContainer) {
			if (Path == PictureFilePath) {
				break;
			}

			++LocalContainerPosition;
		}

		StartupSurface.Hide();
		MainSurface.Show();

		LocalSurface = PVLocalUISurface::MainUI;

		InViewImage = new VImage(PictureFilePath);

		InitPicture();

		ConfigMainUI();
	}
};
