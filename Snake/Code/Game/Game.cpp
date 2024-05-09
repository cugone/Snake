#include "Game/Game.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/KerningFont.hpp"

#include "Engine/Input/InputSystem.hpp"

#include "Engine/Math/Disc2.hpp"

#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Material.hpp"

#include "Engine/Renderer/Mesh.hpp"

#include "Engine/Services/ServiceLocator.hpp"
#include "Engine/Services/IAppService.hpp"

#include "Engine/UI/UISystem.hpp"

#include "Game/GameCommon.hpp"
#include "Game/GameConfig.hpp"

#include "Game/Wall.hpp"

void Game::Initialize() noexcept {
    g_theRenderer->RegisterMaterialsFromFolder(FileUtils::GetKnownFolderPath(FileUtils::KnownPathID::GameMaterials));
    g_theRenderer->RegisterFontsFromFolder(FileUtils::GetKnownFolderPath(FileUtils::KnownPathID::GameFonts));

    _cameraController = OrthographicCameraController();
    _cameraController.SetPosition(Vector2::Zero);
    _cameraController.SetZoomLevel(512.0f);

}

Wall wall;

void Game::BeginFrame() noexcept {
    /* DO NOTHING */
}

void Game::Update(TimeUtils::FPSeconds deltaSeconds) noexcept {
    g_theRenderer->UpdateGameTime(deltaSeconds);

    HandlePlayerInput(deltaSeconds);

    _ui_camera2D.Update(deltaSeconds);
    _cameraController.Update(deltaSeconds);

    wall.Update(deltaSeconds);

}

void Game::Render() const noexcept {
    g_theRenderer->BeginRenderToBackbuffer();

    _cameraController.SetModelViewProjectionBounds();

    //World View
    {
        wall.Render();
    }

    // HUD View
    {
        const auto ui_view_height = static_cast<float>(GetSettings().GetWindowHeight());
        const auto ui_view_width = ui_view_height * _ui_camera2D.GetAspectRatio();
        const auto ui_view_extents = Vector2{ui_view_width, ui_view_height};
        const auto ui_view_half_extents = ui_view_extents * 0.5f;
        const auto ui_cam_pos = Vector2::Zero;
        g_theRenderer->BeginHUDRender(_ui_camera2D, ui_cam_pos, ui_view_height);

    }
}

void Game::EndFrame() noexcept {
    /* DO NOTHING */
}

const GameSettings& Game::GetSettings() const noexcept {
    return GameBase::GetSettings();
}

GameSettings& Game::GetSettings() noexcept {
    return GameBase::GetSettings();
}

void Game::HandlePlayerInput(TimeUtils::FPSeconds deltaSeconds) {
    HandleKeyboardInput(deltaSeconds);
    HandleControllerInput(deltaSeconds);
    HandleMouseInput(deltaSeconds);
}

void Game::HandleKeyboardInput(TimeUtils::FPSeconds /*deltaSeconds*/) {
    if(g_theInputSystem->WasKeyJustPressed(KeyCode::Esc)) {
        auto* app = ServiceLocator::get<IAppService>();
        app->SetIsQuitting(true);
        return;
    }
}

void Game::HandleControllerInput(TimeUtils::FPSeconds /*deltaSeconds*/) {

}

void Game::HandleMouseInput(TimeUtils::FPSeconds /*deltaSeconds*/) {

}

void Game::HandleDebugInput(TimeUtils::FPSeconds deltaSeconds) {
    HandleDebugKeyboardInput(deltaSeconds);
    HandleDebugMouseInput(deltaSeconds);
}

void Game::HandleDebugKeyboardInput(TimeUtils::FPSeconds /*deltaSeconds*/) {
    if(g_theUISystem->WantsInputKeyboardCapture()) {
        return;
    }
    if(g_theInputSystem->WasKeyJustPressed(KeyCode::F4)) {
        g_theUISystem->ToggleImguiDemoWindow();
    }
}

void Game::HandleDebugMouseInput(TimeUtils::FPSeconds /*deltaSeconds*/) {
    if(g_theUISystem->WantsInputMouseCapture()) {
        return;
    }
}
