#include "application.h"

int main()
{
	Application application;

	application.InitApp();

	application.RunMainLoop();

	application.ShutDown();
}