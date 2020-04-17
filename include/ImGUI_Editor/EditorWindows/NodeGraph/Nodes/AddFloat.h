#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "VisualScript/INode.h"
#include "VisualScript/IPinData.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/INode.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/Nodes/AddFloat.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        // Reflection::ReflectionAttribute::VISUAL_SCRIPT_NODE
    )
    class AddFloat :
        public NodeGraph::INode
    {
        GENERATED_BODY
    private:

    public:
        AddFloat();
        ~AddFloat();

        SE_METHOD(
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC,
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE = typeid(float).hash_code()
        )
        void Func(NodeGraph::INode* node, VisualScript::IPinData* input);

        SE_METHOD(
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC,
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW
        )
        void Func2(NodeGraph::INode* node, VisualScript::IPinData* input);

        void Serialize(Utils::json& j) override;
        void Deserialize(const Utils::json& j) override;
    };

}