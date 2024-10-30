// Add this to your imconfig.h
#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <glm/glm.hpp>

#include <cstdint>

#include "imgui-SFML_export.h"

#define IM_VEC2_CLASS_EXTRA                                          \
    template<typename T>                                             \
    ImVec2(const sf::Vector2<T>& v) {                                \
        x = static_cast<float>(v.x);                                 \
        y = static_cast<float>(v.y);                                 \
    }                                                                \
                                                                     \
    template<typename T>                                             \
    operator sf::Vector2<T>() const {                                \
        return sf::Vector2<T>(static_cast<T>(x), static_cast<T>(y)); \
    }\
    template<typename T>\
    ImVec2(const glm::vec<2,T,glm::defaultp> v){\
    x = static_cast<float>(v.x);\
    y = static_cast<float>(v.y);\
    }\
    template<typename T>\
    operator glm::vec<2, T, glm::defaultp>() const {\
        \
            return glm::vec<2, T, glm::defaultp>(static_cast<T>(x), static_cast<T>(y)); \
    }\
    template<typename T>\
    ImVec2(const glm::vec<3,T, glm::defaultp> v){\
    x = static_cast<float>(v.x);\
    y = static_cast<float>(v.y);\
    }\
    template<typename T>\
    operator glm::vec<3, T, glm::defaultp>() const {\
        \
            return glm::vec<3, T, glm::defaultp>(static_cast<T>(x), static_cast<T>(y),0); \
    }\
    template<typename T>\
    ImVec2(const glm::vec<4,T, glm::defaultp> v){\
    x = static_cast<float>(v.x);\
    y = static_cast<float>(v.y);\
    }\
    template<typename T>\
    operator glm::vec<4, T, glm::defaultp>() const {\
        \
            return glm::vec<4, T, glm::defaultp>(\
            static_cast<T>(x),\
            static_cast<T>(y),\
            static_cast<T>(0),\
             static_cast<T>(0)); \
    }



#define IM_VEC4_CLASS_EXTRA                                                                       \
    ImVec4(const sf::Color& c) : x(c.r / 255.f), y(c.g / 255.f), z(c.b / 255.f), w(c.a / 255.f) { \
    }                                                                                             \
    operator sf::Color() const {                                                                  \
        return sf::Color(static_cast<std::uint8_t>(x * 255.f),                                    \
                         static_cast<std::uint8_t>(y * 255.f),                                    \
                         static_cast<std::uint8_t>(z * 255.f),                                    \
                         static_cast<std::uint8_t>(w * 255.f));                                   \
    }\
    template<typename T>\
    ImVec4(const glm::vec<2,T,glm::defaultp> v){\
    x = static_cast<float>(v.x);\
    y = static_cast<float>(v.y);\
    z = 0.0f;\
    w = 0.0f;\
    }\
    template<typename T>\
    operator glm::vec<2, T, glm::defaultp>() const {\
        \
            return glm::vec<2, T, glm::defaultp>(static_cast<T>(x), static_cast<T>(y)); \
    }\
    template<typename T>\
    ImVec4(const glm::vec<3,T, glm::defaultp> v){\
    x = static_cast<float>(v.x);\
    y = static_cast<float>(v.y);\
    z = static_cast<float>(v.z);\
    w = static_cast<float>(0);\
    }\
    template<typename T>\
    operator glm::vec<3, T, glm::defaultp>() const {\
        \
            return glm::vec<3, T, glm::defaultp>(static_cast<T>(x), static_cast<T>(y),static_cast<T>(z)); \
    }\
    template<typename T>\
    ImVec4(const glm::vec<4,T, glm::defaultp> v){\
    x = static_cast<float>(v.x);\
    y = static_cast<float>(v.y);\
    z = static_cast<float>(v.z);\
    w = static_cast<float>(v.w);\
    }\
    template<typename T>\
    operator glm::vec<4, T, glm::defaultp>() const {\
        \
            return glm::vec<4, T, glm::defaultp>(\
            static_cast<T>(x),\
            static_cast<T>(y),\
            static_cast<T>(z),\
            static_cast<T>(w)); \
    }


