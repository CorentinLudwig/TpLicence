// Need this to use the getline C function on Linux. Works without this on MacOs. Not tested on Windows.
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "token.h"
#include "queue.h"
#include "stack.h"

Queue * stringToTokenQueue(const char *expression);
bool isSymbol(char  c);
bool greaterPrecedence(Token * t,Stack * s);
bool TokenIsleftParenthesis(Token *t);
bool TokenIsRightParenthesis(Token *t);
Queue *shuntingYard(Queue* infix);

void printToken(FILE * f, void*e){
	tokenDump(f,e);
}
void computeExpressions(FILE * input){
	char * ligne;
	int nBlus=0;
	size_t  linecapp= 32;
	ligne = malloc(linecapp * (sizeof(char)));

	if(ligne==NULL){
		perror("Echec de l'allocation mémoire");
		exit(1);
	}
	while(((getline(&ligne,&linecapp, input))!=0)&&(nBlus<7)){
		if (strcmp(ligne,"\n")){
			Queue * InfixQ = stringToTokenQueue(ligne);
			fprintf(stdout, "Input	: %sInfix    :",ligne );
			queueDump(stdout, InfixQ, printToken);
			fprintf(stdout,"\nPostfix : ");
			//PB à régler pour initialisation PostfixQ sûrement dans shuntingYard
			//Segmentation Fault
			Queue * PostfixQ = shuntingYard(InfixQ);
			queueDump(stdout,PostfixQ, printToken);
			fprintf(stdout,"\n");
			nBlus++;
		}
	}
	free(ligne);
}

Queue * stringToTokenQueue(const char *expression) {
	Queue * Q= createQueue();
	char *curpos = (char *)expression;
	int longu=1;
	while((*curpos)!='\0'){
//la ligne saute directement les lignes vides et empêche
//que le programme compte +1 caractères sur la ligne
//Infix    :(5) --  1.000000 + 2.000000 * 3.000000 au lieu de
//Infix    :(6) --  1.000000 + 2.000000 * 3.000000
		if (strcmp(curpos,"\n")){
			while(((*curpos)==' ')|| ((*curpos)=='\n')){
				curpos++;
			}
			//Code non atteint par autre chose qu'un chiffre grâce au while et isSymbol
			if(!(isSymbol(*curpos))){
				while(!(isSymbol(*curpos+longu))){
						longu++;
				}
				Token * t= createTokenFromString(curpos, longu);
				queuePush(Q, t);
			}else{
				Token * t= createTokenFromString(curpos, 1);
				queuePush(Q, t);
			}
		}
		 curpos++;
	}
	return Q;
}

bool isSymbol(char  c){
	return c=='+' ||c=='-' ||c=='*' ||c=='/' ||c=='^' ||c=='(' ||c==')';
}

Queue *shuntingYard(Queue* infix){
	Queue * outpoutQ= createQueue();
	Stack * operatorS= createStack(200);
	Token * operator;
	Token * tok;
	while(!queueEmpty(infix)){
		tok =queueTop(infix);
		queuePop(infix);
		if(tokenIsNumber(tok)){
			queuePush(outpoutQ,tok);
		}
		else if(tokenIsOperator(tok)){
			//vérification de la condition pas dans le while sinon illisible
			while(greaterPrecedence(tok,operatorS)){
				operator=stackTop(operatorS);
				queuePush(outpoutQ,operator);
				stackPop(operatorS);
			}
			stackPush(operatorS,tok);
		}
		else if(TokenIsleftParenthesis(tok)){
			stackPush(operatorS,tok);
		}
		else if(TokenIsRightParenthesis(tok)){
			while(!TokenIsleftParenthesis(tok)){
				operator=stackTop(operatorS);
				queuePush(outpoutQ,operator);
				stackPop(operatorS);
			}
			stackPop(operatorS);
		}

	}
	if(queueEmpty(infix)){
		while(!stackEmpty(operatorS)){
			operator=stackTop(operatorS);
			queuePush(outpoutQ,operator);
			stackPop(operatorS);
		}
	}
	return outpoutQ;
}

bool greaterPrecedence(Token * t,Stack * s){
	Token * operator=stackTop(s);
 	return (((tokenGetOperatorPriority(operator)>tokenGetOperatorPriority(t))||
 		(tokenGetOperatorPriority(operator)==tokenGetOperatorPriority(t) && (tokenOperatorIsLeftAssociative(operator)) ))
		&& !TokenIsleftParenthesis(operator));
}

bool TokenIsleftParenthesis(Token *t){
	return tokenIsParenthesis(t)&&tokenGetParenthesisSymbol(t)=='(';
}

bool TokenIsRightParenthesis(Token *t){
	return tokenIsParenthesis(t)&&tokenGetParenthesisSymbol(t)==')';
}

/** Main function for testing.
 * The main function expects one parameter that is the file where expressions to translate are
 * to be read.
 *
 * This file must contain a valid expression on each line
 *
 */
int main(int argc, char **argv){
	FILE *input;

	if (argc<2) {
		fprintf(stderr,"usage : %s filename\n", argv[0]);
		return 1;
	}

	input = fopen(argv[1], "r");

	if ( !input ) {
		perror(argv[1]);
		return 1;
	}

	computeExpressions(input);
	fclose(input);
	return 0;
}
main.c
5 KB