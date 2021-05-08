#pragma once
#include <string>

class Genetics
{
public:
	//No-arg constructor.
	Genetics();

	std::string traitName;
	int traitID;

	//Traits.
	void assignTraitID();
	void flockTrait();
};

