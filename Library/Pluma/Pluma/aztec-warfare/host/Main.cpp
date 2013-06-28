#include <Pluma/Pluma.hpp>
#include "Warrior.hpp"
#include "SimpleWarrior.hpp"
#include <iostream>


int main()
{
    pluma::Pluma pluma;
    pluma.acceptProviderType<WarriorProvider>();

    // Add a default warrior provider 增加一个默认 warrior类型接口提供者
    pluma.addProvider(new SimpleWarriorProvider() );

    // Load libraries 加载库
    pluma.loadFromFolder("plugins");

    // Get warrior providers into a vector 得到一个warrior类型接口提供者向量指针
    std::vector<WarriorProvider*> providers;
    pluma.getProviders(providers);

    // Create a Warrior from each provider 创建一个迭代器
    std::vector<WarriorProvider*>::iterator it;
    for (it = providers.begin() ; it != providers.end() ; ++it)
	{
        // Create a warrior
        Warrior* warrior = (*it)->create();
        // Display it's description
        std::cout << warrior->getDescription() << std::endl;
		std::cout << warrior->GetVersion() << std::endl;
        // (...)
        // don't forget to delete it
        delete warrior;
    }
    pluma.unloadAll();
    std::cout << "Press any key to exit";
    std::cin.ignore(10000, '\n');
    return 0;
}
