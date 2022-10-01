#include "../../include/vml/vml.h"
#include "../../include/vml/vmlwidget.h"

int main() {
	Core::VApplication VMLDemo;
	VML::VMLWidget     Widget(&VMLDemo);

	Widget.LoadVML(L"./mainui.xml", VML::VMLParserParseMode::FromFile);

	return VMLDemo.Exec();
}