// This code is a simple example of how to use VuiLib to create a window and load a VML file.
// VuiLib is a C++ library for creating graphical user interfaces.
// It includes classes for creating windows, buttons, text boxes, and other GUI elements.

// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include "./include/core/animation/vanimation.h"
#include "./include/core/control/vcanvas.h"
#include "./include/core/control/veditor.h"
#include "./include/core/control/veditorhighlighter.h"
#include "./include/core/control/vlineeditor.h"
#include "./include/core/control/vscroller.h"
#include "./include/core/control/vswitchgroup.h"
#include "./include/core/control/vtitlebar.h"
#include "./include/core/control/vviewlabel.h"
#include "./include/core/stl-ex/vstring.h"
#include "./include/core/uibasic/vsmarttimer.h"
#include "./include/core/visual/vvisual.h"
#include "./include/kits/vallocator.h"
#include "./include/vml/vmlwidget.h"
#include <iostream>

#define fn	auto
#define var auto

#define format_parameter(variable) ((const char *const)variable)

fn main()->int
{
	// Create a new VApplication object.
	var Application = Core::VApplication();

	// Create a new VMLMainWindow object with a width of 640 and a height of 800.
	var MainWindow = VML::VMLMainWindow(640, 800, &Application);

	// Show the main window.
	MainWindow.Show();

	// Load a VML file into the main window.
	MainWindow.LoadVML(L"./testvml.xml", VML::VMLParserParseMode::FromFile);

	// Execute the application.
	return Application.Exec();
}
