#include <iostream>
#include <string>
#include "Menu.h"

void Menu::ManageMenu()
{
    PrintMainMenu();
    std::string prompt;
    getline(std::cin,prompt);
}
void Menu::PrintMainMenu()
{
    std::cout << " Menu" << std::endl;
    std::cout << "  1・Dictionary\n";
    std::cout << "  2・Classic Mode\n";
    std::cout << "  3・Practice" << std::endl;
}