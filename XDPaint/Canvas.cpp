#include "Canvas.h"

void Canvas::renderCanvas() {
	canvas.clear(sf::Color::Transparent);
	//canvas.draw(canvasBackground);

	//render all shapes here
	iterating.lock();
	for (auto it = shapes.begin(); it != shapes.end(); std::advance(it, 1)) {
		canvas.draw(**it);
	}
	iterating.unlock();

	canvas.display();
}

sf::Image Canvas::getScreenshot()
{
	return canvas.getTexture().copyToImage();
}

void Canvas::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(shadow, states);
	target.draw(canvasBackground, states);

	sf::Sprite canvasSprite(canvas.getTexture());
	canvasSprite.setPosition(2, 2);

	target.draw(canvasSprite, states);

	sf::RectangleShape res[3] = { resizeBottom, resizeCorner, resizeRight };
	for (auto &r : res) {
		target.draw(r, states);
	}
}

void Canvas::updateCanvasSize() {
	size.x = size.x < 1 ? 1 : size.x;
	size.y = size.y < 1 ? 1 : size.y;
	shadow.setSize(size);
	canvasBackground.setSize(shadow.getSize());
	canvas.create(size.x, size.y);

	resizeBottom.setPosition(
		canvasBackground.getPosition().x + (canvasBackground.getSize().x - resizeBottom.getSize().x) / 2,
		canvasBackground.getPosition().y + canvasBackground.getSize().y + resizeBottom.getOutlineThickness());

	resizeCorner.setPosition(
		canvasBackground.getPosition().x + canvasBackground.getSize().x + resizeCorner.getOutlineThickness(),
		canvasBackground.getPosition().y + canvasBackground.getSize().y + resizeCorner.getOutlineThickness());

	resizeRight.setPosition(
		canvasBackground.getPosition().x + canvasBackground.getSize().x + resizeRight.getOutlineThickness(),
		canvasBackground.getPosition().y + (canvasBackground.getSize().y - resizeRight.getSize().y) / 2);
}

sf::Color Canvas::getFillColor()
{
	return currentFillColor;
}

void Canvas::setFillColor(sf::Color color)
{
	currentFillColor = color;
}

sf::Color Canvas::getOutlineColor()
{
	return currentOutlineColor;
}

void Canvas::setOutlineColor(sf::Color color)
{
	currentOutlineColor = color;
}

void Canvas::setTool(SFMLTool tool)
{
	currentTool = tool;
}

SFMLTool Canvas::getCurrentTool()
{
	return currentTool;
}

void Canvas::setShape(SFMLShape shape)
{
	currentShape = shape;
}

SFMLShape Canvas::getCurrentShape()
{
	return currentShape;
}

void Canvas::setOutlineWidth(int width)
{
	currentOutlineWidth = width;
}

int Canvas::getOutlineWidth()
{
	return currentOutlineWidth;
}

void Canvas::update(sf::RenderWindow &window)
{
	bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	bool moved = currentPos != previousPos;
	sf::Vector2f deltaPos = currentPos - previousPos;
	sf::Vector2f onCanvasPosition = currentPos - canvasPosition;

	if (!wasPressed && !isPressed && moved) { //MOUSEMOVE (NOT PRESSED)

	}
	else if (!wasPressed && isPressed) { //MOUSEDOWN
		/*sf::Image x = canvas.getTexture().copyToImage();
		setBackgroundColor(x.getPixel(currentPos.x - 2, currentPos.y - 2));*/
		for (auto &r : { &resizeBottom, &resizeCorner, &resizeRight }) {
			if (r->getGlobalBounds().contains(currentPos)) {
				rectPressed = r;
			}
		}

		if (canvasBackground.getLocalBounds().contains(onCanvasPosition)) {
			if (currentTool == SFMLTool::Shapes) {
				switch (currentShape)
				{
				case SDiamond:
					editedShape = new Diamond();
					break;
				case SCircle:
					editedShape = new Circle();
					break;
				case SRectangle:
					editedShape = new Rectangle();
					break;
				case STriangle:
					editedShape = new Triangle();
					break;
				default:
					break;
				}
				if (editedShape) {
					shapes.push_back(editedShape);
					editedShape->setPosition(onCanvasPosition);
					editedShape->setFillColor(currentFillColor);
					editedShape->setOutlineColor(currentOutlineColor);
					editedShape->setOutlineThickness(currentOutlineWidth);
				}
			}
		}
	}
	else if (wasPressed && !isPressed) { //MOUSEUP
		rectPressed = nullptr;
		editedShape = nullptr;
	}
	else if (wasPressed && isPressed && moved) { // MOUSEMOVE (PRESSED)
		if (rectPressed) {
			if (rectPressed == &resizeBottom) deltaPos.x = 0;
			if (rectPressed == &resizeRight) deltaPos.y = 0;
			size += deltaPos;
			updateCanvasSize();
		}
		if (currentTool == SFMLTool::Pencil) {
			if (canvasBackground.getLocalBounds().contains(onCanvasPosition)) {
				Rectangle *newLine = new Rectangle();

				newLine->setFillColor(currentOutlineColor);
				newLine->setOutlineColor(currentOutlineColor);
				newLine->setSize(sf::Vector2f(sqrtf(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y), currentOutlineWidth));
				newLine->setPosition(onCanvasPosition - deltaPos);
				newLine->move(-deltaPos / 2.0f);
				newLine->setRotation(atan2f(deltaPos.y, deltaPos.x) * 180 / 3.14159265);

				shapes.push_back(newLine);
			}
		}
		if (editedShape) {
			sf::Vector2f size = onCanvasPosition - editedShape->getPosition();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
				sf::Vector2f normalized = { fabsf(size.x), fabsf(size.y) };
				float smaller = fminf(normalized.x, normalized.y);
				size.x = size.x < 0 ? -smaller : smaller;
				size.y = size.y < 0 ? -smaller : smaller;
			}

			editedShape->setSize(size);
		}
	}
	wasPressed = isPressed;
	previousPos = currentPos;
}

Canvas::Canvas()
{
	for (auto &r : { &resizeBottom, &resizeCorner, &resizeRight }) {
		r->setSize(sf::Vector2f(3, 3));
		r->setFillColor(sf::Color::White);
		r->setOutlineColor(sf::Color::Black);
		r->setOutlineThickness(1);
	}

	canvasBackground.setPosition(canvasPosition);
	shadow.setPosition(6, 6); //4px shadow

	canvasBackground.setFillColor(sf::Color::White);
	shadow.setFillColor(sf::Color(0, 0, 0, 32));

	updateCanvasSize();
}


Canvas::~Canvas()
{
	Clear();
}

void Canvas::Clear()
{
	iterating.lock();
	while (shapes.size()) {
		delete *(shapes.begin());
		shapes.erase(shapes.begin());
	}
	iterating.unlock();
}

sf::Color Canvas::getBackgroundColor()
{
	return canvasBackground.getFillColor();
}

void Canvas::setBackgroundColor(sf::Color color)
{
	canvasBackground.setFillColor(color);
}
