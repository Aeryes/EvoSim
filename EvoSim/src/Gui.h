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

private:
	int m_width;
	int m_height;
	int m_foodAmount = 50;

	sf::RenderWindow m_window;

	sf::Text m_text;
	sf::Text m_numberAlive;
	sf::Text m_foodInPlay;
    sf::Text m_timeElapsed;
	sf::Font m_font;

	sf::Clock m_simTimer;
	sf::Time  m_simTimerSeconds;

	Flock m_flock;
	Flock m_notFlocking;

	float m_organismSize;
	float m_foodSize;

	vector<sf::CircleShape> m_shapes;
	vector<sf::CircleShape> m_shapesNotFlocking;
	vector<Food> m_foodObjStorage;

	void render();
	void handleInput();
	void handleEvents();
	void loadText();
	void displayText();

	bool isCollided(Organism organism, Food food);

	
};

