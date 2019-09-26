#pragma once

namespace SteelEngine { namespace Editor {

    enum ReflectionAttributes
    {
        EDITOR_WINDOW,
        RANGE,
        SCENE_TYPE
    };

    struct Range
    {
        Range(float min, float max) :
            m_Min(min),
            m_Max(max)
        {
            
        }

        float m_Min;
        float m_Max;
    };

}}