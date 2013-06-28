#ifndef JAGUAR_HPP
#define JAGUAR_HPP

#include "Warrior.hpp"

/////////////////////////////////////////////////////////
/// A Jaguar is a Warrior
/////////////////////////////////////////////////////////
class Jaguar: public Warrior{
public:
    std::string getDescription(){
        return "Jaguar�������������һ�����Բ��!!!!!\r\n";
    }

	std::string GetVersion()
	{
		return "Jaguar:0.0.0.1";
	}
};


/////////////////////////////////////////////////////////
/// A JaguarProvider is a WarriorProvider,
/// provide warriors of type Jaguar
/////////////////////////////////////////////////////////
PLUMA_INHERIT_PROVIDER(Jaguar, Warrior);

#endif // JAGUAR_HPP
