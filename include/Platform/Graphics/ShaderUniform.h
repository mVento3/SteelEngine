#pragma once

namespace SteelEngine { namespace Graphics {

    class ShaderUniform
    {
    private:
        void* m_Location;

    public:
        ShaderUniform();
        ShaderUniform(void* location);
        ~ShaderUniform();

        template<typename A>
        static void* Create(const A& location)
        {
            return new A(location);
        }

        void* GetLocation() const { return m_Location; }
    };

}}