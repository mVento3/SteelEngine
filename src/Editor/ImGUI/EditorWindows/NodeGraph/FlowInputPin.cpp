#include "Editor/ImGUI/EditorWindows/NodeGraph/FlowInputPin.h"

#include "Editor/ImGUI/EditorWindows/BlueprintWindow.h"

namespace SteelEngine { namespace NodeGraph {

    void FlowInputPin::Serialize(Utils::json& j)
    {
        j["id"] = GetID();
        j["pin"] = "SteelEngine::NodeGraph::FlowInputPin";
        j["connections"] = Utils::json::array();

        for(VisualScript::IPin* conn : m_Connections)
        {
            Utils::json jConn;

            jConn["id"] = conn->GetID();
            jConn["flow"] = conn->GetFlowType();

            j["connections"].push_back(jConn);
        }
    }

    void FlowInputPin::Deserialize(const Utils::json& j)
    {
        SetID(j["id"]);

        Utils::json jConns = j["connections"];

        for(Utils::json::iterator it = jConns.begin(); it != jConns.end(); ++it)
        {
            Utils::json item = *it;

            m_PendingConnections.push(PendingConnection{ item["id"].get<int>(), item["flow"].get<VisualScript::IPin::Flow>() });
        }
    }

    FlowInputPin::FlowInputPin()
    {
        m_Connected = false;
        m_Color = ImColor(150, 150, 150);
        m_BorderColor = ImColor(255, 255, 255);

        m_Flow = VisualScript::IPin::Flow::INPUT;
        m_Type = IPin::PinType::FLOW;
        m_VariableType = IPin::VariableType::NONE;

        m_TypeID = Reflection::GetType<FlowInputPin>()->GetTypeID();
    }

    FlowInputPin::~FlowInputPin()
    {

    }

    VisualScript::IPin::Flow FlowInputPin::GetFlowType() const
    {
        return m_Flow;
    }

    VisualScript::IPin::PinType FlowInputPin::GetPinType() const
    {
        return m_Type;
    }

    VisualScript::IPin::VariableType FlowInputPin::GetVariableType() const
    {
        return m_VariableType;
    }

    VisualScript::IPinFunction::OutputFunction FlowInputPin::GetOutputFunction() const
    {
        return m_Func;
    }

    void FlowInputPin::SetFunc(OutputFunction func)
    {
        m_Func = func;
    }

    VisualScript::IPinFunction::ReflectedOutputFunction* FlowInputPin::GetReflectedOutputFunction() const
    {
        return m_Func2;
    }

    void FlowInputPin::SetFunc(ReflectedOutputFunction* func)
    {
        m_Func2 = func;
    }

    std::vector<VisualScript::IPin*>& FlowInputPin::GetConnections()
    {
        return m_Connections;
    }

    void FlowInputPin::AddConnection(VisualScript::IPin* pin)
    {
        m_Connections.push_back(pin);
    }

    const std::string& FlowInputPin::GetName() const
    {
        return m_Name;
    }

}}