#include "Game/Wall.hpp"

#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/EngineCommon.hpp"

#include "Engine/Renderer/Renderer.hpp"


void Wall::BeginFrame() noexcept {
    builder.Clear();
}

void Wall::Update([[maybe_unused]] TimeUtils::FPSeconds deltaSeconds) noexcept {

    const auto uvs = AABB2::Zero_to_One;
    const auto mat = g_theRenderer->GetMaterial("wall");

    builder.Begin(PrimitiveType::Triangles);

    builder.SetColor(Rgba::White);

    builder.SetUV(Vector2{ uvs.mins.x, uvs.maxs.y });
    builder.AddVertex(Vector2{ -0.5f, +0.5f });

    builder.SetUV(Vector2{ uvs.mins.x, uvs.mins.y });
    builder.AddVertex(Vector2{ -0.5f, -0.5f });

    builder.SetUV(Vector2{ uvs.maxs.x, uvs.mins.y });
    builder.AddVertex(Vector2{ +0.5f, -0.5f });

    builder.SetUV(Vector2{ uvs.maxs.x, uvs.maxs.y });
    builder.AddVertex(Vector2{ +0.5f, +0.5f });


    builder.AddIndicies(Mesh::Builder::Primitive::Quad);
    builder.End(mat);
}

void Wall::Render() const {
    const auto S = Matrix4::CreateScaleMatrix(Vector2::One * 32.0f);
    const auto R = Matrix4::I;
    auto T = Matrix4::I;
    auto M = Matrix4::MakeSRT(S, R, T);
    g_theRenderer->SetModelMatrix(M);
    Mesh::Render(builder);
}

void Wall::EndFrame() noexcept {

}
