
#pragma once

#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/TimeUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/Mesh.hpp"

#include <vector>

class ConstantBuffer;

class SnakeSegment {
public:
    SnakeSegment() = default;
    SnakeSegment(const SnakeSegment& other) = default;
    SnakeSegment(SnakeSegment&& other) = default;
    SnakeSegment& operator=(const SnakeSegment& other) = default;
    SnakeSegment& operator=(SnakeSegment&& other) = default;
    ~SnakeSegment() = default;

    SnakeSegment(const Vector2& position, const Rgba& color, SnakeSegment* parent = nullptr, SnakeSegment* child = nullptr);

    void SetParent(SnakeSegment* parent) noexcept;
    void SetChild(SnakeSegment* child) noexcept;

    bool IsHead() const noexcept;
    bool IsBody() const noexcept;
    bool IsTail() const noexcept;

    void AddToMesh(Mesh::Builder& builder) noexcept;

    Vector2 position{};

protected:
    void AddChild(SnakeSegment* child) noexcept;
    
    std::vector<Vector2> positions{};
    Rgba color{ Rgba::Green };
    SnakeSegment* parent_segment{ nullptr };
    SnakeSegment* child_segment{ nullptr };
private:

};
