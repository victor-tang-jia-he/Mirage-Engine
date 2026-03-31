#include <application.h>
#include <GameObject/game_object.h>

int main()
{
	Application application;

	application.InitApp();

	application.RunMainLoop();

	application.ShutDown();
}