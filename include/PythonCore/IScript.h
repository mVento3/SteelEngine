#pragma once

#include "RuntimeDatabase/RuntimeDatabase.h"

namespace SteelEngine {

    struct IReflectionData;
    class PythonScriptableInheritance;

    namespace Script { namespace Python {

    struct IScript
    {
        friend struct IReflectionData;
        friend class PythonScriptableInheritance;
    protected:
        virtual void LoadScript(const std::string& scriptFile) = 0;

    public:
        virtual void* MakeTuple(size_t size) = 0;

        virtual void AddReference(void* object) = 0;

        virtual void* From(size_t value) = 0;
        virtual void* From(int value) = 0;
        virtual void* From(double value) = 0;
        virtual void* From(const std::string& value) = 0;
        virtual void* From(const char* value) = 0;

        virtual size_t ToSizeT(void* value) = 0;
        virtual double ToDouble(void* value) = 0;
        virtual const char* ToString(void* value) = 0;

        virtual void DecRef(void* ref) = 0;
        virtual void SetTupleItem(void* args, size_t index, void* value) = 0;

        virtual void* Call(const std::string& name, void* args) = 0;
    };

}}}