#include <8085_gui.h>

class cEmu8085Gui : public emu8085Gui {
  bool check = false;
  float value = 0.0f;
  int counter = 0;
  bool button = false;

public:
  void update();
};
int main() {
  cEmu8085Gui CPU;

  CPU.run();

  return 0;
}

void cEmu8085Gui::update() {
  if (ImGui::Begin("Registers")) {
  }
  ImGui::End();
}