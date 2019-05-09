		%{
		#include <stdio.h>
		//#define YYPARSER

		extern char yytext[];
		extern FILE* yyin;
		FILE *fout;
		int yylex();
		void yyerror(const char *s);
		void parse(FILE*, FILE *);
		int gettoken();
		int op;
		int lineno;
		%}
		%union
		{
		double d;
		char ch[100];
		}

		%token<ch>ID
		%token<d>INUM
		%token<d>FNUM
		%token<ch>CHARCTR

		%token IF INT FLT CHR MAIN VOID 
		%token DOTHFILE
		%token LT LE GT GE EQ NE SEMI COMA SQUOTE
		%token LBRACE RBRACE LBRACKET RBRACKET
		%token INCL
		%left PLUS MINUS
		%left MUL DIV
		%right ASSIGN
		%token ERROR

		%type <ch>identifier
		%type <d>intnum
		%type <d>floatnum
		%type <ch>charconst

%%
program     :includes statments {fprintf(fout, "\nRET");}

statments   :statments statment {}
			|statment {}
					
					
statment	:vardecl {}
			|fndefn{}
			|assignment{}
			|operation{}
			|relational{}
					
relational	:IF LBRACKET condition RBRACKET statment
			|IF LBRACKET condition RBRACKET LBRACE statments RBRACE{}
					
condition	:identifier conditional_operator identifier{fprintf(fout, "\ncmp %s,%s",$1,$3);}

conditional_operator	:LT{}
						|LE{}
						|GT{}
						|GE{}
						|EQ{}
						|NE{}

operation	:identifier ASSIGN identifier operator identifier SEMI
			{
				if (op==1)
						fprintf(fout, "\nadd %s,%s\nmov %s,%s",$3,$5,$1,$3);
					else if (op==2)
						fprintf(fout, "\nsub %s,%s\nmov %s,%s",$3,$5,$1,$3);
					else if (op==3)
						fprintf(fout, "\nmul %s,%s\nmov %s,%s",$3,$5,$1,$3);
					else if (op==4)
						fprintf(fout, "\ndiv %s,%s\nmov %s,%s",$3,$5,$1,$3);
			}
			|identifier ASSIGN identifier conditional_operator identifier SEMI
			{
				fprintf(fout, "\nmp %s,%s\nmov %s,%s",$3,$5,$1,$3);
			}

operator	:PLUS{op=1;}
			|MINUS{op=2;}
			|MUL{op=3;}
			|DIV{op=4;}

vardecl :intdecl {};
		|floatvardecl{};
		|charvardecl{};

intdecl	:INT identifier SEMI {}
		|INT identifier ASSIGN intnum SEMI{fprintf(fout, "\nmov %s,%g",$2,$4);}


floatvardecl 	:FLT identifier SEMI{}
				|FLT identifier ASSIGN floatnum SEMI{fprintf(fout, "\nmov %s,%g",$2,$4);}


charvardecl 	:CHR identifier SEMI{}
				|CHR identifier ASSIGN charconst SEMI{}

				
fndefn	:INT fnstatement{}
		|FLT fnstatement{}
		|CHR fnstatement{}
		|VOID fnstatement{}

fnstatement	:identifier LBRACKET params RBRACKET statment{}
			|identifier LBRACKET params RBRACKET LBRACE statments RBRACE{}
			|identifier LBRACKET RBRACKET statment{}
			|identifier LBRACKET RBRACKET LBRACE statments RBRACE{}
			|identifier LBRACKET params RBRACKET LBRACE RBRACE{}
			|identifier LBRACKET RBRACKET LBRACE RBRACE{}
					
params  :params param{}
		|param COMA param{}
		|param{}
				
param	:FLT identifier{}
		|CHR identifier{}
					
assignment :identifier ASSIGN intnum SEMI{fprintf(fout, "\nmov %s,%g",$1,$3);}
			|identifier ASSIGN floatnum SEMI{fprintf(fout, "\nmov %s,%g",$1,$3);}
			|identifier ASSIGN charconst SEMI{}

intnum			:INUM{}
floatnum        :FNUM{}
identifier  :ID {}		
charconst	:CHARCTR{}
includes    :includes incl{}
		    |incl{}

incl        :INCL LT DOTHFILE GT {}
%%

void parse(FILE *in, FILE *out)
		{
			lineno=1;
			fout = out;
		    yyin =in;
		    yyparse();
		}    

void yyerror(const char *err)
		{	/*
			char msg[256];
			int errno=lineno;
			sprintf(msg,"\n%s at lineno %d",err,errno);		
		    fprintf(fout,"%s",msg);*/
		    fprintf(fout, "ERROR \n");
		}
		


