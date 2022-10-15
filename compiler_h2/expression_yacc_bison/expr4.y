%{
/*********************************************
expr4.y
在第二步要求的基础上，实现功能更强的词法分析和语法分析程序，使
之能支持变量，修改词法分析程序，能识别变量（标识符）和 “=” 符号，修
改语法分析器，使之能分析、翻译 “a=2” 形式的（或更复杂的，“a= 表达
式”）赋值语句，当变量出现在表达式中时，能正确获取其值进行计算（未
赋值的变量取 0）。当然，这些都需要实现符号表功能。
YACC file
Date:2022/10/12
**********************************************/
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#ifndef YYSTYPE
#define YYSTYPE double
#endif
#define maxSym 30
char idStr[50];

struct symbolTable {
        char *id;
        double value;
}symtable[maxSym];

// double symQuery(int p);
// void symSet(char* s, double v);

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
%token ASSIGN


%right ASSIGN
%left ADD MINUS
%left MULTIPLY DIVIDE
%right UMINUS         

%%


lines   :       lines ';'
        |       lines expr ';'  {printf("%f\n", $2); }
        |       {;}
        ;

expr    :       expr ADD expr   { $$=$1+$3; }
        |       expr MINUS expr   { $$=$1-$3; }
        |       expr MULTIPLY expr   { $$=$1*$3; }
        |       expr DIVIDE expr   { $$=$1/$3; }
        |       LEFTBRACKET expr RIGHTBRACKET      { $$=$2;}
        |       MINUS expr %prec UMINUS   {$$=-$2;}
        |       NUMBER  {$$=$1;}
        |       ID  {
                        $$=symtable[(int)$1].value;
                    }
        |       ID ASSIGN expr  {
                                    symtable[(int)$1].value=$3;
                                    $$=$3;
                                }
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
            yylval=0;
            while(isdigit(t)){
                yylval=yylval*10+t-'0';
                t=getchar();
            }
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
            ungetc(t,stdin);
            
            int i=0;
            for(i=0;i<maxSym;i++){
                if(symtable[i].id)
                {
                    if(!strcmp(symtable[i].id,idStr)){
                        break;
                    }
                }else{
                    symtable[i].id=(char*)malloc(50*sizeof(char));
                    strcpy(symtable[i].id,idStr);
                    symtable[i].value=0;
                    break;
                }
            }
            yylval=i;
            return ID;
        }else if(t=='='){
            return ASSIGN;
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