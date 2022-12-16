// #define _SILENCE_AMP_DEPRECATION_WARNINGS
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include "./include/core/control/vlineeditor.h"
#include "./include/core/control/vswitchgroup.h"
#include "./include/core/control/vscroller.h"
#include "./include/core/control/vviewlabel.h"
#include "./include/core/visual/vvisual.h"
#include "./include/core/animation/vanimation.h"

#include "./include/core/uibasic/vsmarttimer.h"
#include "./include/core/control/vcanvas.h"

#include "./include/vml/vmlwidget.h"
#include "./include/kits/vallocator.h"

#define fn  auto
#define var auto

#define format_parameter(variable) ((const char* const)variable)

fn main() -> int {
	var Application = Core::VApplication();
	var MainWindow  = VML::VMLMainWindow(640, 540, &Application);

	HWND LunarWindow = FindWindow(L"LWJGL", L"Lunar Client (1.8.9-93d4dd4/master)");

	MainWindow.Show();
	MainWindow.GetLocalWinId();

	MainWindow.LoadVML(L"./testvml.xml", VML::VMLParserParseMode::FromFile);

	SetParent(LunarWindow, NULL);

	return Application.Exec();
}