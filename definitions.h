#ifndef _definitions_h
#define _definitions_h

// #include "vm.c"
// #include "lexicalAnalyzer.c"
// #include "parserandcode.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// for the lex 
#define MAX_NUMBER_LENGTH 5
#define MAX_IDENTIFIER_LENGTH 11
#define MAX_CODE_LENGTH 32768

// int nulsym = 1, identsym = 2, numbersym = 3, plussym = 4,
// minussym = 5, multsym = 6, slashsym = 7, oddsym = 8, eqlsym = 9,
// neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14,
// lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
// periodsym = 19, becomesym = 20, beginsym = 21, endsym = 22, ifsym = 23,
// thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
// varsym = 29, writesym = 31, readsym = 32;

// typedef enum {
//     nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym
// } token_type;

int reservedIndex(char * identifier);
int mapReserved(int spotInReserved);
int mapSymbol(char * symbol);
int isUpper(char theChar);
int isLower(char theChar);
int isAlpha(char theChar);
int isDigit(char theChar);
int isAlphanumeric(char theChar);
int isInvisible(char theChar);
int isSymbol(char theChar);
int isValid(char theChar);
void throwError(char * message);
char getChar(int ignoreValidity);
void ungetChar();
void clearBuffer();
void addToBuffer(char theChar);
void openFiles(char * inputFile);
void clearLexemeOutput();
void insertToLexemeTable(char * lexeme, int tokenType);
void insertIntToLexemeList(int num);
void insertStrToLexemeList(char * identifier);
void processIdentifier(char * identifier);
void processNumber(char * num);
void processSymbol(char * sym);
void processText();
void echoInput();
void lex(char * argv);

//from parserandcode.c
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym
} token_type;

typedef enum{
	LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO
} opcode;

typedef enum{
	NEG = 1, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
} ALUop;

typedef struct symbol{
	int type;
	char name[12];
	int val;
	int level;
	int addr;
}symbol;

typedef struct code{
	int op;
	int l;
	int m;
}code;

#define CODE_SIZE 500
#define SYMBOL_TABLE_SIZE 100

void parse(void);
void openFile(char ifile[]);
void emit(int op, int l, int m);
void getNextToken();
void error(int code);
void createConstSym();
void createVarSym();
void createProcSym();
void block();
void constDec();
int varDec();
void procDec();
void statement();
void condition();
void expression();
void term();
void factor();
int find(char* ident);
int symbolType(int pos);
int symbolLevel(int pos);
int symbolAddress(int pos);
int symbolVal(int pos);
void outputMachineCode();

//for vm.c
typedef struct instruction
{
    int op;
    int l;
    int m;
    char oper[3];
}instruction;

int base(int l,int base);
int converttoint (char instr[]);
char *converttoword (int number );
void doinstruct(int op, int l, int m, FILE *out, FILE *vmout);
void vm(void);

#endif
