#include <stdio.h>
extern void parse(FILE*, FILE*);
int main()
{
	FILE *fp1, *fp2;
	fp1 = fopen("test1.cm","r");
	fp2 = fopen("testout.txt","w");
	parse(fp1,fp2);
	fclose(fp1);
	fclose(fp2);
	return 0;
}