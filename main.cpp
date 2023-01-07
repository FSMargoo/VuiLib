// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include "./include/core/control/vlineeditor.h"
#include "./include/core/control/vswitchgroup.h"
#include "./include/core/control/vscroller.h"
#include "./include/core/control/vviewlabel.h"
#include "./include/core/visual/vvisual.h"
#include "./include/core/animation/vanimation.h"

#include "./include/core/uibasic/vsmarttimer.h"
#include "./include/core/control/vcanvas.h"

#include "./include/core/control/veditor.h"
#include "./include/core/control/veditorhighlighter.h"

#include "./include/vml/vmlwidget.h"
#include "./include/kits/vallocator.h"

#include <iostream>

#define fn  auto
#define var auto

#define format_parameter(variable) ((const char* const)variable)

fn main() -> int {
	var Application = Core::VApplication();
	var MainWindow  = VML::VMLMainWindow(640, 800, &Application);
	
	MainWindow.Show();

	MainWindow.LoadVML(L"./testvml.xml", VML::VMLParserParseMode::FromFile);

	return Application.Exec();
}