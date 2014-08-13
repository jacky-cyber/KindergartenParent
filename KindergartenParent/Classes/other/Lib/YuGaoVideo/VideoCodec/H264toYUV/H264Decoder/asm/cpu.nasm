
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



SECTION .text

cglobal cpu_cpuid


ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl cpu_cpuid( int op, int *eax, int *ebx, int *ecx, int *edx )
;-----------------------------------------------------------------------------
cpu_cpuid:

    push    ebp
    mov     ebp,    esp
    push    ebx
    push    esi
    push    edi
    
    mov     eax,    [ebp +  8]
    cpuid

    mov     esi,    [ebp + 12]
    mov     [esi],  eax

    mov     esi,    [ebp + 16]
    mov     [esi],  ebx

    mov     esi,    [ebp + 20]
    mov     [esi],  ecx

    mov     esi,    [ebp + 24]
    mov     [esi],  edx

    pop     edi
    pop     esi
    pop     ebx
    pop     ebp
    ret





