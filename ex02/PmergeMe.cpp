#include "PmergeMe.hpp"
static int ncmp = 0;

void    parse_args(char **args, std::vector<double> &a, std::deque<double> &b)
{
    char *end = NULL;
    double n;
    std::vector<double>::iterator it;
    std::vector<double>::iterator it1;

    for (size_t i = 0; args[i]; i++)
    {
        n = std::strtod(args[i], &end);
        if (n < 0 || !args[i][0])
            throw std::runtime_error("Error");
        else if (*end != '\0' && !std::isspace(*end))
            throw std::runtime_error("Error");
        else if (std::isspace(*end))
        {
            while (*end != '\0')
            {
                end++;
                if (!std::isspace(*end) && *end != '\0')
                    throw std::runtime_error("Error");
            }
        }
        a.push_back(n);
        b.push_back(n);
    }
    
    for (it = a.begin(); it != a.end(); it++)
    {
        for ( it1 = it + 1; it1 != a.end(); it1++)
        {
            if (*it == *it1)
                throw std::runtime_error("Error");
        }
    }
}

double getTime(){
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return (time.tv_sec * 1e6) + (time.tv_nsec / 1e3);
}

template <typename T, typename M, typename K>
void    fill_main_pend(M &main, M &pend, T &non_part, T &cont, K &know_end, int NNP)
{
    typename T::iterator it;
    typename T::iterator it1;
    T fill_main;
    int i = 0;
    int j = 0;

    for (it = cont.begin(); it != cont.end(); it++)
    {
        if (it == cont.begin())
        {
            j = i + NNP;
            for (; i < j && it != cont.end(); i++)
            {
                fill_main.push_back(*it);
                it++;
            }
            main.push_back(fill_main);
            fill_main.clear();
            it--;
        }
        else
        {
            j = i + NNP;
            if ((unsigned long)j <= cont.size())
            {
                for (; i < j && it != cont.end(); i++)
                {
                    fill_main.push_back(*it);
                    it++;
                }
                main.push_back(fill_main);
                fill_main.clear();
            }
            j = i + NNP;
            for (; i < j && it != cont.end(); i++)
            {
                fill_main.push_back(*it);
                it++;
            }
            pend.push_back(fill_main);
            fill_main.clear();
            it--;
        }
    }

    for (unsigned long i = 0; i < pend.size(); i++)
    {
        if (pend[i].size() < (unsigned long)NNP)
        {
            for (it = pend[i].begin(); it != pend[i].end(); it++)
                non_part.push_back(*it);
            pend.erase(pend.begin() + i);
        }
    }

    unsigned long s = 2;
    for (unsigned long j = 0; j < pend.size(); j++)
    {
        if (s < main.size())
            know_end.push_back(std::make_pair(*(pend[j].end() - 1), *(main[s].end() - 1)));
        else
            know_end.push_back(std::make_pair(*(pend[j].end() - 1), -1));
        s++;
    }
}

size_t get_JN(size_t n)
{
    if (n == 0)
        return 0;
    else if (n == 1)
        return 1;
    else
        return (get_JN(n - 1) + (2 * get_JN(n - 2)));
}

template <typename Main>
struct _cmp{
    bool operator()(const Main &a, const double value){
        ncmp++;
        return a[a.size() - 1] < value;
    }
};

template <typename K, typename M>
int    finde_range(double f, K &know_end, M &main)
{
    int range = 0;
    double save;

    for (unsigned long i = 0; i < know_end.size(); i++)
    {
        if (know_end[i].first == f)
        {
            save = know_end[i].second;
            break;
        }
    }
    
    for (typename M::iterator it = main.begin(); it != main.end(); it++)
    {
        if (save == *(it->end() - 1))
            break;
        range++;
    }
    return range;
}

template <typename T, typename M, typename K>
void    insert_with_JN(M &main, M &pend, T &cont, K &know_end)
{
    size_t jn = 1;
    int range;
    typename M::iterator it;
    typename T::iterator it1;
    _cmp<typename M::value_type > cmp;
    (void)cont;

    for (; pend.size() > 0;){
        size_t p_jn = jn;
        for (size_t i = 0;; i++)
        {
            size_t n = get_JN(i);
            if (n > jn)
            {
                jn = n;
                break;
            }
        }
        if (pend.size() >= jn - p_jn)
        {
            for (int i = jn - p_jn; i > 0; i--)
            {
                range = finde_range(*(pend[i - 1].end() - 1), know_end, main);
                it = std::lower_bound(main.begin(), main.begin() + range, *(pend[i - 1].end() - 1), cmp);
                main.insert(it, pend[i - 1]);
                pend.erase(pend.begin() + i - 1);
            }
        }
        else
        {
            for (; pend.size() > 0;)
            {
                range = finde_range(*(pend.begin()->end() - 1), know_end, main);
                it = std::lower_bound(main.begin(), main.begin() + range, *(pend.begin()->end() - 1), cmp);
                main.insert(it, *(pend.begin()));
                pend.erase(pend.begin());
            }
        }
    }
}

template <typename T, typename M, typename K>
void   merge_insert_sort(T &cont, M &main, K &know_end,int NNP)
{
    M pend;
    T non_part;
    typename T::iterator it;
    typename T::iterator it_f;
    typename T::iterator it_s;

    for (it = cont.begin(); it != cont.end(); it--)
    {
        it_f = it + (NNP - 1);
        it_s = it_f + NNP;
        if (it_f < cont.end() && it_s < cont.end())
        {
            ncmp++;
            if (*it_f > *it_s)
                std::swap_ranges(it_f - (NNP - 1), it_f + 1, it_s - (NNP - 1));
        }
        else
            break;
        it += NNP * 2 + 1;
    }
    if ((unsigned long)NNP * 4 <= cont.size())
        merge_insert_sort(cont, main, know_end, NNP * 2);
    fill_main_pend(main, pend, non_part, cont, know_end, NNP);
    if (pend.size() > 0)
        insert_with_JN(main, pend, cont, know_end);
    cont.clear();
    for (size_t i = 0; i < main.size(); i++)
    {
        for (typename T::iterator i1 = main[i].begin(); i1 != main[i].end(); i1++)
            cont.push_back(*i1);
    }
    for (typename T::iterator i1 = non_part.begin(); i1 != non_part.end(); i1++)
        cont.push_back(*i1);
    non_part.clear();
    for (unsigned long i = 0; i < main.size(); i++)
        main[i].clear();
    main.clear();
    for (unsigned long i = 0; i < pend.size(); i++)
        pend[i].clear();
    pend.clear();
    non_part.clear();
    know_end.clear();
}

void pmergeme(char **args){
    std::vector<double> a;
    std::deque<double> b;
    std::vector<std::vector<double> > main_vector;
    std::deque<std::deque<double> > main_deque;
    std::vector<std::pair<double, double> > know_endz_v;
    std::vector<std::pair<double, double> > know_endz_d;

    parse_args(++args, a, b);
    std::cout << "Before: ";
    std::vector<double>::iterator it;
    for (it = a.begin(); it != a.end(); it++)
        std::cout << *it << ' ';
    std::cout << std::endl;
    
    double now = getTime();
    merge_insert_sort(a,main_vector, know_endz_v, 1);
    std::cout << "After: ";
    for (it = a.begin(); it != a.end(); it++)
        std::cout << *it << ' ';
    std::cout << std::endl;
    double curr_time = getTime();
    std::cout << "Time to process a range of " << a.size() << " elements with std::[vector] : " << curr_time - now << " us" << std::endl;
    std::cout << "ncmp: " << ncmp << std::endl;

    now = getTime();
    merge_insert_sort(b, main_deque, know_endz_d, 1);
    curr_time = getTime();
    std::cout << "Time to process a range of " << a.size() << " elements with std::[deque] : " << curr_time - now << " us" << std::endl;
    
    
    if (std::is_sorted(a.begin(), a.end()))
    {
        std::cout << "is sorted" << std::endl;
        
    }
    
}
