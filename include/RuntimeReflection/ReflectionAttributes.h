#pragma once

namespace SteelEngine {

    enum ReflectionAttribute
    {
    // If in class: serialize all properties - public, private and protected
    // If in property: serialize only the property
        SE_RUNTIME_SERIALIZE,
    // IDK xd
        SE_UPDATE,  
    // Mark as reflection module used to specify more reflection mdoules
        SE_REFLECTION_MODULE,
    // Dont generate serialization function
        SE_NO_SERIALIZE,
    // Send property over network
        SE_NET_VALUE,
    // Mark as some event that can be transported over network
        SE_NETWORK_COMMAND
    };

}