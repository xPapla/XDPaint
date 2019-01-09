#pragma once
#include <SFML/Graphics.hpp>

#include <thread>

class SFMLWindow
{
	sf::RenderWindow window;
	std::thread thread;
	void Run();
public:
	SFMLWindow(sf::WindowHandle hwnd);
	void Start();
	void Resize(int x, int y, unsigned int width, unsigned int height);
	~SFMLWindow();
};

