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

    ImGui::PushItemWidth(30);
    value = (byte *)term.perform("printr A");
    snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);
    ImGui::Text("A :");
    ImGui::SameLine();
    if (ImGui::InputText("##A", hexBuffer, sizeof(hexBuffer),
                         ImGuiInputTextFlags_CharsHexadecimal))
      ;
    {
      term.perform(std::string("loadr A ") + hexBuffer);
    }

    value = (byte *)term.perform("printr B");
    snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);

    ImGui::Text("B :");
    ImGui::SameLine();
    if (ImGui::InputText("##B", hexBuffer, sizeof(hexBuffer),
                         ImGuiInputTextFlags_CharsHexadecimal)) {
      term.perform(std::string("loadr B ") + hexBuffer);
    }
    ImGui::SameLine(0, 50);
    value = (byte *)term.perform("printr C");
    snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);

    ImGui::Text("C :");
    ImGui::SameLine();
    if (ImGui::InputText("##C", hexBuffer, sizeof(hexBuffer),
                         ImGuiInputTextFlags_CharsHexadecimal)) {
      term.perform(std::string("loadr C ") + hexBuffer);
    }

    value = (byte *)term.perform("printr D");
    snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);

    ImGui::Text("D :");
    ImGui::SameLine();
    if (ImGui::InputText("##D", hexBuffer, sizeof(hexBuffer),
                         ImGuiInputTextFlags_CharsHexadecimal)) {
      term.perform(std::string("loadr D ") + hexBuffer);
    }

    ImGui::SameLine(0, 50);

    value = (byte *)term.perform("printr E");
    snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);

    ImGui::Text("E :");
    ImGui::SameLine();
    if (ImGui::InputText("##E", hexBuffer, sizeof(hexBuffer),
                         ImGuiInputTextFlags_CharsHexadecimal)) {
      term.perform(std::string("loadr E ") + hexBuffer);
    }

    value = (byte *)term.perform("printr H");
    snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);

    ImGui::Text("H :");
    ImGui::SameLine();
    if (ImGui::InputText("##H", hexBuffer, sizeof(hexBuffer),
                         ImGuiInputTextFlags_CharsHexadecimal)) {
      term.perform(std::string("loadr H ") + hexBuffer);
    }

    ImGui::SameLine(0, 50);

    value = (byte *)term.perform("printr L");
    snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);

    ImGui::Text("L :");
    ImGui::SameLine();
    if (ImGui::InputText("##L", hexBuffer, sizeof(hexBuffer),
                         ImGuiInputTextFlags_CharsHexadecimal)) {
      term.perform(std::string("loadr L ") + hexBuffer);
    }
    ImGui::PushItemWidth(40);
    value = (byte *)(term.perform("printr SP"));
    snprintf(whexBuffer, sizeof(whexBuffer), "%04X",
             word(((*value) << 8) + *(value + sizeof(byte))));
    ImGui::Text("SP :");
    ImGui::SameLine();
    ImGui::InputText("##SP", whexBuffer, sizeof(whexBuffer),
                     ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::SameLine(0, 50);

    value = (byte *)(term.perform("printr PC"));
    snprintf(whexBuffer, sizeof(whexBuffer), "%04X",
             word(((*value) << 8) + *(value + sizeof(byte))));

    ImGui::Text("PC :");
    ImGui::SameLine();
    ImGui::InputText("##PC", whexBuffer, sizeof(whexBuffer),
                     ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::SameLine(0, 50);

    value = (byte *)term.perform("printr I");
    snprintf(hexBuffer, sizeof(hexBuffer), "%02X", *value);

    ImGui::Text("I :");
    ImGui::SameLine();
    ImGui::InputText("##I", hexBuffer, sizeof(hexBuffer),
                     ImGuiInputTextFlags_CharsHexadecimal); /*

     value = static_cast<int>(c->S);
     snprintf(hexBuffer, sizeof(hexBuffer), "%02X", value);

     ImGui::Text("S :");
     ImGui::SameLine();
     ImGui::InputText("##S", hexBuffer, sizeof(hexBuffer),
                      ImGuiInputTextFlags_CharsHexadecimal);

     ImGui::SameLine();

     value = static_cast<int>(c->S);
     snprintf(hexBuffer, sizeof(hexBuffer), "%02X", value);
     ImGui::Text("Z :");
     ImGui::SameLine();
     ImGui::InputText("##Z", hexBuffer, sizeof(hexBuffer),
                      ImGuiInputTextFlags_CharsHexadecimal);

     ImGui::SameLine();

     value = static_cast<int>(c->AC);
     snprintf(hexBuffer, sizeof(hexBuffer), "%02X", value);
     ImGui::Text("AC :");
     ImGui::SameLine();
     ImGui::InputText("##AC", hexBuffer, sizeof(hexBuffer),
                      ImGuiInputTextFlags_CharsHexadecimal);

     ImGui::SameLine();

     value = static_cast<int>(c->S);
     snprintf(hexBuffer, sizeof(hexBuffer), "%02X", value);
     ImGui::Text("CY :");
     ImGui::SameLine();
     ImGui::InputText("##CY", hexBuffer, sizeof(hexBuffer),
                      ImGuiInputTextFlags_CharsHexadecimal);

     ImGui::SameLine();

     value = static_cast<int>(c->P);
     snprintf(hexBuffer, sizeof(hexBuffer), "%02X", value);
     ImGui::Text("P :");
     ImGui::SameLine();
     ImGui::InputText("##P", hexBuffer, sizeof(hexBuffer),
                      ImGuiInputTextFlags_CharsHexadecimal);
   */
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