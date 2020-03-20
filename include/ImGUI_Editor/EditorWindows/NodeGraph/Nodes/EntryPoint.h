#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "VisualScript/INode.h"
#include "VisualScript/NodeTemplate.h"

namespace SteelEngine {

    SE_CLASS(
        // Reflection::ReflectionAttribute::VISUAL_SCRIPT_NODE
    )
    class EntryPoint : public VisualScript::NodeTemplate
    {
    private:
        PinTemplateVector m_PinTemplates;

        const IReflectionData* m_Type;

    public:
        EntryPoint();
        ~EntryPoint();

        PinTemplateVector& GetInputPinTemplates() override { return m_PinTemplates; }

        void SetType(const IReflectionData* type) override { m_Type = type; }
        const IReflectionData* GetType() const override { return m_Type; }

        SE_METHOD(
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC,
            Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW
        )
        static void Func(VisualScript::INodeData* node, VisualScript::IPinData* input);
    };

}