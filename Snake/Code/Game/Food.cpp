#include "Game/Food.hpp"

#include "Engine/Math/MathUtils.hpp"

#include "Engine/Core/EngineCommon.hpp"

#include "Engine/Services/ServiceLocator.hpp"
#include "Engine/Services/IRendererService.hpp"

#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/Renderer.hpp"

constexpr float FoodTypeToTextureIndexOffset(Food::Type type) noexcept;
const Food::Type GetRandomFoodType() noexcept;
constexpr const float GetRandomFoodTypeIndex() noexcept;

constexpr float FoodTypeToTextureIndexOffset(Food::Type type) noexcept {
    switch (type) {
    case Food::Type::Apple_Red1: return 0.0f;
    case Food::Type::Apple_Red2: return 2.0f;
    case Food::Type::Apple_Green: return 1.0f;
    case Food::Type::Egg: return 4.0f;
    case Food::Type::Cookie: return 5.0f;
    default: return 0.0f;
    };

}

const Food::Type GetRandomFoodType() noexcept {
    return static_cast<Food::Type>((MathUtils::GetRandomLessThan(static_cast<int>(Food::Type::Max))));
}

constexpr const float GetRandomFoodTypeIndex() noexcept {
    return FoodTypeToTextureIndexOffset(GetRandomFoodType());
}

Food::Food() noexcept
    : Food(Vector2::Zero)
{
    /* DO NOTHING */
}

Food::Food(const Vector2& position) noexcept
    : position(position)
{
    type = GetRandomFoodType();
}

void Food::AddMeshToBuilder(Mesh::Builder& builder) noexcept {

    builder.Begin(PrimitiveType::Triangles);

    builder.SetColor(Rgba::White);

    const auto uvs = AABB2::Zero_to_One;
    builder.SetUV(Vector2{ uvs.mins.x, uvs.maxs.y });
    builder.AddVertex(Vector3{position + Vector2{ -0.5f, +0.5f }, FoodTypeToTextureIndexOffset(type)});

    builder.SetUV(Vector2{ uvs.mins.x, uvs.mins.y });
    builder.AddVertex(Vector3{position + Vector2{ -0.5f, -0.5f }, FoodTypeToTextureIndexOffset(type) });

    builder.SetUV(Vector2{ uvs.maxs.x, uvs.mins.y });
    builder.AddVertex(Vector3{position + Vector2{ +0.5f, -0.5f }, FoodTypeToTextureIndexOffset(type) });

    builder.SetUV(Vector2{ uvs.maxs.x, uvs.maxs.y });
    builder.AddVertex(Vector3{position + Vector2{ +0.5f, +0.5f }, FoodTypeToTextureIndexOffset(type) });

    builder.AddIndicies(Mesh::Builder::Primitive::Quad);

    builder.End(g_theRenderer->GetMaterial("tile"));

}
