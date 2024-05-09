#pragma once

#include "Engine/Core/TimeUtils.hpp"

#include "Engine/Renderer/Mesh.hpp"

class Wall {
public:
    void BeginFrame() noexcept;
    void Update(TimeUtils::FPSeconds deltaSeconds) noexcept;
    void Render() const;
    void EndFrame() noexcept;
protected:
private:
    Mesh::Builder builder{};
};
