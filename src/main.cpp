#include <8085_gui.h>

class cEmu8085Gui : public emu8085Gui {
  bool check = false;
  float value = 0.0f;
  int counter = 0;
  bool button = false;

public:
  void update() {
    ImGui::Begin("My Inner Window"); // Begin an ImGui window
    ImGui::Text("Hello from ImGui!");
    if (ImGui::Button("Click me")) {
      counter++;
    }

    ImGui::Text("Value : %d", counter);
    ImGui::SliderFloat("Value", &value, 0.0f, 1.0f);
    ImGui::End();
  }
};
int main() {
  cEmu8085Gui CPU;

  CPU.run();

  return 0;
}