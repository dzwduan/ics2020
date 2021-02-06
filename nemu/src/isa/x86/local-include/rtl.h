#ifndef __X86_RTL_H__
#define __X86_RTL_H__

#include <rtl/rtl.h>
#include "reg.h"

/* RTL pseudo instructions */

static inline def_rtl(lr, rtlreg_t *dest, int r, int width)
{
  switch (width)
  {
  case 4:
    rtl_mv(s, dest, &reg_l(r));
    return;
  case 1:
    rtl_host_lm(s, dest, &reg_b(r), 1);
    return;
  case 2:
    rtl_host_lm(s, dest, &reg_w(r), 2);
    return;
  default:
    assert(0);
  }
}

static inline def_rtl(sr, int r, const rtlreg_t *src1, int width)
{
  switch (width)
  {
  case 4:
    rtl_mv(s, &reg_l(r), src1);
    return;
  case 1:
    rtl_host_sm(s, &reg_b(r), src1, 1);
    return;
  case 2:
    rtl_host_sm(s, &reg_w(r), src1, 2);
    return;
  default:
    assert(0);
  }
}

//TODO
static inline def_rtl(push, const rtlreg_t *src1)
{
  // esp <- esp - 4
  // M[esp] <- src1
  printf("before push esp : 0x%x\n", cpu.esp);
  int width = (s->isa.is_operand_size_16) ? 2 : 4;
  if (cpu.esp == *src1)
  {
    vaddr_write((cpu.esp - width), *src1, width);
    cpu.esp -= width;
  }
  else
  {
    cpu.esp -= width;
    vaddr_write((cpu.esp), (*src1), width);
  }
  printf("after push esp : 0x%x\n", cpu.esp);
}

static inline def_rtl(pop, rtlreg_t *dest)
{
  // dest <- M[esp]
  // esp <- esp + 4
  int width = (s->isa.is_operand_size_16) ? 2 : 4;
  *dest = vaddr_read(cpu.esp, width);
  cpu.esp += width;
}

static inline def_rtl(is_sub_overflow, rtlreg_t *dest,
                      const rtlreg_t *res, const rtlreg_t *src1, const rtlreg_t *src2, int width)
{
  // dest <- is_overflow(src1 - src2)
  rtl_msb(s, t0, src1, width);
  rtl_msb(s, t1, src2, width);
  rtl_msb(s, dest, res, width);

  if (*t0 != *t1 && *t0 != *dest)
    *dest = 1;
  else
    *dest = 0;
}

static inline def_rtl(is_sub_carry, rtlreg_t *dest,
                      const rtlreg_t *src1, const rtlreg_t *src2)
{
  // dest <- is_carry(src1 - src2)
  *dest = (*src1 < *src2) ? 1 : 0;
}

static inline def_rtl(is_add_overflow, rtlreg_t *dest,
                      const rtlreg_t *res, const rtlreg_t *src1, const rtlreg_t *src2, int width)
{
  // dest <- is_overflow(src1 + src2)
  rtl_msb(s, t0, src1, width);
  rtl_msb(s, t1, src2, width);
  rtl_msb(s, dest, res, width);
  if (*t0 == *t1 && *dest != *t1)
  {
    *dest = 1;
  }
  else
    *dest = 0;
}

static inline def_rtl(is_add_carry, rtlreg_t *dest,
                      const rtlreg_t *res, const rtlreg_t *src1)
{
  // dest <- is_carry(src1 + src2)
  *dest = (*res < *src1);
}

#define def_rtl_setget_eflags(f)                              \
  static inline def_rtl(concat(set_, f), const rtlreg_t *src) \
  {                                                           \
    cpu.f = *src;                                             \
  }                                                           \
  static inline def_rtl(concat(get_, f), rtlreg_t *dest)      \
  {                                                           \
    *dest = cpu.f;                                            \
  }

def_rtl_setget_eflags(CF)
    def_rtl_setget_eflags(OF)
        def_rtl_setget_eflags(ZF)
            def_rtl_setget_eflags(SF)

                static inline def_rtl(update_ZF, const rtlreg_t *result, int width)
{
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
  switch (width)
  {
  case 1:
    cpu.ZF = (*result & 0xFF) == 0;
    break;
  case 2:
    cpu.ZF = (*result & 0xFFFF) == 0;
    break;
  case 4:
    cpu.ZF = (*result & 0xFFFFFFFF) == 0;
    break;
  default:
    assert(0);
  }
}

static inline def_rtl(update_SF, const rtlreg_t *result, int width)
{
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  rtl_msb(s, t0, result, width);
  rtl_set_SF(s, t0);
}

static inline def_rtl(update_ZFSF, const rtlreg_t *result, int width)
{ //printf("before and esp is %d\n",cpu.esp);
  rtl_update_ZF(s, result, width);
  //printf("after updatezf esp is %d\n",cpu.esp);
  //printf("before and esp is %d\n",cpu.esp);
  rtl_update_SF(s, result, width);
  //printf("after updatesf esp is %d\n",cpu.esp);
}

#endif

//basic rtl 里面只能使用t0 t1!!!