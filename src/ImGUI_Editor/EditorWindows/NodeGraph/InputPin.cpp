#include "ImGUI_Editor/EditorWindows/NodeGraph/InputPin.h"

namespace SteelEngine { namespace NodeGraph {

    InputPin::InputPin(VisualScript::IPin::PinType type, VisualScript::IPin::VariableType variableType) :
        m_Type(type),
        m_VariableType(variableType)
    {
        m_Connected = false;
        m_Color = ImColor(150, 150, 150);
        m_BorderColor = ImColor(255, 255, 255);
        m_Flow = VisualScript::IPin::Flow::INPUT;

        m_Data = 0;
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

    size_t InputPin::GetTypeID() const
    {
        return m_TypeID;
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