#include <Pluma/Pluma.hpp>
#include "Warrior.hpp"
#include "SimpleWarrior.hpp"
#include <iostream>


int main()
{
    pluma::Pluma pluma;
    pluma.acceptProviderType<WarriorProvider>();

    // Add a default warrior provider ����һ��Ĭ�� warrior���ͽӿ��ṩ��
    pluma.addProvider(new SimpleWarriorProvider() );

    // Load libraries ���ؿ�
    pluma.loadFromFolder("plugins");

    // Get warrior providers into a vector �õ�һ��warrior���ͽӿ��ṩ������ָ��
    std::vector<WarriorProvider*> providers;
    pluma.getProviders(providers);

    // Create a Warrior from each provider ����һ��������
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
