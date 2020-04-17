#pragma once

#include "VisualScript/IPin.h"
#include "VisualScript/IPinFunction.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/Macro.h"

#include "Serialization/JsonSerialization.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputPin.Generated.h"

namespace SteelEngine {

    class BlueprintWindow;

    namespace NodeGraph {

    class INode;

    SE_CLASS(
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS
    )
    class OutputPin :
        public VisualScript::IPin,
        public VisualScript::IPinFunction,
        public JsonSerialization
    {
    // TODO: Change that to not be friends
        friend class BlueprintWindow;
        GENERATED_BODY
    private:
        INode* m_Node;
        int m_ID;
        std::string m_Name;
        bool m_Connected;
        size_t m_TypeID;

        ImColor m_Color;
        ImColor m_BorderColor;
        ImColor m_TextColor;

        std::vector<VisualScript::IPin*> m_Connections;
        std::stack<PendingConnection> m_PendingConnections;

        VisualScript::IPinFunction::OutputFunction m_Func;
        VisualScript::IPinFunction::ReflectedOutputFunction* m_Func2;

        void SetFlowType(Flow flow) override { m_Flow = flow; }
        void SetPinType(PinType pinType) override { m_Type = pinType; }
        void SetVariableType(VariableType variableType) override { m_VariableType = variableType; }

        void Serialize(Utils::json& j) override;
        void Deserialize(const Utils::json& j) override;

        size_t GetTypeID() const override { return m_TypeID; }
        void SetTypeID(size_t typeID) override { m_TypeID = typeID; }

    public:
        OutputPin();
        OutputPin(VisualScript::IPin::PinType type, VisualScript::IPin::VariableType variableType = VisualScript::IPin::VariableType::NONE);
        ~OutputPin();

        Flow GetFlowType() const override;
        PinType GetPinType() const override;
        VariableType GetVariableType() const override;

        OutputFunction GetOutputFunction() const override;
        void SetFunc(OutputFunction func) override;

        ReflectedOutputFunction* GetReflectedOutputFunction() const override;
        void SetFunc(ReflectedOutputFunction* func) override;

        VisualScript::INodeData* GetNode() override { return (VisualScript::INodeData*)m_Node; }
        void SetNode(VisualScript::INodeData* node) override { m_Node = (INode*)node; }

        bool IsConnected() const override { return m_Connected; }
        void SetConnected(bool connected) override { m_Connected = connected; }

        int GetID() const override { return m_ID; }
        void SetID(int id) override { m_ID = id; }

        std::vector<IPin*>& GetConnections() override;
        void AddConnection(VisualScript::IPin* pin) override;

        std::stack<PendingConnection>& GetPendingConnections() override { return m_PendingConnections; }

        const std::string& GetName() const override;

        VisualScript::IPin::PinType m_Type;
        VisualScript::IPin::Flow m_Flow;
        VisualScript::IPin::VariableType m_VariableType;
    };

}}