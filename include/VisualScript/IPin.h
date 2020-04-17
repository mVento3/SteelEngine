#pragma once

#include "string"
#include "vector"

#include "VisualScript/INode.h"

namespace SteelEngine { namespace VisualScript {

    struct IPin
    {
    public:
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

        struct PendingConnection
        {
            int m_ID;
            Flow m_Flow;
        };

    protected:
        virtual void SetFlowType(Flow flow) = 0;
        virtual void SetPinType(PinType pinType) = 0;
        virtual void SetVariableType(VariableType variableType) = 0;

        virtual void SetTypeID(size_t typeID) = 0;

    public:
        virtual Flow GetFlowType() const = 0;
        virtual PinType GetPinType() const = 0;
        virtual VariableType GetVariableType() const = 0;

        virtual INodeData* GetNode() = 0;
        virtual void SetNode(INodeData* node) = 0;

        virtual bool IsConnected() const = 0;
        virtual void SetConnected(bool connected) = 0;

        virtual int GetID() const = 0;
        virtual void SetID(int id) = 0;

        virtual size_t GetTypeID() const = 0;

        virtual std::vector<IPin*>& GetConnections() = 0;
        virtual std::stack<PendingConnection>& GetPendingConnections() = 0;
        virtual void AddConnection(IPin* pin) = 0;

        void ConnectPendingPins(const std::vector<INode*>& nodes)
        {
            std::stack<PendingConnection>& pendingConnections = GetPendingConnections();

            while(!pendingConnections.empty())
            {
                PendingConnection pending = pendingConnections.top();

                for(VisualScript::INode* node : nodes)
                {
                    std::vector<IPin*>& connections = GetConnections();
                    IPin* pin = 0;

                    if(pending.m_Flow == VisualScript::IPin::Flow::INPUT)
                    {
                        for(VisualScript::IPin* pin : node->GetInputs())
                        {
                            if(pin->GetID() == pending.m_ID)
                            {
                                connections.push_back(pin);
                                SetConnected(true);
                            }
                        }
                    }
                    else if(pending.m_Flow == VisualScript::IPin::Flow::OUTPUT)
                    {
                        for(VisualScript::IPin* pin : node->GetOutputs())
                        {
                            if(pin->GetID() == pending.m_ID)
                            {
                                connections.push_back(pin);
                                SetConnected(true);
                            }
                        }
                    }
                }

                pendingConnections.pop();
            }
        }

        virtual const std::string& GetName() const = 0;
    };

}}