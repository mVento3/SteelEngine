#pragma once

#include "functional"

#include "RuntimeReflection/IProxyMethod.h"

namespace SteelEngine { namespace VisualScript {

    struct INodeData;
    struct IPinData;

    struct IPinFunction
    {
        typedef std::function<void(INodeData*, IPinData*)> OutputFunction;
        typedef IProxyMethod<INodeData*, IPinData*> ReflectedOutputFunction;

        virtual OutputFunction GetOutputFunction() const = 0;
        virtual void SetFunc(OutputFunction func) = 0;

        virtual ReflectedOutputFunction* GetReflectedOutputFunction() const = 0;
        virtual void SetFunc(ReflectedOutputFunction* func) = 0;

        void Call(INodeData* node, IPinData* input)
        {
            OutputFunction func = GetOutputFunction();
            ReflectedOutputFunction* func2 = GetReflectedOutputFunction();

            if(func)
            {
                func(node, input);
            }
            else if(func2)
            {
                func2->Invoke(node, node, input);
            }
        }
    };

}}