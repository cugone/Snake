#include "Game/Game.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/KerningFont.hpp"
#include "Engine/Core/Rgba.hpp"


#include "Engine/Input/InputSystem.hpp"

#include "Engine/Math/Disc2.hpp"

#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Material.hpp"

#include "Engine/Renderer/Mesh.hpp"

#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/Platform/DirectX/DirectX11FrameBuffer.hpp"

#include "Engine/Services/ServiceLocator.hpp"
#include "Engine/Services/IAppService.hpp"

#include "Engine/UI/UISystem.hpp"

#include "Game/GameCommon.hpp"
#include "Game/GameConfig.hpp"

#include "Game/Wall.hpp"

Game::~Game() {
    map.reset();
    m_frameBuffer.reset();
}

void Game::Initialize() noexcept {
    {
        FrameBufferDesc desc{};
        desc.width = 1600;
        desc.height = 900;
        desc.SwapChainTarget = true;
        m_frameBuffer = FrameBuffer::Create(desc);
    }
    {
        auto texture = g_theRenderer->Create2DTextureArrayFromFolder(std::filesystem::path{ "Data/Images/Assets/" });
        GUARANTEE_OR_DIE(texture && g_theRenderer->RegisterTexture("assets", std::move(texture)), "Failed to register texture array of assets.");
        g_theRenderer->RegisterTexturesFromFolder(std::filesystem::path{ "Data/Images/Assets/" });
    }

    g_theRenderer->RegisterMaterialsFromFolder(FileUtils::GetKnownFolderPath(FileUtils::KnownPathID::GameMaterials));
    g_theRenderer->RegisterFontsFromFolder(FileUtils::GetKnownFolderPath(FileUtils::KnownPathID::GameFonts));

    map = std::make_unique<Map>();
}

void Game::BeginFrame() noexcept {
    map->BeginFrame();
}

void Game::Update(TimeUtils::FPSeconds deltaSeconds) noexcept {
    g_theRenderer->UpdateGameTime(deltaSeconds);

    HandlePlayerInput(deltaSeconds);

    _ui_camera2D.Update(deltaSeconds);

    map->Update(deltaSeconds);

}

void Game::Render() const noexcept {

    m_frameBuffer->Bind(Rgba::ForestGreen);

    //World View
    {
        map->Render();
    }

    // HUD View

}

void Game::EndFrame() noexcept {
    map->EndFrame();
    m_frameBuffer->Unbind();
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
