#include "ImGUI_Editor/EditorWindows/NodeGraph/INode.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include "ImGUI_Editor/EditorWindows/BlueprintWindow.h"

#include "Utils/Json.h"

#include "Serialization/JsonSerialization.h"

#include "RuntimeReflection/Reflection.h"

static void to_json(SteelEngine::Utils::json& j, const ImVec2& d)
{
    j["x"] = d.x;
    j["y"] = d.y;
}

static void from_json(const SteelEngine::Utils::json& j, ImVec2& d)
{
    d.x = j["x"];
    d.y = j["y"];
}

namespace SteelEngine { namespace NodeGraph {

    INode::INode()
    {
        m_Position = ImVec2(0, 0);
        m_Size = ImVec2(0, 0);
        m_Name = "";
    }

    INode::INode(const ImVec2& position, const ImVec2& size, const std::string& name) :
        m_Position(position),
        m_Size(size),
        m_Name(name)
    {

    }

    INode::~INode()
    {

    }

    const std::vector<VisualScript::IPin*>& INode::GetInputs()
    {
        return m_Inputs;
    }

    const std::vector<VisualScript::IPin*>& INode::GetOutputs()
    {
        return m_Outputs;
    }

    void INode::AddOutputNode(VisualScript::IPin* pin)
    {
        m_Outputs.push_back(pin);
    }

    void INode::AddInputNode(VisualScript::IPin* pin)
    {
        m_Inputs.push_back(pin);
    }

    const std::string& INode::GetNodeName() const
    {
        return m_Name;
    }

    int INode::GetNodeID() const
    {
        return m_ID;
    }

    void INode::Init(bool genericSize)
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

    void INode::Draw(ImDrawList* drawList)
    {
        ImVec2 position = ImGui::GetCursorScreenPos();
        const char* label = m_Name.c_str();

        drawList->AddRectFilled(position, position + ImVec2(m_Size.x, m_HeaderSize.y), ImColor(100, 0, 0, 100), 4.f);

        ImGui::SetCursorScreenPos(ImVec2(position.x + (m_Size.x / 2) - (m_HeaderSize.x / 2), position.y));
        ImGui::Text(label);
    }

    ImVec2 INode::GetOutputSlotPosition(int id) const
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

    ImVec2 INode::GetInputSlotPosition(int id) const
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

    void INode::Invoke(Type::uint32 index)
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

    void INode::Serialize(Utils::json& j)
    {
        j["id"] = m_ID;
        j["position"] = m_Position;
        j["size"] = m_Size;

        j["inputs"] = Utils::json::array();
        j["outputs"] = Utils::json::array();

        for(VisualScript::IPin* pin : m_Inputs)
        {
            Utils::json jPin;
            IReflectionData* type = Reflection::GetType(pin->GetTypeID());
            JsonSerialization* json = type->Invoke("Cast_JsonSerialization", pin).Convert<JsonSerialization*>();

            json->Serialize(jPin);

            j["inputs"].push_back(jPin);
        }

        for(VisualScript::IPin* pin : m_Outputs)
        {
            Utils::json jPin;
            IReflectionData* type = Reflection::GetType(pin->GetTypeID());
            JsonSerialization* json = type->Invoke("Cast_JsonSerialization", pin).Convert<JsonSerialization*>();

            json->Serialize(jPin);

            j["outputs"].push_back(jPin);
        }

        j["name"] = m_Name;
        j["headerSize"] = m_HeaderSize;
    }

    void INode::Deserialize(const Utils::json& j)
    {
        m_ID = j["id"];
        m_Position = j["position"];
        m_Size = j["size"];

        Utils::json ins = j["inputs"];
        Utils::json::iterator inputIt = ins.begin();
        Utils::json outs = j["outputs"];
        Utils::json::iterator outputIt = outs.begin();

        for(VisualScript::IPin* pin : m_Inputs)
        {
            if(inputIt == ins.end())
            {
                break;
            }

            const IReflectionData* type = Reflection::GetType((*inputIt)["pin"].get<std::string>());
            JsonSerialization* json = type->Invoke("Cast_JsonSerialization", pin).Convert<JsonSerialization*>();

            json->Deserialize(*inputIt);

            ++inputIt;
        }

        for(VisualScript::IPin* pin : m_Outputs)
        {
            if(outputIt == outs.end())
            {
                break;
            }

            const IReflectionData* type = Reflection::GetType((*outputIt)["pin"].get<std::string>());
            JsonSerialization* json = type->Invoke("Cast_JsonSerialization", pin).Convert<JsonSerialization*>();

            json->Deserialize(*outputIt);

            ++outputIt;
        }

        m_Name = j["name"];
        m_HeaderSize = j["headerSize"];
    }

}}