#include "Game/Map.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/TimeUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

#include "Engine/Input/InputSystem.hpp"

Map::Map() noexcept {
    m_cameraController.SetPosition(Vector2::Zero);
    m_cameraController.SetZoomLevel(512.0f);
    m_cameraController.SetSpeedMultiplier(2048);

    m_worldBounds.ScalePadding(55.5f * 0.5f, 30.5f * 0.5f);
    m_validFoodBounds = m_worldBounds;
    m_validFoodBounds.AddPaddingToSides(-1.0f, -1.0f);

    const auto horizontal_start = m_worldBounds.mins.x - 1.0f;
    const auto horizontal_end = m_worldBounds.maxs.x + 1.0f;
    const auto vertical_start = m_worldBounds.mins.y - 1.0f;
    const auto vertical_end = m_worldBounds.maxs.y + 1.0f;
    const auto width = m_worldBounds.CalcDimensions().x;
    const auto height = m_worldBounds.CalcDimensions().y;
    const auto reserve_count = 2.0f * (width + height - 2.0f);
    m_walls.reserve(static_cast<std::size_t>(reserve_count));

    //Horizontal Endcaps
    m_walls.push_back(Wall{ Vector2{horizontal_start + 1.5f, vertical_start + 0.5f}, Wall::Direction::Left });
    m_walls.push_back(Wall{ Vector2{horizontal_end - 1.5f, vertical_start + 0.5f}, Wall::Direction::Right});
    m_walls.push_back(Wall{ Vector2{horizontal_start + 1.5f, vertical_end - 0.5f}, Wall::Direction::Left});
    m_walls.push_back(Wall{ Vector2{horizontal_end - 1.5f, vertical_end - 0.5f}, Wall::Direction::Right });
    
    //Vertical Endcaps
    m_walls.push_back(Wall{ Vector2{horizontal_start + 0.5f, vertical_start + 1.5f}, Wall::Direction::Up });
    m_walls.push_back(Wall{ Vector2{horizontal_start + 0.5f, vertical_end - 1.5f}, Wall::Direction::Down});
    m_walls.push_back(Wall{ Vector2{horizontal_end - 0.5f, vertical_start + 1.5f}, Wall::Direction::Up});
    m_walls.push_back(Wall{ Vector2{horizontal_end - 0.5f, vertical_end - 1.5f}, Wall::Direction::Down});

    //Corners
    m_walls.push_back(Wall{ Vector2{horizontal_start + 0.5f, vertical_start + 0.5f}, Wall::Direction::Full });
    m_walls.push_back(Wall{ Vector2{horizontal_end - 0.5f, vertical_start + 0.5f}, Wall::Direction::Full});
    m_walls.push_back(Wall{ Vector2{horizontal_end - 0.5f, vertical_end - 0.5f}, Wall::Direction::Full });
    m_walls.push_back(Wall{ Vector2{horizontal_start + 0.5f, vertical_end - 0.5f}, Wall::Direction::Full});

    //Top/Bottom
    for(auto x = horizontal_start + 2.0f; x < horizontal_end - 2.0f; ++x) {
        m_walls.push_back(Wall{ Vector2{x, vertical_start + 0.5f}, Wall::Direction::Horizontal });
        m_walls.push_back(Wall{ Vector2{x, vertical_end - 0.5f}, Wall::Direction::Horizontal });
    }
    //Left/Right
    for (auto y = vertical_start + 2.0f; y < vertical_end - 2.0f; ++y) {
        m_walls.push_back(Wall{ Vector2{horizontal_start + 0.5f, y}, Wall::Direction::Vertical});
        m_walls.push_back(Wall{ Vector2{horizontal_end - 0.5f, y}, Wall::Direction::Vertical});
    }

    m_food.position = Vector2{-1.0f, -1.0f};
    SpawnFoodAtRandom();

}

void Map::BeginFrame() noexcept {
    m_builder.Clear();
    if (m_canSpawnFood) {
        SpawnFoodAtRandom();
        m_canSpawnFood = false;
    }
}

void Map::Update(TimeUtils::FPSeconds deltaSeconds) noexcept {
    if (m_isGameOver) {
        return;
    }
    m_cameraController.Update(deltaSeconds);
    m_snake.Update(deltaSeconds);
    for(auto& wall : m_walls) {
        wall.AddMeshToBuilder(m_builder);
    }
    m_food.AddMeshToBuilder(m_builder);
    m_snake.AddMeshToBuilder(m_builder);
}

void Map::Render() const noexcept {

    m_cameraController.SetModelViewProjectionBounds();

    const auto S = Matrix4::CreateScaleMatrix(Vector2::One * 32.0f);
    const auto R = Matrix4::I;
    const auto T = Matrix4::I;
    const auto M = Matrix4::MakeSRT(S, R, T);
    g_theRenderer->SetModelMatrix(M);


    Mesh::Render(m_builder);
}

void Map::EndFrame() noexcept {
    AABB2 food_bounds = AABB2::Zero_to_One;
    food_bounds.SetPosition(m_food.position);
    AABB2 player_bounds = AABB2::Zero_to_One;
    player_bounds.SetPosition(m_snake.GetHeadPosition());

    if (MathUtils::DoAABBsOverlap(food_bounds, player_bounds)) {
        m_canSpawnFood = true;
    }
    for (const auto& wall : m_walls) {
        const auto wall_bounds = [&wall]() {
            auto bounds = AABB2::Zero_to_One;
            bounds.Translate(wall.position);
            return bounds;
            }();
        if (MathUtils::DoAABBsOverlap(wall_bounds, player_bounds)) {
            m_isGameOver = true;
        }
    }
}

void Map::DebugRender() noexcept {

    g_theRenderer->SetMaterial("__2D");
    g_theRenderer->DrawAABB2(m_worldBounds, Rgba::Orange, Rgba::NoAlpha);
    g_theRenderer->DrawAABB2(m_validFoodBounds, Rgba::White, Rgba::NoAlpha);

    AABB2 food_bounds = AABB2::Zero_to_One;
    food_bounds.SetPosition(m_food.position);
    g_theRenderer->DrawAABB2(food_bounds, Rgba::White, Rgba::NoAlpha);

    AABB2 player_bounds = AABB2::Zero_to_One;
    player_bounds.SetPosition(m_snake.GetHeadPosition());
    g_theRenderer->DrawAABB2(player_bounds, Rgba::White, Rgba::NoAlpha);

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
    m_canSpawnFood = false;
}

Vector2 Map::GetDimensions() const noexcept {
    return m_worldBounds.CalcDimensions();
}

bool Map::IsGameOver() const noexcept {
    return m_isGameOver;
}
