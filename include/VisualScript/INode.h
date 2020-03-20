#pragma once

#include "RuntimeReflection/IReflectionData.h"
#include "RuntimeReflection/IProxyMethod.h"

#include "VisualScript/INodeData.h"

namespace SteelEngine { namespace VisualScript {

    struct INode : public INodeData
    {
        virtual const std::vector<IPin*>& GetInputs() override = 0;
        virtual const std::vector<IPin*>& GetOutputs() override = 0;

        virtual void AddOutputNode(IPin* pin) override = 0;
        virtual void AddInputNode(IPin* pin) override = 0;

        virtual const std::string& GetNodeName() const = 0;
        virtual int GetNodeID() const = 0;

        virtual void SetNodeTemplate(NodeTemplate* template_) override = 0;
        virtual NodeTemplate* GetNodeTemplate() const override = 0;

        virtual void Invoke(Type::uint32 index) = 0;
    };

}}