#pragma once

#include "string"

#include "Graphics/IModel.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine { namespace Graphics {

    SE_CLASS()
    class Model : public IModel
    {
    private:
        std::string m_Filename;

    public:
        Model(const char* file);
        ~Model();

        static IModel* Create(const char* file)
        {
            IModel* model = (IModel*)Reflection::CreateInstance<Model>(file);

            return model;
        }

        IMesh* Setup() override;
    };

}}