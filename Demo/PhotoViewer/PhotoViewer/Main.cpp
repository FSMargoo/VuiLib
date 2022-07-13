///////////////////////////////////////////////////////
//                Photo Viewer
// 作       者  : Margoo  ( VS2022 + EasyX_20220610 )
// 最后一次更新  : 2022/7/11

#include "PVApplication.hpp"
#include "PVWidget.hpp"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		PVApplication Application;
		PVMainWindow  MainWindow(1427, 818, &Application);

		return Application.Exec();
	}
	else {
		PVApplication Application;
		PVMainWindow  MainWindow(1427, 818, &Application, PVMainWindow::CodeConvert(argv[1]));

		return Application.Exec();
	}
}