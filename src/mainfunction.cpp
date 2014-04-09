#include <ctime>
#include "header.h"
#include "MyProjectMain.h"

#define BASE_SCREEN_WIDTH  1100
#define BASE_SCREEN_HEIGHT 750

int main(int argc, char *argv[]) {
	MyProjectMain main;
	int result;
	char buf[16];

	srand(time(NULL));
	sprintf(buf, "Submersible");

	result = main.Initialise(buf, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, "../resources/NotoSans-Regular.ttf", 20);
	result = main.MainLoop();
	main.Deinitialise();

	return result;
}