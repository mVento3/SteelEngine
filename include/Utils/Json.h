#pragma once

#include "nlohmann/json.hpp"

#include "filesystem"

namespace SteelEngine { namespace Utils {

    using json = nlohmann::json;

    json loadJsonFile(const std::filesystem::path& path);

}}