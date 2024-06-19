#include "Game/Snake.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"

#include "Game/Map.hpp"

constexpr const float yellow_index_offset = 9.0f;
constexpr const float green_index_offset = 6.0f;

float GetSegmentIndex(const Rgba& color);
float GetHeadIndex(const Rgba& color);
float GetEyesIndex(const Rgba& color);

float GetSegmentIndex(const Rgba& color) {
    if (color == Rgba::Yellow) {
        return yellow_index_offset;
    }
    else {
        return green_index_offset;
    }
}

float GetHeadIndex(const Rgba& color) {
    if (color == Rgba::Yellow) {
        return yellow_index_offset + 1.0f;
    } else {
        return green_index_offset + 1.0f;
    }
}

float GetEyesIndex(const Rgba& color) {
    if (color == Rgba::Yellow) {
        return yellow_index_offset + 2.0f;
    } else {
        return green_index_offset + 2.0f;
    }
}

Snake::Snake(Map* parent_map, const Rgba& color) noexcept
    : m_map(parent_map)
    , m_color(color)
    , m_velocity{Vector2::X_Axis * m_speed}
{
    m_blinkRate.SetFrequency(3);
    m_blinkDelay.SetSeconds(TimeUtils::FPSeconds{2.5f});
    AddSegment();
}

void Snake::Update(TimeUtils::FPSeconds deltaSeconds) noexcept {
    m_canBlink = m_blinkDelay.CheckAndReset();
    m_startBlinking = m_blinkRate.Check();

    if(g_theInputSystem->WasKeyJustPressed(KeyCode::D)) {
        MoveRight();
    } else if (g_theInputSystem->WasKeyJustPressed(KeyCode::A)) {
        MoveLeft();
    } else if (g_theInputSystem->WasKeyJustPressed(KeyCode::W)) {
        MoveUp();
    } else if (g_theInputSystem->WasKeyJustPressed(KeyCode::S)) {
        MoveDown();
    }

    for(auto current_segment = std::rbegin(m_segments); current_segment != std::rend(m_segments); /* DO NOTHING */) {
        const auto next_segment = current_segment + 1;
        if (next_segment == std::rend(m_segments)) {
            break;
        }
        current_segment->position = next_segment->position;
        current_segment = next_segment;
    }
    m_segments[0].position += m_velocity * deltaSeconds.count();

}

bool Snake::Blink() noexcept {
    m_startBlinking = m_blinkRate.CheckAndReset();
    return m_startBlinking && m_canBlink;
}

void Snake::AddMeshToBuilder(Mesh::Builder& builder) noexcept {
    builder.Begin(PrimitiveType::Triangles);
    builder.SetColor(Rgba::White);
    const auto blink = Blink();
    for(auto segment = std::crbegin(m_segments); segment != std::crend(m_segments); ++segment) {

        const auto uvs = AABB2::Zero_to_One;
        builder.SetUV(Vector2{ uvs.mins.x, uvs.maxs.y });
        builder.AddVertex(Vector3{segment->position + Vector2{ -0.5f, +0.5f }, segment->IsHead() ? GetHeadIndex(m_color) : GetSegmentIndex(m_color)});

        builder.SetUV(Vector2{ uvs.mins.x, uvs.mins.y });
        builder.AddVertex(Vector3{segment->position + Vector2{ -0.5f, -0.5f }, segment->IsHead() ? (blink ? GetHeadIndex(m_color) : GetEyesIndex(m_color)) : GetSegmentIndex(m_color) });

        builder.SetUV(Vector2{ uvs.maxs.x, uvs.mins.y });
        builder.AddVertex(Vector3{segment->position + Vector2{ +0.5f, -0.5f }, segment->IsHead() ? (blink ? GetHeadIndex(m_color) : GetEyesIndex(m_color)) : GetSegmentIndex(m_color) });

        builder.SetUV(Vector2{ uvs.maxs.x, uvs.maxs.y });
        builder.AddVertex(Vector3{segment->position + Vector2{ +0.5f, +0.5f }, segment->IsHead() ? (blink ? GetHeadIndex(m_color) : GetEyesIndex(m_color)) : GetSegmentIndex(m_color) });

        builder.AddIndicies(Mesh::Builder::Primitive::Quad);
    }
    builder.End(g_theRenderer->GetMaterial("tile"));


}

void Snake::EndFrame() noexcept {
    /* DO NOTHING */
}

void Snake::AddSegment() noexcept {
    if (m_segments.empty()) {
        m_segments.push_back(SnakeSegment(Vector2::Zero, m_color));
        m_segments.push_back(SnakeSegment(m_segments[0].position - m_velocity, m_color));
        m_segments.push_back(SnakeSegment(m_segments[1].position - m_velocity, m_color));
        auto* head = &m_segments[0];
        auto* middle = &m_segments[1];
        auto* tail = &m_segments[2];
        head->SetParent(nullptr);
        head->SetChild(middle);
        middle->SetParent(head);
        middle->SetChild(tail);
        tail->SetParent(middle);
        tail->SetChild(nullptr);
    } else {
        auto* old_tail = &m_segments.back();
        m_segments.push_back(SnakeSegment(old_tail->position - m_velocity, m_color));
        auto* new_tail = &m_segments.back();
        old_tail->SetChild(new_tail);
        new_tail->SetParent(old_tail);

    }
}

void Snake::SetPosition(const Vector2& newPosition) noexcept {
    for (auto& segment : m_segments) {
        segment.position = newPosition;
    }
}

Vector2 Snake::GetHeadPosition() const noexcept {
    return m_segments[0].position;
}

void Snake::MoveRight() noexcept {
    m_velocity = Vector2::X_Axis * m_speed;
}

void Snake::MoveLeft() noexcept {
    m_velocity = -Vector2::X_Axis * m_speed;
}

void Snake::MoveUp() noexcept {
    m_velocity = -Vector2::Y_Axis * m_speed;
}

void Snake::MoveDown() noexcept {
    m_velocity = Vector2::Y_Axis * m_speed;
}
