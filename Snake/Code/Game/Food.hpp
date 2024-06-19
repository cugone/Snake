#pragma once

#include "Engine/Core/TimeUtils.hpp"

#include "Engine/Math/Vector2.hpp"

#include "Engine/Renderer/Mesh.hpp"

class ConstantBuffer;

class Food {
public:
    enum class Type {
        Apple_Red1
        , Apple_Red2
        , Apple_Green
        , Egg
        , Cookie
        , Max
    };

    Food() noexcept;
    explicit Food(const Vector2& position) noexcept;
    Food(const Food& other) = default;
    Food(Food&& other) = default;
    Food& operator=(const Food& other) = default;
    Food& operator=(Food&& other) = default;
    ~Food() = default;

    void AddMeshToBuilder(Mesh::Builder& builder) noexcept;

    Vector2 position{ Vector2::One };
    Food::Type type{Food::Type::Apple_Red1};
protected:
private:

};
