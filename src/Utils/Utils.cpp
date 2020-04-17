#include "Utils/Utils.h"

#include "Windows.h"

void split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result)
{
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim))
	{
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));

	return elems;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to)
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
	char path[64];

	HMODULE hm = NULL;

	if(GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&getBinaryLocation, &hm) == 0)
	{
		int ret = GetLastError();

		fprintf(stderr, "GetModuleHandle failed, error = %d\n", ret);
	}

	hm = GetModuleHandleA(0);

	if(GetModuleFileNameA(hm, path, sizeof(path)) == 0)
	{
		int ret = GetLastError();

		fprintf(stderr, "GetModuleFileName failed, error = %d\n", ret);
	}

	return std::filesystem::path(path).parent_path();
}