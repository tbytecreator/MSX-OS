; ___________________________________________________________
;/               __           _                              \
;|              / _|         (_)                             |
;|             | |_ _   _ ___ _  ___  _ __                   |
;|             |  _| | | / __| |/ _ \| '_ \                  |
;|             | | | |_| \__ \ | (_) | | | |                 |
;|             |_|  \__,_|___/_|\___/|_| |_| *               |
;|                                                           |
;|               The MSX C Library for SDCC                  |
;|                   V1.0 - 09-10-11 2018                    |
;|                                                           |
;|                Eric Boez &  Fernando Garcia               |
;|                                                           |
;|               A S M  S O U R C E   C O D E                |
;|                                                           |
;|                                                           |
;\___________________________________________________________/
;
; Call Bios functions
;
;    PrintChar



 .area _CODE

;----------------------------
;   MODULE  PrintChar 
;
;   void PrintChar(char c)
;   
;   
;   
;
_PrintChar::
  push  ix
  ld ix,#0
  add ix,sp
  ld  a,4(ix)
  call #0xA2       ; Bios CHPUT
  ei 
  pop ix
  ret


  ;----------------------------
;   MODULE  bchput
;
;   used internaly 
;   
;   bchput(char value)
;   
;
_bchput::
  push  ix
  ld ix,#0
  add ix,sp
  ld  a,4(ix)
  call #0xA2         ; Bios CHPUT
  ei 
  pop ix
  ret