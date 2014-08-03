;=========================================================================================================
;The include file including all the little functions to print the various lines of text required for SMTP
;
;Also contains defines for a user to define.
;=========================================================================================================


;===User defineable defines==================================

    %define PORT 25                  ;port
    %define LISTEN_LIMIT   30        ;Amount of requests to queue
    %define SIZE_OF_SEND_BUFF    64   ;Size of the command buffer (You shouldent need to change this unless you add longer commands)
    %define SIZE_OF_REC_BUFF    64   ;size of the buffer for incomming commands. (As above)
    %define SIZE_OF_MSG_BUFF    1024 ;Size of the buffer holding


section .data

;===User defineable strings ==================================

   Domain: db 0x20,'localhost', 0x20            ;The domain name that this server sits on. Must be an FQDN. 
   Domain_len: equ 11                          ;Must have two space characters before and after the name.
                                                ;Length of the Domain name string. Including space characters.
   
   Greeting_msg: db 0x20,'Hey gorgeous', 0x20    ;Message sent with the initial connection confirmation message (After HELO).
   Greeting_mgs_len: equ 14

;=============================================================

;===SMTP Messages=============================================
;
;   These strings are for use in the SMT Protocol its self.
;   Changing these strings or their corrisponding length values
;   WILL result in unexpected behaviour.
;
;==============================================================
    
    ERR_501             db '501 - Command not recognised'
    ERR_501_len         equ '28'

    CMD_220             db '220'
    CMD_220_len         equ 3
    CMD_250             db '250'
    CMD_250_len         equ 3



;=============================================



;===Defines====================================

%define SYS_EXIT           1      
%define SYS_FORK           2
%define SYS_WRITE          4


%define SYS_SOCKETCALL     102
%define SYS_SOCK_SOCKET    1
%define SYS_SOCK_BIND      2
%define SYS_SOCK_CONNECT   3
%define SYS_SOCK_LISTEN    4
%define SYS_SOCK_ACCEPT    5
%define SYS_SOCK_SEND      9
%define SYS_SOCK_RECV      10
%define SYS_SOCK_SHUTDOWN  13
%define SOCK_STREAM        1
%define AF_INET            2
%define IPPROTO_TCP        6
%define INADDR_ANY         0

%define SIZE_OF_SOCK_INFO  6

;================================================

section .data  

;=== Strings ====================================
;
;   These strings are not integral to the operation of the program. 
;   But i advise that you dont change them.
;
;================================================

    Errormsg            db 'An Error Occured',10
    Errormsg_len        equ 17

    Threadmsg           db 'Forking process...',10
    Threadmsg_len       equ 19

    Startmsg            db 'Starting...',10
    Startmsg_len        equ 13

;===============================================

section .bss

;===Reserved space =============================
;
;   This data is integral to the operation of this program. 
;   I advise that you do not change any of it.
;
;==============================================

    send_buffer: resb SIZE_OF_SEND_BUFF   ;reserve memory as a buffer for commands that are being sent
    rec_buffer:  resb SIZE_OF_REC_BUFF   ;reserve memory for incomming commands.
    msgbuffer: resb 1024*1024*32 ;reserve 32MB for the message
    sock_desc: resb 4    ;reserve 4 bytes for the socket file descriptor
    socket_info: resb SIZE_OF_SOCK_INFO  ;reserve 6 bytes for the return of getsockinfo
    cmp_buffer:  resb SIZE_OF_REC_BUFF   ;reserve bytes in order to compare received strings to commands.

;==============================================
