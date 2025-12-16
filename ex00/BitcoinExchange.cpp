#include "BitcoinExchange.hpp"

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
    printf("--%s\n", value.c_str());
    val = std::strtod(value.c_str(), &end);
    if (val > 21000000)
        return (std::cerr << "Error: bad input => " << line1 << std::endl, false);
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
    line1 = date;
    return true;
}