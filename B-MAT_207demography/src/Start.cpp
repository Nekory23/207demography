/*
** EPITECH PROJECT, 2021
** B-MAT_207demography
** File description:
** Start
*/

#include <cstring>

#include "Demography.hpp"
#include "Error.hpp"

int DisplayHelp(char *prog)
{
    std::cout << "USAGE" << std::endl;
    std::cout << "    "<< prog << " code [...]" << std::endl;
    std::cout << std::endl << "DESCRIPTION" << std::endl;
    std::cout << "    code    country code" << std::endl;
    return SUCCESS;
}

int Start(int ac, char **av)
{
    int ret = SUCCESS;

    if (ac == 2 && !std::strcmp(av[1], "-h"))
        return DisplayHelp(av[0]);
    try {
        Demography demo(ac, av);
        demo.DisplayCountry();
        demo.FirstFit();
        demo.SecondFit();
        demo.Correlation();
    }
    catch (Error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        ret = ERROR;
    }
    catch (...) {}
    return ret;
}