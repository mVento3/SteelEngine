#pragma once

#include "InheritanceTrackKeeper.h"

namespace SteelEngine { namespace HotReloader {

    class ReloadableIneritanceTrackKeeper : public InheritanceTrackKeeper
    {
    private:
        void** m_Ptr;
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

            HotReloader::IRuntimeObject* runtime = m_Type->Invoke("Cast_IRuntimeObject", *m_Ptr).Convert<HotReloader::IRuntimeObject*>();

            runtime->m_Tracker = this;
        }

    public:
        ReloadableIneritanceTrackKeeper()
        {
            m_Type = 0;
            m_Ptr = 0;
            m_Changed = 0;
        }

        ReloadableIneritanceTrackKeeper(const IReflectionData* type, void** ptr) :
            m_Type(type),
            m_Ptr(ptr)
        {
            m_Changed = *m_Ptr;

            InitMethods();
        }

        ~ReloadableIneritanceTrackKeeper()
        {

        }

        void Update() override
        {
            if(*m_Ptr != m_Changed)
            {
                m_Changed = *m_Ptr;

                for(std::map<size_t, void*>::iterator it = m_Ptrs.begin(); it != m_Ptrs.end(); ++it)
                {
                    for(Type::uint32 i = 0; i < m_Methods.size(); i++)
                    {
                        const IReflectionMethod* meth = m_Methods[i];

                        if(meth->GetReturnTypeID() == it->first)
                        {
                            it->second = ((IProxyMethod<>*)meth)->Invoke(m_Changed).Convert<void*>();

                            break;
                        }
                    }
                }
            }
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