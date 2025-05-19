#include <8085_gui.h>

class cEmu8085Gui : public emu8085Gui {
  char inputBuffer[128] = "";
  bool execute = false;
  context *c = (context *)(term.perform("context"));

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
    std::string input = "context";
    if (execute) {
      c = (context *)(term.perform(input));
      execute = !execute;
    }
    ImGui::Text("A : %d", static_cast<int>(c->A));
    ImGui::Text("B : %d\t C : %d", static_cast<int>(c->B),
                static_cast<int>(c->C));

    ImGui::Text("D : %d\t E : %d", static_cast<int>(c->D),
                static_cast<int>(c->E));

    ImGui::Text("H : %d\t L : %d", static_cast<int>(c->H),
                static_cast<int>(c->L));
    ImGui::Text("SP : %d\t PC : %d",
                static_cast<int>(word((c->SPU << 8) + c->SPL)),
                static_cast<int>(word((c->PCU << 8) + c->PCL)));
    ImGui::Text("I : %d", static_cast<int>(c->I));

    ImGui::Text("S : %d  Z : %d  AC : %d  P : %d  CY : %d",
                static_cast<int>(c->S), static_cast<int>(c->Z),
                static_cast<int>(c->AC), static_cast<int>(c->P),
                static_cast<int>(c->CY));
  }
  ImGui::End();

  if (ImGui::Begin("Terminal")) {

    // Label must be unique per field
    ImGui::InputText("Command", inputBuffer, IM_ARRAYSIZE(inputBuffer));

    if (ImGui::Button("Execute")) {
      // Use the input
      std::string input(inputBuffer);
      term.perform(input); // Run command
      execute = !execute;
    }
  }
  ImGui::End();
}