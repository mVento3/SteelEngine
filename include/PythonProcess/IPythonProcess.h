#pragma once

#include "string"

namespace SteelEngine {

    struct IPythonProcess
    {
        virtual void WriteInput(const std::string& input) = 0;
        virtual bool Setup() = 0;
        virtual void Release() = 0;
        virtual void Reset() = 0;

        virtual const bool IsCompileComplete() = 0;
        virtual const bool WasError() = 0;
        virtual void SetError(bool error) = 0;
        virtual const std::string& GetErrorMessage() const = 0;
    };

}