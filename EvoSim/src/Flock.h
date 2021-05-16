/*
    Flock.h

    Description: Flock.h contains the prototypes for flocking oragnisms.

    Credits: Jorge Yanar - https://github.com/jyanar/Boids for the flock.h file.

*/

#pragma once

#include <iostream>
#include <vector>
#include "Organism.h"
#include "SFML/System.hpp"


// Brief description of Flock Class:
// This file contains the class needed to create a flock of boids. It utilizes
// the boids class and initializes boid flocks with parameters that can be
// specified. This class will be utilized in main.

class Flock {
public:
    //Constructors
    Flock() {}

    vector<Organism> flock;

    // Accessor functions
    int getSize();
    Organism &getOrganism(int i);

    void removeOrganism(int index);

    // Mutator Functions
    void addOrganism(const Organism& b);
    void flocking(float separation);
    void nonflocking(float separation);
};


