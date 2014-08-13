

%define PREFIX  ;////----


BITS 32

%macro cglobal 1
    %ifdef PREFIX
        global _%1
        %define %1 _%1
    %else
        global %1
    %endif
%endmacro


SECTION .rodata data align=16

thirty_two_64: dw 32 , 32 , 32 , 32


SECTION .text

cglobal memcpy_4x4_dc_mmx



ALIGN   16
;---------------------------------------------------------------
;   void __cdecl memcpy_4x4_dc_mmx(short * data, short dc)
;---------------------------------------------------------------
memcpy_4x4_dc_mmx:
    push esi
    
    xor    eax , eax
    mov    esi , [esp + 8]
    mov    ax ,  [esp + 12]
    movd   mm0 , eax
    movq   mm1 , [thirty_two_64]
    punpcklwd  mm0 , mm0
    punpcklwd  mm0 , mm0 
    paddw  mm0 , mm1
    psraw  mm0 , 6
    movq   [esi] , mm0
    movq   [esi+32] , mm0 
    movq   [esi+64] , mm0
    movq   [esi+96] , mm0        
    emms
    
    pop esi 
    ret
