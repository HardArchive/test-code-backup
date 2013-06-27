#ifndef JAGUAR_HPP
#define JAGUAR_HPP

#include "Warrior.hpp"

/////////////////////////////////////////////////////////
/// A Jaguar is a Warrior
/////////////////////////////////////////////////////////
class Jaguar: public Warrior{
public:
    std::string getDescription(){
        return "Jaguar插件描述：这是一个测试插件!!!!!\r\n";
    }
};


/////////////////////////////////////////////////////////
/// A JaguarProvider is a WarriorProvider,
/// provide warriors of type Jaguar
/////////////////////////////////////////////////////////
PLUMA_INHERIT_PROVIDER(Jaguar, Warrior);

#endif // JAGUAR_HPP
