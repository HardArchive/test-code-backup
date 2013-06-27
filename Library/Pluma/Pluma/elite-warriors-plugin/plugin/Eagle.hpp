#ifndef EAGLE_HPP
#define EAGLE_HPP

#include "Warrior.hpp"

/////////////////////////////////////////////////////////
/// An Eagle is a Warrior
/////////////////////////////////////////////////////////
class Eagle: public Warrior{
public:
    std::string getDescription(){
        return "Eagle插件描述：这是一个测试插件!!!!!\r\n";
    }
};

/////////////////////////////////////////////////////////
/// An EagleProvider is a WarriorProvider,
/// provide warriors of type Eagle
/////////////////////////////////////////////////////////
PLUMA_INHERIT_PROVIDER(Eagle, Warrior);

#endif // EAGLE_HPP
