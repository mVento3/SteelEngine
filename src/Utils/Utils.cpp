#include "Utils/Utils.h"

#include "Windows.h"

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

void removeSpaces(std::string& str)
{
	int n = str.length();
	int i = 0;
	int j = -1;

	bool spaceFound = false;

	while(++j < n && str[j] == ' ');

	while(j < n)
	{
		if (str[j] != ' ')
		{
			if ((str[j] == '.' || str[j] == ',' ||
				str[j] == '?') && i - 1 >= 0 &&
				str[i - 1] == ' ')
			{
				str[i - 1] = str[j++];
			}
			else
			{
				str[i++] = str[j++];
			}

			spaceFound = false;
		}
		else if(str[j++] == ' ')
		{
			if(!spaceFound)
			{
				str[i++] = ' ';
				spaceFound = true;
			}
		}
	}

	if (i <= 1)
	{
		str.erase(str.begin() + i, str.end());
	}
	else
	{
		str.erase(str.begin() + i - 1, str.end());
	}
}

std::filesystem::path getBinaryLocation()
{
	char binPath[64];
	GetModuleFileNameA(GetModuleHandleA(0), binPath, 64);

	return std::filesystem::path(binPath).parent_path();
}