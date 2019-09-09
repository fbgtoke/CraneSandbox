#pragma once
#include <Crane.hpp>

class SandboxOverlay : public Crane::Layer {
public:
  SandboxOverlay();

protected:
  void onUpdate(Crane::Time deltatime) override;
  void onRender() const override;

private:
  Crane::Shader* m_Shader;

  Crane::VertexArray* m_VertexArray;
  Crane::VertexBuffer* m_VertexBuffer;
  Crane::IndexBuffer* m_IndexBuffer;

  Crane::Font* m_Font;
  Crane::Text* m_Text;
  Crane::Transform m_Transform;

  Crane::OrthographicCamera m_Camera;

  Crane::Time m_LastFrame;
};