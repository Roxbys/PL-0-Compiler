#include "definitions.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

int sp = 0, bp = 1, ir = 0, pc = 0;
int stack[2000] = {0};
int hault = 1;
int R[16] = {0};
int prevsp = 0, registerlength = 1;
int numoperations = 0;

int base(int l,int base) // l stand for L in the instruction format
{
    int b1; //find base L levels down
    b1 = base;
    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }
    return b1;
}
int converttoint (char instr[])
{
    if(strcmp(instr,"lit") == 0)
        return 1;
    if(strcmp(instr,"opr") == 0)
        return 2;
    if(strcmp(instr,"lod") == 0)
        return 3;
    if(strcmp(instr,"sto") == 0)
        return 4;
    if(strcmp(instr,"cal") == 0)
        return 5;
    if(strcmp(instr,"inc") == 0)
        return 6;
    if(strcmp(instr,"jmp") == 0)
        return 7;
    if(strcmp(instr,"jpc") == 0)
        return 8;
    if(strcmp(instr,"sio") == 0)
        return 9;
    if(strcmp(instr,"sio") == 0)
        return 10;
    if(strcmp(instr,"sio") == 0)
        return 11;
    return -1;
}
char *converttoword (int number )
{
    switch (number) {
        case 1:
        return "lit";
        break;
        case 2:
        return "opr";
        break;
        case 3:
        return "lod";
        case 4:
        return "sto";
        break;
        case 5:
        return "cal";
        break;
        case 6:
        return "inc";
        break;
        case 7:
        return "jmp";
        break;
        case 8:
        return "jpc";
        break;
        case 9:
        return "sio";
        break;
        case 10:
        return "sio";
        break;
        case 11:
        return "sio";
        break;
        default:
        break;
    }
    return "";
}
void doinstruct(int op, int l, int m, FILE *out, FILE *vmout)
{
    // case based on the number of the specific intruction
    int i; 
    pc++;
    switch (op) {
        case 1:
        	sp = sp + 1;
            stack[sp] = m;
            break;
        case 2:
        	/*
        	0    RET  (sp  bp -1 and pc  stack[sp + 4] and bp  stack[sp + 3]) 
        	1    NEG (stack[sp]  -stack[sp]) 
        	2    ADD  (sp  sp – 1 and  stack[sp]  stack[sp] + stack[sp + 1]) 
        	3    SUB   (sp  sp – 1 and  stack[sp]  stack[sp] - stack[sp + 1]) 
        	4    MUL  (sp sp – 1 and  stack[sp]  stack[sp] * stack[sp + 1]) 
        	5    DIV  (sp sp – 1 and stack[sp]  stack[sp] / stack[sp + 1]) 
        	6	 ODD (stack[sp]  stack[sp] mod 2) or ord(odd(stack[sp])) 
        	7    MOD (sp  sp – 1 and  stack[sp]  stack[sp] mod stack[sp + 1]) 
        	8    EQL   (sp  sp – 1 and  stack[sp]   stack[sp] = = stack[sp + 1]) 
        	9    NEQ   (sp  sp – 1 and  stack[sp]   stack[sp] != stack[sp + 1]) 
        	10   LSS   (sp  sp – 1 and  stack[sp]   stack[sp]  <  stack[sp + 1])   
        	11   LEQ   (sp  sp – 1 and  stack[sp]   stack[sp] <=  stack[sp + 1]) 
        	12   GTR   (sp  sp – 1 and  stack[sp]   stack[sp] >  stack[sp + 1])
        	13   GEQ   (sp  sp – 1 and  stack[sp]   stack[sp] >= stack[sp + 1])
        	*/
            switch(m)
            {
            	case 0:
	            	sp = bp - 1;
	            	pc = stack[sp + 4];
	            	bp = stack[sp + 3];
            		break; 
            	case 1:
            		stack[sp] = -stack[sp];
            		break;
            	case 2: 
            		sp = sp - 1;
            		stack[sp] = stack[sp] + stack[sp + 1];
            		break; 
            	case 3:
            		sp = sp - 1;
            		stack[sp] = stack[sp] - stack[sp + 1];
            		break; 
            	case 4:
            		sp = sp - 1;
            		stack[sp] = stack[sp] * stack[sp + 1];
            		break;
            	case 5: 
            		sp = sp - 1;
            		stack[sp] = stack[sp] / stack[sp + 1];
            		break;
            	case 6:
            		stack[sp] = stack[sp] % 2;
            		break;
            	case 7: 
            		sp = sp - 1;
            		stack[sp] = stack[sp] % stack[sp + 1];
            		break;
            	case 8: 
            		sp = sp - 1;
            		stack[sp] = stack[sp] == stack[sp + 1];
            		break;
            	case 9:
            		sp = sp - 1;
            		stack[sp] = stack[sp] != stack[sp + 1];
            		break;
            	case 10:
            		sp = sp - 1;
            		stack[sp] = stack[sp] < stack[sp + 1];
            		break;
            	case 11:
           			sp = sp - 1;
            		stack[sp] = stack[sp] <= stack[sp + 1];
            		break;
            	case 12: 
            		sp = sp - 1;
            		stack[sp] = stack[sp] > stack[sp + 1];
            		break;
            	case 13:
            		sp = sp - 1;
            		stack[sp] = stack[sp] >= stack[sp + 1];
            		break;
            	default:
            		break; 
            }
            break;
        case 3:
        	sp = sp + 1;
            stack[sp] = stack[base(l,bp)+m];
            break;
        case 4:
            stack[base(l, bp)+m] = stack[sp];
            sp = sp - 1; 
            break;
        case 5:
           	stack[sp+1] = 0;
            stack[sp+2] = base(l, bp);
            stack[sp+3] = bp;
            stack[sp+4] = pc;
            bp = sp + 1;
            pc = m;
            break;
        case 6:
            sp = sp + m;
            break;
        case 7:
            pc = m;
            break;
        case 8:
            if (stack[sp] == 0)
                pc = m;
        	break;
        case 9:
        	switch (m)
        	{
	        	case 1:
	        		fprintf(vmout,"%d\n", stack[sp]);
	        		sp = sp - 1; 
	        		break;
	        	case 2: 
	        		sp = sp + 1;
	        		printf("Provide some input:");
	        		scanf("%d",&stack[sp]);
	        		break;
	        	case 3:
		        	hault = 0;
		            pc = 0; 
		            bp = 0; 
		            sp = 0;
	        		break;
	        	default:
	        		break;
	        }	
        default:
            break;
    }
    if(sp > prevsp && prevsp == 0)
    	prevsp = registerlength = sp;
    fprintf(out,"%s\t%d\t%d\t\t%d\t%d\t%d\t", converttoword(op), l, m, pc, bp, sp);
    for(i = 1; i<=sp+1; i++)
    {
        fprintf(out," %d", stack[i]);
        if (registerlength * (sp/registerlength) + 1 == i+1)
        	continue;
        else if (stack[bp + 2] > 0 && i%registerlength == 0)
        	fprintf(out,"|");
    }
    // prevsp = sp; 
    if(op == 5)
    {
    	fprintf(out,"|");
    	for(i = sp + 1; i < sp + 5 ; i++)
    		fprintf(out," %d", stack[i]);
    }
    fprintf(out,"\n");
}
void vm(void) {
    FILE *fp = fopen("parserout.txt", "r");
    FILE *out = fopen("vmstackoutput.txt", "w");
    FILE *vmout = fopen("vmoutput.txt", "w");
    char c [81];
    char *token;
    struct instruction structions[500];
    int i=0;
    fprintf(out,"Input Instructions: \n");
    int flag = 0;
    
    // reads in the in input file and parses it to integers (input file must all be integers)
    fprintf(out, "Line\tOp\tL\tM\t\n");
    while (1)
    {
    	fscanf(fp, "%d %d %d", &structions[i].op, &structions[i].l, &structions[i].m);
    	if(feof(fp))
    		break;

        fprintf(out, "%d\t%s\t%d\t%d \n",(i), converttoword(structions[i].op), structions[i].l, structions[i].m);
        i++;
    }
    // does the instructions based on what the program counter is
    fprintf(out,"Output:\t\t\t\tPC\tBP\tSP\tStack\n");
    fprintf(out,"Initial values:\t\t\t%d\t%d\t%d\n",pc,bp,sp);
    // fprintf(out, "%\n");
    while(1)
    {
        if (hault == 0)
        {
            break;
        }
        doinstruct(structions[pc].op, structions[pc].l, structions[pc].m, out, vmout);
    }
    fclose(fp);
    fclose(out);
    fclose(vmout);
}
