#include "Utils/Json.h"

#include "fstream"

namespace SteelEngine { namespace Utils {

    json loadJsonFile(const std::filesystem::path& path)
    {
        std::ifstream file(path);

        json j;

        file >> j;

        return j;
    }

}}