/*
    Flock.cpp

    Description: Flock.cpp contains the defintions for flocking oragnisms.

    Credits: Jorge Yanar - https://github.com/jyanar/Boids for the flock.cpp file.

*/

#include <iostream>
#include <vector>
#include "Flock.h"
#include <SFML/System.hpp>

int Flock::getSize()
{
    return flock.size();
}

Organism Flock::getOrganism(int i)
{
    return flock[i];
}

void Flock::addOrganism(const Organism& b)
{
    flock.push_back(std::move(b));
}

// Runs the run function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::flocking(float separation)
{
    for (int i = 0; i < flock.size(); i++)
        flock[i].run(flock, separation);
}

void Flock::nonflocking(float separation)
{
    for (int i = 0; i < flock.size(); i++)
        flock[i].runNonFlock(flock, separation);
}