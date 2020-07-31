#pragma once

// #include "Graphics/ITexture.h"

namespace SteelEngine { namespace Graphics {

    struct ITexture;

    struct FramebufferAttachment
    {
        enum Attachment
        {
            COLOR_ATTACHMENT0,
            COLOR_ATTACHMENT1,
            COLOR_ATTACHMENT2,
            COLOR_ATTACHMENT3,
            COLOR_ATTACHMENT4,
            COLOR_ATTACHMENT5,
            COLOR_ATTACHMENT6,
            COLOR_ATTACHMENT7,
            COLOR_ATTACHMENT8,
            COLOR_ATTACHMENT9,
            COLOR_ATTACHMENT10,
            COLOR_ATTACHMENT11,
            COLOR_ATTACHMENT12,
            COLOR_ATTACHMENT13,
            COLOR_ATTACHMENT14,
            COLOR_ATTACHMENT15,
            DEPTH_ATTACHMENT = 1 << 4 // Or just hard code to 16
        };

        FramebufferAttachment(ITexture* texture, Attachment attachment) :
            m_Texture(texture),
            m_Attachment(attachment)
        {

        }

        ITexture* m_Texture;
        Attachment m_Attachment;
    };

}}