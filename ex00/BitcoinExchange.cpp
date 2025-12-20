#include "BitcoinExchange.hpp"

bool    parse_date(std::string &date, std::string &line1)
{
    int year = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day = atoi(date.substr(8, 2).c_str());

    if (year <= 2009)
    {
        if (year < 2009)
            return (std::cerr << "Error: btc did not exist in this date => " << line1 << std::endl, false);
        else if (day < 3)
            return (std::cerr << "Error: btc did not exist in this date => " << line1 << std::endl, false);
    }
    if (month > 12)
        return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        if (day > 31)
            return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30)
            return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
    }
    else if (month == 2)
    {
        if (day > 29)
            return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
    }
    line1 = date;
    return true;
}

bool    parse_line1(std::string &line1, double &val){
    size_t pos;
    std::string date;
    std::string value;
    char *end = NULL;

	pos = line1.find(" | ");
    if (pos == std::string::npos)
        return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
    date = line1.substr(0, pos);
    value = line1.substr(pos + 3, line1.size() - pos);
    val = std::strtod(value.c_str(), &end);
    if (val > 1000)
        return (std::cerr << "Error: too large a number." << std::endl, false);
    if (val < 0)
        return (std::cerr << "Error: not a positive number." << std::endl, false);
    if (*end != '\0' || value[0] == ' ')
        return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
    if (date.size() > 10 || date[4] != '-' || date[7] != '-')
        return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
    for (size_t i = 0; date[i]; i++)
    {
        if (i == 4 || i == 7)
            continue;
        else if (date[i] < '0' || date[i] > '9')
            return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
    }
   return (parse_date(date, line1));
}

void    print_the_result(std::map<std::string, float> &data ,std::string &date, double &value){
    std::map<std::string, float>::iterator it = data.lower_bound(date);

    if (data.size() == 0)
    {
        std::cerr << "Error the Data is empty." << std::endl;
        return ;
    }
    if (it != data.begin() && it->first.compare(date))
        --it;
    std::cout << date << " => " << value << " = " << value * it->second << std::endl;
}
