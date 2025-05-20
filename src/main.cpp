#include <8085_gui.h>

std::string formatHexFast(int value, char delimiter = ' ') {
  const char *hexDigits = "0123456789ABCDEF";
  char buffer[6]; // 4 hex digits + 1 delimiter + null terminator

  // Mask to 16 bits to be safe
  value &= 0xFFFF;

  buffer[0] = hexDigits[(value >> 12) & 0xF];
  buffer[1] = hexDigits[(value >> 8) & 0xF];
  buffer[2] = delimiter;
  buffer[3] = hexDigits[(value >> 4) & 0xF];
  buffer[4] = hexDigits[value & 0xF];
  buffer[5] = '\0';

  return std::string(buffer);
}
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
  if (ImGui::BeginTable("LargeTable", 2,
                        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {

    ImGui::TableSetupColumn("Address");
    ImGui::TableSetupColumn("Value");
    ImGui::TableHeadersRow();

    ImGuiListClipper clipper;
    clipper.Begin(65536); // Total number of rows

    while (clipper.Step()) {
      for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%04X", row);
        ImGui::TableSetColumnIndex(1);
        std::string result = std::string("print") + " " + formatHexFast(row);

        context *a = (context *)(term.perform(result));
        ImGui::Text(" %02X", static_cast<int>(a->DB));
      }
    }

    ImGui::EndTable();
  }
}