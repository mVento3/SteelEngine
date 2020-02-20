#pragma once

#include "map"
#include "vector"

#include "RuntimeReflection/IReflectionData.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine { namespace HotReloader {

    class InheritanceTrackKeeper
    {
        friend class RuntimeReloader;
    private:
        struct SwapInheritancePointersEvent
        {

        };

        void** m_Ptr;
        std::map<size_t, void*> m_Ptrs;
        void* m_Changed;
        const IReflectionData* m_Type;
        std::vector<const IReflectionMethod*> m_Methods;

        void InitMethods()
        {
            Event::GlobalEvent::Add<SwapInheritancePointersEvent>(this);

            const std::vector<IReflectionMethod*>& meths = m_Type->GetMethods();

            for(Type::uint32 i = 0; i < meths.size(); i++)
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
        InheritanceTrackKeeper()
        {
            m_Type = 0;
            m_Ptr = 0;
            m_Changed = 0;
        }

        // InheritanceTrackKeeper(const char* name)
        // {
        //     m_Type = Reflection::GetType(name);
        //     m_Ptr = m_Type->Create_();
        //     m_Changed = *m_Ptr;

        //     InitMethods();
        // }

        // InheritanceTrackKeeper(size_t typeID)
        // {
        //     m_Type = Reflection::GetType(typeID);
        //     m_Ptr = m_Type->Create_();
        //     m_Changed = *m_Ptr;

        //     InitMethods();
        // }

        InheritanceTrackKeeper(const IReflectionData* type, void** ptr) :
            m_Type(type),
            m_Ptr(ptr)
        {
            m_Changed = *m_Ptr;

            InitMethods();
        }

        // InheritanceTrackKeeper(const IReflectionData* type) :
        //     InheritanceTrackKeeper(type, type->Create_())
        // {

        // }

        void Update()
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

        template <typename A>
        A* Get()
        {
            static size_t typeID = typeid(A*).hash_code();

            return (A*)m_Ptrs[typeID];
        }

        const IReflectionData* GetType() const
        {
            return m_Type;
        }

        void operator()(const SwapInheritancePointersEvent& event)
        {
            Update();
        }
    };

}}