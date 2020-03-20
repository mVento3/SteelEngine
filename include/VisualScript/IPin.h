#pragma once

#include "string"
#include "vector"

namespace SteelEngine { namespace VisualScript {

    struct INodeData;

    struct IPin
    {
        enum PinType
        {
            FLOW,
            VARIABLE
        };

        enum Flow
        {
            INPUT,
            OUTPUT
        };

        enum VariableType
        {
            NONE,
            INTEGER,
            FLOAT,
            DOUBLE,
            STRING,
            POINTER
        };

        virtual Flow GetFlowType() const = 0;
        virtual PinType GetPinType() const = 0;
        virtual VariableType GetVariableType() const = 0;

        virtual INodeData* GetNode() = 0;
        virtual void SetNode(INodeData* node) = 0;

        virtual bool IsConnected() const = 0;
        virtual void SetConnected(bool connected) = 0;

        virtual int GetID() const = 0;

        virtual std::vector<IPin*>& GetConnections() = 0;
        virtual void AddConnection(IPin* pin) = 0;

        virtual const std::string& GetName() const = 0;
    };

}}