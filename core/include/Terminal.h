#include <8085.h>

struct command {
  byte minLength;
  byte maxLength;
  std::function<void *(void)> function;
};

class terminal {
  emu8085 CPU;

  std::unordered_map<std::string, command> commandRepo;

  std::vector<std::string> token;

  std::string input;

  char delimiter = ' ';

  void init();

  command cEnd;
  command cLoad;
  command cPrint;
  command cReset;
  command cExec;
  command cCounter;
  command cContext;

  void *fEnd();
  void *fLoad();
  void *fPrint();
  void *fReset();
  void *fExec();
  void *fCounter();
  void *fContext();

  void string_parser();

  void *input_parser();

public:
  void *perform(std::string input);
  terminal();
  ~terminal();

  // void perform(std::vector<std::string> token);

  void get_input();
};