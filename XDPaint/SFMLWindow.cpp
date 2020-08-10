#include "SFMLWindow.h"

SFMLWindow::SFMLWindow(sf::WindowHandle hwnd)
{
	window.create(hwnd);
	//window.setMouseCursor(sf::Cursor::);
}

void SFMLWindow::Start()
{
	window.setActive(false);
	thread = std::thread(&SFMLWindow::Run, this);
}

void SFMLWindow::setSize(int x, int y, unsigned int width, unsigned int height) {
	window.setSize(sf::Vector2u(width, height));
	window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
	window.setPosition(sf::Vector2i(x, y));
}

void SFMLWindow::saveColors()
{
	savedColorBG = canvas.getBackgroundColor();
	savedColorFILL = canvas.getFillColor();
	savedColorOUT = canvas.getOutlineColor();
}

void SFMLWindow::restoreColors()
{
	canvas.setBackgroundColor(savedColorBG);
	canvas.setFillColor(savedColorFILL);
	canvas.setOutlineColor(savedColorFILL);
}

void SFMLWindow::updateBackgroundColor(uint8_t r, uint8_t g, uint8_t b)
{
	canvas.setBackgroundColor(sf::Color(r, g, b));
}

void SFMLWindow::updateFillColor(uint8_t r, uint8_t g, uint8_t b)
{
	canvas.setFillColor(sf::Color(r, g, b));
}

void SFMLWindow::updateOutlineColor(uint8_t r, uint8_t g, uint8_t b)
{
	canvas.setOutlineColor(sf::Color(r, g, b));
}

void SFMLWindow::updateTool(SFMLTool tool)
{
	canvas.setTool(tool);
}

SFMLTool SFMLWindow::getCurrentTool()
{
	return canvas.getCurrentTool();
}

void SFMLWindow::updateShape(SFMLShape shape)
{
	canvas.setShape(shape);
}

SFMLShape SFMLWindow::getCurrentShape()
{
	return canvas.getCurrentShape();
}

void SFMLWindow::updateOutlineWidth(int width)
{
	canvas.setOutlineWidth(width);
}

int SFMLWindow::getOutlineWidth()
{
	return canvas.getOutlineWidth();
}

#include <iostream>
#include <fstream> 

void SFMLWindow::saveImage(std::wstring str, bool zTlem)
{
	std::ofstream stream;
	stream.open(str, std::ofstream::out);

	sf::Image img = canvas.getScreenshot();
	sf::Vector2u size = img.getSize();

	stream << "P3" << std::endl
		<< size.x << " " << size.y << " " << 255 << std::endl;

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			sf::Color pixel = img.getPixel(x, y);
			if (zTlem && pixel.a == 0) {
				pixel = canvas.getBackgroundColor();
			}
			stream << (int)pixel.r << " " << (int)pixel.g << " " << (int)pixel.b << " ";
		}
		stream << std::endl;
	}

	if (!zTlem) {
		stream << "#A ";
		int count = 0;
		std::string t = "";
		for (int y = 0; y < size.y; y++) {
			for (int x = 0; x < size.x; x++) {
				sf::Color pixel = img.getPixel(x, y);
				if (pixel.a != 0) continue;
				count++;
				t += (int)x;
				t += " ";
				t += (int)y;
				t += " ";
			}
		}
		stream << count << " " << t << std::endl;
	}

	stream.close();
}

void SFMLWindow::clear()
{
	canvas.Clear();
}

void SFMLWindow::Run()
{
	windowAccess.lock();

	while (window.isOpen() && !closeRequested)
	{
		//PROBABLY EVENT LOOP HERE
		//
		canvas.update(window);
		canvas.renderCanvas();
		window.clear(sf::Color(206, 216, 230));
		window.draw(canvas);
		window.display();
	}
	windowAccess.unlock();
}

SFMLWindow::~SFMLWindow()
{
	closeRequested = true;
	windowAccess.lock();
	window.close();
	thread.join();
	windowAccess.unlock();
}
