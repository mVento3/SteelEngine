#pragma once

#include "VisualScript/IPin.h"

#include "RuntimeReflection/IReflectionData.h"

namespace SteelEngine { namespace VisualScript {

    struct NodeTemplate
    {
    public:
        typedef std::function<void(INodeData*, IPinData*)> InputFlowFunction;

        struct PinTemplate
        {
            PinTemplate(IPin::PinType pinType, size_t typeID, const char* name) :
                m_PinType(pinType),
                m_TypeID(typeID),
                m_Name(name)
            {

            }

            PinTemplate(InputFlowFunction func, const char* name) :
                m_Callback(func),
                m_Name(name)
            {
                m_TypeID = RuntimeDatabase::s_InvalidID;
                m_PinType = IPin::PinType::FLOW;
            }

            IPin::PinType       m_PinType;
            size_t              m_TypeID;
            const char*         m_Name;
            InputFlowFunction   m_Callback;
        };

        typedef std::vector<PinTemplate> PinTemplateVector;

    protected:
        template <typename A>
        void AddInputPin(IPin::PinType pinType, const char* name = "")
        {
            GetInputPinTemplates().push_back(PinTemplate(pinType, typeid(A).hash_code(), name));
        }

        void AddInputPin(IPin::PinType pinType, const char* name = "")
        {
            GetInputPinTemplates().push_back(PinTemplate(pinType, RuntimeDatabase::s_InvalidID, name));
        }

        void AddInputPin(InputFlowFunction func, const char* name = "")
        {
            GetInputPinTemplates().push_back(PinTemplate(func, name));
        }

    public:
        virtual PinTemplateVector& GetInputPinTemplates() = 0;

        virtual void SetType(const IReflectionData* type) = 0;
        virtual const IReflectionData* GetType() const = 0;
    };

}}
