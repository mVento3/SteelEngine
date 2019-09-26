#pragma once

#include "PythonCore/IScript.h"

#include "Python.h"

#include "RuntimeReflection/Macro.h"

#include "PythonCore/Script.Generated.h"

namespace SteelEngine { namespace Script { namespace Python {

    SE_CLASS()
    class Script : public IScript
    {
        GENERATED_BODY
    private:
        PyObject* m_Module;

        std::vector<PyObject*> m_Objects;

        void LoadScript(const std::string& scriptFile) override;

    public:
        Script();
        ~Script();

        void* MakeTuple(size_t size) override;

        void AddReference(void* object) override;

        void* From(size_t value) override;
        void* From(int value) override;
        void* From(double value) override;
        void* From(const std::string& value) override;
        void* From(const char* value) override;

        size_t ToSizeT(void* value) override;
        double ToDouble(void* value) override;
        const char* ToString(void* value) override;

        void DecRef(void* ref) override;
        void SetTupleItem(void* args, size_t index, void* value) override;

        void* Call(const std::string& name, void* args) override;
    };

}}}