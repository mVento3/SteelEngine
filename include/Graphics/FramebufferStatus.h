#pragma once

namespace SteelEngine { namespace Graphics {

    enum class FramebufferStatus
    {
        FRAMEBUFFER_COMPLETE,
        FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
        FRAMEBUFFER_UNDEFINED,
        FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,

    // If we forget to implement some status...
        NOT_IMPLEMENTED
    };

}}