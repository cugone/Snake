#pragma once

#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Core/TimeUtils.hpp"

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector4.hpp"

#include "Engine/Renderer/Mesh.hpp"

#include "Game/SnakeSegment.hpp"

#include <vector>

class Map;

class Snake {
public:
    Snake() = default;
    Snake(const Snake& other) = default;
    Snake(Snake&& other) = default;
    Snake& operator=(const Snake& other) = default;
    Snake& operator=(Snake&& other) = default;
    ~Snake() = default;

    Snake(Map* parent_map, const Vector2& position, const Rgba& color) noexcept;

    void Update(TimeUtils::FPSeconds deltaSeconds) noexcept;
    void AddMeshToBuilder(Mesh::Builder& builder) noexcept;
    void EndFrame() noexcept;

    void AddSegment() noexcept;
    void SetPosition(const Vector2& newPosition) noexcept;

    Vector2 GetHeadPosition() const noexcept;
    void MoveRight() noexcept;
    void MoveLeft() noexcept;
    void MoveUp() noexcept;
    void MoveDown() noexcept;

protected:
private:

    struct selection_t {
        Vector4 selection_padding3{ Vector4::X_Axis };
    };
    std::vector<Vector2> m_positions{};
    Vector2 m_currentPosition{};
    Vector2 m_velocity{};
    float m_speed{10.0f};
    std::vector<SnakeSegment> m_segments{};
    Stopwatch m_blinkrate{};
    Map* m_map{nullptr};
    Rgba m_color{Rgba::Green};
};
