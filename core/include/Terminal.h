#include <8085.h>

struct command {
  byte minLength;
  byte maxLength;
  std::function<void *(void)> function;
};

class terminal {

  emu8085 CPU;

  bool enableMessages = true;

  std::unordered_map<std::string, command> commandRepo;

  std::vector<std::string> token;

  std::string input;

  char delimiter = ' ';

  void init();

  byte flags;

  std::unordered_map<std::string, byte *> regList;

  command cEnd;
  command cLoad;
  command cPrint;
  command cReset;
  command cExec;
  command cCounter;
  command cContext;
  command cMessage;
  command cLoadReg;
  command cLoadRegPair;
  command cPrintReg;
  command cPrintRegPair;
  command cResetReg;
  command cResetMem;
  command cPrintF;

  void *fEnd();
  void *fLoad();
  void *fPrint();
  void *fReset();
  void *fExec();
  void *fCounter();
  void *fContext();
  void *fMessage();
  void *fLoadReg();
  void *fLoadRegPair();
  void *fPrintReg();
  void *fPrintRegPair();
  void *fResetReg();
  void *fResetMem();
  void *fPrintF();

  void string_parser();

  void *input_parser();

public:
  void *perform(std::string input);
  terminal();
  ~terminal();

  // void perform(std::vector<std::string> token);

  void get_input();
};