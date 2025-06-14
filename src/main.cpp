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
  char whexBuffer[5] = {0};
  byte *value;
  byte *flags;

public:
  void update();

  friend void draw_reg(cEmu8085Gui &obj, std::string reg);
  friend void draw_reg_pair(cEmu8085Gui &obj, std::string reg);
  friend void draw_flag(cEmu8085Gui &obj, std::string flagName, bool flag);
};
void draw_reg(cEmu8085Gui &obj, std::string reg) {
  obj.value = (byte *)obj.term.perform("printr " + reg);
  snprintf(obj.hexBuffer, sizeof(obj.hexBuffer), "%02X", *obj.value);
  ImGui::Text((reg + " :").c_str());
  ImGui::SameLine();
  if (ImGui::InputText(("##" + reg).c_str(), obj.hexBuffer,
                       sizeof(obj.hexBuffer),
                       ImGuiInputTextFlags_CharsHexadecimal)) {
    obj.term.perform("loadr " + reg + " " + obj.hexBuffer);
  }
}

void draw_reg_pair(cEmu8085Gui &obj, std::string reg) {
  obj.value = (byte *)(obj.term.perform(("printr " + reg).c_str()));
  snprintf(obj.whexBuffer, sizeof(obj.whexBuffer), "%04X",
           word(((*obj.value) << 8) + *(obj.value + sizeof(byte))));
  ImGui::Text((reg + " :").c_str());
  ImGui::SameLine();
  if (ImGui::InputText(("##" + reg).c_str(), obj.whexBuffer,
                       sizeof(obj.whexBuffer),
                       ImGuiInputTextFlags_CharsHexadecimal)) {
    std::string input = obj.whexBuffer;
    while (input.size() < 4) {

      input = "0" + input;
    }

    obj.term.perform("loadrp " + reg + " " + input[0] + input[1] + " " +
                     input[2] + input[3]);
  }
}

void draw_flag(cEmu8085Gui &obj, std::string flagName, bool flag) {
  snprintf(obj.hexBuffer, sizeof(obj.hexBuffer), "%01X", flag);

  ImGui::Text((flagName + " :").c_str());
  ImGui::SameLine();
  if (ImGui::InputText(("##" + flagName).c_str(), obj.hexBuffer,
                       sizeof(obj.hexBuffer),
                       ImGuiInputTextFlags_CharsHexadecimal)) {
    obj.term.perform("loadf " + flagName + " " + obj.hexBuffer);
  }
}
int main() {
  cEmu8085Gui CPU;

  CPU.run();

  return 0;
}

void cEmu8085Gui::update() {
  ImVec2 viewportSize = ImGui::GetMainViewport()->Size;

  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always, ImVec2(0, 0));

  ImGui::SetNextWindowSize(ImVec2(viewportSize.x / 2, viewportSize.y / 2),
                           ImGuiCond_Always);
  if (ImGui::Begin("Registers", nullptr,
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoMove)) {

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

    draw_reg(*this, "I");

    ImGui::PushItemWidth(15);
    flags = (byte *)term.perform("printf");

    bool S = ((*flags) >> 7);
    bool Z = (*flags) >> 6;
    bool AC = (*flags) >> 5;
    bool CY = (*flags) >> 4;
    bool P = (*flags) >> 3;

    draw_flag(*this, "S", S);

    ImGui::SameLine();

    draw_flag(*this, "Z", Z);

    ImGui::SameLine();

    draw_flag(*this, "AC", AC);

    ImGui::SameLine();

    draw_flag(*this, "CY", CY);

    ImGui::SameLine();

    draw_flag(*this, "P", P);
  }

  ImGui::End();

  ImGui::SetNextWindowSize(ImVec2(viewportSize.x, viewportSize.y / 2),
                           ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2(0, viewportSize.y / 2), ImGuiCond_Always,
                          ImVec2(0, 0));

  if (ImGui::Begin("Terminal", nullptr,
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoMove)) {
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
  ImGui::SetNextWindowPos(ImVec2(viewportSize.x, 0), ImGuiCond_Always,
                          ImVec2(1, 0));
  ImVec2 fixedSize = ImVec2(viewportSize.x / 2, viewportSize.y / 2);
  ImGui::SetNextWindowSize(fixedSize, ImGuiCond_Always);
  ImGui::Begin("Memory Viewer", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoMove);
  ImGui::BeginChild("TableRegion", fixedSize, true,
                    ImGuiWindowFlags_HorizontalScrollbar |
                        ImGuiWindowFlags_AlwaysVerticalScrollbar);
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
    ImGui::EndChild();
    ImGui::End();
  }
}
