#include <SFML/Graphics.hpp>
#include "main.hpp"
#include <iostream>

#include <UIRibbon.h>
#include "UIApplication.hpp"

#include "MainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	MainWindow window(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	window.Run();
	return 0;
}
