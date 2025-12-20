#include "RPN.hpp"

int main(int ac, char **av)
{
    if (ac > 2 || ac == 1)
        return (std::cerr << "Error ./pro [arg1]" << std::endl, 0);
    if (!parse_arg1(av[1]))
        return 0;
    std::stack<int> befure_of_calcule;
    int a, b;
    for (size_t i = 0; av[1][i]; i++)
    {
        if ((av[1][i] < '0' || av[1][i] > '9') && av[1][i] > 32)
        {
            if (befure_of_calcule.size() < 2)
                return (std::cout << "---Error" << std::endl, 0);
            else
            {
                a = befure_of_calcule.top();
                befure_of_calcule.pop();
                b = befure_of_calcule.top();
                befure_of_calcule.pop();
                if (av[1][i] == '*')
                    befure_of_calcule.push(b * a);
                else if (av[1][i] == '/')
                    befure_of_calcule.push(b / a);
                else if (av[1][i] == '-')
                    befure_of_calcule.push(b - a);
                else if (av[1][i] == '+')
                    befure_of_calcule.push(b + a);
            }
        }
        else if (av[1][i] <= 32)
            continue;
        else
            befure_of_calcule.push(av[1][i] - '0');
    }
    std::cout << befure_of_calcule.top() << std::endl;
    return 1;
}
