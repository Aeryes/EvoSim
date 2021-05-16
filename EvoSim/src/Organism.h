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
#include "SFML/Graphics.hpp"
#include <Genetics.h>

class Organism
{
public:
    Organism(float x, float y);
    Organism(float x, float y, bool predCheck, bool hasFlockTrait);

    //Movement values.
    Pvector m_location;
    Pvector m_velocity;
    Pvector m_acceleration;

    float m_maxSpeed;
    float m_maxForce;

    //Trait values.
    bool m_predator;

    //This vector contains a list of trait object belonging to the organism.
    vector<Genetics> m_traitList;

    //Timers.
    sf::Clock m_movementClock;
    sf::Clock m_energyTimer;
    sf::Clock m_mutationTimer;

    //Flocking fields.
    bool m_hasFlockTrait = false;
    float m_desiredseparation;

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

    //Getters and setters.
    void setEnergyStore(int amount);
    int getEnergyStore();

    void setEnergyUsetime(int value);
    int getEnergyUseTime();

    void spendEnergy();

private:
    //Organism life values.
    int m_health = 100;
    int m_defense = 0;
    int m_attack = 0;
    int m_energyStore = 200;
    int m_energyUseLevel = 100;
    int m_energyUseTime = 10;
};


