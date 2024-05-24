#include "Game/Map.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/TimeUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

#include "Engine/Input/InputSystem.hpp"

Map::Map() noexcept {
    m_cameraController.SetPosition(Vector2{895.0f, 496.0f});
    m_cameraController.SetZoomLevel(512.0f);
    m_cameraController.SetSpeedMultiplier(2048);

    m_bounds.StretchToIncludePoint(Vector2(57.0f, 32.0f));

    m_validFoodBounds = m_bounds;
    m_validFoodBounds.AddPaddingToSides(-2.0f, -2.0f);

    const auto width = m_bounds.maxs.x;
    const auto height = m_bounds.maxs.y;
    const auto reserve_count = 2.0f * (width + height - 2.0f);
    m_walls.reserve(static_cast<std::size_t>(reserve_count));

    //Horizontal Endcaps
    m_walls.push_back(Wall{ Vector2{1.0f, 0.0f}, Wall::Direction::Left });
    m_walls.push_back(Wall{ Vector2{width - 2.0f, 0.0f}, Wall::Direction::Right});
    m_walls.push_back(Wall{ Vector2{1.0f, height - 1.0f}, Wall::Direction::Left});
    m_walls.push_back(Wall{ Vector2{width - 2.0f, height - 1.0f}, Wall::Direction::Right });
    
    //Vertical Endcaps
    m_walls.push_back(Wall{ Vector2{0.0f, 1.0f}, Wall::Direction::Up });
    m_walls.push_back(Wall{ Vector2{0.0f, height - 2.0f}, Wall::Direction::Down});
    m_walls.push_back(Wall{ Vector2{width - 1.0f, 1.0f}, Wall::Direction::Up});
    m_walls.push_back(Wall{ Vector2{width - 1.0f, height - 2.0f}, Wall::Direction::Down});

    //Corners
    m_walls.push_back(Wall{ Vector2{0.0f, 0.0f}, Wall::Direction::Full });
    m_walls.push_back(Wall{ Vector2{width - 1.0f, 0.0f}, Wall::Direction::Full});
    m_walls.push_back(Wall{ Vector2{width - 1.0f, height - 1.0f}, Wall::Direction::Full });
    m_walls.push_back(Wall{ Vector2{0.0f, height - 1.0f}, Wall::Direction::Full});

    //Top/Bottom
    for(auto x = 2.0f; x < width - 2.0f; ++x) {
        m_walls.push_back(Wall{ Vector2{x, 0.0f}, Wall::Direction::Horizontal });
        m_walls.push_back(Wall{ Vector2{x, height - 1.0f}, Wall::Direction::Horizontal });
    }
    //Left/Right
    for (auto y = 2.0f; y < height - 2.0f; ++y) {
        m_walls.push_back(Wall{ Vector2{0.0f, y}, Wall::Direction::Vertical});
        m_walls.push_back(Wall{ Vector2{width - 1.0f, y}, Wall::Direction::Vertical});
    }

    m_food.position = Vector2{-1.0f, -1.0f};
    SpawnFoodAtRandom();
}

void Map::BeginFrame() noexcept {
    m_builder.Clear();
    if (m_canSpawnFood) {
        SpawnFoodAtRandom();
    }
}

void Map::Update(TimeUtils::FPSeconds deltaSeconds) noexcept {
    m_cameraController.Update(deltaSeconds);
    for(auto& wall : m_walls) {
        wall.Update(deltaSeconds);
    }
    m_food.Update(deltaSeconds);
}

void Map::Render() const noexcept {

    m_cameraController.SetModelViewProjectionBounds();

    const auto S = Matrix4::CreateScaleMatrix(Vector2::One * 32.0f);
    const auto R = Matrix4::I;
    auto T = Matrix4::I;
    auto M = Matrix4::MakeSRT(S, R, T);
    g_theRenderer->SetModelMatrix(M);

    for (auto& wall : m_walls) {
        wall.Render();
    }
    m_food.Render();

}

void Map::EndFrame() noexcept {
    /* DO NOTHING */
}

void Map::DebugRender() noexcept {

    g_theRenderer->SetMaterial("__2D");
    g_theRenderer->DrawAABB2(m_bounds, Rgba::Orange, Rgba::NoAlpha);
    g_theRenderer->DrawAABB2(m_validFoodBounds, Rgba::White, Rgba::NoAlpha);

    AABB2 food_bounds = AABB2::Zero_to_One;
    food_bounds.SetPosition(m_food.position);

    g_theRenderer->DrawAABB2(food_bounds, Rgba::White, Rgba::NoAlpha);

}

const OrthographicCameraController& Map::GetCameraController() const noexcept {
    return m_cameraController;
}

OrthographicCameraController& Map::GetCameraController() noexcept {
    return m_cameraController;
}

void Map::SpawnFoodAt(const Vector2& position) noexcept {
    m_food.position = MathUtils::CalcClosestPoint(position, m_validFoodBounds);
}

void Map::SpawnFoodAtRandom() noexcept {
    SpawnFoodAt(MathUtils::GetRandomPointInside(m_validFoodBounds));
}
