#include "PmergeMe.hpp"
#include <unistd.h>

void    parse_args(char **args, std::vector<double> &a, std::deque<double> &b)
{
    char *end = NULL;
    double n;

    for (size_t i = 0; args[i]; i++)
    {
        n = std::strtod(args[i], &end);
        if (n < 0 || !args[i][0])
            throw std::runtime_error("Error");
        else if (*end != '\0' && !std::isspace(*end))
            throw std::runtime_error("---Error");
        else if (std::isspace(*end))
        {
            while (*end != '\0')
            {
                end++;
                if (!std::isspace(*end) && *end != '\0')
                    throw std::runtime_error("+++Error");
            }
        }
        a.push_back(n);
        b.push_back(n);
    }
}

double getTime(){
    struct timeval time;

    if (gettimeofday(&time, NULL) == 0)
        return (time.tv_usec + (time.tv_sec * 1000000));
    else
        throw std::runtime_error("Error");
}

template <typename T>
void   merge_insert_sort(T &cont, int NNP)
{
    typename T::iterator it;
    typename T::iterator it_f;
    typename T::iterator it_s;
    for (it = cont.begin(); it != cont.end(); it--)
    {
        it_f = it + (NNP - 1);
        it_s = it_f + NNP;
        if (it_f < cont.end() && it_s < cont.end())
        {
            if (*it_f > *it_s)
                std::swap_ranges(it_f - (NNP - 1), it_f + 1, it_s - (NNP - 1));
        }
        it += NNP * 2 + 1;
        // std::cout << "hhhhhh" << std::endl;
    }
    if ((unsigned long)NNP * 2 < cont.size())
    {
        std::cout << "----" << cont.size() << "---" << NNP << std::endl;
        for (typename T::iterator it3 = cont.begin(); it3 != cont.end(); it3++)
        {
            std::cout << " " << *it3 << ' ';
        }
        std::cout << std::endl;
        merge_insert_sort(cont, NNP * 2);
    }
}

void pmergeme(char **args){
    std::vector<double> a;
    std::deque<double> b;

    parse_args(++args, a, b);
    std::cout << "Before: ";
    std::vector<double>::iterator it;
    for (it = a.begin(); it != a.end(); it++)
        std::cout << *it << ' ';
    std::cout << std::endl;
    
    double now = getTime();
    merge_insert_sort(a, 1);
    std::cout << "After: ";
    for (it = a.begin(); it != a.end(); it++)
        std::cout << *it << ' ';
    std::cout << std::endl;
    double curr_time = getTime();
    std::cout << "Time to process a range of " << a.size() << " elements with std::[vector] : " << curr_time - now << " us" << std::endl;

    now = getTime();
    // merge_insert_sort(b, 1);
    curr_time = getTime();
    std::cout << "Time to process a range of " << a.size() << " elements with std::[deque] : " << curr_time - now << " us" << std::endl;
}
