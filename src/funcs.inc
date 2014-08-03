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

    


send:                        ;Sends the send buffer.
   
    push 0                  ;flags arg (not required)
    push SIZE_OF_SEND_BUFF   ;push arguments
    push send_buffer
    ;push edx
    push WORD [sock_desc]
    
    mov ecx, esp            ;save pointer to args
    mov eax, SYS_SOCKETCALL  ; select sys calls.
    mov ebx, SYS_SOCK_SEND
    int 80h                 ;ring ring
  
    add esp, 16             ;clear the stack  
    
    ret


recv:                       ;Receives stuff into rec buffer

    mov eax, SYS_SOCKETCALL 
    mov ebx, SYS_SOCK_RECV

    push 0
    push SIZE_OF_REC_BUFF
    push rec_buffer
    ;push edx
    push WORD [sock_desc]
    
    mov ecx, esp
    int 80h
    
    cmp eax, 0
    jg recv_ret ;If the connection has been made, Lets get going!
    jl error ; else, we should error.

recv_ret:
    add esp, 16     ;clear the stack
    ret             ;return.

chk_recv:                   ;Check the received string contains the string pointed to by the first stack element (esp)
      
      pop ecx               ;we call this with the amount of characters to check
      mov eax, cmp_buffer   ;move the comparason buffer to eax
      mov ebx, rec_buffer   ;Move the received string to ebx
      xor edx, edx          ;make sure edx is empty
cr_loop:

      cmp edx, ecx          ; compare the amount of characters to check against the amount checked.
      je cr_loop_done       

      inc edx

      cmp [cmp_buffer+edx], [rec_buffer+edx] ;compare the starts of the strings.
      je cr_loop
      jne cr_loop_false

cr_loop_done:
    
    pop ecx
    push 1
    push ecx
    ret

cr_loop_false:
    mov  [send_buffer], [ERR_501]
    call send
    ret
