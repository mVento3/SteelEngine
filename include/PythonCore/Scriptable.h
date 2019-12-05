#pragma once

#include "PythonCore/IScript.h"
#include "PythonCore/ResultConverter.h"

// #include "HotReloader/IRuntimeObject.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

#include "vector"
#include "initializer_list"
#include "stack"

namespace SteelEngine { namespace Script { namespace Python {

    template <typename T>
    size_t unpack(IScript* script, std::stack<void*>& arr, T t)
    {
        void* object = script->From(t);

        arr.push(object);
        script->AddReference(object);

        return 1;
    }

    template <typename T, typename... Rest>
    size_t unpack(IScript* script, std::stack<void*>& arr, T t, Rest... rest)
    {
        return unpack(script, arr, t) + unpack(script, arr, rest...);
    }

    SE_CLASS(SteelEngine::ReflectionAttribute::NO_SERIALIZE)
    class Scriptable
    {
        friend struct IReflectionData;
    private:
        IScript* m_Python;
        std::string m_ScriptName;

    public:
        Scriptable(const std::string& scriptName = "") :
            m_ScriptName(scriptName)
        {
            m_Python = 0;
        }

        ~Scriptable()
        {

        }

        // IDK
        virtual void Serialize(SteelEngine::HotReloader::ISerializer* serializer)
        {

        }

        template <typename... Args>
        ResultConverter* Call(const std::string& name, Args... args)
        {
            size_t size = sizeof...(args);
            void* argsTuple = m_Python->MakeTuple(size);

            std::stack<void*> stack;
            unpack(m_Python, stack, args...);

            for(Type::uint32 i = 0; i < size; i++)
            {
                void* arg = stack.top();

                if(!arg)
                {
                    m_Python->DecRef(argsTuple);

                    return 0;
                }

                m_Python->SetTupleItem(argsTuple, i, arg);
                stack.pop();
            }

            ResultConverter* conv = new ResultConverter();

            conv->m_Python = m_Python;
            conv->m_Value = m_Python->Call(name, argsTuple);

            m_Python->DecRef(argsTuple);

            return conv;
        }

        ResultConverter* Call(const std::string& name)
        {
            ResultConverter* conv = new ResultConverter();

            conv->m_Python = m_Python;
            conv->m_Value = m_Python->Call(name, 0);

            return conv;
        }

        // Should be private, but currently reflection does not support private functions
        SE_METHOD()
        void SetPython(IScript* python)
        {
            m_Python = python;
        }

        // Should be private, but currently reflection does not support private functions
        SE_METHOD()
        const std::string& GetScriptName()
        {
            return m_ScriptName;
        }
    };

}}}