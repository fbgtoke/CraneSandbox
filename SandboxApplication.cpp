#include "SandboxApplication.hpp"
#include "SandboxOverlay.hpp"
#include "SandboxLayer.hpp"

#include <glm/vec3.hpp>

#include <iostream>
#include <vector>


Crane::Application* Crane::Application::createApplication()
{
  return new SandboxApplication();
}

SandboxApplication::SandboxApplication()
  : Crane::Application::Application()
{
  getLayerStack().addLayer(new SandboxLayer());
  getLayerStack().addLayer(new SandboxOverlay());
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
  else if (e->getType() == Crane::Event::KeyPressed)
  {
    Crane::KeyPressedEvent * ke = (Crane::KeyPressedEvent*)e;
    if (ke->getKey() == Crane::Keyboard::Escape)
    {
      end();
    }
  }
}