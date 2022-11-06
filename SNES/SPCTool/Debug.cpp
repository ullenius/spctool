#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	<StdCtrls.hpp>
#include	<stdio.h>
#include	"Map.h"
#include	"Debug.h"
#include	"Assembly.h"
#include	"Help\Index.h"

#pragma package(smart_init)
#pragma resource "*.dfm"


//**************************************************************************************************
static const s8	FRegName[][13]={
		" (test)",
		" Control:  ",
		" DSP Address",
		" DSP Data",
		" Port 0:  ",
		" Port 1:  ",
		" Port 2:  ",
		" Port 3:  ",
		" Not used",
		" Not used",
		" Timer 0:  ",
		" Timer 1:  ",
		" Timer 2:  ",
		" Counter 0",
		" Counter 1",
		" Counter 2"};

static const s8	RegCName[][12]={
		" MVOL(L):  ",
		" MVOL(R):  ",
		" EVOL(L):  ",
		" EVOL(R):  ",
		" KON:  ",
		" KOF:  ",
		" FLG:  ",
		" ENDX:  "};

static const s8	RegDName[][10]={
		" EFB:  ",
		" Not used",
		" PMON:  ",
		" NON:  ",
		" EON:  ",
		" DIR:  ",
		" ESA:  ",
		" EDL:  "};

//Instruction Set ------------------------------

	//Operand Encoding:
	//
	//  Bits 7-6 Size of operand
	//  Bits 5-4 Type of operand
	//  Bits 3-0 Operand specific
	//
	// 0 bytes
	// -------
	//
	// 0 - None
	//
	// 1 - Register
	//       3 Type
	//         0 Register contents
	//         1 Register indexes DP
	//     2:0 Register
	//         0 X
	//         1 A
	//         2 Y
	//         3 YA
	//         4 X+
	//         5 SP
	//         6 PSW
	//         7 C
	//
	// 2 - Call table index
	//     3:0 Index
	//
	// 1 byte
	// ------
	//
	// 0 - Immediate 8-bit value
	//       3 Usage
	//         0 Unsigned byte
	//         1 Indexes UP (i.e. PCALL)
	//     2:0 Location of actual value relative to operand byte
	//         0 Current byte
	//         3 Previous byte (i.e. MOV dp,imm)
	//
	// 1 - Relative (signed byte for branching)
	//
	// 2 - Direct Page offset
	//       3 Reference type
	//         0 Byte
	//         1 Word
	//     2:0 Type
	//         0 dp
	//         1 dp(d) (next byte)
	//         2 dp.bit
	//         3 dp(s) (previous byte)
	//
	// 3 - Direct Page offset with index
	//       2 Pointer method
	//         0 Indirect (i.e. dp+X)
	//         1 Indexed indirect (i.e. [dp]+Y)
	//     1:0 Register
	//         0 X
	//         2 Y
	//
	// 2 bytes
	// -------
	//
	// 2 - Absolute offset
	//       3 Type (if bits 2-0 are 0)
	//         0 Pointer (i.e. MOV A,abs)
	//         1 Location (i.e. CALL abs)
	//       3 Reference bit (if bits 2-0 are 2)
	//         0 Normal
	//         1 Complimented
	//     2:0 Type
	//         0 abs
	//         2 mem.bit
	//
	// 3 - Absolute offset with index
	//       3 Reference type
	//         0 Byte
	//         1 Word
	//       2 Pointer method
	//         0 Indirect
	//         1 Indexed indirect
	//     1:0 Register
	//         0 X
	//         2 Y


const Instruction InsList[256]={
		{"NOp  ",0x00,0x00},  //00 NOP
		{"TCall",0x20,0x00},  //01 TCALL 0
		{"Set1 ",0x62,0x00},  //02 SET1  dp.0
		{"BBS  ",0x62,0x50},  //03 BBS   dp.0
		{"Or   ",0x11,0x60},  //04 OR    A,dp
		{"Or   ",0x11,0xA0},  //05 OR    A,abs
		{"Or   ",0x11,0x18},  //06 OR    A,(X)
		{"Or   ",0x11,0x74},  //07 OR    A,[dp+X]
		{"Or   ",0x11,0x40},  //08 OR    A,#imm
		{"Or   ",0x61,0x63},  //09 OR    dp(d),dp(s)
		{"Or1  ",0x17,0xA2},  //0A OR1   C,mem.bit
		{"ASL  ",0x60,0x00},  //0B ASL   dp
		{"ASL  ",0xA0,0x00},  //0C ASL   abs
		{"Push ",0x16,0x00},  //0D PUSH  PSW
		{"TSet1",0xA0,0x00},  //0E TSET1 abs
		{"Brk  ",0x00,0x00},  //0F BRK

		{"BPl  ",0x50,0x00},  //10 BPL   rel
		{"TCall",0x21,0x00},  //11 TCALL 1
		{"Clr1 ",0x62,0x00},  //12 CLR1  dp.0
		{"BBC  ",0x62,0x50},  //13 BBC   dp.0
		{"Or   ",0x11,0x70},  //14 OR    A,dp+X
		{"Or   ",0x11,0xB0},  //15 OR    A,abs+X
		{"Or   ",0x11,0xB2},  //16 OR    A,abs+Y
		{"Or   ",0x11,0x76},  //17 OR    A,[dp]+Y
		{"Or   ",0x61,0x43},  //18 OR    dp,#imm
		{"Or   ",0x18,0x1A},  //19 OR    (X),(Y)
		{"DecW ",0x68,0x00},  //1A DECW  dp
		{"ASL  ",0x70,0x00},  //1B ASL   dp+X
		{"ASL  ",0x11,0x00},  //1C ASL   A
		{"Dec  ",0x10,0x00},  //1D DEC   X
		{"Cmp  ",0x10,0xA0},  //1E CMP   X,abs
		{"Jmp  ",0xBC,0x00},  //1F JMP   [abs+X]

		{"ClrP ",0x00,0x00},  //20 CLRP
		{"TCall",0x22,0x00},  //21 TCALL 2
		{"Set1 ",0x62,0x00},  //22 SET1  dp.1
		{"BBS  ",0x62,0x50},  //23 BBS   dp.1
		{"And  ",0x11,0x60},  //24 AND   A,dp
		{"And  ",0x11,0xA0},  //25 AND   A,abs
		{"And  ",0x11,0x18},  //26 AND   A,(X)
		{"And  ",0x11,0x74},  //27 AND   A,[dp+X]
		{"And  ",0x11,0x40},  //28 AND   A,#imm
		{"And  ",0x61,0x63},  //29 AND   dp(d),dp(s)
		{"Or1  ",0x17,0xAA},  //2A OR1   C,/mem.bit
		{"RoL  ",0x60,0x00},  //2B ROL   dp
		{"RoL  ",0xA0,0x00},  //2C ROL   abs
		{"Push ",0x11,0x00},  //2D PUSH  A
		{"CBNE ",0x60,0x50},  //2E CBNE  dp,rel
		{"Bra  ",0x50,0x00},  //2F BRA   rel

		{"BMi  ",0x50,0x00},  //30 BMI   rel
		{"TCall",0x23,0x00},  //31 TCALL 3
		{"Clr1 ",0x62,0x00},  //32 CLR1  dp.1
		{"BBC  ",0x62,0x50},  //33 BBC   dp.1
		{"And  ",0x11,0x70},  //34 AND   A,dp+X
		{"And  ",0x11,0xB0},  //35 AND   A,abs+X
		{"And  ",0x11,0xB2},  //36 AND   A,abs+Y
		{"And  ",0x11,0x76},  //37 AND   A,[dp]+Y
		{"And  ",0x61,0x43},  //38 AND   dp,#imm
		{"And  ",0x18,0x1A},  //39 AND   (X),(Y)
		{"IncW ",0x68,0x00},  //3A INCW  dp
		{"RoL  ",0x70,0x00},  //3B ROL   dp+X
		{"RoL  ",0x11,0x00},  //3C ROL   A
		{"Inc  ",0x10,0x00},  //3D INC   X
		{"Cmp  ",0x10,0x60},  //3E CMP   X,dp
		{"Call ",0xA8,0x00},  //3F CALL  abs

		{"SetP ",0x00,0x00},  //40 SETP
		{"TCall",0x24,0x00},  //41 TCALL 4
		{"Set1 ",0x62,0x00},  //42 SET1  dp.2
		{"BBS  ",0x62,0x50},  //43 BBS   dp.2
		{"EOr  ",0x11,0x60},  //44 EOR   A,dp
		{"EOr  ",0x11,0xA0},  //45 EOR   A,abs
		{"EOr  ",0x11,0x18},  //46 EOR   A,(X)
		{"EOr  ",0x11,0x74},  //47 EOR   A,[dp+X]
		{"EOr  ",0x11,0x40},  //48 EOR   A,#imm
		{"EOr  ",0x61,0x63},  //49 EOR   dp(d),dp(s)
		{"And1 ",0x17,0xA2},  //4A AND1  C,mem.bit
		{"LSR  ",0x60,0x00},  //4B LSR   dp
		{"LSR  ",0xA0,0x00},  //4C LSR   abs
		{"Push ",0x10,0x00},  //4D PUSH  X
		{"TClr1",0xA0,0x00},  //4E TCLR  abs
		{"PCall",0x48,0x00},  //4F PCALL up

		{"BVC  ",0x50,0x00},  //50 BVC   rel
		{"TCall",0x25,0x00},  //51 TCALL 5
		{"Clr1 ",0x62,0x00},  //52 CLR1  dp.2
		{"BBC  ",0x62,0x50},  //53 BBC   dp.2
		{"EOr  ",0x11,0x70},  //54 EOR   A,dp+X
		{"EOr  ",0x11,0xB0},  //55 EOR   A,abs+X
		{"EOr  ",0x11,0xB2},  //56 EOR   A,abs+Y
		{"EOr  ",0x11,0x76},  //57 EOR   A,[dp]+Y
		{"EOr  ",0x61,0x43},  //58 EOR   dp,#imm
		{"EOr  ",0x18,0x1A},  //59 EOR   (X),(Y)
		{"CmpW ",0x13,0x68},  //5A CMPW  YA,dp
		{"LSR  ",0x70,0x00},  //5B LSR   dp+X
		{"LSR  ",0x11,0x00},  //5C LSR   A
		{"Mov  ",0x10,0x11},  //5D Mov   X,A
		{"Cmp  ",0x12,0xA0},  //5E CMP   Y,abs
		{"Jmp  ",0xA8,0x00},  //5F JMP   abs

		{"ClrC ",0x00,0x00},  //60 CLRC
		{"TCall",0x26,0x00},  //61 TCALL 6
		{"Set1 ",0x62,0x00},  //62 SET1  dp.3
		{"BBS  ",0x62,0x50},  //63 BBS   dp.3
		{"Cmp  ",0x11,0x60},  //64 CMP   A,dp
		{"Cmp  ",0x11,0xA0},  //65 CMP   A,abs
		{"Cmp  ",0x11,0x18},  //66 CMP   A,(X)
		{"Cmp  ",0x11,0x74},  //67 CMP   A,[dp+X]
		{"Cmp  ",0x11,0x40},  //68 CMP   A,#imm
		{"Cmp  ",0x61,0x63},  //69 CMP   dp(d),dp(s)
		{"And1 ",0x17,0xAA},  //6A AND1  C,/mem.bit
		{"RoR  ",0x60,0x00},  //6B ROR   dp
		{"RoR  ",0xA0,0x00},  //6C ROR   abs
		{"Push ",0x12,0x00},  //6D PUSH  Y
		{"DBNZ ",0x60,0x50},  //6E DBNZ  dp,rel
		{"Ret  ",0x00,0x00},  //6F RET

		{"BVS  ",0x50,0x00},  //70 BVS   rel
		{"TCall",0x27,0x00},  //71 TCALL 7
		{"Clr1 ",0x62,0x00},  //72 CLR1  dp.3
		{"BBC  ",0x62,0x50},  //73 BBC   dp.3
		{"Cmp  ",0x11,0x70},  //74 CMP   A,dp+X
		{"Cmp  ",0x11,0xB0},  //75 CMP   A,abs+X
		{"Cmp  ",0x11,0xB2},  //76 CMP   A,abs+Y
		{"Cmp  ",0x11,0x76},  //77 CMP   A,[dp]+Y
		{"Cmp  ",0x61,0x43},  //78 CMP   dp,#imm
		{"Cmp  ",0x18,0x1A},  //79 CMP   (X),(Y)
		{"AddW ",0x13,0x68},  //7A ADDW  YA,dp
		{"RoR  ",0x70,0x00},  //7B ROR   dp+X
		{"RoR  ",0x11,0x00},  //7C ROR   A
		{"Mov  ",0x11,0x10},  //7D MOV   A,X
		{"Cmp  ",0x12,0x60},  //7E CMP   Y,dp
		{"RetI ",0x00,0x00},  //7F RETI

		{"SetC ",0x00,0x00},  //80 SETC
		{"TCall",0x28,0x00},  //81 TCALL 8
		{"Set1 ",0x62,0x00},  //82 SET1  dp.4
		{"BBS  ",0x62,0x50},  //83 BBS   dp.4
		{"AdC  ",0x11,0x60},  //84 ADC   A,dp
		{"AdC  ",0x11,0xA0},  //85 ADC   A,abs
		{"AdC  ",0x11,0x18},  //86 ADC   A,(X)
		{"AdC  ",0x11,0x74},  //87 ADC   A,[dp+X]
		{"AdC  ",0x11,0x40},  //88 ADC   A,#imm
		{"AdC  ",0x61,0x63},  //89 ADC   dp(d),dp(s)
		{"EOr1 ",0x17,0xA2},  //8A EOR1  C,mem.bit
		{"Dec  ",0x60,0x00},  //8B DEC   dp
		{"Dec  ",0xA0,0x00},  //8C DEC   abs
		{"Mov  ",0x12,0x40},  //8D MOV   Y,#imm
		{"Pop  ",0x16,0x00},  //8E POP   PSW
		{"Mov  ",0x61,0x43},  //8F MOV   dp,#imm

		{"BCC  ",0x50,0x00},  //90 BCC   rel
		{"TCall",0x29,0x00},  //91 TCALL 9
		{"Clr1 ",0x62,0x00},  //92 CLR1  dp.4
		{"BBC  ",0x62,0x50},  //93 BBC   dp.4
		{"AdC  ",0x11,0x70},  //94 ADC   A,dp+X
		{"AdC  ",0x11,0xB0},  //95 ADC   A,abs+X
		{"AdC  ",0x11,0xB2},  //96 ADC   A,abs+Y
		{"AdC  ",0x11,0x76},  //97 ADC   A,[dp]+Y
		{"AdC  ",0x61,0x43},  //98 ADC   dp,#imm
		{"AdC  ",0x18,0x1A},  //99 ADC   (X),(Y)
		{"SubW ",0x13,0x68},  //9A SUBW  YA,dp
		{"Dec  ",0x70,0x00},  //9B DEC   dp+X
		{"Dec  ",0x11,0x00},  //9C DEC   A
		{"Mov  ",0x10,0x15},  //9D MOV   X,SP
		{"Div  ",0x13,0x10},  //9E DIV   YA,X
		{"XCN  ",0x11,0x00},  //9F XCN   A

		{"EI   ",0x00,0x00},  //A0 EI
		{"TCall",0x2A,0x00},  //A1 TCALL 10
		{"Set1 ",0x62,0x00},  //A2 SET1  dp.5
		{"BBS  ",0x62,0x50},  //A3 BBS   dp.5
		{"SbC  ",0x11,0x60},  //A4 SBC   A,dp
		{"SbC  ",0x11,0xA0},  //A5 SBC   A,abs
		{"SbC  ",0x11,0x18},  //A6 SBC   A,(X)
		{"SbC  ",0x11,0x74},  //A7 SBC   A,[dp+X]
		{"SbC  ",0x11,0x40},  //A8 SBC   A,#imm
		{"SbC  ",0x61,0x63},  //A9 SBC   dp(d),dp(s)
		{"Mov1 ",0x17,0xA2},  //AA MOV1  C,mem.bit
		{"Inc  ",0x60,0x00},  //AB INC   dp
		{"Inc  ",0xA0,0x00},  //AC INC   abs
		{"Cmp  ",0x12,0x40},  //AD CMP   Y,#imm
		{"Pop  ",0x11,0x00},  //AE POP   A
		{"Mov  ",0x1C,0x11},  //AF MOV   (X)+,A

		{"BCS  ",0x50,0x00},  //B0 BCS   rel
		{"TCall",0x2B,0x00},  //B1 TCALL 11
		{"Clr1 ",0x62,0x00},  //B2 CLR1  dp.5
		{"BBC  ",0x62,0x50},  //B3 BBC   dp.5
		{"SbC  ",0x11,0x70},  //B4 SBC   A,dp+X
		{"SbC  ",0x11,0xB0},  //B5 SBC   A,abs+X
		{"SbC  ",0x11,0xB2},  //B6 SBC   A,abs+Y
		{"SbC  ",0x11,0x76},  //B7 SBC   A,[dp]+Y
		{"SbC  ",0x61,0x43},  //B8 SBC   dp,#imm
		{"SbC  ",0x18,0x1A},  //B9 SBC   (X),(Y)
		{"MovW ",0x13,0x68},  //BA MOVW  YA,dp
		{"Inc  ",0x70,0x00},  //BB INC   dp+X
		{"Inc  ",0x11,0x00},  //BC INC   A
		{"Mov  ",0x15,0x10},  //BD MOV   SP,X
		{"DAS  ",0x00,0x00},  //BE DAS
		{"Mov  ",0x11,0x1C},  //BF MOV   A,(X)+

		{"DI   ",0x00,0x00},  //C0 DI
		{"TCall",0x2C,0x00},  //C1 TCALL 12
		{"Set1 ",0x62,0x00},  //C2 SET1  dp.6
		{"BBS  ",0x62,0x50},  //C3 BBS   dp.6
		{"Mov  ",0x60,0x11},  //C4 MOV   dp,A
		{"Mov  ",0xA0,0x11},  //C5 MOV   abs,A
		{"Mov  ",0x18,0x11},  //C6 MOV   (X),A
		{"Mov  ",0x74,0x11},  //C7 MOV   [dp+X],A
		{"Cmp  ",0x10,0x40},  //C8 CMP   X,#imm
		{"Mov  ",0xA0,0x10},  //C9 MOV   abs,X
		{"Mov1 ",0xA2,0x17},  //CA MOV1  mem.bit,C
		{"Mov  ",0x60,0x12},  //CB MOV   dp,Y
		{"Mov  ",0xA0,0x12},  //CC MOV   abs,Y
		{"Mov  ",0x10,0x40},  //CD MOV   X,#imm
		{"Pop  ",0x10,0x00},  //CE POP   X
		{"Mul  ",0x13,0x00},  //CF MUL   YA

		{"BNE  ",0x50,0x00},  //D0 BNE   rel
		{"TCall",0x2D,0x00},  //D1 TCALL 13
		{"Clr1 ",0x62,0x00},  //D2 CLR1  dp.6
		{"BBC  ",0x62,0x50},  //D3 BBC   dp.6
		{"Mov  ",0x70,0x11},  //D4 MOV   dp+X,A
		{"Mov  ",0xB0,0x11},  //D5 MOV   abs+X,A
		{"Mov  ",0xB2,0x11},  //D6 MOV   abs+Y,A
		{"Mov  ",0x76,0x11},  //D7 MOV   [dp]+Y,A
		{"Mov  ",0x60,0x10},  //D8 MOV   dp,X
		{"Mov  ",0x72,0x10},  //D9 MOV   dp+Y,X
		{"MovW ",0x68,0x13},  //DA MOVW  dp,YA
		{"Mov  ",0x70,0x12},  //DB MOV   dp+X,Y
		{"Dec  ",0x12,0x00},  //DC DEC   Y
		{"Mov  ",0x11,0x12},  //DD Mov   A,Y
		{"CBNE ",0x70,0x50},  //DE CBNE  dp+X,rel
		{"DAA  ",0x00,0x00},  //DF DAA

		{"ClrV ",0x00,0x00},  //E0 CLRV
		{"TCall",0x2E,0x00},  //E1 TCALL 14
		{"Set1 ",0x62,0x00},  //E2 SET1  dp.7
		{"BBS  ",0x62,0x50},  //E3 BBS   dp.7
		{"Mov  ",0x11,0x60},  //E4 MOV   A,dp
		{"Mov  ",0x11,0xA0},  //E5 MOV   A,abs
		{"Mov  ",0x11,0x18},  //E6 MOV   A,(X)
		{"Mov  ",0x11,0x74},  //E7 MOV   A,[dp+X]
		{"Mov  ",0x11,0x40},  //E8 MOV   A,#imm
		{"Mov  ",0x10,0xA0},  //E9 MOV   X,abs
		{"Not1 ",0xA2,0x00},  //EA NOT1  mem.bit
		{"Mov  ",0x12,0x60},  //EB MOV   Y,dp
		{"Mov  ",0x12,0xA0},  //EC MOV   Y,abs
		{"NotC ",0x00,0x00},  //ED NOTC
		{"Pop  ",0x12,0x00},  //EE POP   Y
		{"Sleep",0x00,0x00},  //EF SLEEP

		{"BEq  ",0x50,0x00},  //F0 BEQ   rel
		{"TCall",0x2F,0x00},  //F1 TCALL 15
		{"Clr1 ",0x62,0x00},  //F2 CLR1  dp.7
		{"BBC  ",0x62,0x50},  //F3 BBC   dp.7
		{"Mov  ",0x11,0x70},  //F4 MOV   A,dp+X
		{"Mov  ",0x11,0xB0},  //F5 MOV   A,abs+X
		{"Mov  ",0x11,0xB2},  //F6 MOV   A,abs+Y
		{"Mov  ",0x11,0x76},  //F7 MOV   A,[dp]+Y
		{"Mov  ",0x10,0x60},  //F8 MOV   X,dp
		{"Mov  ",0x10,0x72},  //F9 MOV   X,dp+Y
		{"Mov  ",0x61,0x63},  //FA MOV   dp(d),dp(s)
		{"Mov  ",0x12,0x70},  //FB MOV   Y,dp+X
		{"Inc  ",0x12,0x00},  //FC INC   Y
		{"Mov  ",0x12,0x11},  //FD Mov   Y,A
		{"DBNZ ",0x12,0x50},  //FE DBNZ  Y,rel
		{"Stop ",0x00,0x00}}; //FF STOP

//Mnemonics ------------------------------------
typedef struct
{
	s8	str[6];									//Instruction mnemonic (all lower case)
	u8	idx;									//Index in opcode table
	u8	num;									//Number of possible opcodes
} Mnemonic;

static const Mnemonic mnemonic[]={
		"adc  ",  0,12,
		"addw ", 12, 1,
		"and  ", 13,12,
		"and1 ", 25, 2,
		"asl  ", 27, 4,
		"bbc  ", 31, 8,
		"bbs  ", 39, 8,
		"bcc  ", 47, 1,
		"bcs  ", 48, 1,
		"beq  ", 49, 1,
		"bmi  ", 50, 1,
		"bne  ", 51, 1,
		"bpl  ", 52, 1,
		"bra  ", 53, 1,
		"brk  ", 54, 1,
		"bvc  ", 55, 1,
		"bvs  ", 56, 1,
		"call ", 57, 1,
		"cbne ", 58, 2,
		"clr1 ", 60, 8,
		"clrc ", 68, 1,
		"clrp ", 69, 1,
		"clrv ", 70, 1,
		"cmp  ", 71,18,
		"cmpw ", 89, 1,
		"daa  ", 90, 1,
		"das  ", 91, 1,
		"dbnz ", 92, 2,
		"dec  ", 94, 6,
		"decw ",100, 1,
		"di   ",101, 1,
		"div  ",102, 1,
		"ei   ",103, 1,
		"eor  ",104,12,
		"eor1 ",116, 1,
		"inc  ",117, 6,
		"incw ",123, 1,
		"jmp  ",124, 2,
		"lsr  ",126, 4,
		"mov  ",130,41,
		"mov1 ",171, 2,
		"movw ",173, 2,
		"mul  ",175, 1,
		"nop  ",176, 1,
		"not1 ",177, 1,
		"notc ",178, 1,
		"or   ",179,12,
		"or1  ",191, 2,
		"pcall",193, 1,
		"pop  ",194, 4,
		"push ",198, 4,
		"ret  ",202, 1,
		"reti ",203, 1,
		"rol  ",204, 4,
		"ror  ",208, 4,
		"sbc  ",212,12,
		"set1 ",224, 8,
		"setc ",232, 1,
		"setp ",233, 1,
		"sleep",234, 1,
		"stop ",235, 1,
		"subw ",236, 1,
		"tcall",237,16,
		"tclr1",253, 1,
		"tset1",254, 1,
		"xcn  ",255, 1};

static const u8 opcode[256]={
		0x84,0x85,0x86,0x87,0x88,0x89,0x94,0x95,0x96,0x97,0x98,0x99,	//ADC
		0x7A,															//ADDW
		0x24,0x25,0x26,0x27,0x28,0x29,0x34,0x35,0x36,0x37,0x38,0x39,	//AND
		0x4A,0x6A,														//AND1
		0x0B,0x0C,0x1B,0x1C,											//ASL
		0x13,0x33,0x53,0x73,0x93,0xB3,0xD3,0xF3,						//BBC
		0x03,0x23,0x43,0x63,0x83,0xA3,0xC3,0xE3,						//BBS
		0x90,															//BCC
		0xB0,															//BCS
		0xF0,															//BEQ
		0x30,															//BMI
		0xD0,															//BNE
		0x10,															//BPL
		0x2F,															//BRA
		0x0F,															//BRK
		0x50,															//BVC
		0x70,															//BVS
		0x3F,															//CALL
		0x2E,0xDE,														//CBNE
		0x12,0x32,0x52,0x72,0x92,0xB2,0xD2,0xF2,						//CLR1
		0x60,															//CLRC
		0x20,															//CLRP
		0xE0,															//CLRV
		0x1E,0x3E,0x5E,0x64,0x65,0x66,0x67,0x68,0x69,0x74,0x75,0x76,	//CMP
		0x77,0x78,0x79,0x7E,0xAD,0xC8,
		0x5A,															//CMPW
		0xBE,															//DAA
		0xDF,															//DAS
		0x6E,0xFE,														//DBNZ
		0x1D,0x8B,0x8C,0x9B,0x9C,0xDC,									//DEC
		0x1A,															//DECW
		0xC0,															//DI
		0x9E,															//DIV
		0xA0,															//EI
		0x44,0x45,0x46,0x47,0x48,0x49,0x54,0x55,0x56,0x57,0x58,0x59,	//EOR
		0x8A,															//EOR1
		0x3D,0xAB,0xAC,0xBB,0xBC,0xFC,									//INC
		0x3A,															//INCW
		0x1F,0x5F,														//JMP
		0x4B,0x4C,0x5B,0x5C,											//LSR
		0x5D,0x7D,0x8D,0x8F,0x9D,0xAF,0xBD,0xBF,0xC4,0xC5,0xC6,0xC7,	//MOV
		0xC9,0xCB,0xCC,0xCD,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDB,0xDD,
		0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEB,0xEC,0xF4,0xF5,0xF6,0xF7,
		0xF8,0xF9,0xFA,0xFB,0xFD,
		0xAA,0xCA,														//MOV1
		0xBA,0xDA,														//MOVW
		0xCF,															//MUL
		0x00,															//NOP
		0xEA,															//NOT1
		0xED,															//NOTC
		0x04,0x05,0x06,0x07,0x08,0x09,0x14,0x15,0x16,0x17,0x18,0x19,	//OR
		0x0A,0x2A,														//OR1
		0x4F,															//PCALL
		0x8E,0xAE,0xCE,0xEE,											//POP
		0x0D,0x2D,0x4D,0x6D,											//PUSH
		0x6F,															//RET
		0x7F,															//RETI
		0x2B,0x2C,0x3B,0x3C,											//ROL
		0x6B,0x6C,0x7B,0x7C,											//ROR
		0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,	//SBC
		0x02,0x22,0x42,0x62,0x82,0xA2,0xC2,0xE2,						//SET1
		0x80,															//SETC
		0x40,															//SETP
		0xEF,															//SLEEP
		0xFF,															//STOP
		0x9A,															//SUBW
		0x01,0x11,0x21,0x31,0x41,0x51,0x61,0x71,0x81,0x91,0xA1,0xB1,	//TCALL
		0xC1,0xD1,0xE1,0xF1,
		0x4E,															//TCLR1
		0x0E,															//TSET1
		0x9F};															//XCN

static const u16 EdelTab[16]={					//Number of bytes needed for echo delay
			 4, 0x800,
		0x1000,0x1800,
		0x2000,0x2800,
		0x3000,0x3800,
		0x4000,0x4800,
		0x5000,0x5800,
		0x6000,0x6800,
		0x7000,0x7800};

static const s8 RegList[8][4]={"X","A","Y","YA","X","SP","PSW","C"};
static const s8 FlagList[8]={"nvpbhizc"};

static const s8	HexTab[16]={"0123456789ABCDEF"};

static const s8 FlagName[8][12] = {	"Negative",		"Overflow",		"Direct Page",	"Break",
									"Half Carry",	"Interrupt",	"Zero",			"Carry"};

static const TColor MapColor[9] =
		{clGray, clBlue, clLime, clAqua, clRed, clFuchsia, clYellow, clWhite, clSilver};

static Stdctrls::TLabel*	lblRAMX[16];		//-> column headers in RAM view
static Stdctrls::TLabel*	lblRAMY[16];		//-> row headers in RAM view
static Stdctrls::TLabel*	lblIn[4];
static Stdctrls::TLabel*	lblOut[4];

static SPCDebug	*oldSPC;						//Old debugging vectors
static DSPDebug	*oldDSP;

static struct									//Registers in the SPC700
{
	u16			pc;
	union
	{
		struct {u8 a,y;};
		u16		ya;
	};
	u8			x;
	SPCFlags	psw;
	u16			sp;
	u8			u0,u1,u2,clk;
} reg;

static HANDLE	th;								//Handle to emulation thread

static DebugCmd	tCmd;							//Command to be carried out
static HANDLE	hEvent;

static u16		step;
static u8		call;

static u32		brkCnt,watchCnt;				//Number of breakpoints and watches
static u16		brk[16];						//Values of PC breakpoints are set at
static u8*		watch[21];						//Pointer to byte to watch
static u8		wbyte[21];						//Copy value of byte being watched for comparison

static u8		oldRAM[256];					//RAM that was displayed last step
static u8		oldReg[128];					//DSP registers that were displayed last step
static u16		oldPC;							//Value of PC at last step

static u32		ramOfs;							//RAM offset
static b8		PosRAM;
static u8		*PMouseByte;

static b8		UseMap;

static struct
{
	u8	tw,th;									//Text width and height (in pixels)
	u8	cw,ch;									//Cell width and height (in pixels)
} GSize;

typedef struct
{
	u8	*pByte;
	s32	mx,my;									//Mouse cursor position (in pixels)
	u8	y,x,n;									//Character position (in characters)
	u8	h;
	u8	c;
} Grid;


static struct
{
	u8		*pByte;
	TLabel	*pLabel;
	u8		mx,my;
	u8		y,x,n;
	u8		cx,cy;

	struct
	{
		u8		x,n;
		u8		*pByte;
		TLabel	*pLabel;
	} reg[8];
} Gspc;

static struct : public Grid
{
	u8	w;
} Gram;

static Grid	Gdsp;

static b8	CtrlKey;


static LONG	OldProcSPC,OldProcRAM,OldProcDSP;


TFrmDebug *FrmDebug;


//**************************************************************************************************
// Private Functions

static void __fastcall GetOperand(s8 **pStr, s8 **pCmnt, u8 *pOpr, u8 enc);
static void	DebugSPCThread(u32 param);
//static void	DebugSPC(volatile u8 *pc, volatile u16 ya, volatile u8 x,
//					 volatile SPCFlags psw, volatile u8 *sp, volatile u32 cnt);

//**************************************************************************************************
__fastcall TFrmDebug::TFrmDebug(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmDebug::FormCreate(TObject *sender)
{
	GSize.tw = pbDSP->Canvas->TextWidth("01AB") / 4;
	GSize.th = pbDSP->Canvas->TextHeight("A");
	GSize.cw = pbRAM->Width / 16;
	GSize.ch = pbRAM->Height / 16;

	lblIn[0] = lblIn0;
	lblIn[1] = lblIn1;
	lblIn[2] = lblIn2;
	lblIn[3] = lblIn3;

	lblOut[0] = lblOut0;
	lblOut[1] = lblOut1;
	lblOut[2] = lblOut2;
	lblOut[3] = lblOut3;

	lblPC->Tag	= 0x000;
	lblYA->Tag	= 0x001;
	lblX->Tag	= 0x002;
	lblSP->Tag	= 0x003;
	lblPSW->Tag	= 0x004;

	lblUp0->Tag	= 0x005;
	lblUp1->Tag	= 0x105;
	lblUp2->Tag	= 0x205;
	lblCyc->Tag	= 0x305;

	lblIn0->Tag = 0x006;
	lblIn1->Tag = 0x106;
	lblIn2->Tag = 0x206;
	lblIn3->Tag = 0x306;

	lblOut0->Tag = 0x007;
	lblOut1->Tag = 0x107;
	lblOut2->Tag = 0x207;
	lblOut3->Tag = 0x307;

	Gspc.reg[0].pLabel = lblPC;
	Gspc.reg[1].pLabel = lblYA;
	Gspc.reg[2].pLabel = lblX;
	Gspc.reg[3].pLabel = lblSP;
	Gspc.reg[4].pLabel = lblPSW;
	Gspc.reg[5].pLabel = lblUp0;
	Gspc.reg[6].pLabel = lblIn0;
	Gspc.reg[7].pLabel = lblOut0;

	Gspc.reg[0].x = lblPC->Left;
	Gspc.reg[1].x = lblYA->Left;
	Gspc.reg[2].x = lblX->Left;
	Gspc.reg[3].x = lblSP->Left + (2 * GSize.tw);
	Gspc.reg[4].x = lblPSW->Left;
	Gspc.reg[5].x = lblUp0->Left;
	Gspc.reg[6].x = lblIn0->Left;
	Gspc.reg[7].x = lblOut0->Left;

	Gspc.reg[0].n = 3;
	Gspc.reg[1].n = 3;
	Gspc.reg[2].n = 1;
	Gspc.reg[3].n = 1;
	Gspc.reg[4].n = 7;
	Gspc.reg[5].n = 1;
	Gspc.reg[6].n = 1;
	Gspc.reg[7].n = 1;

	Gspc.reg[0].pByte = (u8*)&reg.pc;
	Gspc.reg[1].pByte = (u8*)&reg.ya;
	Gspc.reg[2].pByte = &reg.x;
	Gspc.reg[3].pByte = (u8*)&reg.sp;
	Gspc.reg[4].pByte = (u8*)&reg.psw;
	Gspc.reg[5].pByte = &reg.u0;
	Gspc.reg[6].pByte = &Apu.ram[0xF4];
	Gspc.reg[7].pByte = &Apu.ram[0xF4];

	Gspc.cx = Gspc.reg[0].x;
	Gspc.cy = Gspc.reg[0].pLabel->Top;
	Gspc.y = 0;
	Gspc.x = 0;
	Gspc.n = 0;

	Gram.c = 15;
	Gram.w = 16;
	Gram.y = 0;
	Gram.x = 0;
	Gram.n = 0;
	Gram.h = -1;

	Gdsp.c = 7;
	Gdsp.y = 0;
	Gdsp.x = 0;
	Gdsp.n = 0;
	Gdsp.h = -1;

	lblRAMX[0]	= lblRAMX0;
	lblRAMX[1]	= lblRAMX1;
	lblRAMX[2]	= lblRAMX2;
	lblRAMX[3]	= lblRAMX3;
	lblRAMX[4]	= lblRAMX4;
	lblRAMX[5]	= lblRAMX5;
	lblRAMX[6]	= lblRAMX6;
	lblRAMX[7]	= lblRAMX7;
	lblRAMX[8]	= lblRAMX8;
	lblRAMX[9]	= lblRAMX9;
	lblRAMX[10]	= lblRAMXA;
	lblRAMX[11]	= lblRAMXB;
	lblRAMX[12]	= lblRAMXC;
	lblRAMX[13]	= lblRAMXD;
	lblRAMX[14]	= lblRAMXE;
	lblRAMX[15]	= lblRAMXF;

	lblRAMY[0]	= lblRAMY0;
	lblRAMY[1]	= lblRAMY1;
	lblRAMY[2]	= lblRAMY2;
	lblRAMY[3]	= lblRAMY3;
	lblRAMY[4]	= lblRAMY4;
	lblRAMY[5]	= lblRAMY5;
	lblRAMY[6]	= lblRAMY6;
	lblRAMY[7]	= lblRAMY7;
	lblRAMY[8]	= lblRAMY8;
	lblRAMY[9]	= lblRAMY9;
	lblRAMY[10]	= lblRAMYA;
	lblRAMY[11]	= lblRAMYB;
	lblRAMY[12]	= lblRAMYC;
	lblRAMY[13]	= lblRAMYD;
	lblRAMY[14]	= lblRAMYE;
	lblRAMY[15]	= lblRAMYF;

	reg.sp = 0x100;

	brkCnt = 0;
	watchCnt = 0;
	PosRAM = 0;

	pnlRAM->DoubleBuffered = true;
	pnlDSP->DoubleBuffered = true;

	//The TPanel class doesn't expose a method for processing key messages, worse yet, it doesn't
	//return DLGC_WANTARROWS in response to a WM_GETDLGCODE message, so the form interprets the
	//cursor keys as navigation keys to move between components.  So we have to subclass the window
	//and intercept the keyboard messages before the VCL gets them.

	OldProcSPC = SetWindowLong(pnlSPC->Handle, GWL_WNDPROC, (LONG)&MsgProcSPC);
	OldProcRAM = SetWindowLong(pnlRAM->Handle, GWL_WNDPROC, (LONG)&MsgProcRAM);
	OldProcDSP = SetWindowLong(pnlDSP->Handle, GWL_WNDPROC, (LONG)&MsgProcDSP);
}


//**************************************************************************************************
void __fastcall TFrmDebug::FormDestroy(TObject *sender)
{
	SetWindowLong(pnlDSP->Handle, GWL_WNDPROC, OldProcDSP);
	SetWindowLong(pnlRAM->Handle, GWL_WNDPROC, OldProcRAM);
	SetWindowLong(pnlSPC->Handle, GWL_WNDPROC, OldProcSPC);
}


//**************************************************************************************************
void __fastcall TFrmDebug::FormShow(TObject *sender)
{
	s32	x,y,base;


	//Print in-ports ---------------------------
	lblIn0->Caption = Int2Hex(Apu.ram[0xF4], 2);
	lblIn1->Caption = Int2Hex(Apu.ram[0xF5], 2);
	lblIn2->Caption = Int2Hex(Apu.ram[0xF6], 2);
	lblIn3->Caption = Int2Hex(Apu.ram[0xF7], 2);

	//Print memory -----------------------------
	ramOfs = 0;
	UseMap = 0;

	barRAMChange(sender);

	memcpy(oldRAM, &Apu.ram[barRAM->Position * Gram.w + ramOfs], 256);
	memcpy(oldReg, Apu.dsp->reg, 128);

	commit = 0;

	//Start emulation thread ------------------
	oldSPC = Apu.SetSPCDbg(DebugSPCs, 0);

	tCmd = CMD_WAIT;
	hEvent = CreateEvent(NULL, 0, 0, NULL);
	th = (HANDLE)CreateThread(NULL, 512, (LPTHREAD_START_ROUTINE)DebugSPCThread, NULL, 0, (LPDWORD)&x);
}


//**************************************************************************************************
void __fastcall TFrmDebug::FormClose(TObject *sender, TCloseAction &Action)
{
	Apu.SetSPCDbg((SPCDebug*)-1, SPC_HALT);
	Apu.SetDSPDbg((DSPDebug*)-1, DSP_HALT);
	SetCmd(CMD_QUIT);
	if (WaitForSingleObject(th, 500) == WAIT_TIMEOUT) TerminateThread(th, 0);

	CloseHandle(th);						//Deallocate handle resources
	CloseHandle(hEvent);

	Apu.SetSPCDbg(oldSPC, 0);
	Apu.SetDSPDbg((DSPDebug*)-1, 0);
}


//**************************************************************************************************
bool __fastcall TFrmDebug::WantChildKey(Controls::TControl* child, Messages::TMessage &msg)
{
	static b8	shift,ctrl;
	s32			key;


	switch(msg.Msg)
	{
	case(WM_KEYDOWN):
		switch(msg.WParam)
		{
		case(VK_SHIFT):							//Mark shift state
			shift = 1;
			return 0;

		case(VK_CONTROL):						//Mark control state
			ctrl = 1;
			return 0;

		case(VK_RETURN):
			if (ActiveControl == lstBrkAddr) lstBrkAddrDblClick(NULL);
			else
			if (ActiveControl == lstBrkData) lstBrkDataDblClick(NULL);
			return 0;
		}

		//The following commands don't respond to repeating keys
		if (msg.LParamHi & KF_REPEAT) goto Remove;

		switch((s8)msg.WParam | 0x20)
		{
		case('n'):
			FlipPSW(0x80);
			return 1;

		case('v'):
			FlipPSW(0x40);
			return 1;

		case('p'):
			FlipPSW(0x20);
			return 1;

		case('b'):
			FlipPSW(0x10);
			return 1;

		case('h'):
			FlipPSW(0x08);
			return 1;

		case('i'):
			FlipPSW(0x04);
			return 1;

		case('z'):
			FlipPSW(0x02);
			return 1;

		case('c'):
			FlipPSW(0x01);
			return 1;
		}

		goto Remove;

	case(WM_KEYUP):
		switch(msg.WParam)
		{
		case(VK_SHIFT):
			shift = 0;
			return 0;

		case(VK_CONTROL):
			ctrl = 0;
			return 0;
		}

	case(WM_CHAR):								//Keep the application from processing custom keys
Remove:
		switch((s8)msg.WParam | 0x20)
		{
		case('n'):
		case('v'):
		case('p'):
		case('b'):
		case('h'):
		case('i'):
		case('z'):
		case('c'):
			return 1;
		}
	}

	return 0;
}


//**************************************************************************************************
void __fastcall TFrmDebug::SetCmd(DebugCmd cmd)
{
	tCmd = cmd;
	if (cmd != CMD_WAIT) SetEvent(hEvent);
}


//**************************************************************************************************
void DebugSPCThread(u32 param)
{
	u64	temp;

	while (tCmd != CMD_QUIT) Apu.EmuAPU(&temp, 0, 1);
}


//**************************************************************************************************
void TFrmDebug::DebugSPCs(volatile u8 *pc, volatile u16 ya, volatile u8 x, volatile SPCFlags psw,
			  volatile u8 *sp, volatile u32 cnt)
{
	u32	i;


	if (tCmd == CMD_QUIT) return;

	//Test PC against breakpoints --------------
	for (i=0; i<brkCnt; i++)
		if (brk[i] == (u16)pc)
		{
			FrmDebug->Stop();
			goto BreakPoint;
		}

	//Test data for changes --------------------
	for (i=0; i<watchCnt; i++)
		if (*watch[i] != wbyte[i])
		{
			wbyte[i] = *watch[i];
			FrmDebug->Stop();
			goto BreakPoint;
		}

	//Execute command --------------------------
	oldPC = (u16)pc;

	switch(tCmd)
	{
	case(CMD_OUT):	//Step out
		if (*pc == 0x6F && call <= (u8)sp)		//If instruction is RET and SP isn't less than it
		{										// was when the user initiated step out command
			FrmDebug->Stop();
			FrmDebug->SetCmd(CMD_WAIT);
		}
		return;

	case(CMD_OVER):	//Step over
		if ((u16)pc == step)					//Step over sets a temporary breakpoint
		{
			FrmDebug->Stop();
			break;
		}

	case(CMD_RUN):	//Run
		return;									//If command is to run, return to emulation
	}

BreakPoint:
	FrmDebug->SetCmd(CMD_WAIT);					//Disable current command

	reg.pc	= (u16)pc;							//Copy registers into global space
	reg.ya	= ya;
	reg.x	= x;
	reg.psw	= psw;
	reg.sp	= (u16)sp;
	*(u32*)&reg.u0	= cnt;

	FrmDebug->DebugSPC();						//Update form

      	((u16*)&pc)[0]	= reg.pc;					//Save any changes to the registers
	((u16*)&pc)[2]	= reg.ya;					//(In a release build, the compiler will strip out
	((u8*)&pc)[8]	= reg.x;					// code that writes to a parameter directly)
	((u8*)&pc)[12]	= *(u8*)&reg.psw;
	((u16*)&pc)[8]	= reg.sp;
}

void __fastcall TFrmDebug::DebugSPC()
{
	u32	i,j;
	u8	*pNew,*pOld,r,c;
	s8	str[9];

	static	u16	lastPC,lastSP;
	static	u8	lastD;

	//Print registers --------------------------
	lblPC->Caption	= Int2Hex(reg.pc, 4);
	lblYA->Caption	= Int2Hex(((s32)reg.y << 8) | reg.a, 4);
	lblX->Caption	= Int2Hex(reg.x, 2);
	lblSP->Caption	= Int2Hex(reg.sp, 4);

	FlipPSW(0);

	lblUp0->Caption = Int2Hex(reg.u0, 2);
	lblUp1->Caption = Int2Hex(reg.u1, 2);
	lblUp2->Caption = Int2Hex(reg.u2, 2);
	lblCyc->Caption = Int2Hex(reg.clk, 2);

	lblOut0->Caption = Int2Hex(Apu.GetAPUPort(0), 2);
	lblOut1->Caption = Int2Hex(Apu.GetAPUPort(1), 2);
	lblOut2->Caption = Int2Hex(Apu.GetAPUPort(2), 2);
	lblOut3->Caption = Int2Hex(Apu.GetAPUPort(3), 2);

	Disassembly(oldPC);


        //Update Memory Views (Now handled by a 100ms interval timer.)
        //The timer is disabled before an update happens, if another
        //step into/over happens in less than 100ms, and reenabled once this
        //step is done.  So far, it hasn't crashed yet.


        Timer1->Enabled = True;
	WaitForSingleObject(hEvent, INFINITE);
        //Timer1->Enabled = False;

	memcpy(oldRAM, &Apu.ram[barRAM->Position * Gram.w + ramOfs], 256);
	memcpy(oldReg, Apu.dsp->reg, 128);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu Items

//**************************************************************************************************
void __fastcall TFrmDebug::Run()
{
	itmReset->Enabled	= false;
	itmStop->Enabled	= true;
	itmInto->Enabled	= false;
	itmOut->Enabled		= false;
	itmOver->Enabled	= false;
	itmRunTo->Enabled	= false;
	itmBrkAddr->Enabled	= false;
	itmBrkData->Enabled	= false;

	mnuOther->Enabled	= false;
	mnuHelp->Enabled	= false;
	lstDisasm->Enabled	= false;
	pnlSPC->Enabled		= false;
	pnlRAM->Enabled		= false;
	pnlDSP->Enabled		= false;
}

void __fastcall TFrmDebug::itmRunClick(TObject *sender)
{
	if (tCmd != CMD_WAIT) return;

	Run();
	SetCmd(CMD_RUN);
}


//**************************************************************************************************
void __fastcall TFrmDebug::Stop()
{
//	itmReset->Enabled	= true;
	itmStop->Enabled	= false;
	itmInto->Enabled	= true;
	itmOut->Enabled		= true;
	itmOver->Enabled	= true;
	itmRunTo->Enabled	= true;
	itmBrkAddr->Enabled	= true;
	itmBrkData->Enabled	= true;

	mnuOther->Enabled	= true;
	mnuHelp->Enabled	= true;
	lstDisasm->Enabled	= true;
	pnlSPC->Enabled		= true;
	pnlRAM->Enabled		= true;
	pnlDSP->Enabled		= true;
}


void __fastcall TFrmDebug::itmStopClick(TObject *sender)
{
	Stop();
	SetCmd(CMD_WAIT);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmIntoClick(TObject *sender)
{
	if (tCmd != CMD_WAIT) return;

	SetCmd(CMD_STEP);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmOutClick(TObject *sender)
{
	if (tCmd != CMD_WAIT) return;

	if (Apu.ram[reg.pc] == 0x6F) SetCmd(CMD_STEP);

	Run();
	call = reg.sp;
	SetCmd(CMD_OUT);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmOverClick(TObject *sender)
{
	u8	i;


	if (tCmd != CMD_WAIT) return;

	i = Apu.ram[reg.pc];
	if (i == 0x3F || i == 0x4F || (i & 0xF) == 1)	//Only step over call instructions
	{
		Run();
		step = reg.pc + (InsList[i].op1 >> 6) + (InsList[i].op2 >> 6) + 1;
		SetCmd(CMD_OVER);
	}
	else
		SetCmd(CMD_STEP);						//Normal "step into" operation for everything else
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmRunToClick(TObject *sender)
{
	s32	i;


	if (tCmd != CMD_WAIT) return;

	i = lstDisasm->Items->Count;
	while (i--) if (lstDisasm->Selected[i]) break;

	if (i < 0) return;

	Run();
	step = (u16)lstDisasm->Items->Objects[i];
	SetCmd(CMD_OVER);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmBrkAddrClick(TObject *sender)
{
	u32	i;
	u16	pc;


	//Get PC for currently selected line -------
	for (i=0; i<(u32)lstDisasm->Items->Count; i++)
		if (lstDisasm->Selected[i])
		{
			pc = (u16)lstDisasm->Items->Objects[i];
			break;
		}

	//Search breakpoints for PC ----------------
	for (i=0; i<brkCnt; i++)
		if (pc == (u16)lstBrkAddr->Items->Objects[i]) break;

	//Add or remove breakpoint -----------------
	if (i == brkCnt)							//If PC wasn't found, add new breakpoint
	{
		if (brkCnt == 16)						//If list is full, no more breakpoints can be added
		{
			MessageDlg("You can only have 16 breakpoints.  Remove some breakpoints before " \
					   "adding a new one", mtError, TMsgDlgButtons() << mbOK, 0);
			return;
		}

		lstBrkAddr->Items->AddObject(Int2Hex(pc, 4), (TObject*)&Apu.ram[pc]);
		brk[brkCnt++] = pc;
	}
	else
	{
		lstBrkAddr->Items->Delete(i);			//Delete breakpoint in list view

		for (i=0; i<brkCnt; i++)				//Search for PC in internal list
			if (pc == brk[i]) break;

		brkCnt--;										//Decrease breakpoint count
		memcpy(&brk[i], &brk[i+1], (brkCnt - i) * 2);	//Remove breakpoint from internal list
	}

	lstDisasm->Repaint();
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmBrkDataClick(TObject *sender)
{
	//Get pointer to byte to place a breakpoint on
	if (ActiveControl == pnlRAM) Watch(Gram.pByte);
	else
	if (ActiveControl == pnlDSP) Watch(Gdsp.pByte);
	else
//	if (ActiveControl == pnlSPC)
// 	{
//		if (Gspc.y == 7) Watch(Gspc.pByte);
//	}
//	else
		return;

	RepaintCell(-1);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmExitClick(TObject *sender)
{
	Close();
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmCommitClick(TObject *sender)
{
	memcpy(pState->spc.ram, Apu.ram, 0x10000);
	memcpy(pState->spc.dsp.reg, Apu.dsp->reg, 0x80);
	memcpy(pState->spc.xram, &Apu.ram[-0x80], 0x40);
        memcpy(&pState->spc.ram[0xFA], &Apu.ram[-0x6], 3);

	*(u16*)pState->spc.hdr.reg.pc	= reg.pc;
	pState->spc.hdr.reg.a	= reg.a;
	pState->spc.hdr.reg.x	= reg.x;
	pState->spc.hdr.reg.y	= reg.y;
	pState->spc.hdr.reg.psw	= *(u8*)&reg.psw;
	pState->spc.hdr.reg.sp	= reg.sp;

	commit = 1;
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmMapClick(TObject *sender)
{
	s32	i;


	//TODO: Map should not overwrite the main state
	Apu.SaveAPU(&pState->state.spcState, &pState->state.dspState);

	SetCmd(CMD_QUIT);
	if (WaitForSingleObject(th, 500) == WAIT_TIMEOUT) TerminateThread(th, 0);
	CloseHandle(th);

	if (!FrmMap) FrmMap = new TFrmMap(FrmDebug);
	FrmMap->pState = pState;
	UseMap = (FrmMap->ShowModal() == mrOk);
	delete FrmMap;
	FrmMap = NULL;

	Apu.RestoreAPU(&pState->state.spcState, &pState->state.dspState);

	SetCmd(CMD_WAIT);
	th = (HANDLE)CreateThread(NULL, 512, (LPTHREAD_START_ROUTINE)DebugSPCThread, NULL, 0, (LPDWORD)&i);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmGotoPCClick(TObject *sender)
{
	SetRAMPos(reg.pc);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmGotoSPClick(TObject *sender)
{
	SetRAMPos(reg.sp);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmHelpClick(TObject *sender)
{
	//Application->HelpCommand(HELP_CONTEXT, Emu_Debug);
/*
#define HELP_CONTEXT      0x0001L  /* Display topic in ulTopic
#define HELP_QUIT         0x0002L  /* Terminate help
#define HELP_INDEX        0x0003L  /* Display index
#define HELP_CONTENTS     0x0003L
#define HELP_HELPONHELP   0x0004L  /* Display help on using help
#define HELP_SETINDEX     0x0005L  /* Set current Index for multi index help
#define HELP_SETCONTENTS  0x0005L
#define HELP_CONTEXTPOPUP 0x0008L
#define HELP_FORCEFILE    0x0009L
#define HELP_KEY          0x0101L  /* Display topic for keyword in offabData
#define HELP_COMMAND      0x0102L
#define HELP_PARTIALKEY   0x0105L
#define HELP_MULTIKEY     0x0201L
#define HELP_SETWINPOS    0x0203L
#if(WINVER >= 0x0400)
#define HELP_CONTEXTMENU  0x000a
#define HELP_FINDER       0x000b
#define HELP_WM_HELP      0x000c
#define HELP_SETPOPUP_POS 0x000d */
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Disassembly

//**************************************************************************************************
void __fastcall TFrmDebug::lstDisasmDrawItem(TWinControl *control, int idx, TRect &rect, TOwnerDrawState state)
{
	TCanvas	*p;
	u32		i;
	u16		pc;


	p = lstDisasm->Canvas;
	if (state.Contains(odSelected))
	{
		p->Font->Color = clSilver;
		p->Brush->Color = clBlack;
	}

	pc = (u16)lstDisasm->Items->Objects[idx];
	if (reg.pc == pc)
	{
		p->Font->Color = clWhite;
		p->Brush->Color = (TColor)0x404040;
	}

	for (i=0; i<brkCnt; i++)
		if (pc == brk[i])
		{
			p->Font->Color = clRed;
			break;
		}

	p->TextRect(rect, rect.left, rect.top, lstDisasm->Items->Strings[idx]);
}


//**************************************************************************************************
void __fastcall TFrmDebug::Disassembly(u16 pc)
{
	AnsiString	line;

	s32	i,n;
	u16	next;


	n = lstDisasm->Items->Count;
	for (i=0; i<n; i++)
		if (pc == (u16)lstDisasm->Items->Objects[i]) break;

	lstDisasm->Items->BeginUpdate();

	if (i <= (n - 2))
	{
		Disassemble(line, pc);
		lstDisasm->Items->Strings[i] = line;
	}
	else
	if (i == n)
	{
		lstDisasm->Items->Clear();
		n = lstDisasm->Height / lstDisasm->ItemHeight;
		while(n--)
		{
			next = Disassemble(line, pc);
			lstDisasm->Items->AddObject(line, (TObject*)pc);
			pc = next;
		}
	}
	else
	{
		n = i -= (n - 2);
		do lstDisasm->Items->Delete(0); while(--i);

		pc = (u16)lstDisasm->Items->Objects[0];
		while(n--)
		{
			next = Disassemble(line, pc);
			lstDisasm->Items->AddObject(line, (TObject*)pc);
			pc = next;
		}
	}

	lstDisasm->Items->EndUpdate();
}


s8* __fastcall PrintDP(s8 *pStr, u8 a)
{
	if (a < 10)
	{
		*pStr++ = a | 0x30;
		return pStr;
	}

	switch(Cfg.debug.hex)
	{
	case('$'):
		pStr[0] = '$';
		pStr[1] = Cfg.debug.hexChar[a >> 4];
		pStr[2] = Cfg.debug.hexChar[a & 0xF];
		break;

	case('h'):
		if (a > 0x9F) *pStr++ = '0';
		pStr[0] = Cfg.debug.hexChar[a >> 4];
		pStr[1] = Cfg.debug.hexChar[a & 0xF];
		pStr[2] = 'h';
		break;

	case('x'):
		*pStr++ = '0';
		pStr[0] = 'x';
		pStr[1] = Cfg.debug.hexChar[a >> 4];
		pStr[2] = Cfg.debug.hexChar[a & 0xF];
		break;
	}

	return pStr + 3;
}


s8* PrintAbs(s8 *pStr, u16 a)
{
	if (a < 0x100)
	{
		*pStr++ = Cfg.debug.abs;
		return PrintDP(pStr,a);
	}

	switch(Cfg.debug.hex)
	{
	case('$'):
		pStr[0] = '$';
		pStr[1] = Cfg.debug.hexChar[a >> 12];
		pStr[2] = Cfg.debug.hexChar[(a >> 8) & 0xF];
		pStr[3] = Cfg.debug.hexChar[(a >> 4) & 0xF];
		pStr[4] = Cfg.debug.hexChar[a & 0xF];
		break;

	case('h'):
		if (a > 0x9FFF) *pStr++ = '0';
		pStr[0] = Cfg.debug.hexChar[a >> 12];
		pStr[1] = Cfg.debug.hexChar[(a >> 8) & 0xF];
		pStr[2] = Cfg.debug.hexChar[(a >> 4) & 0xF];
		pStr[3] = Cfg.debug.hexChar[a & 0xF];
		pStr[4] = 'h';
		break;

	case('x'):
		*pStr++ = '0';
		pStr[0] = 'x';
		pStr[1] = Cfg.debug.hexChar[a >> 12];
		pStr[2] = Cfg.debug.hexChar[(a >> 8) & 0xF];
		pStr[3] = Cfg.debug.hexChar[(a >> 4) & 0xF];
		pStr[4] = Cfg.debug.hexChar[a & 0xF];
		break;
	}

	return pStr + 5;
}


u16 __fastcall TFrmDebug::Disassemble(AnsiString &line, u16 pc)
{
	u8	*p;
	u32	i;
	s8	str[64],cmnt[32],*s,*c;


	p = &Apu.ram[pc];
	s = str;
	c = cmnt + 1;

	//Print PC -------------------------------
	line = "";
	if (Cfg.debug.printPC)
	{
		i = (u32)p;
		s[3] = Cfg.debug.hexChar[i & 0xF];
		i >>= 4;
		s[2] = Cfg.debug.hexChar[i & 0xF];
		i >>= 4;
		s[1] = Cfg.debug.hexChar[i & 0xF];
		i >>= 4;
		s[0] = Cfg.debug.hexChar[i & 0xF];
		s[4] = ' ';
		s += 5;
	}

	//Print machine code ---------------------
	if (Cfg.debug.printOp)
	{
		s[0] = Cfg.debug.hexChar[p[0] >> 4];
		s[1] = Cfg.debug.hexChar[p[0] & 0xF];
		memset(&s[2], ' ', 7);
		s += 3;

		i = (InsList[*p].op1 >> 6) + (InsList[*p].op2 >> 6);
		if (i)
		{
			s[0] = Cfg.debug.hexChar[p[1] >> 4];
			s[1] = Cfg.debug.hexChar[p[1] & 0xF];
			if (i > 1)
			{
				s[3] = Cfg.debug.hexChar[p[2] >> 4];
				s[4] = Cfg.debug.hexChar[p[2] & 0xF];
			}
		}
		s += 6;
	}

	//Disassemble instruction ----------------
	*s++ = ' ';
	s = CopyCase(s, InsList[*p].mnemonic);
	*s++ = ' ';

	cmnt[0] = ';';
	cmnt[1] = 0;

	GetOperand(&s, &c, p, 1);
	if (InsList[*p].op2)
	{
		*(s++)=',';
		if (cmnt[1]) *c++ = ' ';
		GetOperand(&s, &c, p, 2);
	}

	*s = 0;
	*c = 0;
	line = str;
	if (Cfg.debug.printCmnt && cmnt[1])
		line += AnsiString::StringOfChar(' ', Cfg.debug.cmntCol - (s - str)) + cmnt;

	return pc + 1 + (InsList[*p].op1 >> 6) + (InsList[*p].op2 >> 6);
}


//**************************************************************************************************
// Disassemble Operand
//
// In:
//    str -> Pointer to disassembly output
//    cmnt-> Pointer to comment for disassembly
//    p   -> Instruction
//    opr  = Operand number

static void __fastcall GetOperand(s8 **pStr, s8 **pCmnt, u8 *p, u8 opr)
{
	s8 	*s,*c;
	u8	*pOpr,enc;								//-> instruction operand, operand encoding
	u32	i,j;


	s = *pStr;
	c = *pCmnt;

	if (opr == 1)
	{
		enc = InsList[*p].op1;
		pOpr = p + 1;
	}
	else
	{
		enc = InsList[*p].op2;
		pOpr = p + 1 + (InsList[*p].op1 >> 6);
	}

	switch (enc>>6)
	{
	case(0):
		switch ((enc>>4) & 3)
		{
		case(1):
			if ((enc & 8) == 0)
			{
				s = CopyCase(s, RegList[enc & 7]);
			}
			else
			{
				*s++ = Cfg.debug.openBrace;
				s = CopyCase(s, RegList[enc & 7]);
				*s++ = Cfg.debug.closeBrace;

				if ((enc & 7) == 4)
				{
					if (Cfg.debug.syntax == 1)
					{
						s[-1] = '+';
						*s++ = Cfg.debug.closeBrace;
					}
					else
					{
						*s++ = '+';
					}
				}

				i = ((enc & 3) ? reg.y : reg.x) + ((u32)reg.psw.p << 3);
				sprintf(c, "%04X->%02X", i, Apu.ram[i]);
				c = ::StrEnd(c);
			}
			break;

		case(2):
			i = enc & 0xF;
			sprintf(s, "%i", i);
			s = ::StrEnd(s);

			i = 0xFFC0 + ((15 - i) << 1);
			sprintf(c, "%04X->%04X", i, *(u16*)&Apu.ram[i]);
			c = ::StrEnd(c);
			break;
		}
		break;

	case(1):
		switch ((enc>>4) & 3)
		{
		case(0):
			i = pOpr[((s32)enc << 30) >> 30];
			s = PrintDP(s, i);

			if (enc & 8)
			{
				i += 0xFF00;
				sprintf(c, "%04X->%04X", i, *(s16*)&Apu.ram[i]);
				c = ::StrEnd(c);
			}
			break;

		case(1):
			i = (u16)p + 1 + (InsList[*p].op1 >> 6) + (InsList[*p].op2 >> 6) + *(s8*)pOpr;
			sprintf(s, "%04X", i);
			s = ::StrEnd(s);

			sprintf(c, "%i", *(s8*)pOpr);
			c = ::StrEnd(c);
			break;

		case(2):
			if ((enc & 7) != 2)
			{
				i = pOpr[((s32)enc << 30) >> 30];
				if (Cfg.debug.syntax)
				{
					*s++ = '[';
					s = PrintDP(s, i);
					*s++ = ']';
				}
				else
					s = PrintDP(s, i);

				if (enc & 8)
				{
					sprintf(c, "%04X->%04X", i, *(u16*)&Apu.ram[i]);
					c = ::StrEnd(c);
				}
				else
				{
					sprintf(c, "%04X->%02X", i, Apu.ram[i]);
					c = ::StrEnd(c);
				}
			}
			else
			{
				i = (*p >> 5);
				j = i | 0x30;

				if (Cfg.debug.syntax)
				{
					*s++ = '[';
					s = PrintDP(s, *pOpr);
					s[0] = ']';
					s[1] = ',';
					s[2] = j;
					s += 3;
				}
				else
				{
					s = PrintDP(s, *pOpr);
					*s++ = '.';
					*s++ = j;
				}

				sprintf(c, "%04X.%c->%c", *pOpr + ((u32)reg.psw.p << 3),
						j, ((Apu.ram[*pOpr] >> i) & 1) | 0x30);
				c = ::StrEnd(c);
			}
			break;

		case(3):
			i = ((u32)reg.psw.p << 3) + *pOpr;

			if (Cfg.debug.syntax) *s++ = '[';

			if ((enc & 4) == 0)
			{
				s = PrintDP(s, *pOpr);
				s[0] = '+';
				s = CopyCase(s+1, RegList[enc & 3]);

				i += ((enc & 3) ? reg.y : reg.x);
				if (enc & 8)
					sprintf(c, "%04X->%04X", i, *(u16*)&Apu.ram[i]);
				else
					sprintf(c, "%04X->%02X", i, Apu.ram[i]);
				c = ::StrEnd(c);
			}
			else
			{
				if ((enc & 3) == 0)
				{
					*s++ = Cfg.debug.openBrace;
					s = PrintDP(s, *pOpr);
					*s++ = '+';
					s = CopyCase(s, RegList[0]);
					*s++ = Cfg.debug.closeBrace;

					i += reg.x;
					j = *(u16*)&Apu.ram[i];
				}
				else
				{
					s[0] = Cfg.debug.openBrace;
					s = PrintDP(s+1, *pOpr);
					s[0] = Cfg.debug.closeBrace;
					s[1] = '+';
					s = CopyCase(s+2, RegList[2]);

					j = *(u16*)&Apu.ram[i] + reg.y;
				}

				sprintf(c, "%04X->%04X->%02X", i, j, Apu.ram[j]);
				c = ::StrEnd(c);
			}

			if (Cfg.debug.syntax) *s++ = ']';
		}
		break;

	// 2 - Absolute offset
	//       3 Type (if bits 2-0 are 0)
	//         0 Pointer (i.e. MOV A,abs)
	//         1 Location (i.e. CALL abs)
	//       3 Reference bit (if bits 2-0 are 2)
	//         0 Normal
	//         1 Complimented
	//     2:0 Type
	//         0 abs
	//         2 mem.bit
	//
	// 3 - Absolute offset with index
	//       3 Reference type
	//         0 Byte
	//         1 Word
	//       2 Pointer method
	//         0 Indirect
	//         1 Indexed indirect
	//     1:0 Register
	//         0 X
	//         2 Y
	case(2):
		i = *(u16*)pOpr;

		switch ((enc>>4) & 3)
		{
		case(2):
			if (!(enc & 3))
			{
				if (enc & 8)
				{
					s = PrintAbs(s, i);
				}
				else
				{
					if (Cfg.debug.syntax) *s++ = '[';
					s = PrintAbs(s, i);
					if (Cfg.debug.syntax) *s++ = ']';

					sprintf(c, "%04X->%02X", i, Apu.ram[i]);
					c = ::StrEnd(c);
				}
			}
			else
			{
			}
			break;

		case(3):
			if (Cfg.debug.syntax) *s++ = '[';

			if ((enc & 4) == 0)
			{
				s = PrintAbs(s, i);
				s[0] = '+';
				s = CopyCase(s+1, RegList[enc & 3]);

				i += ((enc & 3) ? reg.y : reg.x);
				if (enc & 8)
					sprintf(c, "%04X->%04X", i, *(u16*)&Apu.ram[i]);
				else
					sprintf(c, "%04X->%02X", i, Apu.ram[i]);
				c = ::StrEnd(c);
			}
			else
			{
				if ((enc & 3) == 0)
				{
					*s++ = Cfg.debug.openBrace;
					s = PrintAbs(s, i);
					*s++ = '+';
					s = CopyCase(s, RegList[0]);
					*s++ = Cfg.debug.closeBrace;

					i += reg.x;
					j = *(u16*)&Apu.ram[i];
				}
				else
				{
					s[0] = Cfg.debug.openBrace;
					s = PrintAbs(s+1, *pOpr);
					s[0] = Cfg.debug.closeBrace;
					s[1] = '+';
					s = CopyCase(s+2, RegList[2]);

					j = *(u16*)&Apu.ram[i] + reg.y;
				}

				sprintf(c, "%04X->%04X->%02X", i, j, *(u16*)&Apu.ram[j]);
				c = ::StrEnd(c);
			}

			if (Cfg.debug.syntax) *s++ = ']';
			break;
		}
		break;
	}

	*pStr = s;
	*pCmnt = c;
}


//**************************************************************************************************
// Determine Envelope Period
//
// Desc:
//    Calculates the amount of time it will take an envelope to decay
//
// In:
//    height = Current envelope height
//    dest   = Destination envelope height
//    rate   = Rate of adjustment in Hz
//
// Out:
//    Number of milliseconds

s32 __fastcall EnvPer(f32 height, f32 dest, s32 rate)
{
	if (rate)
		return (s32)(Logx(256.0 / 255.0, height / dest) * (32000.0 / rate)) / 32;

	return -1;
}


void __fastcall TFrmDebug::RepaintCell(u32 r)
{
	s32	i;


	if (r == -1)
	{
		if (ActiveControl == pnlRAM)
		{
			HideCaret(pnlRAM->Handle);
			PrintRAM(Gram.x, Gram.y);
			ShowCaret(pnlRAM->Handle);
		}
		else
		{
			HideCaret(pnlDSP->Handle);
			PrintDSP(Gdsp.y, Gdsp.x);
			ShowCaret(pnlDSP->Handle);
		}

		return;
	}

	if (r < 0x10000)
	{
		i = r - ((barRAM->Position << 4) + ramOfs);

		if (i < 0 || i >= Gram.w * 16) return;

		if (ActiveControl == pnlRAM)
		{
			HideCaret(pnlRAM->Handle);
			PrintRAM(i % Gram.w, i / Gram.w);
			ShowCaret(pnlRAM->Handle);
		}
		else
			PrintRAM(i % Gram.w, i / Gram.w);
	}
	else
	{
		if (r > 0x1007F) return;

		if (ActiveControl == pnlDSP)
		{
			HideCaret(pnlDSP->Handle);
			PrintDSP(r & 0xF, (u8)r >> 4);
			ShowCaret(pnlDSP->Handle);
		}
		else
			PrintDSP(r & 0xF, (u8)r >> 4);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// SPC700 Registers

//**************************************************************************************************
void __fastcall TFrmDebug::pnlSPCEnter(TObject *sender)
{
	CtrlKey = 0;

	CreateCaret(pnlSPC->Handle, NULL, 0, GSize.th);
	ShowCaret(pnlSPC->Handle);


	SetCaretPos(Gspc.cx, Gspc.cy);
}


//**************************************************************************************************
void __fastcall TFrmDebug::pnlSPCExit(TObject *sender)
{
	DestroyCaret();
}


//**************************************************************************************************
void __fastcall TFrmDebug::KeyDownSPC(s32 key)
{
	u32	b;
	s8	c;


	c = 0;
	switch(key)
	{
	case(VK_LEFT):
		if (Gspc.n)
		{
			Gspc.n--;
			c = 2;
		}
		else
		if (Gspc.y > 4 && Gspc.x > 0)
		{
			Gspc.x--;
			Gspc.n = 1;
			c = 1;
		}
		break;

	case(VK_RIGHT):
Next:
		if (Gspc.n < Gspc.reg[Gspc.y].n)
		{
			Gspc.n++;
			c = 2;
		}
		else
		if (Gspc.y > 4 && Gspc.x < 3)
		{
			Gspc.x++;
			Gspc.n = 0;
			c = 1;
		}
		break;

	case(VK_UP):
		if (Gspc.y > 0)
		{
			Gspc.y--;
			if (Gspc.y <= 4)
			{
				Gspc.x = 0;
				Gspc.n = 0;
				Gspc.cx = Gspc.reg[Gspc.y].x;
			}
			Gspc.cy = Gspc.reg[Gspc.y].pLabel->Top;
			c = 1;
		}
		break;

	case(VK_DOWN):
		if (Gspc.y < 7)
		{
			Gspc.y++;
			if (Gspc.y <= 5)
			{
				Gspc.x = 0;
				Gspc.n = 0;
				Gspc.cx = Gspc.reg[Gspc.y].x;
			}
			Gspc.cy = Gspc.reg[Gspc.y].pLabel->Top;
			c = 1;
		}
		break;

	case(VK_END):
		if (Gspc.y <= 4)
		{
			Gspc.n = Gspc.reg[Gspc.y].n;
			c = 1;
		}
		else
		if (Gspc.x < 3)
		{
			Gspc.x = 3;
			Gspc.n = 0;
			c = 1;
		}
		break;

	case(VK_HOME):
		if (Gspc.y <= 4)
		{
			Gspc.n = 0;
			c = 1;
		}
		else
		if (Gspc.x || Gspc.n)
		{
			Gspc.x = 0;
			Gspc.n = 0;
			c = 1;
		}
		break;

	default:
		c = (s8)key | 0x20;

		if (Gspc.y < 4)
		{
			if ((key < '0' || key > '9') && (c < 'a' || c > 'f')) return;

			if (c >= 'a') c -= 7;
			c &= 0xF;

			b = 0;								//Index correct byte for 16-bit registers
			if (Gspc.reg[Gspc.y].n == 3) b = (Gspc.n >> 1) ^ 1;

			if (Gspc.n & 1)						//Change current nybble
				Gspc.pByte[b] = (Gspc.pByte[b] & 0xF0) | c;
			else
				Gspc.pByte[b] = (Gspc.pByte[b] & 0xF) | (c << 4);

			if (Gspc.y == 2)					//X is only 2 character register
				Gspc.reg[Gspc.y].pLabel->Caption = IntToHex(*Gspc.pByte, 2);
			else
				Gspc.reg[Gspc.y].pLabel->Caption = IntToHex(*(u16*)Gspc.pByte, 4);

			if (Gspc.y == 0 || Gspc.y == 3)		//Repaint RAM if PC or SP changes
			{									//*** This should also check the previous value ***
				if (IsVisible(*(u16*)Gspc.pByte)) pbRAM->Repaint();
			}

			Disassembly(reg.pc);

			goto Next;
		}
		else
		if (Gspc.y == 6)
		{
			if ((key < '0' || key > '9') && (c < 'a' || c > 'f')) return;

			if (c >= 'a') c -= 7;
			c &= 0xF;

			if (Gspc.n & 1)
				Apu.SetAPUPort(Gspc.x, (*Gspc.pByte & 0xF0) | c);
			else
				Apu.SetAPUPort(Gspc.x, (*Gspc.pByte & 0xF) | (c << 4));

			lblIn[Gspc.x]->Caption = IntToHex(*Gspc.pByte, 2);
			RepaintCell(0xF4 + Gspc.x);

			goto Next;
		}
		else
		if (Gspc.y == 4)
		{
			switch(c)
			{
			case('n'): FlipPSW(0x80); break;
			case('v'): FlipPSW(0x40); break;
			case('p'): FlipPSW(0x20); break;
			case('b'): FlipPSW(0x10); break;
			case('h'): FlipPSW(0x08); break;
			case('i'): FlipPSW(0x04); break;
			case('z'): FlipPSW(0x02); break;
			case('c'): FlipPSW(0x01); break;
			case(' '): FlipPSW(0x80 >> Gspc.n); break;
			default: return;
			}

			Disassembly(reg.pc);
			c = 1;
		}
	}

	if (c) SelectSPC();
}


s32	TFrmDebug::MsgProcSPC(HWND hwnd, u32 msg, s32 w, s32 l)
{
	switch(msg)
	{
	case(WM_KEYDOWN):
		FrmDebug->KeyDownSPC(w);

	case(WM_KEYUP):
	case(WM_CHAR):
		return 0;

	case(CN_KEYDOWN):
		switch(w)
		{
		case(VK_LEFT):
		case(VK_RIGHT):
		case(VK_UP):
		case(VK_DOWN):
			return 0;
		}
	}

	return CallWindowProc((FARPROC)OldProcSPC, hwnd, msg, w, l);
}


//**************************************************************************************************
void __fastcall TFrmDebug::lblSPCMouseMove(TObject *sender, TShiftState shift, int x, int y)
{
	Gspc.pLabel = (TLabel*)sender;
	Gspc.mx = x;
	Gspc.my = y;
}


//**************************************************************************************************
void __fastcall TFrmDebug::lblSPCMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	u32	i;


	if (ActiveControl != pnlSPC) return;

	Gspc.y = Gspc.pLabel->Tag & 0xFF;
	Gspc.x = Gspc.pLabel->Tag >> 8;
	Gspc.n = (x + 2) / GSize.tw;
	if (Gspc.n > Gspc.reg[Gspc.y].n) Gspc.n = Gspc.reg[Gspc.y].n;

	Gspc.cx = Gspc.reg[Gspc.y].x;
	Gspc.cy = Gspc.pLabel->Top;

	SelectSPC();
}


//**************************************************************************************************
void __fastcall TFrmDebug::lblSPCClick(TObject *sender)
{
	if (ActiveControl != pnlSPC)
	{
		ActiveControl = pnlSPC;
		lblSPCMouseDown(sender, (TMouseButton)0, TShiftState(), Gspc.x, Gspc.y);
	}
}


//**************************************************************************************************
// Flip flag in PSW

void __fastcall TFrmDebug::FlipPSW(u8 mask)
{
	s8	f[9];


	*(u8*)&reg.psw ^= mask;

	f[0] = FlagList[0] ^ (reg.psw.n << 5);
	f[1] = FlagList[1] ^ (reg.psw.v << 5);
	f[2] = FlagList[2] ^ (reg.psw.p << 5);
	f[3] = FlagList[3] ^ (reg.psw.b << 5);
	f[4] = FlagList[4] ^ (reg.psw.h << 5);
	f[5] = FlagList[5] ^ (reg.psw.i << 5);
	f[6] = FlagList[6] ^ (reg.psw.z << 5);
	f[7] = FlagList[7] ^ (reg.psw.c << 5);
	f[8] = 0;

	lblPSW->Caption = f;
}


//**************************************************************************************************
void __fastcall TFrmDebug::SelectSPC()
{
	Gspc.pByte = &Gspc.reg[Gspc.y].pByte[Gspc.x];

	SetCaretPos(Gspc.cx + (Gspc.x * GSize.cw) + (Gspc.n * GSize.tw),
				Gspc.cy);

	if (Gspc.y < 6) Status->Panels->Items[0]->Text = "";

	switch(Gspc.y)
	{
	case(0):
		Status->Panels->Items[1]->Text = "Program Counter";
		break;

	case(1):
		if (Gspc.n < 2)
			Status->Panels->Items[1]->Text = "Y Index";
		else
			Status->Panels->Items[1]->Text = "Accumulator";
		break;

	case(2):
		Status->Panels->Items[1]->Text = "X Index";
		break;

	case(3):
		Status->Panels->Items[1]->Text = "Stack Pointer";
		break;

	case(4):
		Status->Panels->Items[1]->Text = "Program Status Word:  " + AnsiString(FlagName[Gspc.n]) +
										 " = " + AnsiString((*(u8*)&reg.psw >> (7 - Gspc.n)) & 1);
		break;

	case(5):
		if (Gspc.x == 3)
			Status->Panels->Items[1]->Text = "Cycle counter";
		else
		{
			Status->Panels->Items[1]->Text = "Up counter " + AnsiString(Gspc.x);
		}
		break;

	case(6):
		Status->Panels->Items[0]->Text = " " + IntToHex(0xF4 | Gspc.x, 4);
		Status->Panels->Items[1]->Text = "In-port from 65816";
		break;

	case(7):
		Status->Panels->Items[0]->Text = " " + IntToHex(0x2140 | Gspc.x, 4);
		Status->Panels->Items[1]->Text = "Out-port to 65816";
		break;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Random Access Memory

// 16-bit memory address:
// 0000 0000 0000 00 0 0
// +--+ +--+ +--+ ++ + +
// |    |    |    |  | |
// |    |    |    |  | +Byte
// |    |    |    |  +Word
// |    |    |    +Double Word
// |    |    +Paragraph
// |    +Page
// +Memory Page

//**************************************************************************************************
void __fastcall TFrmDebug::pnlRAMEnter(TObject *sender)
{
	CtrlKey = 0;

	CreateCaret(pnlRAM->Handle, NULL, 0, GSize.th);
	ShowCaret(pnlRAM->Handle);
	SetCaretPos((Gram.x * GSize.cw) + GSize.tw + pbRAM->Left,
				(Gram.y * GSize.ch) + pbRAM->Top);
}


//**************************************************************************************************
void __fastcall TFrmDebug::pnlRAMExit(TObject *sender)
{
	DestroyCaret();

	if (Gram.h != 0xFF)
	{
		SetAPURAM((u32)Gram.pByte, (Gram.h << 4) | (*Gram.pByte & 0xF));
		FrmDebug->PrintRAM(Gram.x, Gram.y);
		Gram.h = -1;
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::KeyDownRAM(s32 key)
{
	s8	c;


	c = (s8)key | 0x20;

	if ((key >= '0' && key <= '9') || (c >= 'a' && c <= 'f'))
	{
		if (c >= 'a') c -= 7;
		c &= 0xF;

		if (Gram.n == 0)
		{
			Gram.h = c;
			HideCaret(pnlRAM->Handle);
			pbRAM->Canvas->TextOut(Gram.x * GSize.cw, Gram.y * GSize.ch, Int2Hex(c, 1));
			ShowCaret(pnlRAM->Handle);
		}
		else
		{
			if (Gram.h == 0xFF) Gram.h = *Gram.pByte >> 4;
			SetAPURAM((u32)Gram.pByte, (Gram.h << 4) | c);
			HideCaret(pnlRAM->Handle);
			PrintRAM(Gram.x, Gram.y);
			ShowCaret(pnlRAM->Handle);
			Gram.h = -1;
		}

		goto Next;
	}

	c = 0;
	switch(key)
	{
	case(VK_LEFT):
		if (CtrlKey)
		{
			if (ramOfs)
			{
				ramOfs--;
				pbRAM->Repaint();
				lblRAMYUpdate();
				c = 1;
			}
		}
		else
		if (Gram.n == 1)
		{
			Gram.n = 0;
			c = 2;
		}
		else
		if (Gram.x > 0)
		{
			Gram.x--;
			Gram.n = 1;
			c = 1;
		}
		else
		if (Gram.y > 0)
		{
			Gram.x = Gram.c;
			Gram.y--;
			Gram.n = 1;
			c = 1;
		}
		break;

	case(VK_RIGHT):
		if (CtrlKey)
		{
			if (ramOfs < Gram.c)
			{
				ramOfs++;
				pbRAM->Repaint();
				lblRAMYUpdate();
				c = 1;
			}
			break;
		}
Next:
		if (Gram.n == 0)
		{
			Gram.n = 1;
			c = 2;
		}
		else
		if (Gram.x < Gram.c)
		{
			Gram.x++;
			Gram.n = 0;
			c = 1;
		}
		else
		if (Gram.y < 15)
		{
			Gram.x = 0;
			Gram.y++;
			Gram.n = 0;
			c = 1;
		}
		break;

	case(VK_UP):
		if (Gram.y > 0 && !CtrlKey)
		{
			Gram.y--;
			c = 1;
		}
		else
		if (barRAM->Position)
		{
			barRAM->Position--;
			c = 1;
		}
		break;

	case(VK_DOWN):
		if (Gram.y < 15 && !CtrlKey)
		{
			Gram.y++;
			c = 1;
		}
		else
		if (barRAM->Position < barRAM->Max)
		{
			barRAM->Position++;
			c = 1;
		}
		break;

	case(VK_END):
		if (CtrlKey)
		{
			if (barRAM->Position < barRAM->Max)
			{
				barRAM->Position = barRAM->Max;
				c = 1;
			}
			else
			if (Gram.y < 15 || Gram.x < Gram.c)
			{
				Gram.y = 15;
				Gram.x = Gram.c;
				Gram.n = 0;
				c = 1;
			}
		}
		else
		if (Gram.x < Gram.c)
		{
			Gram.x = Gram.c;
			Gram.n = 0;
			c = 1;
		}
		else
		if (Gram.y < 15)
		{
			Gram.y = 15;
			Gram.n = 0;
			c = 1;
		}
		break;

	case(VK_HOME):
		if (CtrlKey)
		{
			if (barRAM->Position)
			{
				barRAM->Position = 0;
				c = 1;
			}
			else
			if (Gram.y || Gram.x)
			{
				Gram.y = 0;
				Gram.x = 0;
				Gram.n = 0;
				c = 1;
			}
		}
		else
		if (Gram.x || Gram.n)
		{
			c = (Gram.x == 0) + 1;
			Gram.x = 0;
			Gram.n = 0;
		}
		else
		if (Gram.y)
		{
			Gram.y = 0;
			c = 1;
		}
		break;

	case(VK_NEXT):
		if (Gram.y < 15 && !CtrlKey)
		{
			Gram.y = 15;
			c = 1;
		}
		else
		if (barRAM->Position < barRAM->Max)
		{
			barRAM->Position += 16;
			c = 1;
		}
		break;

	case(VK_PRIOR):
		if (Gram.y > 0 && !CtrlKey)
		{
			Gram.y = 0;
			c = 1;
		}
		else
		if (barRAM->Position)
		{
			barRAM->Position -= 16;
			c = 1;
		}
		break;

	case(VK_RETURN):
		if (CtrlKey)
			SetPC((u16)Gram.pByte);
		else
			SetRAMPos(Apu.ram[(u16)Gram.pByte] | (Apu.ram[(u16)((u32)Gram.pByte + 1)] << 8));
		break;

	case(VK_SPACE):
		SetDisasm((u16)Gram.pByte);
		break;

	case(VK_CONTROL):
		CtrlKey = 1;
		break;
	}

	if (c)
	{
		if (Gram.h != 0xFF && c == 1)
		{
			SetAPURAM((u32)Gram.pByte, (Gram.h << 4) | (*Gram.pByte & 0xF));
			HideCaret(pnlRAM->Handle);
			PrintRAM((u8)Gram.pByte & 0xF, (u32)Gram.pByte >> 4);
			ShowCaret(pnlRAM->Handle);
			Gram.h = -1;
		}

		SelectRAM();
	}
}


s32	TFrmDebug::MsgProcRAM(HWND hwnd, u32 msg, s32 w, s32 l)
{
	switch(msg)
	{
	case(WM_KEYDOWN):
		FrmDebug->KeyDownRAM(w);
		return 0;

	case(WM_KEYUP):
	case(WM_CHAR):
		if (w == VK_CONTROL) CtrlKey = 0;
		return 0;

	case(CN_KEYDOWN):							//*** Workaround for odd VCL behaviour ***
		switch(w)								// A TPanel loses its focus when the cursor keys
		{										// are pressed.
		case(VK_LEFT):							// (see TWinControl.CNKeyDown in controls.pas)
		case(VK_RIGHT):
		case(VK_UP):
		case(VK_DOWN):
			return 0;
		}
		break;

	case(WM_MOUSEWHEEL):
		l = (w >> 16) / WHEEL_DELTA;
		if (w & MK_CONTROL) l <<= 4;
		FrmDebug->barRAM->Position = FrmDebug->barRAM->Position - l;
		return 0;
	}

	return CallWindowProc((FARPROC)OldProcRAM, hwnd, msg, w, l);
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbRAMPaint(TObject *sender)
{
	u32	x,y,base;


	base = ramOfs;
	for (y=0; y<16; y++)
		for (x=0; x<Gram.w; x++, base++)
			PrintRAM(x, y);

	pbRAM->Canvas->Font->Color = clSilver;

	if (Gram.h != 0xFF)
		PrintRAM((u16)Gram.pByte % Gram.w, (u16)Gram.pByte / Gram.w);
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbRAMMouseMove(TObject *sender, TShiftState shift, int x, int y)
{
	Gram.mx = x + (GSize.tw >> 1);
	Gram.my = y;

	if (x < Gram.w * GSize.cw)
	{
		y = Gram.my / GSize.ch;
		x = Gram.mx / GSize.cw;
		PMouseByte = &Apu.ram[(u16)(((barRAM->Position + y) * Gram.w) + x + ramOfs)];

		Status->Panels->Items[2]->Text = IntToHex((u16)PMouseByte, 4) + " ";
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbRAMMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	u32	i;


	if (ActiveControl != pnlRAM) return;

	if (PosRAM)
	{
		SetRAMPos(Apu.ram[(u16)Gram.pByte] | (Apu.ram[(u16)((u32)Gram.pByte + 1)] << 8));
		PosRAM = 0;
		return;
	}

	i = Gram.mx + (GSize.tw >> 1);

	if (i < Gram.w * GSize.cw)
	{
		ActiveControl = pnlRAM;

		Gram.y = Gram.my / GSize.ch;
		Gram.x = i / GSize.cw;
		Gram.n = (i % GSize.cw > GSize.tw);

		SelectRAM();
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbRAMClick(TObject *sender)
{
	if (ActiveControl != pnlRAM)
	{
		ActiveControl = pnlRAM;
		pbRAMMouseDown(NULL, (TMouseButton)0, TShiftState(), 0, 0);
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbRAMDblClick(TObject *sender)
{
	PosRAM = 1;
}


//**************************************************************************************************
// Change width of RAM grid

void __fastcall TFrmDebug::lblRAMXDblClick(TObject *sender)
{
	s32		w,i;
	u16		p;
	TRect	rect;


	w = ((TLabel*)sender)->Tag;
	p = (u16)Gram.pByte;

	if (w == 16) ramOfs = 0;
	Gram.c = w - 1;								//Set width of grid
	Gram.w = w;
	barRAM->Max = ((65536 + Gram.c) / w) - 16;	//(64k / width) - 16 lines (bar pos. is first row)
	SetRAMPos(p);

	for (i=1; i<w; i++)
		lblRAMX[i]->Font->Color = clWhite;

	for (; i<16; i++)
		lblRAMX[i]->Font->Color = clGray;

	lblRAMYUpdate();

	if (w < 16)
	{
		rect = pbRAM->ClientRect;
		rect.Left = w * GSize.cw;

		pbRAM->Canvas->FillRect(rect);
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::lblRAMYUpdate()
{
	u32	i,w;


	w = Gram.c + 1;
	for (i=0; i<16; i++)
		lblRAMY[i]->Caption = Int2Hex((barRAM->Position * w) + (i * w) + ramOfs, 4);
}


//**************************************************************************************************
// Keep the cursor on the current byte while the grid is scrolled up or down

void __fastcall TFrmDebug::barRAMScroll(TObject *sender, TScrollCode scroll, int &pos)
{
	if (pos == barRAM->Position) return;

	if (scroll == scLineUp && Gram.y < 15)
	{
		Gram.y++;
		SelectRAM();
	}

	if (scroll == scLineDown && Gram.y)
	{
		Gram.y--;
		SelectRAM();
	}
}


//**************************************************************************************************
// Update RAM grid when scroll bar position changes

void __fastcall TFrmDebug::barRAMChange(TObject *sender)
{
	s32		x,y,w,base;
	b8		temp;


	w = Gram.c + 1;
	base = barRAM->Position * w + ramOfs;

	for (y=0;y<16;y++)
		lblRAMY[y]->Caption = Int2Hex(base + (y * w), 4);

	pbRAM->Repaint();
}


//**************************************************************************************************
void __fastcall TFrmDebug::popRAMPopup(TObject *sender)
{
	itmRAMSetPC->Caption = "PC = " + IntToHex((u16)PMouseByte, 4);
}


//**************************************************************************************************
void __fastcall TFrmDebug::SetDisasm(u16 addr)
{
	AnsiString	line;
	u16			pc,next;
	u32			i;


	lstDisasm->Items->BeginUpdate();
	lstDisasm->Items->Clear();

	pc = addr;
	i = lstDisasm->Height / lstDisasm->ItemHeight;
	while(i--)
	{
		next = Disassemble(line, pc);
		lstDisasm->Items->AddObject(line, (TObject*)pc);
		pc = next;
	}

	lstDisasm->Items->EndUpdate();
}

void __fastcall TFrmDebug::itmRAMDisasmClick(TObject *sender)
{
	SetDisasm((u16)PMouseByte);
}


//**************************************************************************************************
void __fastcall TFrmDebug::SetPC(u16 addr)
{
	u16	old;


	old = reg.pc;
	reg.pc = addr;

	RepaintCell(old);							//Unhighlight current cell pointed to by PC
	RepaintCell(reg.pc);						//Highlight new cell

	SetDisasm(addr);							//Disassemble at new position
}

void __fastcall TFrmDebug::itmRAMSetPCClick(TObject *sender)
{
	SetPC((u16)PMouseByte);
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmRAMBreakClick(TObject *sender)
{
	Watch(PMouseByte);
	RepaintCell((u16)PMouseByte);
}


//**************************************************************************************************
void __fastcall TFrmDebug::SetAPURAM(u16 addr, u8 val)
{
	Apu.SetAPURAM(addr, val);

	switch(addr)
	{
	case(0xF1):
		lblIn0->Caption = Int2Hex(Apu.ram[0xF4], 2);
		lblIn1->Caption = Int2Hex(Apu.ram[0xF5], 2);
		lblIn2->Caption = Int2Hex(Apu.ram[0xF6], 2);
		lblIn3->Caption = Int2Hex(Apu.ram[0xF7], 2);
		RepaintCell(0xF4);
		RepaintCell(0xF5);
		RepaintCell(0xF6);
		RepaintCell(0xF7);

		if ((barRAM->Position + 16) * Gram.w + ramOfs > 0xFFC0) pbRAM->Repaint();
		break;

	case(0xF2):
		pbDSP->Repaint();
		RepaintCell(0xF3);
		break;

	case(0xF3):
		RepaintCell(0x10000 | Apu.ram[0xF2]);
		break;

	case(0xF4):
		lblOut0->Caption = Int2Hex(Apu.GetAPUPort(0), 2);
		break;

	case(0xF5):
		lblOut1->Caption = Int2Hex(Apu.GetAPUPort(1), 2);
		break;

	case(0xF6):
		lblOut2->Caption = Int2Hex(Apu.GetAPUPort(2), 2);
		break;

	case(0xF7):
		lblOut3->Caption = Int2Hex(Apu.GetAPUPort(3), 2);
		break;
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::PrintRAM(u32 x, u32 y)
{
	u8		*p;
	u16		i;
	u32		n,s;
	RGBA	c;
	Source	*pSrc;


	i = ((barRAM->Position + y) * (Gram.c + 1)) + x + ramOfs;	//Get memory position of cell
        if (i>=0xFA&&i<=0xFC)
                p = &Apu.ram[(i - 0xFA) - 6];
        else
	        p = &Apu.ram[i];

	//Determine cell color ---------------------
	if (UseMap)
	{
		c.rgba = MapColor[MemMap[i]];
		goto Draw;
	}

	c.rgba = 0xC0C0C0;							//Default color is gray

	for (n=0; n<watchCnt; n++)
		if (p == watch[n]) break;

	if (n != watchCnt) c.rgba = 0x0000FF;
	else
	if (i == reg.pc) c.rgba = 0xFF0000;			//If this is cell pointer to by PC, color it blue
	else
	if (i == reg.sp) c.rgba = 0x00FF00;			//If this is cell pointed to by SP, color it green
	else
	if (i >= 0xF0 && i <= 0xFF) c.rgba = 0xFFFFFF;	//If cell is a function register, color it white
	else
	{
		if (!(Apu.dsp->flg & 0x20))				//If the cell is in the echo region, color it yellow
		{
			n = (u32)Apu.dsp->esa << 8;
			if (i >= n && i < EdelTab[Apu.dsp->edl] + n)
			{
				c.rgba = 0xC0DFDF;
				goto Draw;
			}
		}

		pSrc = &pState->src[-1];

		for (s=0; s<256; s++)
		{
			pSrc++;

			if ((pSrc->flags & SF_PLAYED) &&
				i >= pSrc->offset && i < (pSrc->offset + (pSrc->len * 9)))
			{
				c.rgba = 0xDFC0DF;

				i -= pSrc->offset;
				if ((i % 9) == 0) c.rgba = 0xFFF0FF;

				i /= 9;
				if (i == 0) c.r = 0xC0;
				else
				if (i == (pSrc->len - 1)) c.b &= 0xC0;

				break;
			}
		}
	}

Draw:
	pbRAM->Canvas->Font->Color = (TColor)c.rgba;
	pbRAM->Canvas->TextOut(x * GSize.cw, y * GSize.ch, Int2Hex((u32)*p, 2));
}


//**************************************************************************************************
// Is a Byte Visible in the RAM View?

b8 __fastcall TFrmDebug::IsVisible(u16 i)
{
	u16	s,e;


	s = (barRAM->Position * Gram.w) + ramOfs;
	e = s + (Gram.w * 16);

	if (i >= s && i < e) return 1;

	return 0;
}


//**************************************************************************************************
// Set the upper-left byte to the location indexed by 'p'

void __fastcall TFrmDebug::SetRAMPos(u32 p)
{
	u32	b,w;


	w = Gram.w;
	Gram.pByte = &Apu.ram[p];
//	ramOfs = 0;
//	if (w != 16)
//	{
//		ramOfs = p % w;
//		p -= ramOfs;
//	}
//
//	Gram.x = p % w;
//	Gram.y = (p % (w << 4)) / w;
//	barRAM->Position = (p / (w << 4)) << 4;

	if (w == 16)
	{
		p = (u16)(p - ramOfs);
		b = (p >> 8) << 4;
		Gram.y = (u8)p >> 4;
		Gram.x = p & 0xF;
	}
	else
	{
		if (p <= 0x10000 + Gram.c - (16 * w))
		{
			ramOfs = p % w;
			p -= ramOfs;
			b = p / w;
			Gram.y = 0;
			Gram.x = 0;
		}
		else
		{
			ramOfs = 0;
			b = barRAM->Max;
			p -= (b * w);
			Gram.y = p / w;
			Gram.x = p % w;
		}
	}
	Gram.n = 0;

	if (b != barRAM->Position) barRAM->Position = b;
	else barRAMChange(NULL);

	ActiveControl = pnlRAM;

	SelectRAM();
}


//**************************************************************************************************
void __fastcall TFrmDebug::SelectRAM()
{
	AnsiString	desc;
	s32			index;
	s32			i;


	Gram.pByte = &Apu.ram[(u16)(((barRAM->Position + Gram.y) * (Gram.c + 1)) + Gram.x + ramOfs)];

	SetCaretPos((Gram.x * GSize.cw) + (Gram.n * GSize.tw) + pbRAM->Left,
				(Gram.y * GSize.ch) + pbRAM->Top);

	index = (u16)Gram.pByte;
	Status->Panels->Items[0]->Text = " " + IntToHex(index, 4);

	if (index < 0xF0)
		Status->Panels->Items[1]->Text = " Direct Page 0";
	else
	if (index < 0x100)
	{
		index &= 0xF;
		desc = FRegName[index];
		switch(index)
		{
		case(0x1):
			desc += "IPL ROM ";
			if (*Gram.pByte & 0x80)
				desc += "enabled, ";
			else
				desc += "disabled, ";

			desc += "Timers enabled ";
			if (*Gram.pByte & 7)
			{
				if (*Gram.pByte & 1) desc += "0 ";
				if (*Gram.pByte & 2) desc += "1 ";
				if (*Gram.pByte & 4) desc += "2 ";
			}
			else
				desc += "(none)";
			break;

		case(0x4):
		case(0x5):
		case(0x6):
		case(0x7):
			desc += "in ";
			desc += IntToHex(*Gram.pByte, 2);
			desc += " / out ";
			desc += IntToHex((s32)Apu.GetAPUPort(index), 2);
			break;

		case(0xA):
		case(0xB):
			//i = *Gram.pByte;
                        i = Apu.ram[(index-0xA)-6];
			if (!i) i = 0x100;
			desc += IntToStr(8000 / i);
			desc += "Hz";
			break;

		case(0xC):
			//i = *Gram.pByte;
                        i = Apu.ram[-4];
			if (!i) i = 0x100;
			desc += IntToStr(64000 / i);
			desc += "Hz";
			break;
		}

		Status->Panels->Items[1]->Text = desc;
	}
	else
	if (index < 0x200)
	{
		Status->Panels->Items[1]->Text = " Direct Page 1 (stack)";
	}
	else
	if (index >= 0xFF00)
	{
		desc = " Upper Page";

		if (index >= 0xFFC0)
		{
			if (Apu.ram[0xF1] & 0x80)
			{
				desc = "IPL ROM [";
				desc += IntToHex(index - 0xFFC0, 2);
				desc += "]";
			}
			else
			if (index < 0xFFE0)
			{
				desc = "TCALL vector ";
				desc += IntToHex((0xFFDF - index) >> 1, 1);
			}
		}

		Status->Panels->Items[1]->Text = desc;
	}
	else
	{
		if ((Apu.dsp->flg & FLG_ECEN) == 0 &&
			index >= (Apu.dsp->esa << 8) &&
			index < ((Apu.dsp->esa << 8) + EdelTab[Apu.dsp->edl]))
		{
			desc = " Echo Area [";
			desc += IntToStr((index - (Apu.dsp->esa << 8)) >> 2);
			desc += "]";
			Status->Panels->Items[1]->Text = desc;
		}
		else
		if ((index >> 8) >= Apu.dsp->dir &&
			(index >> 8) <= (Apu.dsp->dir + 3))
		{
			desc = " Source ";
			desc += IntToStr((index - (Apu.dsp->dir << 8)) >> 2);
			if (index & 2)
				desc += " (loop)";
			else
				desc += " (start)";

			Status->Panels->Items[1]->Text = desc;
		}
		else
			Status->Panels->Items[1]->Text = "";
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// DSP Registers


//**************************************************************************************************
void __fastcall TFrmDebug::pnlDSPEnter(TObject *sender)
{
	CtrlKey = 0;

	CreateCaret(pnlDSP->Handle, NULL, 0, GSize.th);
	ShowCaret(pnlDSP->Handle);
	SetCaretPos((Gdsp.x * GSize.cw) + GSize.tw + pbDSP->Left,
				(Gdsp.y * GSize.ch) + pbDSP->Top);
}


//**************************************************************************************************
void __fastcall TFrmDebug::pnlDSPExit(TObject *sender)
{
	DestroyCaret();

	if (Gdsp.h != 0xFF)
	{
		SetDSPReg((u32)Gdsp.pByte, (Gdsp.h << 4) | (*Gdsp.pByte & 0xF));
		FrmDebug->PrintDSP(Gdsp.y, Gdsp.x);
		Gdsp.h = -1;
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::KeyDownDSP(s32 key)
{
	s8	c;


	c = (s8)key | 0x20;

	if ((key >= '0' && key <= '9') || (c >= 'a' && c <= 'f'))
	{
		if (c >= 'a') c -= 0x57;
		else c = key & 0xF;

		if (Gdsp.n == 0)
		{
			Gdsp.h = c;
			HideCaret(pnlDSP->Handle);
			pbDSP->Canvas->TextOut(Gdsp.x * GSize.cw, Gdsp.y * GSize.ch, Int2Hex(c, 1));
			ShowCaret(pnlDSP->Handle);
		}
		else
		{
			if (Gdsp.h == 0xFF) Gdsp.h = *Gdsp.pByte >> 4;
			SetDSPReg((u32)Gdsp.pByte, (Gdsp.h << 4) | c);
			HideCaret(pnlDSP->Handle);
			PrintDSP(Gdsp.y, Gdsp.x);
			ShowCaret(pnlDSP->Handle);
			Gdsp.h = -1;
		}


		if (Gdsp.n == 0)
		{
			Gdsp.n = 1;
			SetCaretPos((Gdsp.x * GSize.cw) + GSize.tw + pbDSP->Left,
						(Gdsp.y * GSize.ch) + pbDSP->Top);
		}
		else
		if (Gdsp.x < 7)
		{
			Gdsp.x++;
			Gdsp.n = 0;
			SelectDSP();
		}

		return;
	}

	c = 0;
	switch(key)
	{
	case(VK_LEFT):
		if (Gdsp.n == 1)
		{
			Gdsp.n = 0;
			c = 1;
		}
		else
		if (Gdsp.x > 0)
		{
			Gdsp.x--;
			Gdsp.n = 1;
			c = 1;
		}
		break;

	case(VK_RIGHT):
		if (Gdsp.n == 0)
		{
			Gdsp.n = 1;
			c = 1;
		}
		else
		if (Gdsp.x < 7)
		{
			Gdsp.x++;
			Gdsp.n = 0;
			c = 1;
		}
		break;

	case(VK_UP):
		if (Gdsp.y > 0)
		{
			Gdsp.y--;
			c = 1;
		}
		break;

	case(VK_DOWN):
		if (Gdsp.y < 15)
		{
			Gdsp.y++;
			c = 1;
		}
		break;

	case(VK_END):
		if (Gdsp.x < 7)
		{
			Gdsp.x = 7;
			Gdsp.n = 0;
			c = 1;
		}
		break;

	case(VK_HOME):
		if (Gdsp.x > 0)
		{
			Gdsp.x = 0;
			Gdsp.n = 0;
			c = 1;
		}
		break;

	case(VK_NEXT):
		if (Gdsp.y < 15)
		{
			Gdsp.y = 15;
			c = 1;
		}
		break;

	case(VK_PRIOR):
		if (Gdsp.y > 0)
		{
			Gdsp.y = 0;
			c = 1;
		}
		break;

	case(VK_RETURN):
		if (Gdsp.y == 4)
		{
			SetRAMPos(((u32)Apu.dsp->dir << 8) + ((u32)*Gdsp.pByte << 2));
		}
		else
		if ((u8)Gdsp.pByte == 0x5D || (u8)Gdsp.pByte == 0x6D)
		{
			SetRAMPos((u32)*Gdsp.pByte << 8);
		}
		break;
	}

	if (c)
	{
		if (Gdsp.h != 0xFF)
		{
			SetDSPReg((u32)Gdsp.pByte, (Gdsp.h << 4) | (*Gdsp.pByte & 0xF));
			HideCaret(pnlDSP->Handle);
			PrintDSP((u8)Gdsp.pByte & 0xF, (u32)Gdsp.pByte >> 4);
			ShowCaret(pnlDSP->Handle);
			Gdsp.h = -1;
		}

		SelectDSP();
	}
}


s32	TFrmDebug::MsgProcDSP(HWND hwnd, u32 msg, s32 w, s32 l)
{
	switch(msg)
	{
	case(WM_KEYDOWN):
		FrmDebug->KeyDownDSP(w);

	case(WM_KEYUP):
	case(WM_CHAR):
		return 0;

	case(CN_KEYDOWN):
		switch(w)
		{
		case(VK_LEFT):
		case(VK_RIGHT):
		case(VK_UP):
		case(VK_DOWN):
			return 0;
		}
	}

	return CallWindowProc((FARPROC)OldProcDSP, hwnd, msg, w, l);
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbDSPPaint(TObject *sender)
{
	int	r,v,base;


	for (base=0,v=0;v<8;v++)
		for (r=0;r<16;r++,base++)
			PrintDSP(r, v);

	pbDSP->Canvas->Font->Color = clSilver;

	if (Gdsp.h != 0xFF)
		PrintDSP((u8)Gdsp.pByte & 0xF, (u8)Gdsp.pByte >> 4);
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbDSPMouseMove(TObject *sender, TShiftState shift, int x, int y)
{
	Gdsp.mx = x + (GSize.tw >> 1);
	Gdsp.my = y;

	y = Gdsp.my / GSize.ch;
	x = Gdsp.mx / GSize.cw;
	PMouseByte = &Apu.dsp->reg[(x << 4) | y];
	Status->Panels->Items[2]->Text = IntToHex((u8)PMouseByte, 2) + " ";
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbDSPMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	if (ActiveControl != pnlDSP) return;

	Gdsp.y = Gdsp.my / GSize.ch;
	Gdsp.x = Gdsp.mx / GSize.cw;
	Gdsp.n = (Gdsp.mx % GSize.cw > GSize.tw);

	SelectDSP();
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbDSPClick(TObject *sender)
{
	if (ActiveControl != pnlDSP)
	{
		ActiveControl = pnlDSP;
		pbDSPMouseDown(NULL, (TMouseButton)0, TShiftState(), 0, 0);
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::pbDSPDblClick(TObject *sender)
{
	if (Gdsp.y == 4)
	{
		SetRAMPos(((u32)Apu.dsp->dir << 8) + ((u32)*Gdsp.pByte << 2));
	}
	else
	if ((u8)Gdsp.pByte == 0x5D || (u8)Gdsp.pByte == 0x6D)
	{
		SetRAMPos((u32)*Gdsp.pByte << 8);
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::SetDSPReg(u8 reg, u8 val)
{
	if (Apu.ram[0xF2] == reg)
	{
		Apu.SetAPURAM(0xF3, val);
		RepaintCell(0xF3);
	}
	else
		Apu.SetDSPReg(reg, val);
}


//**************************************************************************************************
void __fastcall TFrmDebug::PrintDSP(u8 r, u8 v)
{
	u32	n,reg;
	u8 	*p;


	reg = (v << 4) | r;
	p = &Apu.dsp->reg[reg];

	//Select color -----------------------------
	for (n=0; n<watchCnt; n++)					//Look for watch point
		if (p == watch[n]) break;

	if (n != watchCnt) pbDSP->Canvas->Font->Color = clRed;
	else
	if (reg == Apu.ram[0xF2]) pbDSP->Canvas->Font->Color = clYellow;
	else
	if (r == 0xA || r == 0xB || r == 0xE || reg == 0x1D) pbDSP->Canvas->Font->Color = clGray;
	else
		pbDSP->Canvas->Font->Color = clSilver;

	//Draw text --------------------------------
	pbDSP->Canvas->TextOut((u8)v * GSize.cw, r * GSize.ch, Int2Hex((u32)*p, 2));
}


//**************************************************************************************************
// Update status bar with description of currently selected DSP register

void __fastcall TFrmDebug::SelectDSP()
{
	AnsiString	desc;
	s8			*p;
	u32			i,j;



	Gdsp.pByte = &Apu.dsp->reg[(Gdsp.x << 4) | Gdsp.y];

	SetCaretPos((Gdsp.x * GSize.cw) + (Gdsp.n * GSize.tw) + pbDSP->Left,
				(Gdsp.y * GSize.ch) + pbDSP->Top);

	Status->Panels->Items[0]->Text = " " + IntToHex((s8)Gdsp.pByte, 2);

	p = Gdsp.pByte;
	if (Gdsp.y < 0xA)
	{
		desc = " [";
		desc += Gdsp.x;
		desc += "] ";
		switch(Gdsp.y)
		{
		case(0):
			desc += "VOL(L):  " + AnsiString((s32)*p);
			break;

		case(1):
			desc += "VOL(R):  " + AnsiString((s32)*p);
			break;

		case(2):
			desc += "P(L):  " + AnsiString((*(u16*)p * 32000) >> 12) + "Hz";
			break;

		case(3):
			p--;
			desc += "P(H):  " + AnsiString((*(u16*)p * 32000) >> 12) + "Hz";
			break;

		case(4):
			desc += "SRCN:  " + AnsiString((s32)*p);
			break;

		case(5):
			desc += "ADSR(1):  ";
			goto Env;

		case(6):
			desc += "ADSR(2):  ";
			p--;
Env:		desc += (*p & 0x80) ? "enabled  " : "disabled  ";
			break;

		case(7):
			desc += "GAIN:  ";
			desc += (p[-2] & 0x80) ? "disabled  " : "enabled  ";

			if (*p & 0x80)
			{
				switch((*p & 0x60) >> 5)
				{
				case(0):
					desc += "-Lin";
					break;

				case(1):
					desc += "-Exp";
					break;

				case(2):
					desc += "+Lin";
					break;

				case(3):
					desc += "+Bent";
					break;
				}
			}
			else
				desc += AnsiString((s32)*p);
			break;

		case(8):
			desc += "ENVX:  " + AnsiString((s32)*p);
			break;

		case(9):
			desc += "OUTX:  " + AnsiString((s32)*p << 8);
			break;
		}
		Status->Panels->Items[1]->Text = desc;
	}
	else
	switch(Gdsp.y)
	{
	case(0xC):
		desc = RegCName[Gdsp.x];

		if (Gdsp.x < 4)		//MVOL or EVOL
		{
			desc += AnsiString((s32)*p);
		}
		else
		if (Gdsp.x == 6)	//FLG
		{
			if (*p & 0x80) desc += "RES, ";
			desc += "MUTE ";
			desc += ((*p & 0x40) ? "on, " : "off, ");
			desc += "ECEN ";
			desc += ((*p & 0x20) ? "disabled, " : "enabled, ");
			desc += "NCK ";
			desc += NClk[*p & 0x1F];
		}
		else				//KON KOF ENDX
		{
			if (*p)
			{
				i = 0;
				j = *(u8*)p;
				while(j)
				{
					if (j & 1) desc += AnsiString(i) + " ";
					i++;
					j >>= 1;
				}
			}
			else
				desc += "no flags";
		}

		Status->Panels->Items[1]->Text = desc;
		break;

	case(0xD):
		desc = RegDName[Gdsp.x];
		j = *(u8*)p;

		switch(Gdsp.x)
		{
		case(0):	//EFB
			desc += AnsiString((*p * 100) >> 7) + "%";
			break;

		case(2):	//PMON
			j &= ~1;							//Clear first flag

		case(3):	//NON
		case(4):	//EON
			if (j)
			{
				i = 0;
				while(j)
				{
					if (j & 1) desc += AnsiString(i) + " ";
					i++;
					j >>= 1;
				}
			}
			else
				desc += "no flags";
			break;

		case(5):	//DIR
		case(6):	//ESA
			desc += IntToHex((s32)(*(u8*)p) << 8, 2) + "h";
			break;

		case(7):	//EDL
			desc += AnsiString((*p & 0xF) << 4) + "ms";
			break;
		}

		Status->Panels->Items[1]->Text = desc;
		break;

	case(0xF):	//FIR filter
		Status->Panels->Items[1]->Text = " C" + AnsiString(Gdsp.x) + ":  " + AnsiString((s32)*p);
		break;

	default:
		Status->Panels->Items[1]->Text = " Not used";
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Breakpoints and Watches


//**************************************************************************************************
void __fastcall TFrmDebug::lstBrkAddrDblClick(TObject *sender)
{
	u32	i;
	u16	pc;


	for (i=0; i<lstBrkAddr->Items->Count; i++)
		if (lstBrkAddr->Selected[i]) break;

	pc = (u16)lstBrkAddr->Items->Objects[i];
	Disassembly(pc);

	for (i=0; i<lstDisasm->Items->Count; i++)
		if (pc == (u16)lstDisasm->Items->Objects[i]) break;

	SendMessage(lstDisasm->Handle, LB_SETCURSEL, i, 0);
	SendMessage(lstDisasm->Handle, LB_SETCARETINDEX, i, 0);

	ActiveControl = lstDisasm;
}


//**************************************************************************************************
void __fastcall TFrmDebug::lstBrkDataDblClick(TObject *sender)
{
	u32	i;


	for (i=0; i<lstBrkData->Items->Count; i++)
		if (lstBrkData->Selected[i]) break;

	i = (u32)lstBrkData->Items->Objects[i];

	if ((i & ~0xFFFF) == (u32)Apu.ram)
	{
		SetRAMPos((u16)i);
		ActiveControl = pnlRAM;
	}
	else
	if ((i & ~0xFF) == (u32)Apu.dsp)
	{
		i &= 0x7F;
		Gdsp.n = 0;
		Gdsp.x = i >> 4;
		Gdsp.y = i & 0xF;
		ActiveControl = pnlDSP;
		SelectDSP();
	}
	else
	{
		i &= 3;
		Gspc.n = 0;
		Gspc.x = i;
		Gspc.y = 7;
		ActiveControl = pnlSPC;
		SelectSPC();
	}
}


//**************************************************************************************************
// Place a Watch on a Byte
//
// Execution will break if the value pointed at changes

void __fastcall TFrmDebug::Watch(u8 *p)
{
	u32	i;


	//Search for existing watch ----------------
	for (i=0; i<watchCnt; i++)
		if (p == (u8*)lstBrkData->Items->Objects[i]) break;

	//Add or remove watch ----------------------
	if (i == watchCnt)							//If watch wasn't found, add new watch
	{
		if (watchCnt == 21)						//If list is full, no more watches can be added
		{
			MessageDlg("You can only have 21 watches.  Remove some watches before adding a new one.",
					   mtError, TMsgDlgButtons() << mbOK, 0);
			return;
		}

		if (((u32)p & ~0xFFFF) == (u32)Apu.ram)
			lstBrkData->Items->AddObject(Int2Hex((u16)p, 4), (TObject*)p);
		else
		if (((u32)p & ~0xFF) == (u32)Apu.dsp)
			lstBrkData->Items->AddObject("  " + IntToHex((u8)p, 2), (TObject*)p);
		else
		{
			lstBrkData->Items->AddObject("   " + AnsiString((s32)p & 3), (TObject*)p);
			lblOut[(s32)p & 3]->Font->Color = clRed;
		}

		wbyte[watchCnt] = *p;
		watch[watchCnt++] = p;
	}
	else
	{
//		if (((u32)p & ~3) == (u32)Apu.outPort)
//			lblOut[(s32)p & 3]->Font->Color = clSilver;

		lstBrkData->Items->Delete(i);				//Delete watch in list view

		for (i=0; i<watchCnt; i++)				//Search for pointer in internal list
			if (p == watch[i]) break;

		watchCnt--;
		memcpy(&wbyte[i], &wbyte[i+1], (watchCnt - i));
		memcpy(&watch[i], &watch[i+1], (watchCnt - i) * 4);
	}
}


//**************************************************************************************************
void __fastcall TFrmDebug::itmBreakClick(TObject *sender)
{
	TLabel	*pLabel;


	Watch(PMouseByte);

	if (((u32)PMouseByte & ~0xFF) == (u32)Apu.dsp)
	{
		RepaintCell((u8)PMouseByte | 0x10000);
	}
	else
	{
		switch((u8)PMouseByte & 3)
		{
		case(0):
			pLabel = lblOut0;
			break;

		case(1):
			pLabel = lblOut1;
			break;

		case(2):
			pLabel = lblOut2;
			break;

		case(3):
			pLabel = lblOut3;
			break;

		}

		if (pLabel->Font->Color == clRed)
			pLabel->Font->Color = clSilver;
		else
			pLabel->Font->Color = clRed;
	}
}
void __fastcall TFrmDebug::Button1Click(TObject *Sender)
{
        pbRAMPaint(Sender);
        pbDSPPaint(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFrmDebug::Timer1Timer(TObject *Sender)
{
        u32	i,j;
	u8	*pNew,*pOld,r,c;
	s8	str[9];

	static	u16	lastPC,lastSP;
	static	u8	lastD;

        Timer1->Enabled = False;
//        pbRAMPaint(Sender);
//        pbDSPPaint(Sender);


        //This code was crashing in the emulation thread.
        //Now a timer handles it.
        //Update memory views ----------------------
	pNew = &Apu.ram[barRAM->Position * Gram.w + ramOfs];
	pOld = oldRAM;
	for (r=0; r<16; r++)
		for (c=0; c<Gram.w; c++)
		{
       			if (*pNew++ != *pOld++) PrintRAM(c, r);
		}

	pNew = Apu.dsp->reg;
	pOld = oldReg;
	for (c=0; c<8; c++)
		for (r=0; r<16; r++)
		{
       			if (*pNew++ != *pOld++) PrintDSP(r, c);
		}

       	RepaintCell(lastPC);
       	RepaintCell(reg.pc);
       	RepaintCell(lastSP);
       	RepaintCell(reg.sp);
	lastPC = reg.pc;
	lastSP = reg.sp;

      	RepaintCell(0x10000 | lastD);
      	RepaintCell(0x10000 | Apu.ram[0xF2]);
	lastD = Apu.ram[0xF2];
}
//---------------------------------------------------------------------------

