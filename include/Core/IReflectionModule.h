#pragma once

#include "fstream"

#include "RuntimeReflection/ReflectionGenerator.h"

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine {

    // TODO: I dont want to inherit from HotReloader::IRuntimeObject every time!!!!!!!
    struct IReflectionModule : public HotReloader::IRuntimeObject
    {
        virtual void GenerateSource(std::ofstream& out) = 0;
        virtual void GenerateHeader(std::vector<std::string>& out) = 0;

        virtual void ProcessStructure(ReflectionGenerator::StructScope* info) = 0;
        virtual void ProcessProperty(ReflectionGenerator::PropertyScope* info) = 0;
        virtual void ProcessFunction(ReflectionGenerator::FunctionScope* info) = 0;
        virtual void ProcessConstructor(ReflectionGenerator::ConstructorScope* info) { }
    };

}