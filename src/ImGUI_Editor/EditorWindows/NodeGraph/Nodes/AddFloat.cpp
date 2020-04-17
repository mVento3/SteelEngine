#include "ImGUI_Editor/EditorWindows/NodeGraph/Nodes/AddFloat.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/InputPin.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputPin.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/FlowInputPin.h"

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

namespace SteelEngine {

    AddFloat::AddFloat()
    {
        NodeGraph::FlowInputPin* flow = new NodeGraph::FlowInputPin();

        flow->SetFunc([](VisualScript::INodeData* node, VisualScript::IPinData* input)
        {
            std::vector<VisualScript::IPin*> inputs = node->GetInputs();

            for(VisualScript::IPin* pin : inputs)
            {
                if(pin->GetPinType() == VisualScript::IPin::PinType::VARIABLE)
                {
                    std::vector<VisualScript::IPin*> conns = pin->GetConnections();

                    for(VisualScript::IPin* conn : conns)
                    {
                        VisualScript::IPinFunction* func = Reflection::GetType<NodeGraph::OutputPin>()->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)conn).Convert<VisualScript::IPinFunction*>();
                        VisualScript::IPinData* in = Reflection::GetType<NodeGraph::InputPin>()->Invoke("Cast_IPinData", (NodeGraph::InputPin*)pin).Convert<VisualScript::IPinData*>();

                        func->Call(conn->GetNode(), in);
                    }
                }
            }

            VisualScript::IPin* flowOutPin = node->GetOutputs()[1];
            VisualScript::IPinFunction* flowOutRaw = Reflection::GetType<NodeGraph::OutputPin>()->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)flowOutPin).Convert<VisualScript::IPinFunction*>();
            std::vector<VisualScript::IPin*> flowConns = flowOutPin->GetConnections();

            for(VisualScript::IPin* flowConn : flowConns)
            {
                if(flowConn->GetPinType() == VisualScript::IPin::PinType::FLOW)
                {
                    VisualScript::IPinFunction* flowConnRaw = Reflection::GetType("SteelEngine::NodeGraph::FlowInputPin")->Invoke("Cast_IPinFunction", (NodeGraph::FlowInputPin*)flowConn).Convert<VisualScript::IPinFunction*>();
                    VisualScript::INodeData* nodeData = flowConn->GetNode();

                    flowConnRaw->Call(nodeData, (VisualScript::IPinData*)flowOutPin->GetNode());
                }
            }
        });

        m_Inputs.push_back(flow);

        AddInputNode(SetupInputPin<float>(VisualScript::IPin::PinType::VARIABLE));
        AddInputNode(SetupInputPin<float>(VisualScript::IPin::PinType::VARIABLE));

        m_TypeID = Reflection::GetType<AddFloat>()->GetTypeID();
    }

    AddFloat::~AddFloat()
    {

    }

    void AddFloat::Func(NodeGraph::INode* node, VisualScript::IPinData* input)
    {
        VisualScript::IPinData* pin1 = Reflection::GetType<NodeGraph::InputPin>()->Invoke("Cast_IPinData", (NodeGraph::InputPin*)node->GetInputs()[1]).Convert<VisualScript::IPinData*>();
        VisualScript::IPinData* pin2 = Reflection::GetType<NodeGraph::InputPin>()->Invoke("Cast_IPinData", (NodeGraph::InputPin*)node->GetInputs()[2]).Convert<VisualScript::IPinData*>();

        input->SetData(pin1->GetData<float>() + pin2->GetData<float>());
    }

    void AddFloat::Func2(NodeGraph::INode* node, VisualScript::IPinData* input)
    {

    }

    void AddFloat::Serialize(Utils::json& j)
    {
        INode::Serialize(j);

        j["type"] = getFullTypename(Reflection::GetType<AddFloat>());
    }

    void AddFloat::Deserialize(const Utils::json& j)
    {
        INode::Deserialize(j);
    }

}