#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputPin.h"

namespace SteelEngine { namespace NodeGraph {

    OutputPin::OutputPin(VisualScript::IPin::PinType type, VisualScript::IPin::VariableType variableType) :
        m_Type(type),
        m_VariableType(variableType)
    {
        m_Connected = false;
        m_Color = ImColor(150, 150, 150);
        m_BorderColor = ImColor(255, 255, 255);
        m_Flow = VisualScript::IPin::Flow::OUTPUT;
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