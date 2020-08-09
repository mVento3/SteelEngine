#pragma once

#include <chrono>

#define CHECK_SPEED(body, rest) \
{ \
auto startTime = std::chrono::high_resolution_clock::now(); \
body \
auto currentTime = std::chrono::high_resolution_clock::now(); \
float delta = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count(); \
startTime = currentTime; \
rest \
}

#define CHECK_STATIC_SPEED(body, rest) \
{ \
static auto startTime = std::chrono::high_resolution_clock::now(); \
body \
auto currentTime = std::chrono::high_resolution_clock::now(); \
float delta = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count(); \
startTime = currentTime; \
rest \
}