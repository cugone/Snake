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
    };
    Wall() = default;
    Wall(const Vector2& position, const Direction& direction) noexcept;
    Wall(const Wall& other) = default;
    Wall(Wall&& rother) = default;
    Wall& operator=(const Wall& other) = default;
    Wall& operator=(Wall&& rother) = default;
    ~Wall() = default;

    void BeginFrame() noexcept;
    void Update(TimeUtils::FPSeconds deltaSeconds) noexcept;
    void Render() const;
    void EndFrame() noexcept;

    void AppendToMesh(Mesh::Builder& builder) noexcept;

    Vector2 position{};
    Direction direction{Direction::Full};
protected:
private:

    struct selection_state_t {
        Vector4 direction_padding3;
    };

    ConstantBuffer* selection_state_cb{ nullptr };
    mutable selection_state_t selection_state{};
};
