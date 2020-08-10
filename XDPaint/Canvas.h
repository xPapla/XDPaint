#pragma once
#include "SFML/Graphics.hpp"
#include "Shape.h"
#include <vector>
#include "Rectangle.h"
#include "Diamond.h"
#include "Circle.h"
#include "Triangle.h"
#include <mutex>

enum SFMLTool {
	Pencil = 1 << 1,
	Text = 1 << 2,
	Pipette = 1 << 3,
	Magnifier = 1 << 4,
	Shapes = 1 << 5,
};

enum SFMLShape {
	SDiamond = 1 << 1,
	SCircle = 1 << 2,
	SRectangle = 1 << 3,
	STriangle = 1 << 4,
};

class Canvas : public sf::Drawable
{
	sf::Vector2f canvasPosition = { 2,2 };
	sf::Vector2f size = { 640, 480 }; //canvas size

	sf::RenderTexture canvas;
	class Rectangle canvasBackground; //only bg
	sf::RectangleShape shadow;

	sf::RectangleShape resizeRight;
	sf::RectangleShape resizeCorner;
	sf::RectangleShape resizeBottom;

	bool wasPressed = false;
	sf::Vector2f previousPos;
	sf::RectangleShape *rectPressed = nullptr;

	void updateCanvasSize();
	std::vector<Shape *> shapes;

	SFMLTool currentTool;
	SFMLShape currentShape;
	Shape *editedShape = nullptr;

	sf::Color currentFillColor = { 255,0,0 };
	sf::Color currentOutlineColor = { 0,0,255};
	int currentOutlineWidth = 1;


	std::mutex iterating;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
public:
	Canvas();
	~Canvas();

	void Clear();

	sf::Color getBackgroundColor();
	void setBackgroundColor(sf::Color color);

	sf::Color getFillColor();
	void setFillColor(sf::Color color);

	sf::Color getOutlineColor();
	void setOutlineColor(sf::Color color);

	void setTool(SFMLTool tool);
	SFMLTool getCurrentTool();

	void setShape(SFMLShape shape);
	SFMLShape getCurrentShape();

	void setOutlineWidth(int width);
	int getOutlineWidth();

	void update(sf::RenderWindow & window);
	void renderCanvas();

	sf::Image getScreenshot();
};

