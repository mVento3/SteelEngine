#include "ImGUI_Editor/EditorWindows/NodeGraph/InputPin.h"

#include "ImGUI_Editor/EditorWindows/BlueprintWindow.h"

namespace SteelEngine { namespace NodeGraph {

    void InputPin::Serialize(Utils::json& j)
    {
        j["id"] = GetID();
        j["pin"] = "SteelEngine::NodeGraph::InputPin";
        j["variableType"] = m_VariableType;
        j["connections"] = Utils::json::array();

        if(m_VariableType == IPin::VariableType::FLOAT)
        {
            j["value"] = GetData<float>();
        }
        else if(m_VariableType == IPin::VariableType::INTEGER)
        {
            j["value"] = GetData<int>();
        }

        for(VisualScript::IPin* conn : m_Connections)
        {
            Utils::json jConn;

            jConn["id"] = conn->GetID();
            jConn["flow"] = conn->GetFlowType();

            j["connections"].push_back(jConn);
        }
    }

    void InputPin::Deserialize(const Utils::json& j)
    {
        SetID(j["id"]);
        SetVariableType(j["variableType"]);

        if(m_VariableType == IPin::VariableType::FLOAT)
        {
            m_Data = new float(j["value"]);
            m_ValueTypeID = typeid(float).hash_code();
        }
        else if(m_VariableType == IPin::VariableType::INTEGER)
        {
            m_Data = new int(j["value"]);
            m_ValueTypeID = typeid(float).hash_code();
        }

        Utils::json jConns = j["connections"];

        for(Utils::json::iterator it = jConns.begin(); it != jConns.end(); ++it)
        {
            Utils::json item = *it;

            m_PendingConnections.push(PendingConnection{ item["id"].get<int>(), item["flow"].get<VisualScript::IPin::Flow>() });
        }
    }

    InputPin::InputPin()
    {
        m_Connected = false;
        m_Color = ImColor(150, 150, 150);
        m_BorderColor = ImColor(255, 255, 255);
        m_Flow = VisualScript::IPin::Flow::INPUT;

        m_Data = 0;

        m_TypeID = Reflection::GetType<InputPin>()->GetTypeID();
        m_ValueTypeID = RuntimeDatabase::s_InvalidID;
    }

    InputPin::InputPin(VisualScript::IPin::PinType type, VisualScript::IPin::VariableType variableType) :
        InputPin()
    {
        m_Type = type;
        m_VariableType = variableType;
    }

    InputPin::~InputPin()
    {

    }

    VisualScript::IPin::Flow InputPin::GetFlowType() const
    {
        return m_Flow;
    }

    VisualScript::IPin::PinType InputPin::GetPinType() const
    {
        return m_Type;
    }

    VisualScript::IPin::VariableType InputPin::GetVariableType() const
    {
        return m_VariableType;
    }

    void* InputPin::GetRawData() const
    {
        return m_Data;
    }

    void InputPin::SetRawData(void* data)
    {
        m_Data = data;
    }

    size_t InputPin::GetValueTypeID() const
    {
        return m_ValueTypeID;
    }

    std::vector<VisualScript::IPin*>& InputPin::GetConnections()
    {
        return m_Connections;
    }

    void InputPin::AddConnection(VisualScript::IPin* pin)
    {
        m_Connections.push_back(pin);
    }

    const std::string& InputPin::GetName() const
    {
        return m_Name;
    }

}}