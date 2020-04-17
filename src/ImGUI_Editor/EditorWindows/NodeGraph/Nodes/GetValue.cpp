#include "ImGUI_Editor/EditorWindows/NodeGraph/Nodes/GetValue.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/InputPin.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputPin.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/FlowInputPin.h"

namespace SteelEngine {

    GetValue::GetValue()
    {
        m_TypeID = Reflection::GetType<GetValue>()->GetTypeID();
    }

    GetValue::~GetValue()
    {

    }

    void GetValue::ChangeFunc(const IReflectionMethod* meth)
    {
        const IReflectionData* type = Reflection::GetType("SteelEngine::NodeGraph::OutputPin");
        VisualScript::IPinFunction* func =
            type->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)GetOutputs()[0]).Convert<VisualScript::IPinFunction*>();

        func->SetFunc((VisualScript::IPinFunction::ReflectedOutputFunction*)meth);
    }

    void GetValue::Serialize(Utils::json& j)
    {
        INode::Serialize(j);

        j["type"] = getFullTypename(Reflection::GetType<GetValue>());
        j["property"] = m_Property->GetName();
    }

    void GetValue::Deserialize(const Utils::json& j)
    {
        INode::Deserialize(j);

        m_Property = m_PropertyType->GetProperty(j["property"].get<std::string>().c_str());
    }

    void GetValue::GetFloat(NodeGraph::INode* node, VisualScript::IPinData* input)
    {
        input->SetData(m_Property->GetValue(m_Object->GetObject_()).Convert<float>());
    }

    // void GetValue::GetInt(NodeGraph::INode* node, VisualScript::IPinData* input)
    // {
    //     input->SetData(m_Property->GetValue(m_Object->GetObject_()).Convert<int>());
    // }

}