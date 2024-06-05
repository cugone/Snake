#include "Game/Wall.hpp"

#include "Engine/Core/EngineCommon.hpp"

#include "Engine/Services/ServiceLocator.hpp"
#include "Engine/Services/IRendererService.hpp"

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"

#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"

#include <type_traits>

constexpr static float wall_index_offset = 12.0f;

constexpr float WallDirectionToTextureIndexOffset(Wall::Direction direction) noexcept;
Wall::Direction GetRandomDirection() noexcept;

constexpr float WallDirectionToTextureIndexOffset(Wall::Direction direction) noexcept {
    switch (direction) {
    case Wall::Direction::Full:
        return wall_index_offset + 0.0f;
    case Wall::Direction::Left:
        return wall_index_offset + 1.0f;
    case Wall::Direction::Horizontal:
        return wall_index_offset + 2.0f;
    case Wall::Direction::Right:
        return wall_index_offset + 3.0f;
    case Wall::Direction::Up:
        return wall_index_offset + 4.0f;
    case Wall::Direction::Vertical:
        return wall_index_offset + 5.0f;
    case Wall::Direction::Down:
        return wall_index_offset + 6.0f;
    default:
        return wall_index_offset;
    }
}

Wall::Direction GetRandomDirection() noexcept {
    return static_cast<Wall::Direction>(MathUtils::GetRandomLessThan(static_cast<std::underlying_type_t<Wall::Direction>>(Wall::Direction::Max)));
}

Wall::Wall() noexcept
    : Wall(Vector2::Zero, Wall::Direction::Full)
{
    /* DO NOTHING */
}

Wall::Wall(const Vector2& position, const Wall::Direction& direction) noexcept 
: position(position)
, direction(direction)
{
    /* DO NOTHING */
}

void Wall::AddMeshToBuilder(Mesh::Builder& builder) noexcept {

    builder.Begin(PrimitiveType::Triangles);

    builder.SetColor(Rgba::White);

    const auto uvs = AABB2::Zero_to_One;
    builder.SetUV(Vector2{ uvs.mins.x, uvs.maxs.y });
    builder.AddVertex(Vector3{position + Vector2{ -0.5f, +0.5f }, WallDirectionToTextureIndexOffset(direction)});

    builder.SetUV(Vector2{ uvs.mins.x, uvs.mins.y });
    builder.AddVertex(Vector3{position + Vector2{ -0.5f, -0.5f }, WallDirectionToTextureIndexOffset(direction) });

    builder.SetUV(Vector2{ uvs.maxs.x, uvs.mins.y });
    builder.AddVertex(Vector3{position + Vector2{ +0.5f, -0.5f }, WallDirectionToTextureIndexOffset(direction) });

    builder.SetUV(Vector2{ uvs.maxs.x, uvs.maxs.y });
    builder.AddVertex(Vector3{position + Vector2{ +0.5f, +0.5f }, WallDirectionToTextureIndexOffset(direction) });

    builder.AddIndicies(Mesh::Builder::Primitive::Quad);

    builder.End(g_theRenderer->GetMaterial("tile"));

}
