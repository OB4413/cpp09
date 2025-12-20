#include "RPN.hpp"

bool parse_arg1(char *arg1){
    for (size_t i = 0; arg1[i] != '\0'; i++)
    {
        if (arg1[i] <= 32)
            continue;
        if (arg1[i] < '0' || arg1[i] > '9')
        {
            if (arg1[i] != '*' && arg1[i] != '/' && arg1[i] != '-' && arg1[i] != '+')
                return (std::cout << "Error" << std::endl, false);
        }
        if (arg1[i + 1] != '\0' && arg1[i + 1] != ' ')
            return (std::cout << "Error" << std::endl, false);
        if (arg1[i + 1] != '\0')
            i += 1;
    }
    return true;
}
