;==============================================
;   This file is part of ASMTP.
;
;   ASMTP is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   ASMTP is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with ASMTP.  If not, see <http://www.gnu.org/licenses/>.
;
;=============================================


;This header file contains a lot of the functions required to make ASMTP work.
;Things like getting string lengths, combining strings and other such magical stuff.

section .text

error:          ;called if there is an error. TODO: Make this take arguments and output more appropriate error messages.
    pusha
    mov edx, Errormsg_len
    mov ecx, Errormsg
    mov ebx, 1
    mov eax, 4
    int 80h

    jmp exit

    
print:
    
    pop edx
    pop ecx
    mov ebx, 1
    mov eax, 4
    int 80h
    ret


send:                        ;Sends the send buffer.
   
    push 0                  ;flags arg (not required)
    push SIZE_OF_SEND_BUFF   ;push arguments
    push send_buffer
    push edx
    ;push WORD [sock_desc]
    
    mov ecx, esp            ;save pointer to args
    mov eax, SYS_SOCKETCALL  ; select sys calls.
    mov ebx, SYS_SOCK_SEND
    int 80h                 ;ring ring
  
    add esp, 16             ;clear the stack  
    
    ret


recv:                       ;Receives stuff into rec buffer
    ;pop edx
    mov eax, SYS_SOCKETCALL 
    mov ebx, SYS_SOCK_RECV

    push 0
    push SIZE_OF_REC_BUFF
    mov ecx, rec_buffer
    push ecx 
    ;mov edx, sock_desci        ;Okay, this doesnt work, so we have to just use this and HOPE that edx still contains the socket file descriptor.
    push edx
    
    mov ecx, esp
    int 80h
    
    mov ecx, rec_buffer
    cmp eax, 0
    jg recv_ret ;If the connection has been made, Lets get going!
    ;jl error ; else, we should error.

recv_ret:
    add esp, 16     ;clear the stack
    ret             ;return.


    
chk_recv:                   ;Check the received string contains the string pointed to by the first stack element 

    
    
    ;pop cx               ;we call this with the amount of characters to check
    mov cl, 1*8-8
    mov ch, 0
    ;mov eax, [cmp_buffer]   ;move the comparason buffer to eax
    ;mov ebx, [rec_buffer]   ;Move the received string to ebx
    mov eax, 'H'
    mov ebx, [rec_buffer]
    ;xor edx, edx          ;make sure edx is empty
    
    cmp al, bl              ;Compare only the lower 8 bits, because a char isnt going to be more than 8 bits is it!
    je cr_loop 
    jne error               ; For now, if the string's first letter is wrong, just error == TODO Make sure to send the right error ad support EHLO message erroring. ==
    cmp cl, 0               ;If we're only meant to be comparing one characer, the jump to done.
    je cr_loop              ;If equa, then we should start checking the other letters.
    ;jl error                ;If lower, then...well we really shouldent be comparing les than one The thats not right.
;    jmp exit


cr_loop:
break:
    
    mov eax, dbg1_len
    push eax
    mov eax, dbg1
    push eax
    call print

    
    cmp cl, ch            ; compare the amount of characters to check against the amount checked.
    je cr_loop_done       

    add ch, 8                ;incrememt the counter.
    mov esi, cmp_buffer
    mov edi, rec_buffer
    mov eax, [esi+ebx]
    mov ebx,[edi+ebx]
    ;mov eax, [cmp_buffer+ch*8]
    ;mov  ebx, [rec_buffer+ch*8]
    cmp al, bl ;compare the starts of the strings.
cmptst:
   je exit 
     ;je cr_loop
     jne cr_loop_false

cr_loop_done:
    
    pop ecx
    push 1
    push ecx
    ret

cr_loop_false:
     
    mov esi, ERR_501        ;point esi to wards the string we're moving
    mov edi,  send_buffer   ;point edi towards the string we're moving it too 
    mov ecx, 28    ;move the length of ecx
    rep movsb               ;copy.
    
    jmp error
    ;call send
    ret


get_domain:         ;This function returns a string (in client_domain) which is taken from the received "HELO" string.
   
    mov ebx, 24
   
initialcheck:
    add ebx, 8
    mov eax, [rec_buffer+ebx] ;move into eax the character that should be the start of the external domain.
    cmp eax, 0x20    ;compare EAX to the space character
    jne initialcheck      ;if the character we checked is a space, then jump to recheck 
  
    push ebx             ;push if the char isnt a space, push the starting char of the domain. 
recheck:
   add ebx, 8
    
    mov eax, [rec_buffer+ebx]
    cmp eax, 0x20
    jne recheck
    
copystr:
    pop eax
    lea eax, [rec_buffer+eax]
    mov esi, eax
    mov edi, external_domain
    mov ecx, edx
    
   jmp exit 

