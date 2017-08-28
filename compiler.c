#include "definitions.h"
int main(int argc, char ** argv)
{
	lex(argv[1]);
	parse();
	vm();
	char line[80];
	int i;
	for(i = 2; i < argc; i++)
		if(strcmp(argv[i],"-v") == 0)
		{
			FILE *streamser = fopen("vmoutput.txt", "r");
			while(fgets(line, sizeof(line), streamser))
				printf("%s", line);
			fclose(streamser);
		}
		else if(strcmp(argv[i],"-l") == 0)
		{
			FILE *streamser = fopen("lexout.txt", "r");
			while(fgets(line, sizeof(line), streamser))
				printf("%s", line);
			printf("\n");
			fclose(streamser);
		}
		else if(strcmp(argv[i],"-a") == 0)
		{
			FILE *streamser = fopen("parserout.txt", "r");
			while(fgets(line, sizeof(line), streamser))
				printf("%s", line);
			fclose(streamser);
		}
		else
			printf("Invalid Argument\n");


	return 0;
}
