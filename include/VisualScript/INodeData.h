#pragma once

#include "vector"
#include "functional"

#include "VisualScript/IPinData.h"
#include "VisualScript/IPinFunction.h"
#include "VisualScript/IPin.h"
#include "VisualScript/NodeTemplate.h"

namespace SteelEngine { namespace VisualScript {

    struct INodeData
    {
        virtual const std::vector<IPin*>& GetInputs() = 0;
        virtual const std::vector<IPin*>& GetOutputs() = 0;

        virtual void AddOutputNode(IPin* pin) = 0;
        virtual void AddInputNode(IPin* pin) = 0;

        virtual void SetNodeTemplate(NodeTemplate* template_) = 0;
        virtual NodeTemplate* GetNodeTemplate() const = 0;

        virtual void Invoke(Type::uint32 index) = 0;
    };

}}