#include "ImGUI_Editor/EditorWindows/NodeGraph/FlowInputPin.h"

namespace SteelEngine { namespace NodeGraph {

    FlowInputPin::FlowInputPin()
    {
        m_Connected = false;
        m_Color = ImColor(150, 150, 150);
        m_BorderColor = ImColor(255, 255, 255);

        m_Flow = VisualScript::IPin::Flow::INPUT;
        m_Type = IPin::PinType::FLOW;
        m_VariableType = IPin::VariableType::NONE;
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