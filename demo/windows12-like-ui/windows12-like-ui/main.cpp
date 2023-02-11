#include "../../../include/vml/vmlwidget.h"

#include "status-bar.h"

int main()
{
	Core::VApplication App;
	VML::VMLMainWindow Widget(640, 480, &App);

	StatusPercentBar PercentBar(&Widget, 240, 12);
	PercentBar.SetRange(0.f, 0.4f);

	Widget.Show();

	return App.Exec();
}