%{
/*********************************************
expr3.y
修改 Yacc 程序，不进行表达式的计算，而是实现中缀表达式到后缀表
达式的转换
YACC file
Date:2022/10/12
**********************************************/
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#ifndef YYSTYPE
#define YYSTYPE char*
#endif
char idStr[50];
char numStr[50];
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
%}

%token ADD
%token MINUS
%token MULTIPLY
%token DIVIDE
%token LEFTBRACKET
%token RIGHTBRACKET
%token NUMBER
%token ID
%left ADD MINUS
%left MULTIPLY DIVIDE
%right UMINUS         

%%


lines   :       lines expr ';' { printf("%s\n", $2); }
        |       lines ';'
        |
        ;

expr    :       expr ADD expr   { $$=(char*)malloc(50*sizeof(char));strcpy($$,$1);strcat($$,$3);strcat($$,"+ "); }
        |       expr MINUS expr   { $$=(char*)malloc(50*sizeof(char));strcpy($$,$1);strcat($$,$3);strcat($$,"- "); }
        |       expr MULTIPLY expr   { $$=(char*)malloc(50*sizeof(char));strcpy($$,$1);strcat($$,$3);strcat($$,"* "); }
        |       expr DIVIDE expr   { $$=(char*)malloc(50*sizeof(char));strcpy($$,$1);strcat($$,$3);strcat($$,"/ "); }
        |       LEFTBRACKET expr RIGHTBRACKET      { $$=$2;}
        |       MINUS expr %prec UMINUS   {$$=(char*)malloc(50*sizeof(char));strcpy($$,"-");strcat($$,$2);}
        |       NUMBER  {$$=(char*)malloc(50*sizeof(char));strcpy($$,$1);strcat($$," ");}
        |       ID  {$$=(char*)malloc(50*sizeof(char));strcpy($$,$1);strcat($$," ");}
        ;

%%

// programs section

int yylex()
{
    int t;
    while(1){
        t=getchar();
        if(t==' '||t=='\t'||t=='\n'){
            //do noting
        }else if(isdigit(t)){
            int ti=0;
            while(isdigit(t)){
                numStr[ti]=t;
                t=getchar();
                ti++;
            }
            numStr[ti]='\0';
            yylval=numStr;
            ungetc(t,stdin);
            return NUMBER;
        }else if((t>='a'&&t<='z')||(t>='A'&&t<='Z')||(t=='_')){
            int ti=0;
            while((t>='a'&&t<='z')||(t>='A'&&t<='Z')||(t=='_')||(t>='0'&&t<='9')){
                idStr[ti]=t;
                ti++;
                t=getchar();
            }
            idStr[ti]='\0';
            yylval=idStr;
            ungetc(t,stdin);
            return ID;
        }else if(t=='+'){
            return ADD;
        }else if(t=='-'){
            return MINUS;
        }else if(t=='*'){
            return MULTIPLY;
        }else if(t=='/'){
            return DIVIDE;
        }else if(t=='('){
            return LEFTBRACKET;
        }else if(t==')'){
            return RIGHTBRACKET;
        }else{
            return t;
        }
    }
}

int main(void)
{
    yyin=stdin;
    do{
        yyparse();
    }while(!feof(yyin));
    return 0;
}
void yyerror(const char* s){
    fprintf(stderr,"Parse error: %s\n",s);
    exit(1);
}