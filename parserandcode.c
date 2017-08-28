#include "definitions.h"


char currentIdentifier[12];
int currentNumber;

int idFlag = 0;
token_type currentToken = 1;
int codeIndex = 0;
int symbolIndex = 0;
int currentLevel = -1;
int procCount = 0;
symbol symbolTable[SYMBOL_TABLE_SIZE];
code codeStore[CODE_SIZE];
FILE* inputStream;

void parse(void){
	openFile("lexout.txt");
	getNextToken();
	block();
	if(currentToken != periodsym)
		error(9);
	emit(SIO, 0, 3);
	fclose(inputStream);
	outputMachineCode();
}
void openFile(char ifile[]){
	if((inputStream = fopen(ifile, "r")) == NULL) {
        printf("Unable to locate/open input file\n\nExiting program\n");
        exit(EXIT_FAILURE);
    }
}
void emit(int op, int l, int m){

	if(codeIndex > CODE_SIZE)
		error(25);
	if(op == SIO){
		if(m == 1)
			op = 9;
		else if(m == 2)
			op = 9;
		else if(m == 3)
			op = 9;
		else
		{
			error(100);
		}
	}
	codeStore[codeIndex].op = op; // opcode
	codeStore[codeIndex].l = l; // lex level
	codeStore[codeIndex].m = m; // modifier
	codeIndex++;
}
void getNextToken(){
	fscanf(inputStream, "%d", &currentToken);
	// printf("%d \n", currentToken);
	if(currentToken == identsym){
		fscanf(inputStream, " %s", currentIdentifier);
	}
	else if(currentToken == numbersym)
		fscanf(inputStream, "%d", &currentNumber);
}

void error(int code){
	printf("ERROR: ");
	switch(code){
		case 1:
			printf("Use '='' instead of ':='\n");
			break;
		case 2:
			printf("'=' must be followed by a number\n");
			break;
		case 3:
			printf("Indentifier must be followed by ':='\n");
			break;
		case 4:
			printf("Const/Var/Procedure must be followed by an identifer\n");
			break;
		case 5:
			printf("Semicolon or comma missing\n");
			break;
		case 6:
			printf("Incorrect symbol after procedure declaration\n");
			break;
		case 7:
			printf("Statement expected\n");
			break;
		case 8:
			printf("Incorrect symbol after statement part in block\n");
			break;
		case 9:
			printf("Period expected\n");
			break;
		case 10:
			printf("Semicolon between statements missing\n");
			break;
		case 11:
			printf("Undeclared identifier: %s\n", currentIdentifier);
			break;
		case 12:
			printf("Assignment to constant or procedure is not allowed\n");
			break;
		case 13:
			printf("Assignment operator expected\n");
			break;
		case 14:
			printf("'call' must be followed by an identifier\n");
			break;
		case 15:
			printf("Call of a constant or variable is meaningless\n");
			break;
		case 16:
			printf("'then' expected\n");
			break;
		case 17:
			printf("Semicolon or '}' expected\n");
			break;
		case 18:
			printf("'do' expected\n");
			break;
		case 19:
			printf("Incorrect symbol following statement\n");
			break;
		case 20:
			printf("Relational operator expected\n");
			break;
		case 21:
			printf("Expression must not contain a procedure identifier\n");
			break;
		case 22:
			printf("Right parenthesis missing\n");
			break;
		case 23:
			printf("The preceding factor cannot begin with this symbol\n");
			break;
		case 24:
			printf("An expression cannot begin with this symbol\n");
			break;
		case 25:
			printf("This number is too large\n");
			break;
		case 26:
			printf("case not found");
			break;
		default:
			printf("Indeterminate error!\n");
			break;
	}
	printf("Exiting.\n");
	exit(EXIT_FAILURE);
}
void createConstSym(){
	symbol sym;
	sym.type = 1;
	strcpy(sym.name, currentIdentifier);
	sym.val = currentNumber;
	sym.level = currentLevel;
	symbolTable[symbolIndex++] = sym;
}
void createVarSym(){
	symbol sym;
	sym.type = 2;
	strcpy(sym.name, currentIdentifier);
	sym.level = currentLevel;
	symbolTable[symbolIndex++] = sym;
}
void createProcSym(){
	symbol sym;
	sym.type = 3;
	strcpy(sym.name, currentIdentifier);
	sym.level = currentLevel;
	sym.addr = codeIndex;
	symbolTable[symbolIndex++] = sym;
}

void block(){
	currentLevel++;
	int space = 4;
	int jmpAddr = codeIndex;
	emit(JMP, 0, 0);
	if(currentToken == constsym)
		constDec();
	if(currentToken == varsym)
		space += varDec();
	if(currentToken == procsym)
		procDec();
	codeStore[jmpAddr].m = codeIndex;
	emit(INC, 0, space); //allocate enough space for locals and AR info
	statement();
	if(currentLevel != 0) //dont want return command from Program block
		emit(OPR, 0, 0); //return
	currentLevel--;
}

void constDec(){
	do{
		getNextToken();
		if(currentToken != identsym) error(4);
		getNextToken();
		if(currentToken != eqsym) error(13);
		getNextToken();
		if(currentToken != numbersym) error(2);
		createConstSym();
		getNextToken();
	} while(currentToken == commasym);

	if(currentToken != semicolonsym) error(5);
	getNextToken();
}

int varDec(){
	int varCount = 0;
	do{
		getNextToken();
		if(currentToken != identsym) error(4);
		createVarSym();
		symbolTable[symbolIndex - 1].addr = 4 + varCount;
		varCount++;
		getNextToken();
	} while(currentToken == commasym);

	if(currentToken != semicolonsym) error(5);
	getNextToken();
	return varCount;
}

void procDec(){
	while(currentToken == procsym){
		getNextToken();
		if(currentToken != identsym) error(4);
		createProcSym();
		getNextToken();
		if(currentToken != semicolonsym) error(5);
		getNextToken();
		block();
		if(currentToken != semicolonsym) error(5);
		getNextToken();
	}
}

void statement(){
	if(currentToken == identsym){
		int pos = find(currentIdentifier);
		if(pos == -1) error(11);
		if(symbolTable[pos].type != 2) error(12);
		getNextToken();
		if(currentToken != becomessym) error(3);
		getNextToken();
		expression();
		emit(STO, currentLevel - symbolLevel(pos), symbolAddress(pos));
	}
	else if(currentToken == callsym){
		getNextToken();
		if(currentToken != identsym) error(14);
		int pos = find(currentIdentifier);
		if(pos == -1) error(11);
		if(symbolType(pos) == 3)
			emit(CAL, currentLevel - symbolLevel(pos), symbolAddress(pos));
		else error(14);
		getNextToken();
	}
	else if(currentToken == beginsym){
		getNextToken();
		statement();
		while(currentToken == semicolonsym){
			getNextToken();
			statement();
		}
		if(currentToken != endsym) 
		{
			error(26);
		}
		getNextToken();
	}
	else if(currentToken == ifsym){
		getNextToken();
		condition();
		if(currentToken != thensym) error(16);
		getNextToken();
		int temp = codeIndex;
		emit(JPC, 0, 0);
		statement();
		if(currentToken == elsesym){
			getNextToken();
			int temp2 = codeIndex;
			emit(JMP, 0, 0);
			codeStore[temp].m = codeIndex;
			statement();
			codeStore[temp2].m = codeIndex;
		}
		else{
			emit(JMP, 0, codeIndex + 1);
			codeStore[temp].m = codeIndex;
		}
	}
	else if(currentToken == whilesym){
		int start = codeIndex;
		getNextToken();
		condition();
		int temp = codeIndex;
		emit(JPC, 0, 0);
		if (currentToken != dosym)
			error(18);
		getNextToken();
		statement();
		emit(JMP, 0, start);
		codeStore[temp].m = codeIndex;
	}
	else if(currentToken == readsym){
		getNextToken();
		if(currentToken != identsym) error(29);
		getNextToken();
		int pos = find(currentIdentifier);
		if(pos == -1) error(11);
		if(symbolType(pos) != 2) error(12);
		emit(SIO, 0, 2);
		emit(STO, currentLevel - symbolLevel(pos), symbolAddress(pos));
	}
	else if(currentToken == writesym){
		getNextToken();
		if(currentToken != identsym) error(29);
		getNextToken();
		int pos = find(currentIdentifier);
		if(pos == -1) error(11);
		if(symbolType(pos) != 2) error(12);
		emit(LOD, currentLevel - symbolLevel(pos), symbolAddress(pos));
		emit(SIO, 0, 1);
	}
}
void condition(){
    if(currentToken == oddsym){
        getNextToken();
        expression();
        emit(OPR, 0, ODD);
    }
    else{
        expression();
        int relOp = currentToken - 1;
        if(currentToken != eqsym && currentToken != neqsym && currentToken != gtrsym && currentToken != geqsym
           && currentToken != lessym && currentToken != leqsym){
            error(20);
        }
        getNextToken();
        expression();
        emit(OPR, 0, relOp);
    }
}
void expression(){
	int addingOp;
  if (currentToken == plussym || currentToken == minussym){
      addingOp = currentToken;
      getNextToken();
      term();
      emit(OPR, 0, NEG);
  }
  else
  	term();
  while (currentToken == plussym || currentToken == minussym){
      addingOp = currentToken;
      getNextToken();
      term();
      if(addingOp == plussym)
      	emit(OPR, 0, ADD);
      else if(addingOp == minussym)
      	emit(OPR, 0, SUB);
  }
}
void term(){
	int multiplyOp;
    factor();
    while (currentToken == multsym || currentToken == slashsym){
        multiplyOp = currentToken;
        getNextToken();
        factor();

        if(multiplyOp == multsym)
            emit(OPR, 0, MUL);
        else if(multiplyOp == slashsym)
            emit(OPR, 0, DIV);
  }
}
void factor(){
    if (currentToken == identsym){
        int pos = find(currentIdentifier);
        if(pos == -1) error(11);
        if(symbolType(pos) == 2)
        	emit(LOD, currentLevel - symbolLevel(pos), symbolAddress(pos)); //load a variable
        else if(symbolType(pos) == 1)
        	emit(LIT, 0, symbolVal(pos)); //push a constant
        else
        	error(21);
        getNextToken();
    }
    else if (currentToken == numbersym){
        emit(LIT, 0, currentNumber);
        getNextToken();
    }
    else if (currentToken == lparentsym){
        getNextToken();
        expression();
        if (currentToken != rparentsym) error(22);

        getNextToken();
    }
    else
    {
    	error(100);
    }
}

int find(char* ident){
	int i;
	for(i = symbolIndex - 1; i >= 0; i--){
		if(strcmp(symbolTable[i].name, ident) == 0 && symbolTable[i].level <= currentLevel)
			return i;
		else if(strcmp(symbolTable[i].name, ident) == 0 && symbolType(i) == 1)
			return i;
	}
	return -1;
}
int symbolType(int pos){
	return symbolTable[pos].type;
}
int symbolLevel(int pos){
	return symbolTable[pos].level;
}
int symbolAddress(int pos){
	return symbolTable[pos].addr;
}
int symbolVal(int pos){
	return symbolTable[pos].val;
}

void outputMachineCode(){
	int i;
	FILE*out = fopen("parserout.txt","w");
	// printf("No errors, program is syntactically correct\n");
	for(i = 0; i < codeIndex; i++){
		fprintf(out,"%d %d %d\n", codeStore[i].op, codeStore[i].l, codeStore[i].m);
	}
	fclose(out);
}
