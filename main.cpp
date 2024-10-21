#include "roblox/classes/classes.hpp"
#include "utils/configs/configs.hpp"

#include <filesystem>
#include <iostream>
#include <thread>

#include "keyauth/auth.hpp"
#include <string>
#include "keyauth/utils.hpp"
#include "keyauth/skStr.h"


int main(/* credits to AGC / A GREAT CHAOS / KEN CARSON / mogus (lua env) */)
{
    std::string consoleTitle = skCrypt("kitt").decrypt();
    SetConsoleTitleA(consoleTitle.c_str());

    std::string appdata = chaos::utils::appdata_path();
    if (!std::filesystem::exists(appdata + "\\Configs"))
    {
        std::filesystem::create_directory(appdata + "\\Configs");
    }

    if (!std::filesystem::exists(appdata + "\\Configs\\configs"))
    {
        std::filesystem::create_directory(appdata + "\\Configs\\configs");

    }

    chaos::roblox::init();

    printf("Press Enter to Exit");
    std::cin.get();

    //system("pause");
}