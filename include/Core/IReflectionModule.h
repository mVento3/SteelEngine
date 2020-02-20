#pragma once

#include "fstream"

#include "RuntimeReflection/Parser.h"

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine {

    // TODO: I dont want to inherit from HotReloader::IRuntimeObject every time!!!!!!!
    struct IReflectionModule : public HotReloader::IRuntimeObject
    {
        virtual void GenerateSource(std::ofstream& out) = 0;
        virtual void GenerateHeader(std::vector<std::string>& out) = 0;

        virtual void ProcessStructure(Parser::StructScope* info) = 0;
        virtual void ProcessProperty(Parser::PropertyScope* info) = 0;
        virtual void ProcessFunction(Parser::FunctionScope* info) = 0;
        virtual void ProcessConstructor(Parser::ConstructorScope* info) { }
    };

}