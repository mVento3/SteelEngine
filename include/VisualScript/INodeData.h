#pragma once

#include "vector"
#include "functional"

#include "Utils/Json.h"

namespace SteelEngine { namespace VisualScript {

    struct IPin;

    struct INodeData
    {
        virtual const std::vector<IPin*>& GetInputs() = 0;
        virtual const std::vector<IPin*>& GetOutputs() = 0;

        virtual void AddOutputNode(IPin* pin) = 0;
        virtual void AddInputNode(IPin* pin) = 0;

        virtual void Serialize(Utils::json& j) = 0;
        virtual void Deserialize(const Utils::json& j) = 0;

        virtual void Invoke(Type::uint32 index) = 0;
    };

}}