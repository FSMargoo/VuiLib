#include "core/control/vdropdown.h"
#include "vml/vmlwidget.h"

void Test(Core::VDropContextBase *Context)
{
	MessageBox(NULL, Context->Text.CStyleString(), Context->Text.CStyleString(), MB_OK);
}

int main()
{
	//	Core::VElementUITheme ElementStyle;

	auto Application = Core::VApplication();

	auto MainWindow = VML::VMLMainWindow(1000, 800, &Application);
	/* auto DropDownTest = new Core::VDropDown(400, 40, &MainWindow);

	Core::VDropGroup GroupTest;
	GroupTest.Text = VStr("Test");

	Core::VDropContext Context;
	Context.Text = VStr("Test1");
	Core::VDropContext Context2;
	Context2.Text = VStr("Test2");
	Core::VDropContext Context3;
	Context3.Text = VStr("Test3");

	DropDownTest->AddContext(&GroupTest);
	DropDownTest->AddContext(&Context);
	DropDownTest->AddContext(&Context2);
	DropDownTest->AddContext(&Context3);
	DropDownTest->Move(100, 100);

	MainWindow.Show(); */

	MainWindow.RegisterMetaFunction(VML_META_FUNCTION(Test));
	MainWindow.LoadVML("./testvml.xml", VML::VMLParserParseMode::FromFile);
	MainWindow.Show();

	return Application.Exec();
}
