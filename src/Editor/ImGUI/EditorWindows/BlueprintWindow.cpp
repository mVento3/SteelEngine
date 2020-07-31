#include "Editor/ImGUI/EditorWindows/BlueprintWindow.h"

#include "imgui/imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include "functional"

#include "Core/Core.h"

#include "Input/Events/KeyDownEvent.h"
#include "Input/Events/KeyUpEvent.h"

#include "SDL.h"

#include "Editor/ImGUI/EditorWindows/NodeGraph/Nodes/GetValue.h"

#include "Profiler/ScopeTimer.h"

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

    struct ImProjectResult
    {
        ImVec2 m_Point;
        float  m_Time;
        float  m_Distance;
    };

    const ImVec2 BlueprintWindow::NODE_WINDOW_PADDING = ImVec2(4.f, 4.f);
    const float BlueprintWindow::NODE_SLOT_RADIUS = 8.0f;
    const ImColor BlueprintWindow::CONNECTION_COLOR = ImColor(180, 180, 180);
    const float BlueprintWindow::CONNECTION_THICKNESS = 2.f;
    const ImColor BlueprintWindow::CONNECTION_SELECTED_COLOR = ImColor(255, 128, 0);
    const float BlueprintWindow::CONNECTION_SELECTED_THICKNESS = 4.f;
    const Type::uint32 BlueprintWindow::CONNECTION_CURVE_SEGMENTS = 20;
    const ImColor BlueprintWindow::NODE_COLOR = ImColor(30, 30, 30);
    const ImVec2 BlueprintWindow::NODE_SHADOW_SIZE = ImVec2(5, 5);

    const ImColor BlueprintWindow::FLOAT_COLOR =            ImColor(200, 3, 148);
    const ImColor BlueprintWindow::FLOAT_BORDER_COLOR =     ImColor(255, 0, 200);
    const ImColor BlueprintWindow::INTEGER_COLOR =          ImColor(50, 0, 0);
    const ImColor BlueprintWindow::INTEGER_BORDER_COLOR =   ImColor(100, 0, 0);
    const ImColor BlueprintWindow::DOUBLE_COLOR =           ImColor(240, 3, 252);
    const ImColor BlueprintWindow::DOUBLE_BORDER_COLOR =    ImColor(240, 3, 252);
    const ImColor BlueprintWindow::STRING_COLOR =           ImColor(200, 0, 0);
    const ImColor BlueprintWindow::STRING_BORDER_COLOR =    ImColor(255, 0, 0);
    const ImColor BlueprintWindow::POINTER_COLOR =          ImColor(3, 252, 194);
    const ImColor BlueprintWindow::POINTER_BORDER_COLOR =   ImColor(3, 255, 255);

    void bezier(ImVec2 p0, ImVec2 p1, ImVec2 p2, ImVec2 p3, int numSegments, std::function<void(ImVec2)> func)
    {
        float t_step = 1.0f / (float)numSegments;

        for(int i_step = 1; i_step <= numSegments; i_step++)
        {
            func(ImBezierCalc(p0, p1, p2, p3, t_step * i_step));
        }
    }

    bool checkIfPointIsOnBezier(const std::vector<ImVec2>& positions, const ImVec2& point)
    {
        for(Type::uint32 i = 1; i < positions.size(); i++)
        {
            ImVec2 point1 = positions[i - 1];
            ImVec2 point2 = positions[i];

            int a1 = (point1.x - point2.x);

            if(a1 != 0)
            {
                float a = (point1.y - point2.y) / (float)a1;
                float b = -(a * point1.x) + point1.y;

                float aNormal = -1.f / a;
                float bFisrtNormal = -(aNormal * point1.x) + point1.y;
                float bSecondNormal = -(aNormal * point2.x) + point2.y;

                if(
                    point.y >= (a * point.x + (b - 5)) &&
                    point.y <= (a * point.x + (b + 5)))
                {
                    if(a > 0)
                    {
                        if(point.y < (aNormal * point.x + bFisrtNormal) &&
                            point.y > (aNormal * point.x + bSecondNormal))
                        {
                            return true;
                        }
                        else if(point.y > (aNormal * point.x + bFisrtNormal) &&
                            point.y < (aNormal * point.x + bSecondNormal))
                        {
                            return true;
                        }
                    }

                    if(a < 0)
                    {
                        if(point.y > (aNormal * point.x + bFisrtNormal) &&
                            point.y < (aNormal * point.x + bSecondNormal))
                        {
                            return true;
                        }
                        else if(point.y < (aNormal * point.x + bFisrtNormal) &&
                            point.y > (aNormal * point.x + bSecondNormal))
                        {
                            return true;
                        }
                    }
                }
            }
            else
            {
            // Straight line up or down
                if(point.x > point1.x - 5 && point.x < point1.x + 5 &&
                    point.y < point1.y && point.y > point2.y)
                {
                    return true;
                }
                else if(point.x > point1.x - 5 && point.x < point1.x + 5 &&
                    point.y < point2.y && point.y > point1.y)
                {
                    return true;
                }
            }
        }

        return false;
    }

    inline ImProjectResult checkIfPointIsOnBezier(const ImVec2& point, const ImVec2& p0, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const int subdivisions)
    {
        const float epsilon    = 1e-5f;
        const float fixed_step = 1.0f / static_cast<float>(subdivisions - 1);

        ImProjectResult result;

        result.m_Point    = point;
        result.m_Time     = 0.0f;
        result.m_Distance = FLT_MAX;

    // Step 1: Coarse check
        for(int i = 0; i < subdivisions; ++i)
        {
            auto t = i * fixed_step;
            auto p = ImBezierCalc(p0, p1, p2, p3, t);
            auto s = point - p;
            auto d = ImDot(s, s);

            if(d < result.m_Distance)
            {
                result.m_Point    = p;
                result.m_Time     = t;
                result.m_Distance = d;
            }
        }

        if(result.m_Time == 0.0f || ImFabs(result.m_Time - 1.0f) <= epsilon)
        {
            result.m_Distance = ImSqrt(result.m_Distance);

            return result;
        }

    // Step 2: Fine check
        auto left  = result.m_Time - fixed_step;
        auto right = result.m_Time + fixed_step;
        auto step  = fixed_step * 0.1f;

        for(auto t = left; t < right + step; t += step)
        {
            auto p = ImBezierCalc(p0, p1, p2, p3, t);
            auto s = point - p;
            auto d = ImDot(s, s);

            if(d < result.m_Distance)
            {
                result.m_Point    = p;
                result.m_Time     = t;
                result.m_Distance = d;
            }
        }

        result.m_Distance = ImSqrt(result.m_Distance);

        return result;
    }

    void nodes(VisualScript::INodeData* node, std::vector<VisualScript::INodeData*>& nodesVec)
    {
        for(Type::uint32 i = 0; i < node->GetOutputs().size(); i++)
        {
            VisualScript::IPin* pin = node->GetOutputs()[i];
            VisualScript::IPinFunction* pinData = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)pin).Convert<VisualScript::IPinFunction*>();

            for(VisualScript::IPin* conn : pin->GetConnections())
            {
                if(conn->GetPinType() == VisualScript::IPin::PinType::FLOW)
                {
                    for(VisualScript::IPin* conn2 : conn->GetConnections())
                    {
                        if(conn2 == pin)
                        {
                            nodesVec.push_back(conn->GetNode());

                            nodes(conn->GetNode(), nodesVec);
                        }
                    }
                }
            }
        }
    }

    void BlueprintWindow::PopulateConnectedNodes(std::vector<BlueprintWindow::Connection>::iterator it, VisualScript::INodeData* node, std::vector<VisualScript::INodeData*>& nodes)
    {
        if(it != m_Connections.end())
        {
            for(VisualScript::IPin* pin : node->GetOutputs())
            {
                VisualScript::IPinFunction* out = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)pin).Convert<VisualScript::IPinFunction*>();

                if((pin->GetPinType() == VisualScript::IPin::PinType::FLOW || it->m_Start->GetPinType() == VisualScript::IPin::PinType::FLOW) && pin == it->m_Start)
                {
                    VisualScript::INodeData* nextNode = it->m_End->GetNode();

                    nodes.push_back(nextNode);

                    node = nextNode;
                }
            }
        }
    }

    void BlueprintWindow::CheckIfCanConnect(VisualScript::IPin* pin1, VisualScript::IPin* pin2, bool& canConnect)
    {
        if(pin1)
        {
            if(((VisualScript::INode*)pin1->GetNode())->GetNodeID() == ((VisualScript::INode*)pin2->GetNode())->GetNodeID())
            {
                ImGui::BeginTooltip();
                {
                    ImGui::Text("Can't connect to the same node");
                }
                ImGui::EndTooltip();

                canConnect = false;
            }

            if((pin1->GetFlowType() == VisualScript::IPin::Flow::INPUT && pin2->GetFlowType() == VisualScript::IPin::Flow::INPUT) ||
                (pin1->GetFlowType() == VisualScript::IPin::Flow::OUTPUT && pin2->GetFlowType() == VisualScript::IPin::Flow::OUTPUT))
            {
                ImGui::BeginTooltip();
                {
                    ImGui::Text("Can't connect to the same pin flow");
                }
                ImGui::EndTooltip();

                canConnect = false;
            }

            if(pin1->GetVariableType() != pin2->GetVariableType())
            {
                ImGui::BeginTooltip();
                {
                    ImGui::Text("Pin variable type is not the same");
                }
                ImGui::EndTooltip();

                canConnect = false;
            }
        }
    }

    NodeGraph::INode* BlueprintWindow::AddNode(const ImVec2& position, const ImVec2& size, const IReflectionData* nodeType, bool init)
    {
        NodeGraph::INode* renderableNode = (NodeGraph::INode*)nodeType->Create();
        const Vector<IReflectionMethod>& meths = nodeType->GetMethods();

        renderableNode->m_Position = position;
        renderableNode->m_Size = size;

        for(Type::uint32 i = 0; i < meths.Size(); i++)
        {
            const IReflectionMethod* meth = meths[i];
            Variant* variant = meth->GetMetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC);

            if(!variant || !variant->IsValid())
            {
                continue;
            }

            Variant* variableTypeVariant = meth->GetMetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE);
            VisualScript::IPin::VariableType variableType = VisualScript::IPin::VariableType::NONE;

            if(variableTypeVariant && variableTypeVariant->IsValid())
            {
                size_t typeID = variableTypeVariant->Convert<size_t>();

                if(typeID == typeid(float).hash_code())
                {
                    variableType = VisualScript::IPin::VariableType::FLOAT;
                }
                else if(typeID == typeid(int).hash_code())
                {
                    variableType = VisualScript::IPin::VariableType::INTEGER;
                }
            }

            Variant* isFlow = meth->GetMetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW);

            NodeGraph::OutputPin* renderablePin = new NodeGraph::OutputPin(
                (isFlow && (isFlow->IsValid() || isFlow->Convert<bool>()) ? VisualScript::IPin::PinType::FLOW : VisualScript::IPin::PinType::VARIABLE),
                (isFlow && (isFlow->IsValid() || isFlow->Convert<bool>()) ? VisualScript::IPin::VariableType::NONE : variableType)
            );

            renderablePin->m_Name = meth->GetName();
            renderablePin->m_ID = GetID();

            renderablePin->SetFunc((VisualScript::IPinFunction::ReflectedOutputFunction*)meth);
            renderableNode->AddOutputNode(renderablePin);
        }

        for(VisualScript::IPin* pin : renderableNode->GetInputs())
        {
            pin->SetID(GetID());
            pin->SetNode(renderableNode);

            if(pin->GetPinType() == VisualScript::IPin::PinType::VARIABLE)
            {
                NodeGraph::InputPin* renderablePin = (NodeGraph::InputPin*)pin;

                if(renderablePin->GetVariableType() == VisualScript::IPin::VariableType::INTEGER)
                {
                    renderablePin->m_TextColor = INTEGER_COLOR;
                    renderablePin->m_Color = INTEGER_COLOR;
                    renderablePin->m_BorderColor = INTEGER_BORDER_COLOR;

                    renderablePin->m_Data = new int();
                    renderablePin->m_ValueTypeID = typeid(int).hash_code();
                }
                else if(renderablePin->m_VariableType == VisualScript::IPin::VariableType::FLOAT)
                {
                    renderablePin->m_TextColor = FLOAT_COLOR;
                    renderablePin->m_Color = FLOAT_COLOR;
                    renderablePin->m_BorderColor = FLOAT_BORDER_COLOR;

                    renderablePin->m_Data = new float();
                    renderablePin->m_ValueTypeID = typeid(float).hash_code();
                }
            }
        }

        renderableNode->m_Name = nodeType->GetTypeName();
        renderableNode->m_ID = GetID();

        if(init)
        {
            renderableNode->Init(true);
        }

        m_Nodes.push_back(renderableNode);

        return renderableNode;
    }

    BlueprintWindow::BlueprintWindow()
    {
        m_SelectedNodeID = -1;
        m_SelectedInputPin = 0;
        m_SelectedOutputPin = 0;
        m_IsAnyPinHovered = false;
        m_SelectedConnection = m_Connections.end();

        for(Type::uint32 i = 0; i < 256; i++)
        {
            m_Keys[i] = false;
        }

        m_SavingChanged = false;

        m_PropertyDragging = 0;
    }

    BlueprintWindow::~BlueprintWindow()
    {

    }

    void BlueprintWindow::Init()
    {
        for(Type::uint32 i = 0; i < 1000; i++)
        {
            m_AvailableIDs.push(i);
        }

        IReflectionData const* const* types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            IReflectionData const* type = types[i];
            const Vector<IReflectionInheritance>& inhs = type->GetInheritances();

            for(Type::uint32 i = 0; i < inhs.Size(); i++)
            {
                const IReflectionInheritance* inh = inhs[i];

                if(inh->GetTypeID() == typeid(NodeGraph::INode).hash_code())
                {
                    m_AvailableTemplates.push_back(type);

                    break;
                }
            }
        }

        Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::EVENT_MANAGER)->Convert<IEventManager*>()->AddEventObserver(
            Reflection::GetType<BlueprintWindow>()->Invoke("Cast_EventObserver", this).Convert<EventObserver*>()
        );
    }

    void BlueprintWindow::Draw()
    {
        m_IsAnyPinHovered = false;
        m_HoveredConnection = m_Connections.end();
        m_HoveredNodeID = -1;

        ImVec2 offset = ImGui::GetCursorScreenPos() + m_View;
        auto* drawList = ImGui::GetWindowDrawList();

        ImGui::BeginChild("properties_region", ImVec2(200, 0), true, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
        {
            if(ImGui::Button("Simulate"))
            {
                VisualScript::INodeData* startPoint = 0;

                for(NodeGraph::INode* node : m_Nodes)
                {
                    if(node->m_Name == "EntryPoint")
                    {
                        startPoint = node;

                        break;
                    }
                }

                if(startPoint != NULL)
                {
                    VisualScript::IPin* flowOutPin = startPoint->GetOutputs()[0];
                    VisualScript::IPinFunction* flowOutRaw = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)flowOutPin).Convert<VisualScript::IPinFunction*>();
                    std::vector<VisualScript::IPin*> flowConns = flowOutPin->GetConnections();

                    for(VisualScript::IPin* flowConn : flowConns)
                    {
                        if(flowConn->GetPinType() == VisualScript::IPin::PinType::FLOW)
                        {
                            VisualScript::IPinFunction* flowConnRaw = Reflection::GetType("SteelEngine::NodeGraph::FlowInputPin")->Invoke("Cast_IPinFunction", (NodeGraph::FlowInputPin*)flowConn).Convert<VisualScript::IPinFunction*>();

                            flowConnRaw->Call(flowConn->GetNode(), (VisualScript::IPinData*)flowOutPin); 
                        }
                    }
                }
            }

            const Vector<IReflectionProperty>& props = m_Type->GetProperties();

            static bool isItemClicked = false;

            for(Type::uint32 i = 0; i < props.Size(); i++)
            {
                const IReflectionProperty* prop = props[i];
                // ImGui::Text("%f", m_Type->GetProperty(prop->GetName().c_str(), m_Object->GetObject_()).Convert<float>());
                ImGui::Text("%s", prop->GetName().c_str());

                if(ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    {
                        ImGui::Text("%f", prop->GetValue(m_Object->GetObject_()).Convert<float>());
                    }
                    ImGui::EndTooltip();
                }

                if(ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    isItemClicked = true;

                    m_PropertyDragging = prop;
                }

                if(isItemClicked && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    isItemClicked = false;

                    const IReflectionData* type = Reflection::GetType<GetValue>();
                    const Vector<IReflectionMethod> meths = type->GetMethods();
                    GetValue* node = (GetValue*)AddNode(ImGui::GetMousePos() - offset, ImVec2(0, 0), type);


                    node->m_Object = m_Object;
                    node->m_PropertyType = m_Type;
                    node->m_Property = m_PropertyDragging;

                    for(Type::uint32 i = 0; i < meths.Size(); i++)
                    {
                        const IReflectionMethod* meth = meths[i];
                        Variant* meta = meth->GetMetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE);

                        if(meta && meta->IsValid())
                        {
                            if(meta->Convert<size_t>() == m_PropertyDragging->GetTypeID())
                            {
                                node->ChangeFunc(meth);

                                break;
                            }
                        }
                    }

                    m_PropertyDragging = 0;
                }

                if(isItemClicked && ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    // Eventually animate dragging
                }
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        if(ImGui::BeginChild("nodes_region", ImVec2(0, 0), true, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove))
        {
            ImU32 GRID_COLOR = IM_COL32(5, 5, 5, 40);
            float GRID_SZ = 64.0f;
            ImVec2 win_pos = ImGui::GetCursorScreenPos();
            ImVec2 canvas_sz = ImGui::GetWindowSize();

            for(float x = fmodf(m_View.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            {
                drawList->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
            }

            for(float y = fmodf(m_View.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            {
                drawList->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
            }

        // Channels:
        // 0. Background shadow
        // 1. Actual rect node with borders
        // 2. Text, pins, etc. 
            drawList->ChannelsSplit(3);

        // Nodes drawing
            for(Type::uint32 i = 0; i < m_Nodes.size(); i++)
            {
                NodeGraph::INode* node = m_Nodes[i];

                ImVec2 nodeRectMin = offset + node->m_Position;
                ImVec2 nodeRectMax = nodeRectMin + node->m_Size;

                ImGui::PushID(node->m_ID);

            // Draw the shadow of node
                drawList->ChannelsSetCurrent(0);
                drawList->AddRectFilled(nodeRectMin + NODE_SHADOW_SIZE, nodeRectMax + NODE_SHADOW_SIZE, ImColor(10, 10, 10), 4.0f);

            // Draw the node
                drawList->ChannelsSetCurrent(1);
                drawList->AddRectFilled(nodeRectMin, nodeRectMax, NODE_COLOR, 4.0f);

            // Draw user specified code
                drawList->ChannelsSetCurrent(2);

                ImGui::SetCursorScreenPos(nodeRectMin);
                ImGui::BeginGroup();
                {
                    node->Draw(drawList);
                }
                ImGui::EndGroup();

                for(Type::uint32 j = 0; j < node->m_Outputs.size(); j++)
                {
                    VisualScript::IPin* pin = node->m_Outputs[j];
                    VisualScript::IPinFunction* notCasted = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)pin).Convert<VisualScript::IPinFunction*>();
                    ImVec2 position = offset + node->GetOutputSlotPosition(pin->GetID());
                    const char* title = pin->GetName().c_str();
                    ImVec2 titleSize = ImGui::CalcTextSize(title);

                    if(titleSize.x == 0)
                    {
                        titleSize.x = 20;
                    }

                    ImGui::SetCursorScreenPos(ImVec2(position.x - titleSize.x - 4 - NODE_SLOT_RADIUS, position.y - NODE_SLOT_RADIUS));
                    ImGui::InvisibleButton("test", ImVec2(NODE_SLOT_RADIUS + titleSize.x + 4, NODE_SLOT_RADIUS * 2));

                    if(ImGui::IsItemHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
                    {
                        ImVec2 hoveredPinRectMin = ImVec2(position.x - titleSize.x - 4 - NODE_SLOT_RADIUS, position.y - NODE_SLOT_RADIUS);
                        ImVec2 hoveredPinRectMax = hoveredPinRectMin + ImVec2(titleSize.x + NODE_SLOT_RADIUS + 4, NODE_SLOT_RADIUS * 2);

                        drawList->AddRectFilled(hoveredPinRectMin, hoveredPinRectMax, ImColor(120, 120, 120, 120), 4.f);

                        m_IsAnyPinHovered = true;

                        bool canConnect = true;

                        CheckIfCanConnect(m_SelectedInputPin, pin, canConnect);

                        if((ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) || (m_SelectedInputPin && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))) && canConnect)
                        {
                            m_SelectedOutputPin = pin;
                        }
                    }

                    if(pin->GetPinType() == VisualScript::IPin::PinType::VARIABLE)
                    {
                        drawList->AddCircleFilled(position, NODE_SLOT_RADIUS, ((NodeGraph::OutputPin*)pin)->m_Color);
                        drawList->AddCircle(position, NODE_SLOT_RADIUS, ((NodeGraph::OutputPin*)pin)->m_BorderColor);
                        drawList->AddText(ImVec2(position.x - NODE_SLOT_RADIUS - 2 - titleSize.x, position.y - NODE_SLOT_RADIUS), ((NodeGraph::OutputPin*)pin)->m_TextColor, title);
                    }
                    else if(pin->GetPinType() == VisualScript::IPin::PinType::FLOW)
                    {
                        ImVec2 p0 = ImVec2(position.x - NODE_SLOT_RADIUS, position.y - NODE_SLOT_RADIUS);
                        ImVec2 p1 = ImVec2(position.x - NODE_SLOT_RADIUS, position.y + NODE_SLOT_RADIUS);
                        ImVec2 p2 = ImVec2(position.x + NODE_SLOT_RADIUS, position.y);

                        drawList->AddTriangleFilled(
                            p0,
                            p1,
                            p2,
                            ImColor(150, 150, 150)
                        );
                        drawList->AddTriangle(
                            p0,
                            p1,
                            p2,
                            ImColor(255, 255, 255),
                            2
                        );
                        drawList->AddText(ImVec2(position.x - titleSize.x - NODE_SLOT_RADIUS - 2, position.y - NODE_SLOT_RADIUS), ImColor(200, 200, 200), title);
                    }
                }

                for(Type::uint32 j = 0; j < node->m_Inputs.size(); j++)
                {
                    VisualScript::IPin* pin = node->m_Inputs[j];
                    VisualScript::IPinData* notCasted = Reflection::GetType("SteelEngine::NodeGraph::InputPin")->Invoke("Cast_IPinData", (NodeGraph::InputPin*)pin).Convert<VisualScript::IPinData*>();
                    NodeGraph::InputPin* pin2 = static_cast<NodeGraph::InputPin*>(pin);
                    ImVec2 position = offset + node->GetInputSlotPosition(pin->GetID());
                    const char* title = pin->GetName().c_str();
                    ImVec2 titleSize = ImGui::CalcTextSize(title);

                    if(titleSize.x == 0)
                    {
                        titleSize.x = 20;
                    }

                    ImGui::SetCursorScreenPos(ImVec2(position.x, position.y - NODE_SLOT_RADIUS));
                    ImGui::InvisibleButton("test", ImVec2(NODE_SLOT_RADIUS + titleSize.x + 4, NODE_SLOT_RADIUS * 2));

                    if(ImGui::IsItemHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
                    {
                        ImVec2 hoveredPinRectMin = ImVec2(position.x, position.y - NODE_SLOT_RADIUS);
                        ImVec2 hoveredPinRectMax = hoveredPinRectMin + ImVec2(titleSize.x + NODE_SLOT_RADIUS + 4, NODE_SLOT_RADIUS * 2);

                        drawList->AddRectFilled(hoveredPinRectMin, hoveredPinRectMax, ImColor(120, 120, 120, 120), 4.f);

                        m_IsAnyPinHovered = true;

                        bool canConnect = true;

                        CheckIfCanConnect(m_SelectedOutputPin, pin, canConnect);

                        if((ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) || (m_SelectedOutputPin && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))) && canConnect)
                        {
                            m_SelectedInputPin = pin;
                        }
                    }

                    if(pin->GetPinType() == VisualScript::IPin::PinType::VARIABLE)
                    {
                        drawList->AddCircleFilled(position, NODE_SLOT_RADIUS, pin2->m_Color);
                        drawList->AddCircle(position, NODE_SLOT_RADIUS, pin2->m_BorderColor);
                        drawList->AddText(ImVec2(position.x + NODE_SLOT_RADIUS + 2, position.y - NODE_SLOT_RADIUS), pin2->m_TextColor, title);

                        ImGui::SetCursorScreenPos(ImVec2(position.x + NODE_SLOT_RADIUS + 2, position.y - NODE_SLOT_RADIUS + titleSize.y));

                        if(!pin2->m_Connected)
                        {
                            ImGui::PushItemWidth((node->m_Size.x / 2) - (NODE_SLOT_RADIUS + 2));
                            ImGui::PushID(pin->GetID());

                            if(pin->GetVariableType() == VisualScript::IPin::VariableType::FLOAT)
                            {
                                ImGui::InputFloat("##label", (float*)notCasted->GetRawData());
                            }
                            else if(pin->GetVariableType() == VisualScript::IPin::VariableType::INTEGER)
                            {
                                ImGui::InputInt("##label", (int*)notCasted->GetRawData());
                            }

                            ImGui::PopID();
                            ImGui::PopItemWidth();
                        }
                        else
                        {
                            if(pin->GetVariableType() == VisualScript::IPin::VariableType::FLOAT)
                            {
                                ImGui::TextColored(pin2->m_TextColor, "%f", notCasted->GetData<float>());
                            }
                            else if(pin->GetVariableType() == VisualScript::IPin::VariableType::INTEGER)
                            {
                                ImGui::TextColored(pin2->m_TextColor, "%i", notCasted->GetData<int>());
                            }
                        }
                    }
                    else if(pin->GetPinType() == VisualScript::IPin::PinType::FLOW)
                    {
                        ImVec2 p0 = ImVec2(position.x - NODE_SLOT_RADIUS, position.y - NODE_SLOT_RADIUS);
                        ImVec2 p1 = ImVec2(position.x - NODE_SLOT_RADIUS, position.y + NODE_SLOT_RADIUS);
                        ImVec2 p2 = ImVec2(position.x + NODE_SLOT_RADIUS, position.y);

                        drawList->AddTriangleFilled(
                            p0,
                            p1,
                            p2,
                            ImColor(150, 150, 150)
                        );
                        drawList->AddTriangle(
                            p0,
                            p1,
                            p2,
                            ImColor(255, 255, 255),
                            2
                        );
                        drawList->AddText(ImVec2(position.x + NODE_SLOT_RADIUS + 2, position.y - NODE_SLOT_RADIUS), ImColor(200, 200, 200), title);
                    }
                }

                ImGui::SetCursorScreenPos(nodeRectMin);
                ImGui::InvisibleButton("node", node->m_Size);

                bool isNodeHovered = ImGui::IsItemHovered();

                if(isNodeHovered)
                {
                    m_HoveredNodeID = node->m_ID;
                }

                drawList->ChannelsSetCurrent(1);

                if(m_SelectedNodeID == node->m_ID || m_HoveredNodeID == node->m_ID)
                {
                    drawList->AddRect(nodeRectMin, nodeRectMax, CONNECTION_SELECTED_COLOR, 4.0f);
                }
                else
                {
                    drawList->AddRect(nodeRectMin, nodeRectMax, ImColor(100, 100, 100), 4.0f);
                }

            // Node selection
                if(ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    if(isNodeHovered)
                    {
                        m_SelectedNodeID = node->m_ID;
                    }
                }
                else if(isNodeHovered && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    m_SelectedNodeID = node->m_ID;
                }

                if(ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Left) && m_SelectedNodeID == node->m_ID && !m_SelectedInputPin && !m_SelectedOutputPin)
                {
                    node->m_Position = node->m_Position + ImGui::GetIO().MouseDelta;
                }

                ImGui::PopID();
            }

            drawList->ChannelsMerge();

            if(ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Middle) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive())
            {
                m_View += ImGui::GetIO().MouseDelta;
            }

            for(std::vector<Connection>::iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
            {
                VisualScript::IPin* start = it->m_Start;
                VisualScript::IPin* end = it->m_End;

                ImVec2 p0;
                ImVec2 p1;
                ImVec2 p2;
                ImVec2 p3;

                if(start->GetFlowType() == VisualScript::IPin::Flow::INPUT)
                {
                    NodeGraph::INode* node = (NodeGraph::INode*)start->GetNode();

                    p0 = offset + node->GetInputSlotPosition(start->GetID());
                    p1 = offset + node->GetInputSlotPosition(start->GetID()) + ImVec2(-50, 0);
                    
                }
                else if(start->GetFlowType() == VisualScript::IPin::Flow::OUTPUT)
                {
                    NodeGraph::INode* node = (NodeGraph::INode*)start->GetNode();

                    p2 = offset + node->GetOutputSlotPosition(start->GetID()) + ImVec2(50, 0);
                    p3 = offset + node->GetOutputSlotPosition(start->GetID());
                }

                if(end->GetFlowType() == VisualScript::IPin::Flow::INPUT)
                {
                    NodeGraph::INode* node = (NodeGraph::INode*)end->GetNode();

                    p0 = offset + node->GetInputSlotPosition(end->GetID());
                    p1 = offset + node->GetInputSlotPosition(end->GetID()) + ImVec2(-50, 0);
                }
                else if(end->GetFlowType() == VisualScript::IPin::Flow::OUTPUT)
                {
                    NodeGraph::INode* node = (NodeGraph::INode*)end->GetNode();

                    p2 = offset + node->GetOutputSlotPosition(end->GetID()) + ImVec2(50, 0);
                    p3 = offset + node->GetOutputSlotPosition(end->GetID());
                }

                if(checkIfPointIsOnBezier(
                    ImGui::GetMousePos(), p0, p1, p2, p3, CONNECTION_CURVE_SEGMENTS).m_Distance <= CONNECTION_SELECTED_THICKNESS ||
                    m_SelectedConnection == it)
                {
                    drawList->AddBezierCurve(
                        p0,
                        p1,
                        p2,
                        p3,
                        CONNECTION_SELECTED_COLOR,
                        CONNECTION_SELECTED_THICKNESS
                    );

                    m_HoveredConnection = it;
                }

                drawList->AddBezierCurve(
                    p0,
                    p1,
                    p2,
                    p3,
                    CONNECTION_COLOR,
                    CONNECTION_THICKNESS
                );
            }

            bool isConnectionHovered = m_HoveredConnection != m_Connections.end();

            if(ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
            {
                if(!ImGui::IsAnyItemHovered() && !isConnectionHovered)
                {
                    m_SelectedNodeID = -1;

                    ImGui::OpenPopup("canvas_context");
                }
                else if(m_SelectedNodeID != -1)
                {
                    ImGui::OpenPopup("node_context");
                }
                else if(isConnectionHovered && m_SelectedConnection == m_Connections.end())
                {
                    m_SelectedConnection = m_HoveredConnection;

                    ImGui::OpenPopup("connection_context");
                }
            }

            if(ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                if(!ImGui::IsAnyItemHovered())
                {
                    m_SelectedNodeID = -1;
                    m_SelectedConnection = m_Connections.end();
                }
            }

            if(ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                if(m_SelectedInputPin && m_SelectedOutputPin && m_IsAnyPinHovered)
                {
                    bool found = false;

                    for(std::vector<Connection>::iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
                    {
                        if((it->m_Start == m_SelectedInputPin && it->m_End == m_SelectedOutputPin) || (it->m_End == m_SelectedInputPin && it->m_Start == m_SelectedOutputPin))
                        {
                            found = true;
                        }
                    }

                    if(!found)
                    {
                        m_Connections.push_back(Connection(m_SelectedOutputPin, m_SelectedInputPin));

                        m_SelectedOutputPin->AddConnection(m_SelectedInputPin);
                        m_SelectedInputPin->AddConnection(m_SelectedOutputPin);

                        m_SelectedOutputPin->SetConnected(true);
                        m_SelectedInputPin->SetConnected(true);
                    }
                }

                m_SelectedInputPin = 0;
                m_SelectedOutputPin = 0;
            }

            if(ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                if(m_SelectedOutputPin)
                {
                    NodeGraph::INode* node = (NodeGraph::INode*)m_SelectedOutputPin->GetNode();

                    ImVec2 p0 = offset + node->GetOutputSlotPosition(m_SelectedOutputPin->GetID());
                    ImVec2 p1 = offset + node->GetOutputSlotPosition(m_SelectedOutputPin->GetID()) + ImVec2(50, 0);
                    ImVec2 p2 = ImGui::GetMousePos() + ImVec2(-50, 0);
                    ImVec2 p3 = ImGui::GetMousePos();

                    drawList->AddBezierCurve(
                        p0,
                        p1,
                        p2,
                        p3,
                        CONNECTION_COLOR,
                        CONNECTION_THICKNESS
                    );
                }
                else if(m_SelectedInputPin)
                {
                    NodeGraph::INode* node = (NodeGraph::INode*)m_SelectedInputPin->GetNode();

                    ImVec2 p3 = offset + node->GetInputSlotPosition(m_SelectedInputPin->GetID());
                    ImVec2 p2 = offset + node->GetInputSlotPosition(m_SelectedInputPin->GetID()) + ImVec2(-50, 0);
                    ImVec2 p1 = ImGui::GetMousePos() + ImVec2(50, 0);
                    ImVec2 p0 = ImGui::GetMousePos();

                    drawList->AddBezierCurve(
                        p0,
                        p1,
                        p2,
                        p3,
                        CONNECTION_COLOR,
                        CONNECTION_THICKNESS
                    );
                }
            }

        // Popups
            if(ImGui::BeginPopup("canvas_context"))
            {
                ImVec2 scenePos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;

                if(ImGui::BeginMenu("Add..."))
                {
                    for(const IReflectionData* template_ : m_AvailableTemplates)
                    {
                        if(ImGui::MenuItem(template_->GetTypeName()))
                        {
                            AddNode(scenePos, ImVec2(0, 0), template_);
                        }
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndPopup();
            }

            if(m_SelectedNodeID != -1 && ImGui::BeginPopup("node_context"))
            {
                NodeGraph::INode* node = 0;
                Type::uint32 index = 0;

                for(Type::uint32 i = 0; i < m_Nodes.size(); i++)
                {
                    NodeGraph::INode* node_ = m_Nodes[i];

                    if(node_->m_ID == m_SelectedNodeID)
                    {
                        node = node_;
                        index = i;

                        break;
                    }
                }

                if(node)
                {
                    ImGui::Text("Node name: %s", node->m_Name.c_str());

                    if(ImGui::Button("Delete node"))
                    {
                        NodeGraph::INode* node = m_Nodes[index];

                        m_AvailableIDs.push(node->GetNodeID());

                        for(VisualScript::IPin* pin : node->GetInputs())
                        {
                            m_AvailableIDs.push(pin->GetID());
                        }

                        for(VisualScript::IPin* pin : node->GetOutputs())
                        {
                            m_AvailableIDs.push(pin->GetID());
                        }

                        m_Nodes.erase(m_Nodes.begin() + index);

                        delete node;
                        node = 0;

                        std::stack<Type::uint32> connectionToErase;

                        for(Type::uint32 i = 0; i < m_Connections.size(); i++)
                        {
                            Connection conn = m_Connections[i];
                            VisualScript::INode* node = (VisualScript::INode*)conn.m_Start->GetNode();
                            VisualScript::INode* node2 = (VisualScript::INode*)conn.m_End->GetNode();

                            if(node->GetNodeID() == m_SelectedNodeID || node2->GetNodeID() == m_SelectedNodeID)
                            {
                                if(node->GetNodeID() == m_SelectedNodeID)
                                {
                                    for(VisualScript::IPin* pin_ : node->GetInputs())
                                    {
                                        VisualScript::IPinData* pin = Reflection::GetType("SteelEngine::NodeGraph::InputPin")->Invoke("Cast_IPinData", (NodeGraph::InputPin*)pin_).Convert<VisualScript::IPinData*>();

                                        pin_->GetConnections().clear();
                                    }

                                    for(VisualScript::IPin* pin_ : node->GetOutputs())
                                    {
                                        VisualScript::IPinFunction* pin = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)pin).Convert<VisualScript::IPinFunction*>();

                                        pin_->GetConnections().clear();
                                    }
                                }
                                else if(node2->GetNodeID() == m_SelectedNodeID)
                                {
                                    for(VisualScript::IPin* pin_ : node2->GetInputs())
                                    {
                                        VisualScript::IPinData* pin = Reflection::GetType("SteelEngine::NodeGraph::InputPin")->Invoke("Cast_IPinData", (NodeGraph::InputPin*)pin_).Convert<VisualScript::IPinData*>();

                                        pin_->GetConnections().clear();
                                    }

                                    for(VisualScript::IPin* pin_ : node2->GetOutputs())
                                    {
                                        VisualScript::IPinFunction* pin = Reflection::GetType("SteelEngine::NodeGraph::OutputPin")->Invoke("Cast_IPinFunction", (NodeGraph::OutputPin*)pin).Convert<VisualScript::IPinFunction*>();

                                        pin_->GetConnections().clear();
                                    }
                                }

                                connectionToErase.push(i);
                            }
                        }

                        while(!connectionToErase.empty())
                        {
                            m_Connections.erase(m_Connections.begin() + connectionToErase.top());

                            connectionToErase.pop();
                        }

                        m_SelectedNodeID = -1;

                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::EndPopup();
            }

            if(ImGui::BeginPopup("connection_context"))
            {
                if(ImGui::Button("Delete connection"))
                {
                    if(m_SelectedConnection != m_Connections.end())
                    {
                        VisualScript::IPin* end = (VisualScript::IPin*)m_SelectedConnection->m_End;
                        VisualScript::IPin* start = (VisualScript::IPin*)m_SelectedConnection->m_Start;

                        end->SetConnected(false);
                        start->SetConnected(false);

                        end->GetConnections().clear();
                        start->GetConnections().clear();

                        m_Connections.erase(m_SelectedConnection);

                        m_SelectedConnection = m_Connections.end();

                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::EndPopup();
            }

            if(m_Keys[SDL_SCANCODE_S] && m_Keys[SDL_SCANCODE_LCTRL])
            {
            // TODO: Instead of simulating it, we should save the blueprint here lol

                if(!m_SavingChanged)
                {
                    std::ofstream file(m_Path);
                    Utils::json res;

                    res["nodes"] = Utils::json::array();
                    res["name"] = m_Name;
                    res["type"] = getFullTypename(m_Type);

                    for(NodeGraph::INode* node : m_Nodes)
                    {
                        Utils::json jNode;

                        node->Serialize(jNode);

                        res["nodes"].push_back(jNode);
                    }

                    file << res.dump(4);

                    file.close();

                    m_SavingChanged = true;
                }
            }
            else
            {
                m_SavingChanged = false;
            }

            ImGui::EndChild();
        }
    }

    int BlueprintWindow::GetID()
    {
        int id = m_AvailableIDs.top();

        m_AvailableIDs.pop();

        return id;
    }

    void BlueprintWindow::Import(const Utils::json& nodes)
    {
        for(auto it = nodes["nodes"].begin(); it != nodes["nodes"].end(); ++it)
        {
            Utils::json item = *it;
            NodeGraph::INode* node = AddNode(item["position"], item["size"], Reflection::GetType(item["type"].get<std::string>()), false);

            if(node->m_TypeID == typeid(GetValue).hash_code())
            {
                ((GetValue*)node)->m_Object = m_Object;
                ((GetValue*)node)->m_PropertyType = m_Type;
            }

            node->Deserialize(item);
            node->Init(true);
        }

        std::vector<VisualScript::INode*> nodesVec;

        for(NodeGraph::INode* node : m_Nodes)
        {
            nodesVec.push_back(node);
        }

        for(NodeGraph::INode* node : m_Nodes)
        {
            m_AvailableIDs.pop();

            for(VisualScript::IPin* pin : node->GetInputs())
            {
                pin->ConnectPendingPins(nodesVec);

                for(VisualScript::IPin* conn : pin->GetConnections())
                {
                    m_Connections.push_back(Connection{ pin, conn });
                }

                m_AvailableIDs.pop();
            }

            for(VisualScript::IPin* pin : node->GetOutputs())
            {
                pin->ConnectPendingPins(nodesVec);

                m_AvailableIDs.pop();
            }
        }
    }

    void BlueprintWindow::OnEvent(Event::Naive* event)
    {
        static size_t event1 = KeyUpEvent::GetStaticType();
        static size_t event2 = KeyDownEvent::GetStaticType();

        if(event->GetType() == event2)
        {
            KeyDownEvent* keyDownEvent = (KeyDownEvent*)event;

            m_Keys[keyDownEvent->m_KeyCode] = true;
        }
        else if(event->GetType() == event1)
        {
            KeyUpEvent* keyDownEvent = (KeyUpEvent*)event;

            m_Keys[keyDownEvent->m_KeyCode] = false;
        }
    }

}