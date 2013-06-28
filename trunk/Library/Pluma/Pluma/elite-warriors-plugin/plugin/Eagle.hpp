#ifndef EAGLE_HPP
#define EAGLE_HPP

#include "Warrior.hpp"

/////////////////////////////////////////////////////////
/// An Eagle is a Warrior
/////////////////////////////////////////////////////////
class Eagle: public Warrior{
public:
    std::string getDescription(){
        return "Eagle�������������һ�����Բ��!!!!!\r\n";
    }

	std::string GetVersion()
	{
		return "Eagle:0.0.0.1";
	}
};

/////////////////////////////////////////////////////////
/// An EagleProvider is a WarriorProvider,
/// provide warriors of type Eagle
/////////////////////////////////////////////////////////
PLUMA_INHERIT_PROVIDER(Eagle, Warrior);

#endif // EAGLE_HPP
