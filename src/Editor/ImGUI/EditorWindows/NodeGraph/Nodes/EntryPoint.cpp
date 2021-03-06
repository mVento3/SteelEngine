#include "Editor/ImGUI/EditorWindows/NodeGraph/Nodes/EntryPoint.h"

#include "Editor/ImGUI/EditorWindows/NodeGraph/InputPin.h"
#include "Editor/ImGUI/EditorWindows/NodeGraph/OutputPin.h"

namespace SteelEngine {

    EntryPoint::EntryPoint()
    {
        m_TypeID = Reflection::GetType<EntryPoint>()->GetTypeID();
    }

    EntryPoint::~EntryPoint()
    {

    }

    void EntryPoint::Serialize(Utils::json& j)
    {
        INode::Serialize(j);

        j["type"] = getFullTypename(Reflection::GetType<EntryPoint>());
    }

    void EntryPoint::Deserialize(const Utils::json& j)
    {
        INode::Deserialize(j);
    }

    void EntryPoint::Func(NodeGraph::INode* node, VisualScript::IPinData* input)
    {

    }

}