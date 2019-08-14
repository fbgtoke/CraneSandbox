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
  /* Text setup                                                               */
  /****************************************************************************/
  m_Font = Crane::Font::create("resources/fonts/font.ttf");
  m_Text = Crane::Text::create(m_Font, "HELLO");

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

  std::ifstream fragment_fstream("resources/shaders/text.frag");
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
  
  //std::cout << "Frame time: " << deltatime.asSeconds() << "s ("
  //          << 1.f/deltatime.asSeconds() << "fps)" << std::endl;

  std::ostringstream ss;
  ss << 1.f/deltatime.asSeconds() << "fps";
  m_Text->setString(ss.str());

  m_Transform.recomputeMatrix();
}

void SandboxOverlay::onRender() const
{
  Crane::Layer::onRender();

  m_ShaderProgram->use();
  m_ShaderProgram->setUniformMat4f("VP", &m_Camera.getViewProjectionMatrix()[0][0]);
  m_ShaderProgram->setUniformMat4f("TG", &m_Transform.getTransformMatrix()[0][0]);

  Crane::Renderer::render(
    m_ShaderProgram,
    m_Text
  );
}
