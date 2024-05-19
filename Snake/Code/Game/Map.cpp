#include "Game/Map.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"

Map::Map() noexcept {
    m_cameraController.SetPosition(Vector2{895.0f, 496.0f});
    m_cameraController.SetZoomLevel(512.0f);
    m_cameraController.SetSpeedMultiplier(2048);
    m_walls.reserve(100);
    for(auto y = 0.0f; y < 32.0f; ++y) {
        for (auto x = 0.0f; x < 57.0f; ++x) {
            m_walls.push_back(Wall{ Vector2{x, y}, Wall::Direction::Full });
        }
    }
}

void Map::BeginFrame() noexcept {
    m_builder.Clear();
}

void Map::Update(TimeUtils::FPSeconds deltaSeconds) noexcept {
    m_cameraController.Update(deltaSeconds);
    for(auto& wall : m_walls) {
        wall.AppendToMesh(m_builder);
    }
}

void Map::Render() const noexcept {
    
    g_theRenderer->SetOrthoProjectionFromViewWidth(static_cast<float>(1600), m_cameraController.GetAspectRatio(), 0.01f, 1.0f);
    g_theRenderer->SetCamera(m_cameraController.GetCamera());

    m_cameraController.SetModelViewProjectionBounds();

    const auto S = Matrix4::CreateScaleMatrix(Vector2::One * 32.0f);
    const auto R = Matrix4::I;
    auto T = Matrix4::I;
    auto M = Matrix4::MakeSRT(S, R, T);
    g_theRenderer->SetModelMatrix(M);

    for (auto& wall : m_walls) {
        wall.Render();
    }
    Mesh::Render(m_builder);
}

void Map::EndFrame() noexcept {
    /* DO NOTHING */
}

const OrthographicCameraController& Map::GetCameraController() const noexcept {
    return m_cameraController;
}

OrthographicCameraController& Map::GetCameraController() noexcept {
    return m_cameraController;
}
