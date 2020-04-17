#pragma once

#include "HotReloader/InheritanceTrackKeeper.h"

namespace SteelEngine { namespace HotReloader {

    class NonReloadableITK : public InheritanceTrackKeeper
    {
    private:
        std::map<size_t, void*> m_Ptrs;
        void* m_Changed;
        const IReflectionData* m_Type;
        std::vector<const IReflectionMethod*> m_Methods;

        std::map<size_t, void*>& GetInheritances() override
        {
            return m_Ptrs;
        }

        void InitMethods()
        {
            Event::GlobalEvent::Add<SwapInheritancePointersEvent>(this);

            const Vector<IReflectionMethod>& meths = m_Type->GetMethods();

            for(Type::uint32 i = 0; i < meths.Size(); i++)
            {
                const IReflectionMethod* meth = meths[i];
                Variant* meta = meth->GetMetaData(Reflection::ReflectionAttribute::CAST_FUNCTION);

                if(meta->IsValid() && meta->Convert<bool>())
                {
                    m_Methods.push_back(meth);
                }
            }

            for(Type::uint32 i = 0; i < m_Methods.size(); i++)
            {
                const IReflectionMethod* meth = m_Methods[i];

                m_Ptrs[meth->GetReturnTypeID()] = ((IProxyMethod<>*)meth)->Invoke(m_Changed).Convert<void*>();
            }

            HotReloader::IRuntimeObject* runtime = m_Type->Invoke("Cast_IRuntimeObject", m_Changed).Convert<HotReloader::IRuntimeObject*>();

            runtime->m_Tracker = this;
        }

    public:
        NonReloadableITK()
        {
            m_Type = 0;
            m_Changed = 0;
        }

        NonReloadableITK(const IReflectionData* type, void* ptr) :
            m_Type(type),
            m_Changed(ptr)
        {
            InitMethods();
        }

        ~NonReloadableITK()
        {

        }

        void Update() override
        {

        }

        void* GetObject_() override
        {
            return m_Changed;
        }

        const IReflectionData* GetType() const override
        {
            return m_Type;
        }

        void operator()(const SwapInheritancePointersEvent& event) override
        {
            Update();
        }
    };

}}