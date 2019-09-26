#pragma once

namespace SteelEngine {

    enum ReflectionAttribute
    {
    // If in class: serialize all properties - public, private and protected
    // If in property: serialize only the property
        RUNTIME_SERIALIZE,
    // IDK xd
        UPDATE,
    // Mark as reflection module used to specify more reflection mdoules
        REFLECTION_MODULE,
    // Dont generate serialization function
        NO_SERIALIZE,
    // Send property over network
        NET_VALUE,
    // Mark as some event that can be transported over network
        NETWORK_COMMAND,
    // Editor
        EDITOR,
    // Editor name
        EDITOR_NAME,
        PROJECTS_PATH,
        VIRTUAL_PROJECT,
        GAME_SCRIPT,
        ENGINE_START_TYPE,
        GENERATE_CAST_FUNCTIONS,
        SYSTEMS_INFORMATION_TRACKER,
        NO_RECOMPILE
    };

}