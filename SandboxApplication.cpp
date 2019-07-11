#include "SandboxApplication.hpp"

#include <Render/Renderer.hpp>
#include <Render/Shader.hpp>

#include <iostream>

SandboxApplication::SandboxApplication()
  : Crane::Application::Application()
{
  std::cerr << "SandboxApplication constructor" << std::endl;

  /****************************************************************************/
  /* Arrays and buffers setup                                                 */
  /****************************************************************************/
  m_VertexArray.create();
  m_VertexArray.bind();

  float vertices[9] = {
  //    x      y     z
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  m_VertexBuffer.create(sizeof(vertices), (void*)&vertices[0]);
  m_VertexBuffer.setLayout({
    { "position", Crane::ShaderDatatype::Float3 }
  });

  m_VertexArray.addVertexBuffer(&m_VertexBuffer);

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
}

void SandboxApplication::onRender()
{
  Crane::Application::onRender();
  //std::cout << "Render" << std::endl;

  Crane::Renderer::render(m_VertexArray, m_ShaderProgram);
}
