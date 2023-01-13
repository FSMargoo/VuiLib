#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

#include "vstudioapp.h"

int wmain(int argc, wchar_t* argv[]) {
	Core::VApplication App;

	if (argc == 1) {
		VStudioApp* VStudio = new VStudioApp(&App);
	}
	else {
		VStudioApp* VStudio = new VStudioApp(&App, argv[1]);
	}

	return App.Exec();
}