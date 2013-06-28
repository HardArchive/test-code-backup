#ifndef SIMPLE_WARRIOR_HPP
#define SIMPLE_WARRIOR_HPP

#include "Warrior.hpp"

/////////////////////////////////////////////////////////
/// Default Warrior
/////////////////////////////////////////////////////////
class SimpleWarrior: public Warrior{
public:
    std::string getDescription(){
		return "Author:Create by Rg";
    }

	std::string GetVersion()
	{
		return "SimpleWarrior:0.0.0.1";
	}
};

/////////////////////////////////////////////////////////
/// SimpleWarriorProvider provide default warriors
/////////////////////////////////////////////////////////
PLUMA_INHERIT_PROVIDER(SimpleWarrior, Warrior);

#endif // SIMPLE_WARRIOR_HPP
