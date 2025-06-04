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

  char hexBuffer[3];
  char whexBuffer[5];
  byte *value;
  byte *flags;

public:
  void update();

  friend void draw_reg(cEmu8085Gui &obj, std::string reg);
  friend void draw_reg_pair(cEmu8085Gui &obj, std::string reg);
  friend void draw_flag(cEmu8085Gui &obj, bool flag);
};
void draw_reg(cEmu8085Gui &obj, std::string reg) {
  obj.value = (byte *)obj.term.perform("printr " + reg);
  snprintf(obj.hexBuffer, sizeof(obj.hexBuffer), "%02X", *obj.value);
  ImGui::Text("A :");
  ImGui::SameLine();
  if (ImGui::InputText("##A", obj.hexBuffer, sizeof(obj.hexBuffer),
                       ImGuiInputTextFlags_CharsHexadecimal))
    ;
  {
    obj.term.perform(std::string("loadr A ") + obj.hexBuffer);
  }
}

void draw_reg_pair(cEmu8085Gui &obj, std::string reg) {
  obj.value = (byte *)(obj.term.perform("printr SP"));
  snprintf(obj.whexBuffer, sizeof(obj.whexBuffer), "%04X",
           word(((*obj.value) << 8) + *(obj.value + sizeof(byte))));
  ImGui::Text("SP :");
  ImGui::SameLine();
  ImGui::InputText("##SP", obj.whexBuffer, sizeof(obj.whexBuffer),
                   ImGuiInputTextFlags_CharsHexadecimal);
}

void draw_flag(cEmu8085Gui &obj, bool flag) {
  snprintf(obj.hexBuffer, sizeof(obj.hexBuffer), "%01X", flag);

  ImGui::Text("S :");
  ImGui::SameLine();
  ImGui::InputText("##S", obj.hexBuffer, sizeof(obj.hexBuffer),
                   ImGuiInputTextFlags_CharsHexadecimal);
}
int main() {
  cEmu8085Gui CPU;

  CPU.run();

  return 0;
}

void cEmu8085Gui::update() {
  if (ImGui::Begin("Registers")) {

    ImGui::PushItemWidth(25);

    draw_reg(*this, "A");

    draw_reg(*this, "B");

    ImGui::SameLine(0, 50);

    draw_reg(*this, "C");

    draw_reg(*this, "D");

    ImGui::SameLine(0, 50);

    draw_reg(*this, "E");

    draw_reg(*this, "H");

    ImGui::SameLine(0, 50);

    draw_reg(*this, "L");

    ImGui::PushItemWidth(35);

    draw_reg_pair(*this, "SP");

    ImGui::SameLine(0, 50);

    draw_reg_pair(*this, "PC");

    ImGui::SameLine(0, 50);

    draw_reg_pair(*this, "I");

    ImGui::PushItemWidth(15);
    flags = (byte *)term.perform("printf");

    bool S = ((*flags) >> 7);
    bool Z = (*flags) >> 6;
    bool AC = (*flags) >> 5;
    bool CY = (*flags) >> 4;
    bool P = (*flags) >> 3;

    draw_flag(*this, S);

    ImGui::SameLine();

    draw_flag(*this, Z);

    ImGui::SameLine();

    draw_flag(*this, AC);

    ImGui::SameLine();

    draw_flag(*this, CY);

    ImGui::SameLine();

    draw_flag(*this, P);
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
        std::string result = std::string("print ") + formatHexFast(row);

        value = (byte *)term.perform(result);
        snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);
        if (ImGui::InputText(("##" + std::to_string(row)).c_str(), hexBuffer,
                             sizeof(hexBuffer),
                             ImGuiInputTextFlags_CharsHexadecimal |
                                 ImGuiInputTextFlags_EnterReturnsTrue |
                                 ImGuiInputTextFlags_AutoSelectAll)) {
          std::string cmd = "load " + formatHexFast(row) + " " + hexBuffer;
          term.perform(cmd);
        }
      }
    }

    ImGui::EndTable();
  }
}