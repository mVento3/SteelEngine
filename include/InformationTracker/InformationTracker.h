#pragma once

#include "InformationTracker/IInformationTracker.h"
#include "InformationTracker/ISystemTracker.h"

namespace SteelEngine {

    class InformationTracker : public IInformationTracker
    {
    private:
        std::vector<ISystemTracker*> m_Systems;

    public:
        InformationTracker();
        ~InformationTracker();

        void Add(ISystemTracker* tracker) override;
        ISystemTracker* Get(size_t typeID) override;
    };

}