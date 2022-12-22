#include "../../include/vml/vmlwidget.h"

int main() {
	Core::VApplication App;
	VML::VMLMainWindow VMLWindow(&App);

	VMLWindow.Show();

	return App.Exec();
}