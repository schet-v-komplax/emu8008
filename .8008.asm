;
; .8008.asm
;
; file organization:
; OFFSET        AREA
; 00000Q        RST AREA
; 00100Q        CODE AREA
; 10000Q        DATA AREA
;

CONS    EQU     322Q            ; console device

ADDR    EQU     8               ; address port
STATUS  EQU     9               ; status port
READ    EQU     10              ; read (data) port
WRITE   EQU     11              ; write port
COM1    EQU     12              ; status port
COM2    EQU     13              ; cursor x
COM3    EQU     14              ; cursor y
COM4    EQU     15              ; COM4 port

DEBUG   EQU     31              ; emu8008 debug

;
; RST AREA
; COMMAND       FUNCTION        INPUT           OUTPUT          DESTROYES
; RST   0       INIT            -               -               A B C
; RST   1       ATTACH          B=device        -               A
; RST   2       GETC            -               A=char            B C     H L
; RST   3       PUTC            A=char          -               A B C     H L
;
        ORG     0Q
        JMP     INIT

        ORG     10Q
ATTACH  LAB
        OUT     ADDR
        INP     0
        RAR
        JTC     ATTACH          ; not ready
        RET

        ORG     20Q
GETC    LBI     CONS
        RST     1               ; attach console
        OUT     READ
        INP     0               ; read A
        RST     3               ; put A on screen
        RET

        ORG     30Q
PUTC    LCA                     ; save A
        LBI     CONS
        RST     1               ; attach console
        LAC                     ; restore A
        OUT     WRITE           ; write A
        LLI     CONX
        LHI     0               ; M=CONX
        LBM
        INB
        LAB
        CPI     80
        JTS     PUT2
        XRB                     ; x=0
        INL                     ; M=CONY
        LCM
        INC                     ; y++
        CPI     12
        JTS     .4
        XRC
        LAC
        OUT     COM3            ; set cursor y
        LMA                     ; save y
        DCL                     ; M=CONX
        LAB                     ; A=x
PUT2    OUT     COM2            ; set cursor x
        LMA                     ; save x
        RET

;
; CODE AREA
; FUNCTION      INPUT           OUTPUT          DESTROYES
; PRINT         E=msg*          A=0               B C   E H L          
;
; * within DATA AREA
;
        ORG     100Q
INIT    LBI     CONS
        RST     1               ; attach console
        XRA
        OUT     COM2
        OUT     COM3            ; x=y=0
        XRA
        LEI     SIGN
        CAL     PRINT

        JMP     .

PRINT   LLE
        LHI     20Q             ; data area high byte
        LAM
        ORA
        RTZ
        RST     3
        INE
        JMP     PRINT

;
; DATA AREA
;
        ORG     10000Q          ; must be dividable by 256
SIGN    DFB     145Q
        DFB     155Q
        DFB     165Q
        DFB     070Q
        DFB     060Q
        DFB     060Q
        DFB     070Q            ; "emu8008"
        DFB     0
CONX    DFB     0
CONY    DFB     0
