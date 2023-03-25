#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include "login-ui.h"

int main()
{
	Core::VApplication App;
	LoginUI			   LoginUIDemo(&App);

	return App.Exec();
}