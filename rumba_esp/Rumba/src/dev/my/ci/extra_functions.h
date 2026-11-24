void print_number_wan (void) {
  rda = 16 + (_t / 10); rdb = 16 + (_t % 10);
  #asm
      ; enter:  A = row position (0..23)
      ;         C = col position (0..31/63)
      ;         D = pallette #
      ;         E = graphic #

      ld  a, (_rda)
      ld  e, a

      ld  a, (__n)
      ld  d, a
      
      ld  a, (__x)
      ld  c, a

      ld  a, (__y)

      call SPPrintAtInv

      ld  a, (_rdb)
      ld  e, a

      ld  a, (__n)
      ld  d, a
      
      ld  a, (__x)
      inc a
      ld  c, a

      ld  a, (__y)

      call SPPrintAtInv
  #endasm
}