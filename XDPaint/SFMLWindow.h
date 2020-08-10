#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <thread>
#include "Canvas.h"
#include <string>

class SFMLWindow
{
	Canvas canvas;
	std::mutex windowAccess;
	sf::RenderWindow window;
	std::thread thread;
	bool closeRequested = false;
	void Run();
	sf::Color savedColorBG;
	sf::Color savedColorFILL;
	sf::Color savedColorOUT;
public:
	SFMLWindow(sf::WindowHandle hwnd);
	void Start();
	void setSize(int x, int y, unsigned int width, unsigned int height);
	void saveColors();
	void restoreColors();
	void updateBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
	void updateFillColor(uint8_t r, uint8_t g, uint8_t b);
	void updateOutlineColor(uint8_t r, uint8_t g, uint8_t b);

	void updateTool(SFMLTool tool);
	SFMLTool getCurrentTool();

	void updateShape(SFMLShape shape);
	SFMLShape getCurrentShape();

	void updateOutlineWidth(int width);
	int getOutlineWidth();

	void saveImage(std::wstring str, bool zTlem);
	void clear();

	~SFMLWindow();
};

