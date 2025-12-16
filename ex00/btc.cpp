#include "BitcoinExchange.hpp"

int main(int ac, char** av)
{
	if (ac == 1)
		return (std::cerr << "Error: could not open file." << std::endl, 0);
	else
	{
		std::ifstream inFile("data.csv");
		if (!inFile.is_open())
			return (std::cerr << "Error can not open the file data.csv" << std::endl, 0);
		std::string line;
		std::map<std::string, float> data;
		size_t pos;
		std::getline(inFile, line);
		while (std::getline(inFile, line))
		{
			pos = line.find(",");
			data[line.substr(0, pos)] = std::strtof(line.substr(pos + 1, line.size() - pos).c_str(), NULL);
		}
		inFile.close();
		std::ifstream inFile1(av[1]);
		if (!inFile1.is_open())
			return (std::cerr << "Error can not open the file" << std::endl, 0);
		std::string line1;
		std::getline(inFile1, line1);
		if (line1.compare("date | value"))
			return (std::cerr << "Error syntax in file " << av[1] << std::endl, 0);
		double value;
		while (std::getline(inFile1, line1))
		{
			parse_line1(line1, value);
		}
	}
	return 1;
}
