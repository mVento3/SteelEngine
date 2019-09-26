#pragma once

#include "InformationTracker/ISystemTracker.h"

#include "map"

namespace SteelEngine {

    template <class A>
    class SystemTracker : public ISystemTracker
    {
        friend A;
    private:
        size_t m_TypeID;

    public:
        SystemTracker()
        {
            m_TypeID = typeid(A).hash_code();
        }

        ~SystemTracker()
        {

        }

        void Track(const std::string& type, const std::string& info) override
        {

        }

        inline size_t GetTypeID() override { return m_TypeID; }
    };

}