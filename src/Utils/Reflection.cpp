#include "Utils/Reflection.h"

#include "vector"

#include "RuntimeReflection/IReflectionData.h"

namespace SteelEngine {

    std::string getFullTypename(const IReflectionData* type)
    {
        std::vector<std::string> combined = type->GetNamespaces();
        std::string typeName;

        for(std::string item : combined)
        {
            typeName += item + "::";
        }

        typeName += type->GetTypeName();

        return typeName;
    }

}