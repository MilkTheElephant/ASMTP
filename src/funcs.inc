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

send:                        ;Sends the send buffer.
   
    push 0                  ;flags arg (not required)
    push SIZE_OF_SEND_BUFF   ;push arguments
    push send_buffer
    ;mov edx, [sock_desc]
    push edx
    
    mov ecx, esp            ;save pointer to args
    mov eax, SYS_SOCKETCALL  ; select sys calls.
    mov ebx, SYS_SOCK_SEND
    int 80h                 ;ring ring
   

    pusha
    xor ecx, ecx
    mov edx, SIZE_OF_REC_BUFF ;write message
    mov ecx, rec_buffer
    mov ebx, 1
    mov eax, 4
    int 80h
    popa
    
    ret
