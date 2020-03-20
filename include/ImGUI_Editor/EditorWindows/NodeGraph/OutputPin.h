#pragma once

#include "VisualScript/IPin.h"
#include "VisualScript/IPinFunction.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/Macro.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputPin.Generated.h"

namespace SteelEngine {

    class BlueprintWindow;

    namespace NodeGraph {

    SE_CLASS(
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS
    )
    class OutputPin :
        public VisualScript::IPin,
        public VisualScript::IPinFunction
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

        VisualScript::IPinFunction::OutputFunction m_Func;
        VisualScript::IPinFunction::ReflectedOutputFunction* m_Func2;

    public:
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