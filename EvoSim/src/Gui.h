/*
    Gui.h

	Description: Gui.h is the header file for the programs visual interface.
	Gui.h provides prototypes for the functions that will display graphics to
	the user.

	Credits: Jorge Yanar - https://github.com/jyanar/Boids for the boids.cpp file.

*/

#pragma once

#include <Flock.h>
#include "Food.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

class Gui
{
public:
	//Constructors.
	Gui();

	//Creates the programs initial state and loop.
	void run();

	//Start a new clock for simulation elapsed time.
	sf::Clock m_clock;

	//Getters and setters.
	int getWidth();
	int getHeight();
	int getFoodAmount();
	int getFoodAmountRespawn();
	int getStartingPop();
	int getFoodRespawn();

	float getOrganismSize();
	float getFoodSize();

	void setWidth(int value);
	void setHeight(int value);
	void setFoodAmount(int value);
	void setFoodAmountRespawn(int value);
	void setStartingPop(int value);
	void setFoodRespawn(int value);
	void setOrganismSize(float value);
	void setFoodSize(float value);

private:
	int m_width;
	int m_height;
	int m_foodAmount = 150;
	int m_foodAmountRespawn = 20;
	int m_startingPop = 10;
	int m_foodRespawn = 10;

	float m_organismSize;
	float m_foodSize;

	sf::RenderWindow m_window;

	sf::Text m_text;
	sf::Text m_numberAlive;
	sf::Text m_foodInPlay;
    sf::Text m_timeElapsed;
	sf::Font m_font;

	sf::Clock m_simTimer;
	sf::Clock m_foodTimer;
	sf::Clock m_reproductionTimer;
	sf::Time  m_simTimerSeconds;

	Flock m_flock;
	Flock m_notFlocking;

	vector<sf::CircleShape> m_shapes;
	vector<sf::CircleShape> m_shapesNotFlocking;
	vector<Food> m_foodObjStorage;

	void render();
	void handleInput();
	void handleEvents();
	void loadText();
	void displayText();

	bool isCollided(Organism organism, Food food);

	//Adaption functons for organisms.
	void createOrganism(Organism parent);
};

