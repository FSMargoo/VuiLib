#include "pvmainwindow.h"

#include <comutil.h>

#ifdef _DEBUG
#pragma comment(lib, "comsuppwd.lib")
#else
#pragma comment(lib, "comsuppw.lib")
#endif

std::wstring CodeConvert(const char *String)
{
	_bstr_t Temp(String);

	return Temp.operator const wchar_t *();
}

int main(int args, const char *argvs[])
{
	Core::VApplication PhotoViewerX;
	if (args == 1)
	{
		PVMainWindow PVMainWindow(&PhotoViewerX);

		return PhotoViewerX.Exec();
	}
	if (args > 1)
	{
		PVMainWindow PVMainWindow(&PhotoViewerX, CodeConvert(argvs[1]));

		return PhotoViewerX.Exec();
	}

	return -1;
}