#include "SandboxLayer.hpp"

#include <fstream>
#include <iostream>
#include <vector>

SandboxLayer::SandboxLayer()
  : Crane::Layer::Layer(), m_Camera(M_PI/3.f, 1.f, 0.01f, 10.f)
{
  /****************************************************************************/
  /* Load models                                                              */
  /****************************************************************************/
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  if (!Crane::ObjReader::read("resources/models/cube.obj", vertices, indices))
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
  /* Texture setup                                                            */
  /****************************************************************************/
  std::size_t w, h;
  std::vector<unsigned char> data;
  if(!Crane::BmpReader::read("resources/textures/quad.bmp", w, h, data))
  {
    exit(EXIT_FAILURE);
  }
  m_Texture = Crane::Texture::create(w, h, &data[0]);

  if(!Crane::BmpReader::read("resources/textures/quad2.bmp", w, h, data))
  {
    exit(EXIT_FAILURE);
  }
  m_Texture2 = Crane::Texture::create(w, h, &data[0]);

  /****************************************************************************/
  /* Shaders setup                                                            */
  /****************************************************************************/
  m_Shader = Crane::Shader::create("resources/shaders/simple.glsl");
  
  /****************************************************************************/
  /* Camera setup                                                             */
  /****************************************************************************/
  m_Camera.getTransform().setPosition(Crane::Math::Vec3(0.f, 0.f, -5.f));

  /****************************************************************************/
  /* Transforms setup                                                         */
  /****************************************************************************/
  m_Transform2.move(Crane::Math::Vec3(4.f, 0.f, 2.f));
}

void SandboxLayer::onUpdate(Crane::Time deltatime)
{
  Crane::Layer::onUpdate(deltatime);

  /* Transform controls */
  if (Crane::Input::isKeyPressed(Crane::Keyboard::A))
  {
    m_Transform.move(deltatime.asSeconds() * Crane::Math::Vec3{-1.f, 0.f, 0.f});
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::D))
  {
    m_Transform.move(deltatime.asSeconds() * Crane::Math::Vec3{1.f, 0.f, 0.f});
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::W))
  {
    m_Transform.move(deltatime.asSeconds() * Crane::Math::Vec3{0.f, 1.f, 0.f});
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::S))
  {
    m_Transform.move(deltatime.asSeconds() * Crane::Math::Vec3{0.f, -1.f, 0.f});
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::Z))
  {
    m_Transform.scale(deltatime.asSeconds() * Crane::Math::Vec3(0.1f));
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::C))
  {
    m_Transform.scale(deltatime.asSeconds() * Crane::Math::Vec3(-0.1f));
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::Q))
  {
    m_Transform.rotate(2.f * deltatime.asSeconds() * Crane::Math::Vec3{0.f, 0.f, 0.1f});
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::E))
  {
    m_Transform.rotate(2.f * deltatime.asSeconds() * Crane::Math::Vec3{0.f, 0.f, -0.1f});
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::Num8))
  {
    m_Transform.rotate(2.f * deltatime.asSeconds() * Crane::Math::Vec3{1.f, 0.f, 0.f});
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::Num2))
  {
    m_Transform.rotate(2.f * deltatime.asSeconds() * Crane::Math::Vec3{-1.f, 0.f, 0.f});
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::Num6))
  {
    m_Transform.rotate(2.f * deltatime.asSeconds() * Crane::Math::Vec3{0.f, 1.f, 0.f});
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::Num4))
  {
    m_Transform.rotate(2.f * deltatime.asSeconds() * Crane::Math::Vec3{0.f, -1.f, 0.f});
  }

  /* Camera controls */
  if (Crane::Input::isKeyPressed(Crane::Keyboard::Left))
  {
    m_Camera.getTransform().move(deltatime.asSeconds() * Crane::Math::Vec3{-1.f, 0.f, 0.f});
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::Right))
  {
    m_Camera.getTransform().move(deltatime.asSeconds() * Crane::Math::Vec3{1.f, 0.f, 0.f});
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::Up))
  {
    m_Camera.getTransform().move(deltatime.asSeconds() * Crane::Math::Vec3{0.f, 1.f, 0.f});
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::Down))
  {
    m_Camera.getTransform().move(deltatime.asSeconds() * Crane::Math::Vec3{0.f, -1.f, 0.f});
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::PageUp))
  {
    m_Camera.getTransform().rotate(deltatime.asSeconds() * Crane::Math::Vec3(0.8f, 0.f, 0.f));
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::PageDown))
  {
    m_Camera.getTransform().rotate(deltatime.asSeconds() * Crane::Math::Vec3(-0.8f, 0.f, 0.f));
  }

  m_Camera.recomputeMatrices();

  m_Transform.recomputeMatrix();
  m_Transform2.recomputeMatrix();

  //m_Transform2 = m_Transform;
  //m_Transform2.move(Crane::Math::Vec3(2.f, 0.f, 2.f));
  //m_Transform2.recomputeMatrix();
}

void SandboxLayer::onRender() const
{
  Crane::Layer::onRender();

  m_Shader->use();

  m_Shader->setUniformMat4f("VP", &m_Camera.getViewProjectionMatrix()[0][0]);

  m_Shader->setUniformMat4f("TG", &m_Transform.getTransformMatrix()[0][0]);
  Crane::Renderer::renderIndexed(
    m_VertexArray,
    m_Shader,
    m_Texture
  );

  m_Shader->setUniformMat4f("TG", &m_Transform2.getTransformMatrix()[0][0]);
  Crane::Renderer::renderIndexed(
    m_VertexArray,
    m_Shader,
    m_Texture2
  );
}
