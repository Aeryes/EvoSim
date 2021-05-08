/*
	Food.cpp

	Description: Food.cpp provides definitions for the Food.h file. This cpp
	deals with foood for organisms to consume along with the energy values of that food.

*/

#include "Food.h"

Food::Food(int energyLevel, sf::CircleShape shape)
{
	this->energyLevel = energyLevel;
	this->shape = shape;
}

void Food::getPosition()
{	
	sf::Vector2f sfmlPosition = this->shape.getPosition();
	this->position.x = sfmlPosition.x;
	this->position.y = sfmlPosition.y;
}
