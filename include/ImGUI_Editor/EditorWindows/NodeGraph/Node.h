#pragma once

#include "imgui/imgui.h"

#include "string"
#include "vector"

#include "ImGUI_Editor/EditorWindows/NodeGraph/InputPin.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputPin.h"

#include "VisualScript/INode.h"

namespace SteelEngine {

    class BlueprintWindow;

    namespace NodeGraph {

    class Node : public VisualScript::INode
    {
        friend class BlueprintWindow;
    private:
        ImVec2 m_Position;
        ImVec2 m_Size;
        int m_ID;
        std::string m_Name;
        std::vector<VisualScript::IPin*> m_Inputs;
        std::vector<VisualScript::IPin*> m_Outputs;

        VisualScript::NodeTemplate* m_Template;

        ImVec2 m_HeaderSize;

    public:
        Node(const ImVec2& position, const ImVec2& size, const std::string& name);
        ~Node();

        const std::vector<VisualScript::IPin*>& GetInputs() override;
        const std::vector<VisualScript::IPin*>& GetOutputs() override;

        void AddOutputNode(VisualScript::IPin* pin) override;
        void AddInputNode(VisualScript::IPin* pin) override;

        const std::string& GetNodeName() const override;
        int GetNodeID() const override;

        void SetNodeTemplate(VisualScript::NodeTemplate* template_) override;
        VisualScript::NodeTemplate* GetNodeTemplate() const override;

        void Invoke(Type::uint32 index) override;

        void Init(bool genericSize = false);
        void Draw(ImDrawList* drawList);

        ImVec2 GetOutputSlotPosition(int id) const;
        ImVec2 GetInputSlotPosition(int id) const;
    };

}}