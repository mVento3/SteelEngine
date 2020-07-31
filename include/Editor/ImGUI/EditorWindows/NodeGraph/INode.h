#pragma once

#include "imgui/imgui.h"

#include "string"
#include "vector"

#include "VisualScript/INode.h"
#include "VisualScript/IPin.h"

#include "Editor/ImGUI/EditorWindows/NodeGraph/FlowInputPin.h"
#include "Editor/ImGUI/EditorWindows/NodeGraph/InputPin.h"

namespace SteelEngine {

    class BlueprintWindow;

    namespace NodeGraph {

    class INode : public VisualScript::INode
    {
        friend class BlueprintWindow;
    protected:
        ImVec2 m_Position;
        ImVec2 m_Size;
        int m_ID;
        std::string m_Name;
        std::vector<VisualScript::IPin*> m_Inputs;
        std::vector<VisualScript::IPin*> m_Outputs;

        ImVec2 m_HeaderSize;

        template <typename A>
        static VisualScript::IPin* SetupInputPin(VisualScript::IPin::PinType type)
        {
            VisualScript::IPin* pin = 0;
            size_t typeID = typeid(A).hash_code();
            VisualScript::IPin::VariableType variableType = VisualScript::IPin::VariableType::NONE;

            if(typeid(float).hash_code() == typeID)
            {
                variableType = VisualScript::IPin::VariableType::FLOAT;
            }
            else if(typeid(int).hash_code() == typeID)
            {
                variableType = VisualScript::IPin::VariableType::INTEGER;
            }

            if(type == VisualScript::IPin::PinType::VARIABLE)
            {
                pin = new NodeGraph::InputPin(type, variableType);

                NodeGraph::InputPin* curr = (NodeGraph::InputPin*)pin;

                curr->m_ValueTypeID = typeID;
            }
            else if(type == VisualScript::IPin::PinType::FLOW)
            {
                pin = new NodeGraph::FlowInputPin();

                NodeGraph::FlowInputPin* curr = (NodeGraph::FlowInputPin*)pin;
            }

            return pin;
        }

    public:
        INode();
        INode(const ImVec2& position, const ImVec2& size, const std::string& name);
        ~INode();

        size_t m_TypeID;

        const std::vector<VisualScript::IPin*>& GetInputs() override;
        const std::vector<VisualScript::IPin*>& GetOutputs() override;

        void AddOutputNode(VisualScript::IPin* pin) override;
        void AddInputNode(VisualScript::IPin* pin) override;

        const std::string& GetNodeName() const override;
        int GetNodeID() const override;

        virtual void Serialize(Utils::json& j) override;
        virtual void Deserialize(const Utils::json& j) override;

        void Invoke(Type::uint32 index) override;

        void Init(bool genericSize = false);
        void Draw(ImDrawList* drawList);

        ImVec2 GetOutputSlotPosition(int id) const;
        ImVec2 GetInputSlotPosition(int id) const;
    };

}}