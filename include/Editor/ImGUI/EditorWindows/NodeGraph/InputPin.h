#pragma once

#include "imgui/imgui.h"

#include "vector"
#include "string"

#include "VisualScript/IPinData.h"
#include "VisualScript/IPin.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "Serialization/JsonSerialization.h"

#include "Editor/ImGUI/EditorWindows/NodeGraph/InputPin.Generated.h"

namespace SteelEngine {

    class BlueprintWindow;

    namespace NodeGraph {

    class INode;

    SE_CLASS(
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS
    )
    class InputPin :
        public VisualScript::IPin,
        public VisualScript::IPinData,
        public JsonSerialization
    {
        friend class BlueprintWindow;
        friend class INode;
        GENERATED_BODY
    private:
        INode* m_Node;
        int m_ID;
        std::string m_Name;
        bool m_Connected;
        size_t m_ValueTypeID;
        size_t m_TypeID;

        ImColor m_Color;
        ImColor m_BorderColor;
        ImColor m_TextColor;

        std::vector<VisualScript::IPin*> m_Connections;
        std::stack<PendingConnection> m_PendingConnections;

        void* m_Data;

        void SetFlowType(Flow flow) override { m_Flow = flow; }
        void SetPinType(PinType pinType) override { m_Type = pinType; }
        void SetVariableType(VariableType variableType) override { m_VariableType = variableType; }

        void Serialize(Utils::json& j) override;
        void Deserialize(const Utils::json& j) override;

        size_t GetTypeID() const override { return m_TypeID; }
        void SetTypeID(size_t typeID) override { m_TypeID = typeID; }

    public:
        InputPin();
        InputPin(VisualScript::IPin::PinType type, VisualScript::IPin::VariableType variableType = VisualScript::IPin::VariableType::NONE);
        ~InputPin();

        Flow GetFlowType() const override;
        PinType GetPinType() const override;
        VariableType GetVariableType() const override;

        void* GetRawData() const override;
        void SetRawData(void* data) override;
        size_t GetValueTypeID() const override;

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