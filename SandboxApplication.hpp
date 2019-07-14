#pragma once
#include <Crane.hpp>

class SandboxApplication : public Crane::Application {
public:
  SandboxApplication();

protected:
  void onEvent(Crane::Event* e) override;
};