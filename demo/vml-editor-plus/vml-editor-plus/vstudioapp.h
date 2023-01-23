#pragma once

#include "../../../include/core/control/veditorhighlighter.h"
#include "../../../include/vml/vmlwidget.h"

class VStudioApp : public VML::VMLMainWindow
{
private:
	void MinisizeWindow();
	void MaxsizeWindow();
	void ExitApp();
	void StartVMLView();

private:
	std::wstring GetFileName(const std::wstring &FilePath);
	std::wstring GetWorkspacePath(const std::wstring &FilePath);
	void		 OpenFile();
	void		 SaveFile();
	void		 CreateVMLFile();

private:
	void ExecInfoBoxAnimation();
	void ExecSyntaxBoxAnimation();
	void CodeChanged(const std::wstring &);
	void SetTitle();
	void PopFailureDialog(const std::wstring Title, const std::wstring Text);
	void PopSyntaxErrorDialog(const std::wstring Title, const std::wstring Text);

private:
	void CheckFrame() override;
	bool CatchMessage(Core::VMessage *Message) override;

public:
	VStudioApp(Core::VApplication *App);
	VStudioApp(Core::VApplication *App, const std::wstring &FilePath);
	~VStudioApp();

private:
	bool Maximize;
	bool InEditMode;
	bool InViewing;
	bool FileSaved;
	bool ThreadCurrentRunning;

	std::wstring FileName;
	std::wstring CodeFilePath;
	std::wstring CodeWorkspacePath;

	Core::VSmartTimer	  NoteBoxAnimaitonTimer;
	Core::VSmartTimer	  SyntaxBoxAnimaitonTimer;
	Core::VEditor		 *CodeEditor;
	Core::VVMLHighlighter VMLHighlighter;

	STARTUPINFO			StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
};
