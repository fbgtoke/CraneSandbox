#include <Core/Application.hpp>
#include <Core/Time.hpp>
#include <Render/IndexBuffer.hpp>
#include <Render/OrthographicCamera.hpp>
#include <Render/Texture.hpp>
#include <Render/ShaderProgram.hpp>
#include <Render/VertexArray.hpp>
#include <Render/VertexBuffer.hpp>
#include <Window/Event.hpp>

class SandboxApplication : public Crane::Application {
public:
  SandboxApplication();
  ~SandboxApplication() override;

protected:
  void onEvent(Crane::Event* e) override;
  void onUpdate() override;
  void onRender() override;

private:
  Crane::VertexArray m_VertexArray;
  Crane::VertexBuffer m_VertexBuffer;
  Crane::IndexBuffer m_IndexBuffer;

  Crane::Texture m_Texture;

  Crane::ShaderProgram m_ShaderProgram;

  Crane::OrthographicCamera m_Camera;

  Crane::Time m_LastFrame;
};