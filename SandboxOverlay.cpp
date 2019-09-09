#include "SandboxOverlay.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

SandboxOverlay::SandboxOverlay()
  : Crane::Layer::Layer(), m_Camera(-1.f, 1.f, 1.f, -1.f)
{
  /****************************************************************************/
  /* Load models                                                              */
  /****************************************************************************/
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  if (!Crane::ObjReader::read("resources/models/quad.obj", vertices, indices))
  {
    exit(EXIT_FAILURE);
  }

  /****************************************************************************/
  /* Arrays and buffers setup                                                 */
  /****************************************************************************/
  m_VertexArray = Crane::VertexArray::create();
  m_VertexArray->bind();

  m_VertexBuffer = Crane::VertexBuffer::create(
    vertices.size() * sizeof(float), (void*)&vertices[0]
  );
  m_VertexBuffer->setLayout({
    { "position", Crane::Shader::Float3 },
    { "uvs", Crane::Shader::Float2 },
    { "normal", Crane::Shader::Float3 }
  });
  m_VertexArray->addVertexBuffer(m_VertexBuffer);

  m_IndexBuffer = Crane::IndexBuffer::create(
    indices.size() * sizeof(unsigned int), &indices[0]
  );
  m_VertexArray->setIndexBuffer(m_IndexBuffer);

  /****************************************************************************/
  /* Text setup                                                               */
  /****************************************************************************/
  m_Font = Crane::Font::create("resources/fonts/font.ttf");
  m_Text = Crane::Text::create(m_Font, "HELLO");

  /****************************************************************************/
  /* Shaders setup                                                            */
  /****************************************************************************/
  m_Shader = Crane::Shader::create("resources/shaders/simple.glsl", "simple");
  
  /****************************************************************************/
  /* Transforms setup                                                         */
  /****************************************************************************/
  m_Transform.setPosition(Crane::Math::Vec3(-0.9f, -0.9f, 0.f));
  //m_Transform.move(Crane::Math::Vec3(-.5f, -.5f, 0.f));
  m_Transform.setScale(.005f);
}

void SandboxOverlay::onUpdate(Crane::Time deltatime)
{
  Crane::Layer::onUpdate(deltatime);

  std::ostringstream ss;
  ss << 1.f/deltatime.asSeconds() << "fps";
  m_Text->setString(ss.str());

  m_Transform.recomputeMatrix();
}

void SandboxOverlay::onRender() const
{
  Crane::Layer::onRender();

  m_Shader->use();
  m_Shader->setUniformMat4f("VP", &m_Camera.getViewProjectionMatrix()[0][0]);
  m_Shader->setUniformMat4f("TG", &m_Transform.getTransformMatrix()[0][0]);

  Crane::Renderer::render(
    m_Shader,
    m_Text
  );
}
