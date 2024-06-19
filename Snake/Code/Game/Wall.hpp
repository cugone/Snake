#pragma once

#include "Engine/Core/TimeUtils.hpp"

#include "Engine/Math/Vector2.hpp"

#include "Engine/Renderer/Mesh.hpp"

class ConstantBuffer;

class Wall {
public:
    enum class Direction : int {
        Full,
        Left,
        Horizontal,
        Right,
        Up,
        Vertical,
        Down,
        Max,
    };
    Wall() noexcept;
    Wall(const Vector2& position, const Direction& direction) noexcept;
    Wall(const Wall& other) = default;
    Wall(Wall&& rother) = default;
    Wall& operator=(const Wall& other) = default;
    Wall& operator=(Wall&& rother) = default;
    ~Wall() = default;

    void AddMeshToBuilder(Mesh::Builder& builder) noexcept;

    Vector2 position{};
    Direction direction{Direction::Full};
protected:
private:
};
