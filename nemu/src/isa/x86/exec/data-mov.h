static inline def_EHelper(mov) {
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
  rtl_push(s,ddest);
  print_asm_template1(push);
}

static inline def_EHelper(push_imm8) {
  rtl_sext(s,s0,ddest,1);
  rtl_push(s,s0);
  print_asm("push_imm8");
}

static inline def_EHelper(pop) {
  rtl_pop(s,ddest);
  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {

  print_asm("pusha");
}

static inline def_EHelper(popa) {
  TODO();
  print_asm("popa");
}

static inline def_EHelper(leave) {
  //printf("when in leave pc : 0x%x\n\n",s->seq);
  if(s->isa.is_operand_size_16){
    cpu.sp = cpu.bp;
    rtl_pop(s,(rtlreg_t*)&cpu.bp);
  }else{
    cpu.esp = cpu.ebp;
    rtl_pop(s,&cpu.ebp);
  }
  print_asm("leave");
}

static inline def_EHelper(cltd) {
  if (s->isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }
  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
  if (s->isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }
  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movzx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);
  print_asm_template2(lea);
}
