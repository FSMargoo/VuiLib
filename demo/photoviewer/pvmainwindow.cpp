#include <Shlobj.h>

#include "pvmainwindow.h"

VDragbleImageLabel::VDragbleImageLabel(Core::VUIObject* Parent, Core::VImage* Image) : Core::VDragControlBase(Parent) {
	ViewImage = Image;
}
void VDragbleImageLabel::OnPaint(Core::VCanvasPainter* Painter) {
	Painter->BeginDraw();
	
	Painter->DrawImage(Core::VRect {
							   0, 0,
							   static_cast<int>(GetRegion().GetWidth()),
							   static_cast<int>(GetRegion().GetHeight()),
		},
		ViewImage,
		Core::VRect{
				0, 0,
				static_cast<int>(ViewImage->GetDirectXObject()->GetSize().width),
				static_cast<int>(ViewImage->GetDirectXObject()->GetSize().height),
		},
		1.f
		);

	Painter->EndDraw();
}
void VDragbleImageLabel::SetImage(Core::VImage* Image) {
	ViewImage = Image;

	Update();
}

void PVMainWindow::ZoomUpButtonOnClicked() {
	if (ViewingProject->LocalScalePercent - 0.1f >= 0.f) {
		ViewingProject->LocalScalePercent -= 0.1f;

		PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
		ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);

		WindowOnResize(0, 0);
	}
}
void PVMainWindow::ZoomDownButtonOnClicked() {
	if (ViewingProject->LocalScalePercent + 0.1f <= 20.f) {
		ViewingProject->LocalScalePercent += 0.1f;

		PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
		ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);

		WindowOnResize(0, 0);
	}
}
void PVMainWindow::OneToOneButtonOnClicked() {
	ViewingProject->LocalScalePercent = 1.f;

	PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
	ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);

	WindowOnResize(0, 0);
}

void PVMainWindow::OnMessage(Core::VMessage* Message) {
	if (Message->GetType() == Core::VMessageType::KeyClickedMessage) {
		auto KeyMessage = static_cast<Core::VKeyClickedMessage*>(Message);

		if (KeyMessage->KeyStats == Core::VkeyClickedFlag::Down) {
			if (KeyMessage->KeyVKCode == VK_LEFT) {
				if (ViewingProject->LocalViewingFile - 1 >= 0) {
					--ViewingProject->LocalViewingFile;

					ViewingImage = new Core::VImage(ViewingProject->FileList[ViewingProject->LocalViewingFile],
						CallWidgetGetRenderHandle());

					BlurBackgroundImage->SetImage(ViewingImage);
					ViewingLabel->SetImage(ViewingImage);

					if (ViewingImage->GetWidth() > GetWidth() ||
						ViewingImage->GetHeight() > GetHeight()) {
						if (ViewingImage->GetWidth() > GetWidth()) {
							ViewingProject->LocalScalePercent = double(GetWidth()) / ViewingImage->GetWidth();
						}
						else {
							ViewingProject->LocalScalePercent = double(GetHeight()) / ViewingImage->GetHeight();
						}

						PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
						ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);
					}
					else {
						PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
						ViewingLabel->Resize(ViewingImage->GetWidth(), ViewingImage->GetHeight());
					}

					TitleText->SetPlaneText(GetFileName(ViewingProject->FileList[ViewingProject->LocalViewingFile]));
				}
			}
			if (KeyMessage->KeyVKCode == VK_RIGHT) {
				if (ViewingProject->LocalViewingFile + 1 < ViewingProject->FileList.size()) {
					++ViewingProject->LocalViewingFile;

					ViewingImage = new Core::VImage(ViewingProject->FileList[ViewingProject->LocalViewingFile],
						CallWidgetGetRenderHandle());

					BlurBackgroundImage->SetImage(ViewingImage);
					ViewingLabel->SetImage(ViewingImage);

					if (ViewingImage->GetWidth() > GetWidth() ||
						ViewingImage->GetHeight() > GetHeight()) {
						if (ViewingImage->GetWidth() > GetWidth()) {
							ViewingProject->LocalScalePercent = double(GetWidth()) / ViewingImage->GetWidth();
						}
						else {
							ViewingProject->LocalScalePercent = double(GetHeight()) / ViewingImage->GetHeight();
						}

						PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
						ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);
					}
					else {
						PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
						ViewingLabel->Resize(ViewingImage->GetWidth(), ViewingImage->GetHeight());
					}

					TitleText->SetPlaneText(GetFileName(ViewingProject->FileList[ViewingProject->LocalViewingFile]));
				}
			}
		}
	}
	if (Message->GetType() == Core::VMessageType::MouseWheelMessage) {
		auto MouseWheelMessage = static_cast<Core::VMouseWheelMessage*>(Message);

		if (MouseWheelMessage->WheelValue > 0) {
			if (ViewingProject->LocalScalePercent + 0.1f >= 0.f) {
				ViewingProject->LocalScalePercent += 0.1f;

				PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
				ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);
			}
		}
		else {
			if (ViewingProject->LocalScalePercent - 0.1f <= 20.f) {
				ViewingProject->LocalScalePercent -= 0.1f;

				PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
				ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);
			}
		}

		WindowOnResize(0, 0);
	}
}

void PVMainWindow::WindowOnResize(const int&, const int&) {
	auto NewX = GetWidth() / 2 - ViewingLabel->GetWidth() / 2;
	auto NewY = GetHeight() / 2 - ViewingLabel->GetHeight() / 2;

	NewX += CenterOffsetPoint.X;
	NewY += CenterOffsetPoint.Y;

	ViewingLabel->Move(NewX, NewY);
}

void PVMainWindow::ImageStartDrag() {
	StartDrag = true;
}
void PVMainWindow::ImageOnDrag(const int& X, const int& Y) {
	if (StartDrag) {
		StartDrag = false;

		OffsetPoint = { X,  Y };
	}

	ViewingLabel->Move(ViewingLabel->GetX() + X - OffsetPoint.X, ViewingLabel->GetY() + Y - OffsetPoint.Y);

	auto CenterX = GetWidth() / 2 - ViewingLabel->GetWidth() / 2;
	auto CenterY = GetHeight() / 2 - ViewingLabel->GetHeight() / 2;

	CenterOffsetPoint = { ViewingLabel->GetX() - CenterX , ViewingLabel->GetY() - CenterY };
}

PVMainWindow::PVMainWindow(Core::VApplication* Parent) : VMLMainWindow(Parent) {
	LoadVML(L"./mainui.xml", VML::VMLParserParseMode::FromFile);

	MiniSizeButton	= operator[](L"photo-viewer-widget")[L"minisize-button"].Get<Core::VPushButton>();
	MaxSizeButton	= operator[](L"photo-viewer-widget")[L"maxsize-button"].Get<Core::VPushButton>();
	CloseButton		= operator[](L"photo-viewer-widget")[L"close-button"].Get<Core::VPushButton>();
	MainuiContainer	= operator[](L"photo-viewer-widget")[L"main-ui"].Get();

	FileOpenButton	= operator[](L"photo-viewer-widget")[L"startup-ui"][L"open-file-button"].Get<Core::VPushButton>();

	BlurBackgroundImage = operator[](L"photo-viewer-widget")[L"main-ui"][L"view-background"].Get<Core::VImageLabel>();
	ViewingLabel		 = operator[](L"photo-viewer-widget")[L"main-ui"][L"view-image"].Get<Core::VImageLabel>();

	StartupuiContainer = operator[](L"photo-viewer-widget")[L"startup-ui"].Get();

	TitleText		= operator[](L"photo-viewer-widget")[L"title-text"].Get<Core::VTextLabel>();

	PercentTagText  = operator[](L"photo-viewer-widget")[L"main-ui"][L"zoom-tag"].Get<Core::VTextLabel>();

	ZoomUpButton	= operator[](L"photo-viewer-widget")[L"zoom-down-button"].Get<Core::VPushButton>();
	ZoomDownButton  = operator[](L"photo-viewer-widget")[L"zoom-up-button"].Get<Core::VPushButton>();
	OneToOneButton  = operator[](L"photo-viewer-widget")[L"one-to-one-button"].Get<Core::VPushButton>();

	ZoomUpButton->ButtonPushed.Connect(this, &PVMainWindow::ZoomUpButtonOnClicked);
	ZoomDownButton->ButtonPushed.Connect(this, &PVMainWindow::ZoomDownButtonOnClicked);
	OneToOneButton->ButtonPushed.Connect(this, &PVMainWindow::OneToOneButtonOnClicked);

	MiniSizeButton->ButtonPushed.Connect(this, &PVMainWindow::MiniSizeButtonOnClicked);
	MaxSizeButton->ButtonPushed.Connect(this, &PVMainWindow::MaxSizeButtonOnClicked);
	CloseButton->ButtonPushed.Connect(this, &PVMainWindow::CloseButtonOnClicked);

	FileOpenButton->ButtonPushed.Connect(this, &PVMainWindow::OpenViewProject);

	Resized.Connect(this, &PVMainWindow::WindowOnResize);

	ImageDragger	   = new Core::VDragControlBase(ViewingLabel);
	ImageDraggerLayout = new Core::VScaleLayout(ImageDragger, ViewingLabel);
	ImageDraggerLayout->SetWidthScalePercent(1.f);
	ImageDraggerLayout->SetHeightScalePercent(1.f);

	ImageDragger->UserStartDrag.Connect(this, &PVMainWindow::ImageStartDrag);
	ImageDragger->MouseDraggedPosition.Connect(this, &PVMainWindow::ImageOnDrag);

	ViewingProject	= nullptr;
	ViewingImage		= nullptr;
}
PVMainWindow::PVMainWindow(Core::VApplication* Parent, const std::wstring& FilePath) : VMLMainWindow(Parent) {
	if (!Core::VImage::IsValidBitmapFile(FilePath)) {
		exit(-1);
	}

	wchar_t LocalPath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, LocalPath, MAX_PATH);

	std::wstring PathString = LocalPath;

	PathString = PathString.substr(0, PathString.find_last_of(L"\\"));

	_wchdir(PathString.c_str());

	LoadVML(L"./mainui.xml", VML::VMLParserParseMode::FromFile);

	MiniSizeButton = operator[](L"photo-viewer-widget")[L"minisize-button"].Get<Core::VPushButton>();
	MaxSizeButton = operator[](L"photo-viewer-widget")[L"maxsize-button"].Get<Core::VPushButton>();
	CloseButton = operator[](L"photo-viewer-widget")[L"close-button"].Get<Core::VPushButton>();
	MainuiContainer = operator[](L"photo-viewer-widget")[L"main-ui"].Get();

	FileOpenButton = operator[](L"photo-viewer-widget")[L"startup-ui"][L"open-file-button"].Get<Core::VPushButton>();

	BlurBackgroundImage = operator[](L"photo-viewer-widget")[L"main-ui"][L"view-background"].Get<Core::VImageLabel>();
	ViewingLabel = operator[](L"photo-viewer-widget")[L"main-ui"][L"view-image"].Get<Core::VImageLabel>();

	StartupuiContainer = operator[](L"photo-viewer-widget")[L"startup-ui"].Get();

	TitleText = operator[](L"photo-viewer-widget")[L"title-text"].Get<Core::VTextLabel>();

	PercentTagText = operator[](L"photo-viewer-widget")[L"main-ui"][L"zoom-tag"].Get<Core::VTextLabel>();

	ZoomUpButton = operator[](L"photo-viewer-widget")[L"zoom-down-button"].Get<Core::VPushButton>();
	ZoomDownButton = operator[](L"photo-viewer-widget")[L"zoom-up-button"].Get<Core::VPushButton>();
	OneToOneButton = operator[](L"photo-viewer-widget")[L"one-to-one-button"].Get<Core::VPushButton>();

	ZoomUpButton->ButtonPushed.Connect(this, &PVMainWindow::ZoomUpButtonOnClicked);
	ZoomDownButton->ButtonPushed.Connect(this, &PVMainWindow::ZoomDownButtonOnClicked);
	OneToOneButton->ButtonPushed.Connect(this, &PVMainWindow::OneToOneButtonOnClicked);

	MiniSizeButton->ButtonPushed.Connect(this, &PVMainWindow::MiniSizeButtonOnClicked);
	MaxSizeButton->ButtonPushed.Connect(this, &PVMainWindow::MaxSizeButtonOnClicked);
	CloseButton->ButtonPushed.Connect(this, &PVMainWindow::CloseButtonOnClicked);

	FileOpenButton->ButtonPushed.Connect(this, &PVMainWindow::OpenViewProject);

	Resized.Connect(this, &PVMainWindow::WindowOnResize);

	ImageDragger = new Core::VDragControlBase(ViewingLabel);
	ImageDraggerLayout = new Core::VScaleLayout(ImageDragger, ViewingLabel);
	ImageDraggerLayout->SetWidthScalePercent(1.f);
	ImageDraggerLayout->SetHeightScalePercent(1.f);

	ImageDragger->UserStartDrag.Connect(this, &PVMainWindow::ImageStartDrag);
	ImageDragger->MouseDraggedPosition.Connect(this, &PVMainWindow::ImageOnDrag);

	ViewingProject = nullptr;
	ViewingImage = nullptr;
	
	CreateViewingProject(FilePath);

	ViewingImage = new Core::VImage(ViewingProject->FileList[ViewingProject->LocalViewingFile],
		CallWidgetGetRenderHandle());

	BlurBackgroundImage->SetImage(ViewingImage);
	ViewingLabel->SetImage(ViewingImage);

	if (ViewingImage->GetWidth() > GetWidth() ||
		ViewingImage->GetHeight() > GetHeight()) {
		if (ViewingImage->GetWidth() > GetWidth()) {
			ViewingProject->LocalScalePercent = double(GetWidth()) / ViewingImage->GetWidth();
		}
		else {
			ViewingProject->LocalScalePercent = double(GetHeight()) / ViewingImage->GetHeight();
		}

		PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
		ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);
	}
	else {
		PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
		ViewingLabel->Resize(ViewingImage->GetWidth(), ViewingImage->GetHeight());
	}

	TitleText->SetPlaneText(GetFileName(ViewingProject->FileList[ViewingProject->LocalViewingFile]));

	ZoomUpButton->Show();
	ZoomDownButton->Show();
	OneToOneButton->Show();

	StartupuiContainer->Hide();
	MainuiContainer->Show();
}

void PVMainWindow::MiniSizeButtonOnClicked() {
	PostMessage(GetLocalWinId(), WM_SYSCOMMAND, SC_MINIMIZE, NULL);
}
void PVMainWindow::MaxSizeButtonOnClicked() {
	if (!InMaxSize) {
		PostMessage(CallWidgetGetHWND(), WM_SYSCOMMAND, SC_MAXIMIZE, NULL);
	}
	else {
		PostMessage(CallWidgetGetHWND(), WM_SYSCOMMAND, SC_RESTORE, NULL);
	}

	InMaxSize = !InMaxSize;
}
void PVMainWindow::CloseButtonOnClicked() {
	exit(0);
}

std::wstring PVMainWindow::GetPercentString(const double& ScalePercent) {
	if (ScalePercent >= 1) {
		return std::to_wstring(ScalePercent * 100).substr(0, 3) + L"%";
	}
	if (ScalePercent <= 1 && ScalePercent >= 0.1) {
		return std::to_wstring(ScalePercent * 100).substr(0, 2) + L"%";
	}
	if (ScalePercent <= 0.1) {
		return std::to_wstring(ScalePercent * 100).substr(0, 1) + L"%";
	}

	return L"Error%";
}
std::wstring PVMainWindow::GetFileName(const std::wstring& FilePath) {
	int SliptPosition = static_cast<int>(FilePath.find_last_of(L"\\")) + 1;

	return FilePath.substr(SliptPosition, FilePath.size() - SliptPosition);
}
std::wstring PVMainWindow::GetFolderPath(const std::wstring& FilePath) {
	int SliptPosition = static_cast<int>(FilePath.find_last_of(L"\\")) + 1;

	return FilePath.substr(0, SliptPosition);
}

void PVMainWindow::CreateViewingProject(const std::wstring& FilePath) {
	ViewingProject = new PVFileReadingProject;

	auto LocalFileName   = GetFileName(FilePath);
	auto LocalFolderPath = GetFolderPath(FilePath);

	intptr_t    FileHandle = NULL;
	_wfinddata_t FileInfo;

	std::wstring FindPath;
	std::wstring TempPath;
	FindPath = FindPath.assign(LocalFolderPath.c_str()).append(L"*");

	if ((FileHandle = _wfindfirst(FindPath.c_str(), &FileInfo)) != -1) {
		do {
			if (!(FileInfo.attrib & _A_SUBDIR)) {
				if (FileInfo.name == LocalFileName) {
					ViewingProject->LocalViewingFile = ViewingProject->FileList.size();
				}

				TempPath = LocalFolderPath + FileInfo.name;

				if (TempPath.find(L".jpg") == TempPath.size() - 4 ||
					TempPath.find(L".png") == TempPath.size() - 4 ||
					TempPath.find(L".gif") == TempPath.size() - 4 ||
					TempPath.find(L".jpge") == TempPath.size() - 5 ||
					TempPath.find(L".jpg") == TempPath.size() - 4) {
					if (Core::VImage::IsValidBitmapFile(TempPath) == true) {
						ViewingProject->FileList.push_back(TempPath);
					}
				}
			}
		} while (_wfindnext(FileHandle, &FileInfo) == 0);
	}
}

void PVMainWindow::OpenViewProject() {
	LPWSTR  FilePath = new WCHAR[MAX_PATH];
	HRESULT OperationResult;

	std::thread FileDialogThread([](LPWSTR FilePath, HRESULT* OperationResult) -> void {
		CoInitialize(NULL);
		IFileDialog* FileDialog = NULL;
		HRESULT		 Status = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&FileDialog));

		DWORD			  OptionFlags;
		COMDLG_FILTERSPEC FileFilter[] = { { L"All files", L"*.*" }, };

		FileDialog->GetOptions(&OptionFlags);
		FileDialog->SetOptions(OptionFlags | FOS_FORCEFILESYSTEM);
		FileDialog->SetFileTypes(ARRAYSIZE(FileFilter), FileFilter);
		FileDialog->SetFileTypeIndex(1);

		*OperationResult = FileDialog->Show(NULL);

		FileDialog->ClearClientData();
		FileDialog->Close(*OperationResult);

		if (SUCCEEDED((*OperationResult))) {
			IShellItem* SellItem;
			FileDialog->GetResult(&SellItem);
			LPWSTR OpenPath;
			SellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &OpenPath);

			SellItem->Release();
			CoTaskMemFree(OpenPath);

			FileDialog->ClearClientData();
			FileDialog->Close(*OperationResult);

			wcscpy_s(FilePath, MAX_PATH, OpenPath);
		}

		FileDialog->Release();

		CoUninitialize();

		}, FilePath, &OperationResult);

	FileDialogThread.join();

	if (SUCCEEDED(OperationResult)) {
		CreateViewingProject(FilePath);

		ViewingImage = new Core::VImage(ViewingProject->FileList[ViewingProject->LocalViewingFile],
			CallWidgetGetDCRenderTarget());

		BlurBackgroundImage->SetImage(ViewingImage);
		ViewingLabel->SetImage(ViewingImage);

		if (ViewingImage->GetWidth() > GetWidth() ||
			ViewingImage->GetHeight() > GetHeight()) {
			if (ViewingImage->GetWidth() > GetWidth()) {
				ViewingProject->LocalScalePercent = double(GetWidth()) / ViewingImage->GetWidth();
			} else {
				ViewingProject->LocalScalePercent = double(GetHeight()) / ViewingImage->GetHeight();
			}

			PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
			ViewingLabel->Resize(ViewingImage->GetWidth() * ViewingProject->LocalScalePercent, ViewingImage->GetHeight() * ViewingProject->LocalScalePercent);
		} 
		else {
			PercentTagText->SetPlaneText(GetPercentString(ViewingProject->LocalScalePercent));
			ViewingLabel->Resize(ViewingImage->GetWidth(), ViewingImage->GetHeight());
		}

		TitleText->SetPlaneText(GetFileName(ViewingProject->FileList[ViewingProject->LocalViewingFile]));

		ZoomUpButton->Show();
		ZoomDownButton->Show();
		OneToOneButton->Show();

		StartupuiContainer->Hide();
		MainuiContainer->Show();
	}
}
