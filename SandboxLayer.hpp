#pragma once
#include <Crane.hpp>

class SandboxLayer : public Crane::Layer {
public:
  SandboxLayer();

protected:
  void onUpdate(Crane::Time deltatime) override;
  void onRender() const override;

private:
  Crane::VertexArray m_VertexArray;
  Crane::VertexBuffer m_VertexBuffer;
  Crane::IndexBuffer m_IndexBuffer;

  Crane::Texture m_Texture;

  Crane::ShaderProgram m_ShaderProgram;

  Crane::OrthographicCamera m_Camera;

  Crane::Time m_LastFrame;

  Crane::Transform m_Transform;
};