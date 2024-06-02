#include "Game/SnakeSegment.hpp"

#include "Engine/Core/EngineCommon.hpp"

#include "Engine/Math/AABB2.hpp"

#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/Renderer.hpp"

SnakeSegment::SnakeSegment(const Vector2& position, const Rgba& color, SnakeSegment* parent /*= nullptr*/, SnakeSegment* child /*= nullptr*/)
    : color(color)
    , parent_segment(parent)
    , child_segment(child)
{
    positions.reserve(100);
    positions.push_back(position);

    AddChild(child);
    SetParent(parent);
}

void SnakeSegment::SetParent(SnakeSegment* parent) noexcept {
    if(parent) {
        this->parent_segment = parent;
        this->parent_segment->child_segment = this;
    }
}

void SnakeSegment::SetChild(SnakeSegment* child) noexcept {
    AddChild(child);
}

bool SnakeSegment::IsHead() const noexcept {
    return !parent_segment && child_segment;
}

bool SnakeSegment::IsBody() const noexcept {
    return parent_segment && child_segment;
}

bool SnakeSegment::IsTail() const noexcept {
    return parent_segment && !child_segment;
}

void SnakeSegment::AddToMesh(Mesh::Builder& builder) noexcept {

    builder.Begin(PrimitiveType::Triangles);

    builder.SetColor(Rgba::White);

    const auto uvs = AABB2::Zero_to_One;
    builder.SetUV(Vector2{ uvs.mins.x, uvs.maxs.y });
    builder.AddVertex(positions[0] + Vector2{ -0.5f, +0.5f });

    builder.SetUV(Vector2{ uvs.mins.x, uvs.mins.y });
    builder.AddVertex(positions[0] + Vector2{ -0.5f, -0.5f });

    builder.SetUV(Vector2{ uvs.maxs.x, uvs.mins.y });
    builder.AddVertex(positions[0] + Vector2{ +0.5f, -0.5f });

    builder.SetUV(Vector2{ uvs.maxs.x, uvs.maxs.y });
    builder.AddVertex(positions[0] + Vector2{ +0.5f, +0.5f });

    builder.AddIndicies(Mesh::Builder::Primitive::Quad);
    builder.End(g_theRenderer->GetMaterial("tile"));
}

void SnakeSegment::AddChild(SnakeSegment* child) noexcept {
    if(child) {
        this->child_segment = child;
        this->child_segment->parent_segment = this;
        this->child_segment->parent_segment->child_segment = this->child_segment;
    }
}
