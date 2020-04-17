#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/IReflectionData.h"

#include "VisualScript/INode.h"
#include "VisualScript/IPinData.h"

#include "HotReloader/InheritanceTrackKeeper.h"

#include "ImGUI_Editor/EditorWindows/NodeGraph/INode.h"

namespace SteelEngine {

    SE_CLASS(
        // Reflection::ReflectionAttribute::VISUAL_SCRIPT_NODE
    )
    class GetValue :
        public NodeGraph::INode
    {
    private:

    public:
        GetValue();
        ~GetValue();

        const IReflectionProperty* m_Property;
        const IReflectionData* m_PropertyType;
        HotReloader::InheritanceTrackKeeper* m_Object;

        void Serialize(Utils::json& j) override;
        void Deserialize(const Utils::json& j) override;

        void ChangeFunc(const IReflectionMethod* meth);

        SE_METHOD(
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC,
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE = typeid(float).hash_code()
        )
        void GetFloat(NodeGraph::INode* node, VisualScript::IPinData* input);

        // SE_METHOD(
        //     Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC,
        //     Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE = typeid(int).hash_code()
        // )
        // void GetInt(NodeGraph::INode* node, VisualScript::IPinData* input);
    };

}