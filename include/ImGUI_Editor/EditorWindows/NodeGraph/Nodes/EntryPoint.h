#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "VisualScript/INode.h"
#include "VisualScript/IPinData.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/INode.h"

namespace SteelEngine {

    SE_CLASS(
        // Reflection::ReflectionAttribute::VISUAL_SCRIPT_NODE
    )
    class EntryPoint : public NodeGraph::INode
    {
    private:

    public:
        EntryPoint();
        ~EntryPoint();

        void Serialize(Utils::json& j) override;
        void Deserialize(const Utils::json& j) override;

        SE_METHOD(
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC,
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW
        )
        void Func(NodeGraph::INode* node, VisualScript::IPinData* input);
    };

}