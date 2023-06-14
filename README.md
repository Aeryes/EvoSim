# EvoSim
This project simulates a simple evolution of virtual organisms.

## Description:
EvoSim uses SFML as a graphics framework to display simple shapes which represent organisms and food. Organisms each have their own self evolving state that is affected by the world around them, what they do, and how much food they manage to consume. The organisms can adapt to pick up new traits that will further their chances of survival as the simulation progresses. Organisms reproduce in an asexual manner. Food has its own energy values that provides energy to the organisms upon consumption. 

When it comes time for an organism to reproduce they will replicate an exact copy of themselves that contains all the parents genetic information. The thought process here is that as the simulation progresses only organisms that are adapted well to the changing enviornment will survive, leaving only one type of genetic variation by the end of the simulation.

All of the code is well documented to make it easier for others to build upon or use portions of the source code.

## Traits List:
- Predator
- Slower starvation
- Flocking
- Increased health
- Increased defense
- Increased attack
- Increased offspring amount
- Reduced reproduction wait time

The repo contains the Visual Studio project files and libraries that are needed for the project to work inside the Visual Studio enviornment.

## Libraries used:
- SFML 2.5.1

This project greatly extends and builds upon the repo found here:
https://github.com/jyanar/Boids

SFML can be downloaded here:
https://www.sfml-dev.org/download/sfml/2.5.1/
