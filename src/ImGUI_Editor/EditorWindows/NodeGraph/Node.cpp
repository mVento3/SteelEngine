#include "ImGUI_Editor/EditorWindows/NodeGraph/Node.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include "ImGUI_Editor/EditorWindows/BlueprintWindow.h"

namespace SteelEngine { namespace NodeGraph {

    Node::Node(const ImVec2& position, const ImVec2& size, const std::string& name) :
        m_Position(position),
        m_Size(size),
        m_Name(name)
    {

    }

    Node::~Node()
    {

    }

    const std::vector<VisualScript::IPin*>& Node::GetInputs()
    {
        return m_Inputs;
    }

    const std::vector<VisualScript::IPin*>& Node::GetOutputs()
    {
        return m_Outputs;
    }

    void Node::AddOutputNode(VisualScript::IPin* pin)
    {
        m_Outputs.push_back(pin);
    }

    void Node::AddInputNode(VisualScript::IPin* pin)
    {
        m_Inputs.push_back(pin);
    }

    const std::string& Node::GetNodeName() const
    {
        return m_Name;
    }

    int Node::GetNodeID() const
    {
        return m_ID;
    }

    void Node::Invoke(Type::uint32 index)
    {
        VisualScript::IPin* pin = m_Outputs[index];

        VisualScript::IPinFunction* out = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)out).Convert<VisualScript::IPinFunction*>();

        if(pin->GetPinType() == VisualScript::IPin::PinType::FLOW)
        {
            std::vector<VisualScript::IPin*> ins = pin->GetConnections();

            for(VisualScript::IPin* in : ins)
            {
                VisualScript::IPinData* pin2 = Reflection::GetType("SteelEngine::NodeGraph::InputPin")->Invoke("Cast_IPinData", (NodeGraph::InputPin*)in).Convert<VisualScript::IPinData*>();

                out->Call(this, pin2);
            }
        }
    }

    void Node::SetNodeTemplate(VisualScript::NodeTemplate* template_)
    {
        m_Template = template_;
    }

    VisualScript::NodeTemplate* Node::GetNodeTemplate() const
    {
        return m_Template;
    }

    void Node::Init(bool genericSize)
    {
        m_HeaderSize = ImGui::CalcTextSize(m_Name.c_str());

        m_HeaderSize.y += 4;

        for(Type::uint32 i = 0; i < m_Inputs.size(); i++)
        {
            VisualScript::IPin* pin = Reflection::GetType("SteelEngine::NodeGraph::InputPin")->Invoke("Cast_IPin", (NodeGraph::InputPin*)m_Inputs[i]).Convert<VisualScript::IPin*>();

            pin->SetNode(this);
        }

        for(Type::uint32 i = 0; i < m_Outputs.size(); i++)
        {
            VisualScript::IPin* pin = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPin", (NodeGraph::OutputPin*)m_Outputs[i]).Convert<VisualScript::IPin*>();

            pin->SetNode(this);
        }

        if(genericSize)
        {
            Type::uint32 yInputsSize = 0;
            Type::uint32 yOutputsSize = 0;

            for(Type::uint32 i = 0; i < m_Inputs.size(); i++)
            {
                yInputsSize += BlueprintWindow::NODE_SLOT_RADIUS * 6;
            }

            for(Type::uint32 i = 0; i < m_Outputs.size(); i++)
            {
                yOutputsSize += BlueprintWindow::NODE_SLOT_RADIUS * 6;
            }

            m_Size.y = (yInputsSize < yOutputsSize ? yOutputsSize : yInputsSize) + m_HeaderSize.y;
            m_Size.x = 200;
        }
    }

    void Node::Draw(ImDrawList* drawList)
    {
        ImVec2 position = ImGui::GetCursorScreenPos();
        const char* label = m_Name.c_str();

        drawList->AddRectFilled(position, position + ImVec2(m_Size.x, m_HeaderSize.y), ImColor(100, 0, 0, 100), 4.f);

        ImGui::SetCursorScreenPos(ImVec2(position.x + (m_Size.x / 2) - (m_HeaderSize.x / 2), position.y));
        ImGui::Text(label);
    }

    ImVec2 Node::GetOutputSlotPosition(int id) const
    {
        for(Type::uint32 i = 0; i < m_Outputs.size(); i++)
        {
            VisualScript::IPin* pin = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPin", (NodeGraph::OutputPin*)m_Outputs[i]).Convert<VisualScript::IPin*>();

            if(pin->GetID() == id)
            {
                return ImVec2(m_Position.x + m_Size.x, m_Position.y + ((i + 1) * ((BlueprintWindow::NODE_SLOT_RADIUS * 4))));
            }
        }
    }

    ImVec2 Node::GetInputSlotPosition(int id) const
    {
        for(Type::uint32 i = 0; i < m_Inputs.size(); i++)
        {
            VisualScript::IPin* pin = Reflection::GetType("SteelEngine::NodeGraph::InputPin")->Invoke("Cast_IPin", (NodeGraph::InputPin*)m_Inputs[i]).Convert<VisualScript::IPin*>();

            if(pin->GetID() == id)
            {
                return ImVec2(m_Position.x, m_Position.y + ((i + 1) * ((BlueprintWindow::NODE_SLOT_RADIUS * 4))));
            }
        }
    }

}}