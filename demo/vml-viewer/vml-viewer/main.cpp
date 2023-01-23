#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include "../../../include/vml/vmlwidget.h"

#include <comutil.h>

#pragma comment(lib, "comsuppw.lib")

int main(int args, const char *argvs[])
{
	if (args >= 2)
	{
		Core::VApplication App;
		VML::VMLMainWindow MainWindow(&App);

		_bstr_t RawString = argvs[1];

		std::wstring FilePath((const wchar_t *)RawString);

		int			 SliptPosition = static_cast<int>(FilePath.find_last_of(L"\\")) + 1;
		std::wstring Workspace	   = FilePath.substr(0, SliptPosition);

		_wchdir(Workspace.c_str());

		auto Result = MainWindow.LoadVML((const wchar_t *)RawString, VML::VMLParserParseMode::FromFile);

		return App.Exec();
	}
	else
	{
		return -1;
	}
}