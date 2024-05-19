#pragma once

#include "Engine/Core/OrthographicCameraController.hpp"
#include "Engine/Core/TimeUtils.hpp"
#include "Engine/Renderer/Mesh.hpp"

#include "Game/Wall.hpp"

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

    const OrthographicCameraController& GetCameraController() const noexcept;
    OrthographicCameraController& GetCameraController() noexcept;

protected:
private:
    Mesh::Builder m_builder{};
    std::vector<Wall> m_walls{};
    OrthographicCameraController m_cameraController{};
};
