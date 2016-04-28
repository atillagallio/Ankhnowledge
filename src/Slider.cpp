/*
 * Slider.cpp
 *
 *  Created on: Oct 28, 2013
 *      Author: al
 */

#include "Slider.h"

Slider::Slider(Sprite* button, Sprite* bar, float x, float y):GameObject(getX(),getY()) {
	this->button = button;
	this->bar = bar;
	this->x = x;
	this->y = y;
	this->xBar = x;
	this->xButton = x + 270;
	this->yBar = y;
	this->yButton = y;
	this->slideButton = false;
	this->input = InputManager::getInstance();
}

Slider::~Slider() {

}


void Slider::render(float camera_x, float camera_y) {
	if ((this->button) && (this->bar))	{
		this->bar->render(this->xBar - camera_x,this->yBar - camera_y);
		this->button->render(this->xButton - camera_x,this->yButton + bar->getHeight()/2 - button->getHeight()/2 - camera_y);
	}

	else {
		// Nothing to do
	}
}

int Slider::update(int dt) {

	if (input->isMousePressed(1) && onButton() == 1) {
		slideButton = true;
	}

	else if (input->isMousePressed(1) == 0) {
		slideButton = false;
	}

	else {
		// Nothing to do
	}

	slide();
	return 0;
}

void Slider::slide() {
	if (this->slideButton) {
		pastXButton = xButton;
		xButton = input->mousePosX();
		if ((xButton + button->getWidth()/2 > this->xBar + bar->getWidth()) || (xButton + button->getWidth()/2 < this->xBar)) {
			xButton = pastXButton;
		}

		else {
			// Nothing to do
		}
	}

	else {
		// Nothing to do
	}
}

float Slider::calculateValue() {
	return ((xButton + button->getWidth()/2 - xBar)/bar->getWidth());
}

bool Slider::getSlideButton() {
	return this->slideButton;
}

bool Slider::onButton() {
	return ((input->mousePosX() > this->xButton) && (input->mousePosX() < (this->xButton + this->button->getWidth())) &&
			(input->mousePosY() > this->yButton) && (input->mousePosY() < (this->yButton + this->button->getHeight())));
}
