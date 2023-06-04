// This code is a simple example of how to use VuiLib to create a window and load a VML file.
// VuiLib is a C++ library for creating graphical user interfaces.
// It includes classes for creating windows, buttons, text boxes, and other GUI elements.

#include "./include/vml/vmlwidget.h"
#include <iostream>

int main()
{
	// Create a new VApplication object.
	auto Application = Core::VApplication();

	// Create a new VMLMainWindow object with a width of 640 and a height of 800.
	auto MainWindow = VML::VMLMainWindow(640, 800, &Application);

	// Show the main window.
	MainWindow.Show();

	// Load a VML file into the main window.
	// MainWindow.LoadVML(L"./testvml.xml", VML::VMLParserParseMode::FromFile);

	// Execute the application.
	return Application.Exec();
}
