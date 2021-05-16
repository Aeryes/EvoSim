/*
	Food.cpp

	Description: Food.cpp provides definitions for the Food.h file. This cpp
	deals with foood for organisms to consume along with the energy values of that food.

*/

#include "Food.h"

Food::Food(int energyAmount, sf::CircleShape shape)
{
	m_energyAmount = energyAmount;
	m_shape = shape;
}

//Getters and setters.
void Food::setPosition(Pvector vector)
{	
	//sf::Vector2f sfmlPosition = m_shape.getPosition();
	m_position.x = vector.x;
	m_position.y = vector.y;
}

Pvector Food::getPosition()
{
	return m_position;
}
