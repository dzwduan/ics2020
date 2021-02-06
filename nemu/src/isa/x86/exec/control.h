#include "cc.h"

static inline def_EHelper(jmp)
{
  // the target address is calculated at the decode stage
  rtl_j(s, s->jmp_pc);

  print_asm("jmp %x", s->jmp_pc);
}

static inline def_EHelper(jcc)
{
  // the target address is calculated at the decode stage
  uint32_t cc = s->opcode & 0xf; //opcode取低4位
  rtl_setcc(s, s0, cc);
  rtl_jrelop(s, RELOP_NE, s0, rz, s->jmp_pc); //条件满足则跳转

  print_asm("j%s %x", get_cc_name(cc), s->jmp_pc);
}

static inline def_EHelper(jmp_rm)
{
  rtl_jr(s, ddest);

  print_asm("jmp *%s", id_dest->str);
}

static inline def_EHelper(call)
{
  // the target address is calculated at the decode stage
 
    //printf("seq_pc : %p  cpu.pc : %p\n", &s->seq_pc, &cpu.pc);
    rtl_push(s, &s->seq_pc);
    rtl_j(s, s->jmp_pc);
  
  print_asm("call %x", s->jmp_pc);
}

static inline def_EHelper(ret)
{
  //printf("before ret : jmp_pc is %d ### cpu.pc is %d\n", s->jmp_pc, cpu.pc);
  rtl_pop(s, s0);
  s->jmp_pc = *s0;
  rtl_j(s, s->jmp_pc);
  //printf("after ret : jmp_pc is %d  ### cpu.pc is %d\n", s->jmp_pc,cpu.pc);
  print_asm("ret");
}

static inline def_EHelper(ret_imm)
{
  TODO();
  print_asm("ret %s", id_dest->str);
}

static inline def_EHelper(call_rm)
{
  TODO();
  print_asm("call *%s", id_dest->str);
}
