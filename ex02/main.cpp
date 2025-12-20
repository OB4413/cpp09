#include "PmergeMe.hpp"

int main(int ac, char **av)
{
    if(ac <= 2)
        return (std::cout << "Error ./pro [arg1] [arg2] ..." << std::endl, 0);
    try{
        pmergeme(av);
    }
    catch (const std::exception &e){
        std::cout << e.what() << std::endl;
    }
    return 1;
}
