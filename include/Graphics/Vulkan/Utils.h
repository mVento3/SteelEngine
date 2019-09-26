#pragma once

#define SE_VK_CHECK(func) \
    if(func != VK_SUCCESS) \
    { \
        return SE_FALSE; \
    }