#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputPin.h"

#include "ImGUI_Editor/EditorWindows/BlueprintWindow.h"

namespace SteelEngine { namespace NodeGraph {

    void OutputPin::Serialize(Utils::json& j)
    {
        j["id"] = GetID();
        j["pin"] = "SteelEngine::NodeGraph::OutputPin";
        j["connections"] = Utils::json::array();

        for(VisualScript::IPin* conn : m_Connections)
        {
            Utils::json jConn;

            jConn["id"] = conn->GetID();
            jConn["flow"] = conn->GetFlowType();

            j["connections"].push_back(jConn);
        }
    }

    void OutputPin::Deserialize(const Utils::json& j)
    {
        SetID(j["id"]);

        Utils::json jConns = j["connections"];

        for(Utils::json::iterator it = jConns.begin(); it != jConns.end(); ++it)
        {
            Utils::json item = *it;

            m_PendingConnections.push(PendingConnection{ item["id"].get<int>(), item["flow"].get<VisualScript::IPin::Flow>() });
        }
    }

    OutputPin::OutputPin()
    {
        m_Connected = false;
        m_Color = ImColor(150, 150, 150);
        m_BorderColor = ImColor(255, 255, 255);
        m_Flow = VisualScript::IPin::Flow::OUTPUT;

        m_TypeID = Reflection::GetType<OutputPin>()->GetTypeID();
    }

    OutputPin::OutputPin(VisualScript::IPin::PinType type, VisualScript::IPin::VariableType variableType) :
        OutputPin()
    {
        m_Type = type;
        m_VariableType = variableType;
    }

    OutputPin::~OutputPin()
    {

    }

    VisualScript::IPin::Flow OutputPin::GetFlowType() const
    {
        return m_Flow;
    }

    VisualScript::IPin::PinType OutputPin::GetPinType() const
    {
        return m_Type;
    }

    VisualScript::IPin::VariableType OutputPin::GetVariableType() const
    {
        return m_VariableType;
    }

    VisualScript::IPinFunction::OutputFunction OutputPin::GetOutputFunction() const
    {
        return m_Func;
    }

    void OutputPin::SetFunc(OutputFunction func)
    {
        m_Func = func;
    }

    VisualScript::IPinFunction::ReflectedOutputFunction* OutputPin::GetReflectedOutputFunction() const
    {
        return m_Func2;
    }

    void OutputPin::SetFunc(VisualScript::IPinFunction::ReflectedOutputFunction* func)
    {
        m_Func2 = func;
    }

    std::vector<VisualScript::IPin*>& OutputPin::GetConnections()
    {
        return m_Connections;
    }

    void OutputPin::AddConnection(VisualScript::IPin* pin)
    {
        m_Connections.push_back(pin);
    }

    const std::string& OutputPin::GetName() const
    {
        return m_Name;
    }

}}