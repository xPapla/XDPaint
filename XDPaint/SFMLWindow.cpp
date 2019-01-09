#include "SFMLWindow.h"

SFMLWindow::SFMLWindow(sf::WindowHandle hwnd)
{
	window.create(hwnd);
}

void SFMLWindow::Start()
{
	window.setActive(false);
	thread = std::thread(&SFMLWindow::Run, this);
}

void SFMLWindow::Resize(int x, int y, unsigned int width, unsigned int height) {
	window.setSize(sf::Vector2u(width, height));
	window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
	window.setPosition(sf::Vector2i(x, y));
}

void SFMLWindow::Run()
{
	static float size = 100.f;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}		
		size += 0.5f - ((float)rand() / (float)RAND_MAX);
		sf::CircleShape shape(size);
		shape.setFillColor(sf::Color::Green);
		window.clear();
		window.draw(shape);
		window.display();
	}
}

SFMLWindow::~SFMLWindow()
{
	window.close();
	thread.join();
}
