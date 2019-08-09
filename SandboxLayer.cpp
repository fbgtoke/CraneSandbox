#include "SandboxLayer.hpp"

#include <fstream>
#include <iostream>
#include <vector>

SandboxLayer::SandboxLayer()
  : Crane::Layer::Layer(), m_Camera(-1.f, 1.f, -1.f, 1.f)
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
    { "position", Crane::ShaderDatatype::Float3 },
    { "uvs", Crane::ShaderDatatype::Float2 },
    { "normal", Crane::ShaderDatatype::Float3 }
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
  m_Texture.create(w, h, &data[0]);

  if (!m_Font.loadFromFile("resources/fonts/font.ttf"))
  {
    exit(EXIT_FAILURE);
  }

  /****************************************************************************/
  /* Shaders setup                                                            */
  /****************************************************************************/
  std::ifstream vertex_fstream("resources/shaders/simple.vert");
  std::string vertex_src;
  vertex_src.assign(
    std::istreambuf_iterator<char>(vertex_fstream),
    std::istreambuf_iterator<char>()
  );
  Crane::Shader* vertex_shader =
    Crane::Shader::create(Crane::Shader::Vertex, vertex_src);

  std::ifstream fragment_fstream("resources/shaders/simple.frag");
  std::string fragment_src;
  fragment_src.assign(
    std::istreambuf_iterator<char>(fragment_fstream),
    std::istreambuf_iterator<char>()
  );
  Crane::Shader* fragment_shader =
    Crane::Shader::create(Crane::Shader::Fragment, fragment_src);

  m_ShaderProgram = Crane::ShaderProgram::create();
  m_ShaderProgram->attach(vertex_shader);
  m_ShaderProgram->attach(fragment_shader);
  if (!m_ShaderProgram->link())
  {
    vertex_shader->destroy();
    fragment_shader->destroy();
    m_ShaderProgram->destroy();

    exit(EXIT_FAILURE);
  }

  m_ShaderProgram->detach(vertex_shader);
  m_ShaderProgram->detach(fragment_shader);
}

void SandboxLayer::onUpdate(Crane::Time deltatime)
{
  Crane::Layer::onUpdate(deltatime);
  
  std::cout << "Frame time: " << deltatime.asSeconds() << "s ("
            << 1.f/deltatime.asSeconds() << "fps)" << std::endl;

  /* Transform controls */
  if (Crane::Input::isKeyPressed(Crane::Keyboard::A))
  {
    m_Transform.move(deltatime.asSeconds() * Crane::Vec3(-1.f, 0.f, 0.f));
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::D))
  {
    m_Transform.move(deltatime.asSeconds() * Crane::Vec3(1.f, 0.f, 0.f));
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::W))
  {
    m_Transform.move(deltatime.asSeconds() * Crane::Vec3(0.f, 1.f, 0.f));
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::S))
  {
    m_Transform.move(deltatime.asSeconds() * Crane::Vec3(0.f, -1.f, 0.f));
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::Z))
  {
    m_Transform.scale(deltatime.asSeconds() * Crane::Vec3(0.1f));
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::C))
  {
    m_Transform.scale(deltatime.asSeconds() * Crane::Vec3(-0.1f));
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::Q))
  {
    m_Transform.rotate(deltatime.asSeconds() * Crane::Vec3(0.f, 0.f, 0.1f));
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::E))
  {
    m_Transform.rotate(deltatime.asSeconds() * Crane::Vec3(0.f, 0.f, -0.1f));
  }

  /* Camera controls */
  if (Crane::Input::isKeyPressed(Crane::Keyboard::Left))
  {
    m_Camera.move(deltatime.asSeconds() * Crane::Vec3(1.f, 0.f, 0.f));
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::Right))
  {
    m_Camera.move(deltatime.asSeconds() * Crane::Vec3(-1.f, 0.f, 0.f));
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::Up))
  {
    m_Camera.move(deltatime.asSeconds() * Crane::Vec3(0.f, -1.f, 0.f));
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::Down))
  {
    m_Camera.move(deltatime.asSeconds() * Crane::Vec3(0.f, 1.f, 0.f));
  }

  if (Crane::Input::isKeyPressed(Crane::Keyboard::PageUp))
  {
    m_Camera.rotate(0.2f * deltatime.asSeconds());
  }
  else if (Crane::Input::isKeyPressed(Crane::Keyboard::PageDown))
  {
    m_Camera.rotate(-0.2f * deltatime.asSeconds());
  }

  m_Camera.recomputeMatrices();
  m_Transform.recomputeMatrix();
}

void SandboxLayer::onRender() const
{
  Crane::Layer::onRender();

  m_ShaderProgram->use();

  m_ShaderProgram->setUniformMat4f("VP", &m_Camera.getViewProjectionMatrix()[0][0]);
  m_ShaderProgram->setUniformMat4f("TG", &m_Transform.getTransformMatrix()[0][0]);
  Crane::Renderer::renderIndexed(
    m_VertexArray,
    m_ShaderProgram,
    *m_Font.getCharacterTexture('A')
    //m_Texture
  );
}
