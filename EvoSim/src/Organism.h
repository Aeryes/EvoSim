/*
	Organism.h

	Description: Organism.h contains the prototype functions for organism operation.

    Credits: Jorge Yanar - https://github.com/jyanar/Boids for the boids.h file.
*/

/*
    Note:
    I decided not to have levels of inheritance for different types of organisms because they need to be able to
    mutate freely while the program executes. Having child classes of Organism would resullt in a more restricted
    mutation capability where Organisms would be less free to adapt and change to each other.
*/

#pragma once

#include <vector>
#include "Pvector.h"
#include "SFML/System.hpp"
#include <Genetics.h>

class Organism
{
public:
    Organism(float x, float y);
    Organism(float x, float y, bool predCheck, bool hasFlockTrait);

    //Movement values.
    Pvector location;
    Pvector velocity;
    Pvector acceleration;

    float maxSpeed;
    float maxForce;

    //Organism life values.
    int health = 100;
    int defense = 0;
    int attack = 0;
    int energyStore = 1000;
    int energyUseLevel = 50;
    int energyGainLevel = 5;

    //Trait values.
    bool predator;

    //This vector contains a list of trait object belonging to the organism.
    vector<Genetics> traitList;

    //Timers.
    sf::Clock movementClock;
    sf::Clock timeAlive;
    sf::Clock mutationTimer;

    //Flocking fields.
    bool hasFlockTrait = false;
    float desiredseparation;

    void applyForce(const Pvector& force);

    // Three Laws that boids follow
    Pvector separation(const vector<Organism>& Organsims, float sep);
    Pvector nonFlockSep(const vector<Organism>& organisms, float sep);
    Pvector alignment(const vector<Organism>& Organsims);
    Pvector cohesion(const vector<Organism>& Organisms);

    //Functions involving visualisation linking
    Pvector seek(const Pvector& v);

    void run(const vector<Organism>& v, float sep);
    void runNonFlock(const vector <Organism>& v, float separation);
    void update();
    void updateNonFlock();
    void flock(const vector<Organism>& v, float separaton);
    void nonFlockMovement(const vector<Organism>& v, float separation);
    void borders();
    float angle(const Pvector& v);


};

