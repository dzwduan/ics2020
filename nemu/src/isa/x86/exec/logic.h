#include "cc.h"

static inline def_EHelper(test)
{
  *s0 = *ddest&*dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s,s0,id_dest->width);
  //operand_write(s, id_dest, s0);
  print_asm_template2(test);
}

static inline def_EHelper(and)
{
  //printf("before and esp is %d\n",cpu.esp);
  cpu.CF = cpu.OF = 0;
  *s0 = *ddest & *dsrc1;
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  //printf("after and esp is %d\n",cpu.esp);
  print_asm_template2(and);
}

static inline def_EHelper(xor)
{
  cpu.CF = cpu.OF = 0;
  *s0 = *ddest ^ *dsrc1;
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  print_asm_template2(xor);
}

static inline def_EHelper(or)
{
  cpu.CF = cpu.OF = 0;
  *s0 = *ddest | *dsrc1;
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  print_asm_template2(or);
}

static inline def_EHelper(not )
{
  cpu.CF = cpu.OF = 0;
  *s0 = ~(*ddest); 
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  print_asm_template1(not );
}

//算术右
static inline def_EHelper(sar)
{
  *s0 = (*dsrc1) & c_shift_mask; //移动位数
  // *s1 = *ddest;                  //要移动的数
  // rtl_msb(s, s2, ddest, id_dest->width); //s2是符号位
  *s1 = *ddest>>(*s0);
  *s2 = ((*s1)^(1<<(8*id_dest->width-1-(*s0)))) - (1<<(8*id_dest->width-1-(*s0)));

  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s,s2,id_dest->width);
  operand_write(s, id_dest, s2);
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(sar);
}

static inline def_EHelper(shl)
{
  *s0 = *ddest<<*dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s,s0,id_dest->width);
  operand_write(s, id_dest, s0);
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shl);
}

static inline def_EHelper(shr)
{
  *s0 = *ddest>>*dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s,s0,id_dest->width);
  operand_write(s, id_dest, s0);
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shr);
}

static inline def_EHelper(setcc)
{
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}
