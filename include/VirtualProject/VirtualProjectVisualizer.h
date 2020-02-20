#pragma once

#include "VirtualProject/IVirtualProjectVisualizer.h"

#include "RuntimeReflection/Macro.h"

#include "VirtualProject/VirtualProjectVisualizer.Generated.h"

namespace SteelEngine {

    class VirtualProject;

    SE_CLASS(
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class VirtualProjectVisualizer : public IVirtualProjectVisualizer
    {
        GENERATED_BODY
    private:
        VirtualProject** m_VirtualProject;
        IContext* m_Context;

        const size_t m_UserInterfaceTypeID;

        bool m_Loaded;

    public:
        VirtualProjectVisualizer(VirtualProject** virtualProject);
        ~VirtualProjectVisualizer();

        void Render() override;
        void Init(IContext* context) override;

        void OnProjectLoad() override;
    };

}