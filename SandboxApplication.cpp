#include "SandboxApplication.hpp"

#include <Render/Renderer.hpp>
#include <Render/Shader.hpp>
#include <Serialization/BmpReader.hpp>
#include <Serialization/ObjReader.hpp>

#include <iostream>
#include <vector>

SandboxApplication::SandboxApplication()
  : Crane::Application::Application(), m_Camera(-1.f, 1.f, -1.f, 1.f)
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
  m_VertexArray.create();
  m_VertexArray.bind();

  m_VertexBuffer.create(vertices.size() * sizeof(float), (void*)&vertices[0]);
  m_VertexBuffer.setLayout({
    { "position", Crane::ShaderDatatype::Float3 },
    { "uvs", Crane::ShaderDatatype::Float2 },
    { "normal", Crane::ShaderDatatype::Float3 }
  });
  m_VertexArray.addVertexBuffer(&m_VertexBuffer);

  m_IndexBuffer.create(indices.size() * sizeof(unsigned int), &indices[0]);
  m_VertexArray.setIndexBuffer(&m_IndexBuffer);

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

  /****************************************************************************/
  /* Shaders setup                                                            */
  /****************************************************************************/
  Crane::Shader vertex_shader;
  vertex_shader.create(Crane::Shader::Vertex);
  vertex_shader.compileFromFile("resources/shaders/simple.vert");
  if (!vertex_shader.isCompiled())
  {
    std::cerr << vertex_shader.getInfoLog() << std::endl;
    
    vertex_shader.destroy();

    exit(EXIT_FAILURE);
  }

  Crane::Shader fragment_shader;
  fragment_shader.create(Crane::Shader::Fragment);
  fragment_shader.compileFromFile("resources/shaders/simple.frag");
  if (!fragment_shader.isCompiled())
  {
    std::cerr << fragment_shader.getInfoLog() << std::endl;

    vertex_shader.destroy();
    fragment_shader.destroy();

    exit(EXIT_FAILURE);
  }

  m_ShaderProgram.create();
  m_ShaderProgram.attach(vertex_shader);
  m_ShaderProgram.attach(fragment_shader);
  m_ShaderProgram.link();
  if (!m_ShaderProgram.isLinked())
  {
    std::cerr << m_ShaderProgram.getInfoLog() << std::endl;

    vertex_shader.destroy();
    fragment_shader.destroy();
    m_ShaderProgram.destroy();

    exit(EXIT_FAILURE);
  }

  m_ShaderProgram.detach(vertex_shader);
  m_ShaderProgram.detach(fragment_shader);
}

SandboxApplication::~SandboxApplication()
{

}

void SandboxApplication::onEvent(Crane::Event* e)
{
  Crane::Application::onEvent(e);

  if (e->getType() == Crane::Event::WindowClosed)
  {
    end();
  }
  else if (e->getType() == Crane::Event::WindowResized)
  {
    Crane::WindowResizedEvent* re = (Crane::WindowResizedEvent*)e;
    std::cout << "Resized to ("
              << re->getWidth() << "," 
              << re->getHeight() << ")" 
              << std::endl;
  }
}

void SandboxApplication::onUpdate()
{
  Crane::Application::onUpdate();
  //std::cout << "Update" << std::endl;

  m_Camera.move({ 0.001f, 0.001f, 0.f });
  m_Camera.rotate(0.01f);
  m_Camera.recomputeMatrices();
}

void SandboxApplication::onRender()
{
  Crane::Application::onRender();
  //std::cout << "Render" << std::endl;

  m_ShaderProgram.setUniformMat4f(0, &m_Camera.getViewProjectionMatrix()[0][0]);
  Crane::Renderer::renderIndexed(m_VertexArray, m_ShaderProgram, m_Texture);
}
