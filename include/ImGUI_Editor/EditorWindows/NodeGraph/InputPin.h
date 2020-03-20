#pragma once

#include "imgui/imgui.h"

#include "vector"
#include "string"

#include "VisualScript/IPinData.h"
#include "VisualScript/IPin.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/InputPin.Generated.h"

namespace SteelEngine {

    class BlueprintWindow;

    namespace NodeGraph {

    class Node;

    SE_CLASS(
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS
    )
    class InputPin :
        public VisualScript::IPin,
        public VisualScript::IPinData
    {
        friend class BlueprintWindow;
        friend class Node;
        GENERATED_BODY
    private:
        Node* m_Node;
        int m_ID;
        std::string m_Name;
        bool m_Connected;

        ImColor m_Color;
        ImColor m_BorderColor;
        ImColor m_TextColor;

        std::vector<VisualScript::IPin*> m_Connections;

        void* m_Data;
        size_t m_TypeID;

    public:
        InputPin(VisualScript::IPin::PinType type, VisualScript::IPin::VariableType variableType = VisualScript::IPin::VariableType::NONE);
        ~InputPin();

        Flow GetFlowType() const override;
        PinType GetPinType() const override;
        VariableType GetVariableType() const override;

        void* GetRawData() const override;
        void SetRawData(void* data) override;
        size_t GetTypeID() const override;

        VisualScript::INodeData* GetNode() override { return (VisualScript::INodeData*)m_Node; }
        void SetNode(VisualScript::INodeData* node) override { m_Node = (Node*)node; }

        bool IsConnected() const override { return m_Connected; }
        void SetConnected(bool connected) override { m_Connected = connected; }

        int GetID() const override { return m_ID; }

        std::vector<IPin*>& GetConnections() override;
        void AddConnection(VisualScript::IPin* pin) override;

        const std::string& GetName() const override;

        VisualScript::IPin::PinType m_Type;
        VisualScript::IPin::Flow m_Flow;
        VisualScript::IPin::VariableType m_VariableType;
    };

}}