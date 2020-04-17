#pragma once

#include "RuntimeReflection/Macro.h"

#include "EditorComponents/ImGUI/UserInterface.h"

#include "ImGUI_Editor/ReflectionAttribs.h"
#include "ImGUI_Editor/WindowType.h"

#include "Graphics/ITexture.h"

#include "NodeGraph/InputPin.h"
#include "NodeGraph/OutputPin.h"
#include "NodeGraph/FlowInputPin.h"
#include "NodeGraph/INode.h"

#include "RuntimeReflection/IReflectionData.h"

#include "vector"

#include "Event/EventObserver.h"

#include "ImGUI_Editor/EditorWindows/BlueprintWindow.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Editor::ReflectionAttributes::WINDOW_TYPE = Editor::WindowType::CONTEXT,
        Editor::ReflectionAttributes::EDITOR_WINDOW,
        Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS,
        EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW,
        Reflection::ReflectionAttribute::RUNTIME_SERIALIZE,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class BlueprintWindow :
        public EditorComponents::ImGUI::UserInterface,
        public HotReloader::IRuntimeObject,
        public EventObserver
    {
        GENERATED_BODY
    public:
        struct Connection
        {
            Connection(VisualScript::IPin* start, VisualScript::IPin* end) :
                m_Start(start),
                m_End(end)
            {

            }

            VisualScript::IPin* m_Start;
            VisualScript::IPin* m_End;
        };

        struct Flow
        {

        };

        static const ImVec2 NODE_WINDOW_PADDING;
        static const float NODE_SLOT_RADIUS;
        static const ImColor CONNECTION_COLOR;
        static const float CONNECTION_THICKNESS;
        static const ImColor CONNECTION_SELECTED_COLOR;
        static const float CONNECTION_SELECTED_THICKNESS;
        static const Type::uint32 CONNECTION_CURVE_SEGMENTS;
        static const ImColor NODE_COLOR;
        static const ImVec2 NODE_SHADOW_SIZE;

        static const ImColor FLOAT_COLOR;
        static const ImColor FLOAT_BORDER_COLOR;
        static const ImColor INTEGER_COLOR;
        static const ImColor INTEGER_BORDER_COLOR;
        static const ImColor DOUBLE_COLOR;
        static const ImColor DOUBLE_BORDER_COLOR;
        static const ImColor STRING_COLOR;
        static const ImColor STRING_BORDER_COLOR;
        static const ImColor POINTER_COLOR;
        static const ImColor POINTER_BORDER_COLOR;

    private:
        ImVec2 m_View;

        int m_SelectedNodeID;
        int m_HoveredNodeID;
        VisualScript::IPin* m_SelectedInputPin;
        VisualScript::IPin* m_SelectedOutputPin;
        bool m_IsAnyPinHovered;

        std::vector<Connection>::iterator m_HoveredConnection;
        std::vector<Connection>::iterator m_SelectedConnection;

        std::stack<int> m_AvailableIDs;

        std::vector<NodeGraph::INode*> m_Nodes;
        std::vector<Connection> m_Connections;

        std::vector<const IReflectionData*> m_AvailableTemplates;

        bool m_Keys[256];
        bool m_SavingChanged;

        const IReflectionProperty* m_PropertyDragging;

        void PopulateConnectedNodes(std::vector<BlueprintWindow::Connection>::iterator it, VisualScript::INodeData* node, std::vector<VisualScript::INodeData*>& nodes);
        void CheckIfCanConnect(VisualScript::IPin* pin1, VisualScript::IPin* pin2, bool& canConnect);

        NodeGraph::INode* AddNode(const ImVec2& position, const ImVec2& size, const IReflectionData* nodeType, bool init = true);

    public:
        BlueprintWindow();
        ~BlueprintWindow();

        std::filesystem::path m_Path;
        IReflectionData* m_Type;
        std::string m_Name;
        HotReloader::InheritanceTrackKeeper* m_Object;

        void Init() override;
        void Draw() override;

        int GetID();

        void Import(const Utils::json& nodes);

        void OnEvent(Event::Naive* event) override;
    };

}