#include "vstudioapp.h"

#include <Shlobj.h>
#include <comutil.h>

#pragma comment(lib, "comsuppw")

void VStudioApp::MinisizeWindow()
{
	ShowWindow(GetLocalWinId(), SW_MINIMIZE);
}
void VStudioApp::MaxsizeWindow()
{
	Maximize = !Maximize;

	auto MaxsizeButton = operator[](L"main-widget")[L"max-size-button"].Get<Core::VPushButton>();

	if (Maximize)
	{
		ShowWindow(GetLocalWinId(), SW_MAXIMIZE);
	}
	else
	{
		ShowWindow(GetLocalWinId(), SW_RESTORE);
	}

	MaxsizeButton->GetTheme()->LabelFont->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
	MaxsizeButton->GetTheme()->LabelFont->SetParagraphAlignment(
		Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}
void VStudioApp::ExitApp()
{
	if (InViewing)
	{
		operator[](L"main-widget")[L"start-vml-view-button"].Get<Core::VPushButton>()->SetPlaneText(L"\ueea3");
		operator[](L"main-widget")[L"start-vml-view-button"]
			.Get<Core::VPushButton>()
			->GetTheme()
			->LabelFont->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
		operator[](L"main-widget")[L"start-vml-view-button"]
			.Get<Core::VPushButton>()
			->GetTheme()
			->LabelFont->SetParagraphAlignment(Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		TerminateProcess(ProcessInfo.hProcess, 4);

		CloseHandle(ProcessInfo.hProcess);
		CloseHandle(ProcessInfo.hThread);
	}

	exit(0);
}
void VStudioApp::SaveFile()
{
	if (FileSaved)
	{
		std::ofstream FileStream(CodeFilePath);
		auto		  PlaneText = CodeEditor->GetPlaneText();

		FileStream.imbue(std::locale("chs"));

		std::wstring_convert<std::codecvt_utf8<wchar_t>> CodeConvert;

		auto   Result	= CodeConvert.to_bytes(PlaneText);
		size_t Position = 0;

		while (true)
		{
			Position = Result.find('\r', Position);

			if (Position == -1)
			{
				break;
			}

			Result.erase(Result.begin() + Position);

			++Position;
		}

		FileStream << Result;

		FileStream.close();

		FileSaved = false;

		SetTitle();
	}
}
void VStudioApp::CreateVMLFile()
{
	LPWSTR	FilePath = new WCHAR[MAX_PATH];
	HRESULT OperationResult;

	std::thread FileDialogThread(
		[&](LPWSTR FilePath, HRESULT *OperationResult) -> void {
			if (CoInitialize(NULL))
			{
				;
			}

			IFileSaveDialog *FileDialog = NULL;
			HRESULT			 Status =
				CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&FileDialog));

			DWORD			  OptionFlags;
			COMDLG_FILTERSPEC FileFilter[] = {
				{L"XML files", L"*.xml"}, {L"VML files", L"*.vml"}, {L"All files", L"*.*"}};

			FileDialog->GetOptions(&OptionFlags);
			FileDialog->SetOptions(OptionFlags | FOS_FORCEFILESYSTEM | FOS_FORCEFILESYSTEM | FOS_OVERWRITEPROMPT);
			FileDialog->SetDefaultExtension(L"xml");
			FileDialog->SetFileTypes(_countof(FileFilter), FileFilter);
			FileDialog->SetFileTypeIndex(1);

			*OperationResult = FileDialog->Show(GetLocalWinId());

			if (SUCCEEDED((*OperationResult)))
			{
				IShellItem *SellItem;
				FileDialog->GetResult(&SellItem);

				LPWSTR OpenPath = new wchar_t[MAX_PATH];
				SellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &OpenPath);

				wcscpy_s(FilePath, MAX_PATH, OpenPath);

				SellItem->Release();
				CoTaskMemFree(OpenPath);
			}

			FileDialog->ClearClientData();
			FileDialog->Close(*OperationResult);
			FileDialog->Release();

			CoUninitialize();
		},
		FilePath, &OperationResult);

	FileDialogThread.join();

	if (SUCCEEDED(OperationResult))
	{
		FileName		  = GetFileName(FilePath);
		CodeFilePath	  = FilePath;
		CodeWorkspacePath = GetWorkspacePath(FilePath);

		if (_waccess(CodeWorkspacePath.c_str(), 06) != 0)
		{
			PopFailureDialog(L"Can not open target file", L"The file can't be created!");

			return;
		}

		HANDLE FileHandle =
			CreateFile(CodeFilePath.c_str(), GENERIC_ALL, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		if (FileHandle == INVALID_HANDLE_VALUE)
		{
			PopFailureDialog(L"Can not open target file", L"The file can't be created!");

			return;
		}

		CloseHandle(FileHandle);

		operator[](L"main-widget")[L"caption"][L"title"].Get<Core::VTextLabel>()->SetPlaneText(L"VStudio (" + FileName +
																							   L")");

		operator[](L"main-widget")[L"main-ui"]->Show();
		operator[](L"main-widget")[L"startup-ui"]->Hide();

		CodeEditor->SetPlaneText(VKits::VParserHelper::ReadFromFile(CodeFilePath, VKits::VDocumentEncoding::UTF8));

		InEditMode = true;
	}
	else
	{
		PopFailureDialog(L"Can not open target file", L"The file can't be open!");
	}
}
void VStudioApp::SetTitle()
{
	if (FileSaved)
	{
		operator[](L"main-widget")[L"caption"][L"title"].Get<Core::VTextLabel>()->SetPlaneText(L"VStudio (" + FileName +
																							   L")*");
	}
	else if (InViewing)
	{
		operator[](L"main-widget")[L"caption"][L"title"].Get<Core::VTextLabel>()->SetPlaneText(L"VStudio (" + FileName +
																							   L") [Viewing]");
	}
	else
	{
		operator[](L"main-widget")[L"caption"][L"title"].Get<Core::VTextLabel>()->SetPlaneText(L"VStudio (" + FileName +
																							   L")");
	}
}
void VStudioApp::CodeChanged(const std::wstring &)
{
	if (InEditMode)
	{
		FileSaved = true;

		SetTitle();
	}
}
void VStudioApp::CheckFrame()
{
	VML::VMLMainWindow::CheckFrame();

	if (!ThreadCurrentRunning && InViewing)
	{
		InViewing = false;

		operator[](L"main-widget")[L"start-vml-view-button"].Get<Core::VPushButton>()->SetPlaneText(L"\ueea3");
		operator[](L"main-widget")[L"start-vml-view-button"]
			.Get<Core::VPushButton>()
			->GetTheme()
			->LabelFont->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
		operator[](L"main-widget")[L"start-vml-view-button"]
			.Get<Core::VPushButton>()
			->GetTheme()
			->LabelFont->SetParagraphAlignment(Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		SetTitle();
	}
}
void VStudioApp::PopFailureDialog(const std::wstring Title, const std::wstring Text)
{
	auto ErrorInfoWidget = operator[](L"main-widget")[L"run-failed-note"].Get();

	ErrorInfoWidget->Move(ErrorInfoWidget->GetX(), 55);

	operator[](L"main-widget")[L"run-failed-note"][L"failure-title"].Get<Core::VTextLabel>()->SetPlaneText(Title);
	operator[](L"main-widget")[L"run-failed-note"][L"failure-sub-content-table"][L"failure-info"]
		.Get<Core::VTextLabel>()
		->SetPlaneText(Text);
	operator[](L"main-widget")[L"run-failed-note"][L"run-in-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition({ErrorInfoWidget->GetX(), ErrorInfoWidget->GetY() + 10});
	operator[](L"main-widget")[L"run-failed-note"][L"run-in-animation"].Get<Core::VPositionAnimation>()->Start();
	operator[](L"main-widget")[L"run-failed-note"][L"fade-in-animation"].Get<Core::VOpacityAnimation>()->Start();

	NoteBoxAnimaitonTimer.Start(2000);
}
void VStudioApp::PopSyntaxErrorDialog(const std::wstring Title, const std::wstring Text)
{
	auto ErrorInfoWidget = operator[](L"main-widget")[L"view-failed-note"].Get();

	ErrorInfoWidget->Move(ErrorInfoWidget->GetX(), GetHeight() - 20 - 123 - 40);

	operator[](L"main-widget")[L"view-failed-note"][L"failure-title"].Get<Core::VTextLabel>()->SetPlaneText(Title);
	operator[](L"main-widget")[L"view-failed-note"][L"failure-info"].Get<Core::VTextLabel>()->SetPlaneText(Text);
	operator[](L"main-widget")[L"view-failed-note"][L"run-in-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition({ErrorInfoWidget->GetX(), ErrorInfoWidget->GetY() + 10});
	operator[](L"main-widget")[L"view-failed-note"][L"run-in-animation"].Get<Core::VPositionAnimation>()->Start();
	operator[](L"main-widget")[L"view-failed-note"][L"fade-in-animation"].Get<Core::VOpacityAnimation>()->Start();

	SyntaxBoxAnimaitonTimer.Start(2000);
}
void VStudioApp::StartVMLView()
{
	if (!InEditMode)
	{
		PopFailureDialog(L"Can not start VML view", L"Not valid VML file found!");
	}
	else
	{
		VML::VMLParser GrammarChecker(CodeEditor->GetPlaneText());
		auto		   ParseResult = GrammarChecker.ParseVML();

		if (ParseResult.ParserStatus != VML::VMLParserStatus::Ok)
		{
			PopSyntaxErrorDialog(L"Failed to start view", L"Syntax Error (line " +
															  std::to_wstring(ParseResult.ErrorInfo[0].Line) + L") : " +
															  ParseResult.ErrorInfo[0].ErrorString);

			return;
		}

		SaveFile();
		std::wstring CommandLine = L"./viewer.exe " + std::wstring(L"\"") + CodeFilePath + L"\"";

		if (!InViewing)
		{
			if (CreateProcess(NULL, (LPWSTR)CommandLine.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &StartupInfo,
							  &ProcessInfo))
			{
				std::thread PatchThread([&]() -> void {
					ThreadCurrentRunning = true;

					WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

					ThreadCurrentRunning = false;
				});
				PatchThread.detach();

				operator[](L"main-widget")[L"start-vml-view-button"].Get<Core::VPushButton>()->SetPlaneText(L"\uee4a");
				operator[](L"main-widget")[L"start-vml-view-button"]
					.Get<Core::VPushButton>()
					->GetTheme()
					->LabelFont->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
				operator[](L"main-widget")[L"start-vml-view-button"]
					.Get<Core::VPushButton>()
					->GetTheme()
					->LabelFont->SetParagraphAlignment(
						Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

				InViewing = true;
			}
		}
		else
		{
			InViewing = false;

			operator[](L"main-widget")[L"start-vml-view-button"].Get<Core::VPushButton>()->SetPlaneText(L"\ueea3");
			operator[](L"main-widget")[L"start-vml-view-button"]
				.Get<Core::VPushButton>()
				->GetTheme()
				->LabelFont->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
			operator[](L"main-widget")[L"start-vml-view-button"]
				.Get<Core::VPushButton>()
				->GetTheme()
				->LabelFont->SetParagraphAlignment(Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

			TerminateProcess(ProcessInfo.hProcess, 4);

			CloseHandle(ProcessInfo.hProcess);
			CloseHandle(ProcessInfo.hThread);
		}

		SetTitle();
	}
}
void VStudioApp::ExecInfoBoxAnimation()
{
	auto ErrorInfoWidget = operator[](L"main-widget")[L"run-failed-note"].Get();

	operator[](L"main-widget")[L"run-failed-note"][L"run-out-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition({ErrorInfoWidget->GetX(), ErrorInfoWidget->GetY() + 10});
	operator[](L"main-widget")[L"run-failed-note"][L"run-out-animation"].Get<Core::VPositionAnimation>()->Start();
	operator[](L"main-widget")[L"run-failed-note"][L"fade-out-animation"].Get<Core::VOpacityAnimation>()->Start();
}
void VStudioApp::ExecSyntaxBoxAnimation()
{
	auto ErrorInfoWidget = operator[](L"main-widget")[L"view-failed-note"].Get();

	operator[](L"main-widget")[L"view-failed-note"][L"run-out-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition({ErrorInfoWidget->GetX(), ErrorInfoWidget->GetY() + 10});
	operator[](L"main-widget")[L"view-failed-note"][L"run-out-animation"].Get<Core::VPositionAnimation>()->Start();
	operator[](L"main-widget")[L"view-failed-note"][L"fade-out-animation"].Get<Core::VOpacityAnimation>()->Start();
}
std::wstring VStudioApp::GetFileName(const std::wstring &FilePath)
{
	int SliptPosition = static_cast<int>(FilePath.find_last_of(L"\\")) + 1;

	return FilePath.substr(SliptPosition, FilePath.size() - SliptPosition);
}
std::wstring VStudioApp::GetWorkspacePath(const std::wstring &FilePath)
{
	int SliptPosition = static_cast<int>(FilePath.find_last_of(L"\\")) + 1;

	return FilePath.substr(0, SliptPosition);
}
void VStudioApp::OpenFile()
{
	LPWSTR	FilePath = new WCHAR[MAX_PATH];
	HRESULT OperationResult;

	std::thread FileDialogThread(
		[](LPWSTR FilePath, HRESULT *OperationResult) -> void {
			if (CoInitialize(NULL))
			{
				;
			}

			IFileDialog *FileDialog = NULL;
			HRESULT		 Status =
				CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&FileDialog));

			DWORD			  OptionFlags;
			COMDLG_FILTERSPEC FileFilter[] = {
				{L"XML files", L"*.xml"}, {L"VML files", L"*.vml"}, {L"All files", L"*.*"}};

			FileDialog->GetOptions(&OptionFlags);
			FileDialog->SetOptions(OptionFlags | FOS_FORCEFILESYSTEM);
			FileDialog->SetFileTypes(ARRAYSIZE(FileFilter), FileFilter);
			FileDialog->SetFileTypeIndex(1);

			*OperationResult = FileDialog->Show(NULL);

			FileDialog->ClearClientData();
			FileDialog->Close(*OperationResult);

			if (SUCCEEDED((*OperationResult)))
			{
				IShellItem *SellItem;
				FileDialog->GetResult(&SellItem);

				LPWSTR OpenPath = new wchar_t[MAX_PATH];
				SellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &OpenPath);

				wcscpy_s(FilePath, MAX_PATH, OpenPath);

				SellItem->Release();
				CoTaskMemFree(OpenPath);
			}

			FileDialog->ClearClientData();
			FileDialog->Close(*OperationResult);
			FileDialog->Release();

			CoUninitialize();
		},
		FilePath, &OperationResult);

	FileDialogThread.join();

	if (SUCCEEDED(OperationResult) && _waccess(FilePath, 06) == 0)
	{
		FileName		  = GetFileName(FilePath);
		CodeFilePath	  = FilePath;
		CodeWorkspacePath = GetWorkspacePath(FilePath);

		operator[](L"main-widget")[L"caption"][L"title"].Get<Core::VTextLabel>()->SetPlaneText(L"VStudio (" + FileName +
																							   L")");

		operator[](L"main-widget")[L"main-ui"]->Show();
		operator[](L"main-widget")[L"startup-ui"]->Hide();

		CodeEditor->SetPlaneText(VKits::VParserHelper::ReadFromFile(CodeFilePath, VKits::VDocumentEncoding::UTF8));

		InEditMode = true;
	}
	else
	{
		PopFailureDialog(L"Can not open target file", L"The file can't be open!");
	}
}

VStudioApp::VStudioApp(Core::VApplication *App)
	: VMLMainWindow(App), NoteBoxAnimaitonTimer(this), SyntaxBoxAnimaitonTimer(this),
	  VMLHighlighter(CallWidgetGetStaticRenderHandle(), Core::VBuiltInHightlighterTheme::FleetDark)
{
	AddFontResource(L"./vml-editor-plus/font/vml-editor-plus.ttf");
	AddFontResource(L"./vml-editor-plus/font/icomoon.ttf");

	StartupInfo			 = {sizeof(StartupInfo)};
	ProcessInfo			 = PROCESS_INFORMATION{};
	ThreadCurrentRunning = false;

	SetQuitEvent([&]() -> bool {
		if (InViewing)
		{
			operator[](L"main-widget")[L"start-vml-view-button"].Get<Core::VPushButton>()->SetPlaneText(L"\ueea3");
			operator[](L"main-widget")[L"start-vml-view-button"]
				.Get<Core::VPushButton>()
				->GetTheme()
				->LabelFont->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
			operator[](L"main-widget")[L"start-vml-view-button"]
				.Get<Core::VPushButton>()
				->GetTheme()
				->LabelFont->SetParagraphAlignment(Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

			TerminateProcess(ProcessInfo.hProcess, 4);

			CloseHandle(ProcessInfo.hProcess);
			CloseHandle(ProcessInfo.hThread);
		}

		return true;
	});

	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::MinisizeWindow, MinisizeWindow));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::MaxsizeWindow, MaxsizeWindow));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::ExitApp, ExitApp));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::OpenFile, OpenFile));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::StartVMLView, StartVMLView));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::CreateVMLFile, CreateVMLFile));

	auto Result = LoadVML(L"./vml-editor-plus/ui/mainui.xml", VML::VMLParserParseMode::FromFile);

	CodeEditor = operator[](L"main-widget")[L"main-ui"][L"code-editor"].Get<Core::VEditor>();

	NoteBoxAnimaitonTimer.OnTime.Connect(this, &VStudioApp::ExecInfoBoxAnimation);
	SyntaxBoxAnimaitonTimer.OnTime.Connect(this, &VStudioApp::ExecSyntaxBoxAnimation);
	CodeEditor->TextOnChange.Connect(this, &VStudioApp::CodeChanged);

	VMLHighlighter.BindEditor(CodeEditor);

	InEditMode = false;
	Maximize   = false;
	InViewing  = false;
	FileSaved  = false;

	Show();
}
VStudioApp::VStudioApp(Core::VApplication *App, const std::wstring &FilePath)
	: VMLMainWindow(App), NoteBoxAnimaitonTimer(this), SyntaxBoxAnimaitonTimer(this),
	  VMLHighlighter(CallWidgetGetStaticRenderHandle(), Core::VBuiltInHightlighterTheme::FleetDark)
{
	wchar_t LocalPath[MAX_PATH + 1] = {0};
	GetModuleFileName(NULL, LocalPath, MAX_PATH);

	std::wstring PathString = LocalPath;
	PathString				= PathString.substr(0, PathString.find_last_of(L"\\"));

	_wchdir(PathString.c_str());

	AddFontResource(L"./vml-editor-plus/font/vml-editor-plus.ttf");
	AddFontResource(L"./vml-editor-plus/font/icomoon.ttf");

	StartupInfo = {sizeof(StartupInfo)};
	ProcessInfo = PROCESS_INFORMATION{};

	SetQuitEvent([&]() -> bool {
		if (InViewing)
		{
			operator[](L"main-widget")[L"start-vml-view-button"].Get<Core::VPushButton>()->SetPlaneText(L"\ueea3");
			operator[](L"main-widget")[L"start-vml-view-button"]
				.Get<Core::VPushButton>()
				->GetTheme()
				->LabelFont->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
			operator[](L"main-widget")[L"start-vml-view-button"]
				.Get<Core::VPushButton>()
				->GetTheme()
				->LabelFont->SetParagraphAlignment(Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

			TerminateProcess(ProcessInfo.hProcess, 4);

			CloseHandle(ProcessInfo.hProcess);
			CloseHandle(ProcessInfo.hThread);
		}

		return true;
	});

	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::MinisizeWindow, MinisizeWindow));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::MaxsizeWindow, MaxsizeWindow));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::ExitApp, ExitApp));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::OpenFile, OpenFile));
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &VStudioApp::StartVMLView, StartVMLView));

	auto Result = LoadVML(L"./vml-editor-plus/ui/mainui.xml", VML::VMLParserParseMode::FromFile);

	CodeEditor = operator[](L"main-widget")[L"main-ui"][L"code-editor"].Get<Core::VEditor>();

	NoteBoxAnimaitonTimer.OnTime.Connect(this, &VStudioApp::ExecInfoBoxAnimation);
	SyntaxBoxAnimaitonTimer.OnTime.Connect(this, &VStudioApp::ExecSyntaxBoxAnimation);
	CodeEditor->TextOnChange.Connect(this, &VStudioApp::CodeChanged);

	VMLHighlighter.BindEditor(CodeEditor);

	InEditMode = false;
	Maximize   = false;
	InViewing  = false;
	FileSaved  = false;

	Show();

	if (_waccess(FilePath.c_str(), 06) == 0)
	{
		FileName		  = GetFileName(FilePath);
		CodeFilePath	  = FilePath;
		CodeWorkspacePath = GetWorkspacePath(FilePath);

		operator[](L"main-widget")[L"caption"][L"title"].Get<Core::VTextLabel>()->SetPlaneText(L"VStudio (" + FileName +
																							   L")");

		operator[](L"main-widget")[L"main-ui"]->Show();
		operator[](L"main-widget")[L"startup-ui"]->Hide();

		CodeEditor->SetPlaneText(VKits::VParserHelper::ReadFromFile(CodeFilePath, VKits::VDocumentEncoding::UTF8));

		InEditMode = true;
	}
	else
	{
		PopFailureDialog(L"Can not open target file", L"The file can't be open!");
	}
}

bool VStudioApp::CatchMessage(Core::VMessage *Message)
{
	if (Message->GetType() == Core::VMessageType::KeyClickedMessage && Message->Win32ID == WM_KEYDOWN && InEditMode)
	{
		auto KeyClickedMessage = static_cast<Core::VKeyClickedMessage *>(Message);
		if (KeyClickedMessage->KeyVKCode == 'S' && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
		{
			SaveFile();

			return true;
		}
		if (KeyClickedMessage->KeyVKCode == VK_F5)
		{
			StartVMLView();

			return true;
		}
	}
	if (Message->GetType() == Core::VMessageType::UnknowMessage && Message->Win32ID == WM_SIZE)
	{
		auto MaxsizeButton = operator[](L"main-widget")[L"max-size-button"].Get<Core::VPushButton>();

		if (Message->wParameter == SIZE_MAXIMIZED)
		{
			MaxsizeButton->SetPlaneText(L"\ue904");
			MaxsizeButton->SetTextSize(12);
		}
		if (Message->wParameter == SIZE_RESTORED)
		{
			MaxsizeButton->SetPlaneText(L"\ue903");
			MaxsizeButton->SetTextSize(100);
		}

		MaxsizeButton->GetTheme()->LabelFont->SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
		MaxsizeButton->GetTheme()->LabelFont->SetParagraphAlignment(
			Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		return true;
	}

	return false;
}

VStudioApp::~VStudioApp()
{
	RemoveFontResource(L"./vml-editor-plus/font/vml-editor-plus.ttf");
	RemoveFontResource(L"./vml-editor-plus/font/icomoon.ttf");
}
