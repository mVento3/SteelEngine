#include "ImGUI_Editor/EditorWindows/NodeGraph/Nodes/AddFloat.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/InputPin.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputPin.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/FlowInputPin.h"

namespace SteelEngine {

    AddFloat::AddFloat()
    {
        m_InputPin = Reflection::GetType("SteelEngine::NodeGraph::InputPin");
        m_OutputPin = Reflection::GetType("SteelEngine::NodeGraph::OutputPin");

        AddInputPin([&](VisualScript::INodeData* node, VisualScript::IPinData* input)
        {
            std::vector<VisualScript::IPin*> inputs = node->GetInputs();

            for(VisualScript::IPin* pin : inputs)
            {
                if(pin->GetPinType() == VisualScript::IPin::PinType::VARIABLE)
                {
                    std::vector<VisualScript::IPin*> conns = pin->GetConnections();

                    for(VisualScript::IPin* conn : conns)
                    {
                        VisualScript::IPinFunction* func = m_OutputPin->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)conn).Convert<VisualScript::IPinFunction*>();
                        VisualScript::IPinData* in = m_InputPin->Invoke("Cast_IPinData", (NodeGraph::InputPin*)pin).Convert<VisualScript::IPinData*>();

                        func->Call(conn->GetNode(), in);
                    }
                }
            }

            VisualScript::IPin* flowOutPin = node->GetOutputs()[1];
            VisualScript::IPinFunction* flowOutRaw = m_OutputPin->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)flowOutPin).Convert<VisualScript::IPinFunction*>();
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
        AddInputPin<float>(VisualScript::IPin::PinType::VARIABLE, "a");
        AddInputPin<float>(VisualScript::IPin::PinType::VARIABLE, "b");
    }

    AddFloat::~AddFloat()
    {

    }

    void AddFloat::Func(VisualScript::INodeData* node, VisualScript::IPinData* input)
    {
        AddFloat* template_ = (AddFloat*)node->GetNodeTemplate();

        VisualScript::IPinData* pin1 = template_->m_InputPin->Invoke("Cast_IPinData", (NodeGraph::InputPin*)node->GetInputs()[1]).Convert<VisualScript::IPinData*>();
        VisualScript::IPinData* pin2 = template_->m_InputPin->Invoke("Cast_IPinData", (NodeGraph::InputPin*)node->GetInputs()[2]).Convert<VisualScript::IPinData*>();

        input->SetData(pin1->GetData<float>() + pin2->GetData<float>());
    }

    void AddFloat::Func2(VisualScript::INodeData* node, VisualScript::IPinData* input)
    {

    }

}