#include "Utils/Utils.h"

void split(const std::string & s, char delim, std::back_insert_iterator<std::vector<std::string>> result)
{
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim))
	{
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string & s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));

	return elems;
}

void replaceAll(std::string & str, const std::string & from, const std::string & to)
{
	if (from.empty())
		return;

	size_t start_pos = 0;

	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}
