#include "../../../include/core/widget/vwidget.h"
#include "../../../include/core/control/vtextlabel.h"
#include "../../../include/core/control/vimagelabel.h"
#include "../../../include/core/control/vpushbutton.h"

#include <shobjidl.h>

class SimpleDemo : public Core::VMainWindow {
public:
	SimpleDemo(Core::VApplication* AppParent) : VMainWindow(640, 480, AppParent, false) {
		ViewImage	   = nullptr;
		FileName	   = new Core::VTextLabel(640, 20, this);
		ImageLabel	   = new Core::VImageLabel(640 * 0.7, 480 * 0.6, ViewImage, this);
		FileOpenButton = new Core::VPushButton(240, 40, L"Open File", this);

		FileName->SetTextColor(Core::VColor::White);
		FileName->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);

		FileName->Move(0, 40);
		FileOpenButton->Move(GetWidth() / 2 - FileOpenButton->GetWidth() / 2, GetHeight() / 2 - FileOpenButton->GetHeight() / 2);
		ImageLabel->Move(GetWidth() / 2 - ImageLabel->GetWidth() / 2, 100);

		FileOpenButton->ButtonPushed.Connect(this, &SimpleDemo::OpenFile);

		Show();
	}

public:
	void OpenFile() {
		LPWSTR  FilePath = new WCHAR[MAX_PATH];;
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

		if (SUCCEEDED(OperationResult) && Core::VImage::IsValidBitmapFile(FilePath)) {
			ViewImage = new Core::VImage(FilePath, CallWidgetGetDCRenderTarget());

			FileOpenButton->Hide();

			ImageLabel->SetImage(ViewImage);
			FileName->SetPlaneText(FilePath);
		}
	}

private:
	Core::VImage*	   ViewImage;
	Core::VTextLabel*  FileName;
	Core::VImageLabel* ImageLabel;
	Core::VPushButton* FileOpenButton;
};

int main() {
	Core::VApplication App;
	SimpleDemo		   MainWindow(&App);

	return App.Exec();
}