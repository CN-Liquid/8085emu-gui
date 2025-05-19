#pragma once
#include <array>
#include <functional>
#include <iostream>
#include <stdint.h>
#include <unordered_map>
typedef uint8_t byte;
typedef uint16_t word;

struct context {
  // word size registers are divided into upper and lower bytes

  // main registers
  byte A, B, C, D, E, H, L;

  // stack register
  byte SPU, SPL;

  // program counter register
  byte PCU, PCL;

  // status register
  byte S : 1;  // sign
  byte Z : 1;  // zero
  byte AC : 1; // auxiliary carry
  byte P : 1;  // parity
  byte CY : 1; // carry

  byte I;

  // the 8-bit data bus
  byte DB;
};

class emu8085 {

  // word size registers are divided into upper and lower bytes

  // main registers
  byte A, B, C, D, E, H, L;

  // stack register
  byte SPU, SPL;

  // program counter register
  byte PCU, PCL;

  // status register
  byte S : 1;  // sign
  byte Z : 1;  // zero
  byte AC : 1; // auxiliary carry
  byte P : 1;  // parity
  byte CY : 1; // carry

  // The 64KB memory for the CPU
  byte *mem;

  // Instruction register
  byte I;

  // the 8-bit data bus
  byte DB;

  // on/off state of the machine
  bool running = false;

  // The fetch-decode-execute-store cycle for the 8085
  void op_fetch();

  // The functions that fetches next bytes from the memory depending on the
  // instruction byte for a two byte instruction and word for a three byte
  // instruction
  byte populate_arguments_byte();
  word populate_arguments_word();

  // This function takes an opcode and executes it
  std::function<void()> function_recognizer(byte opcode);

  void initializeInstructionTable();

public:
  context cachedContext;
  // Transfer commands only to be used to define 8085 instructions
  void load_reg_pair(byte &regPair, word data);
  void load_reg(byte &reg, byte data);
  void mem_read(word memloc);
  void mem_write(word memloc);
  void load_DB(byte data);

  void execute();
  void print(word memLoc);

  void refresh_context();

  emu8085();

  ~emu8085();

  // resets all registers and flags to their default
  void reset_reg();

  // resets all memory to 0x00;
  void reset_mem();

  // resets both the cpu context and the memory
  void reset();

  // prints the current context of the cpu
  void print_reg();

  // Sets and returns the current stack pointer
  void set_PC(word memLoc);
  word get_PC();

  // 8085 instructions
  // The instructions defined here are abstracted , R represents register , M
  // represents data at that memory , D represents immediate memory and RP
  // represents register pair . Eg :- R_M represents from register to memory

  // Data transfer instsructions
  void MOV_R_R(byte &reg1, byte &reg2);
  void MOV_R_M(byte &reg);
  void MOV_M_R(byte &reg);

  void MVI_R_D(byte &reg);
  void MVI_M_D();

  void LXI_RP_D(byte &regPair);

  void LDA_M();

  void LDAX_RP(byte &regPair);

  void LHLD_D();

  void STA_M();

  void STAX_RP(byte &regpair);

  void SHLD_M();

  void XCHG();

  // Arithmetic Instructions
  void ADD_R(byte &reg);
  void ADD_M();

  void ADC_R(byte &reg);
  void ADC_M();

  void ADI_D();

  void ACI_D();

  void DAD_RP(byte &regPair);

  void SUB_R(byte &reg);
  void SUB_M();

  void SBB_R(byte &reg);
  void SBB_M();

  void SUI_D();

  void SBI_D();

  void INR_R(byte &reg);
  void INR_M();

  void INX_RP(byte &bytePair);

  void DCR_R(byte &reg);
  void DCR_M();

  void DCX_RP(byte &bytePair);

  // Logical instructions

  void CMP_R(byte &reg);
  void CMP_M();

  void CPI_D();

  void ANA_R(byte &reg);
  void ANA_M();

  void ANI_D();

  void XRA_R(byte &reg);
  void XRA_M();

  void XRI_D();

  void ORA_R(byte &reg);
  void ORA_M();

  void ORI_D();

  void RLC();

  void RRC();

  void RAL();

  void RAR();

  void CMA();

  void CMC();

  void STC();

  // Branching instructions
  void JMP_M();

  void JC_M();
  void JNC_M();
  void JP_M();
  void JM_M();
  void JZ_M();
  void JNZ_M();
  void JPE_M();
  void JPO_M();

  void CALL_M();

  void CC_M();
  void CNC_M();
  void CP_M();
  void CM_M();
  void CZ_M();
  void CNZ_M();
  void CPE_M();
  void CPO_M();

  void RET();

  void RC();
  void RNC();
  void RP();
  void RM();
  void RZ();
  void RNZ();
  void RPE();
  void RPO();

  // Control operations
  void NOP();

  void HLT();

  void DI();

  void EI();

  // Stack operations

  void PUSH_RP(byte &regpair);
  void PUSH_M();
  void POP_RP(byte &regPair);
  void POP_M();
};
