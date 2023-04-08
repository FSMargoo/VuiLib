#include "vml/vmlwidget.h"

int main()
{
	Core::VApplication FluentDesignApp;
	VML::VMLMainWindow MainWindow(&FluentDesignApp);

	MainWindow.LoadVML(L"./main.xml", VML::VMLParserParseMode::FromFile);
	MainWindow.Show();

	return FluentDesignApp.Exec();
}