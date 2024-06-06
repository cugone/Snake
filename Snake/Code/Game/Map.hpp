#pragma once

#include "Engine/Core/OrthographicCameraController.hpp"
#include "Engine/Core/TimeUtils.hpp"

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"

#include "Engine/Renderer/Mesh.hpp"

#include "Game/Wall.hpp"
#include "Game/Food.hpp"
#include "Game/Snake.hpp"

#include <vector>

class Map {
public:
    Map() noexcept;
    Map(const Map& other) = default;
    Map(Map&& other) = default;
    Map& operator=(const Map& other) = default;
    Map& operator=(Map&& other) = default;
    ~Map() = default;

    void BeginFrame() noexcept;
    void Update(TimeUtils::FPSeconds deltaSeconds) noexcept;
    void Render() const noexcept;
    void EndFrame() noexcept;

    void DebugRender() noexcept;

    const OrthographicCameraController& GetCameraController() const noexcept;
    OrthographicCameraController& GetCameraController() noexcept;

    Vector2 GetDimensions() const noexcept;

protected:
private:

    void SpawnFoodAt(const Vector2& position) noexcept;
    void SpawnFoodAtRandom() noexcept;

    Mesh::Builder m_builder{};
    std::vector<Wall> m_walls{};
    Food m_food{ Vector2::Zero };
    OrthographicCameraController m_cameraController{};
    Snake m_snake{this, Rgba::Green};
    AABB2 m_bounds{AABB2::Neg_One_to_One};
    AABB2 m_validFoodBounds{};
    bool m_canSpawnFood{false};
};
