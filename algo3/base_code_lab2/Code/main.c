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


bool isSymbole(char c){
	bool t = false;
	if(c=='+' || c=='-'|| c=='+'|| c=='/'|| c=='^'|| c=='('|| c==')')
		t=true;
	return t;
}

bool isSeparation(char c){
	bool t = false;
	if(c == ' ' || c == '\n')
		t=true;
	return t;
}

bool tokenIsleftBracket(Token *t){
	return tokenIsParenthesis(t)&&tokenGetParenthesisSymbol(t)=='(';
}

bool tokenIsrightBracket(Token *t){
	return tokenIsParenthesis(t)&&tokenGetParenthesisSymbol(t)==')';
}



Queue *shuntingYard(Queue* infix){
	Queue *postfix = createQueue();
	Stack *stack = createStack(queueSize(infix));
	while(!queueEmpty(infix)){
		Token *token = queueTop(infix);
		queuePop(infix);
		if(tokenIsNumber(token))
			queuePush(postfix, token);
		if(tokenIsOperator(token)){
			if(!stackEmpty(stack)){
				if(!tokenIsleftBracket(stackTop(stack))){
					int p =tokenGetOperatorPriority(token);
					while((tokenGetOperatorPriority(stackTop(stack))>p||(tokenGetOperatorPriority(stackTop(stack))==p 
					&&tokenOperatorIsLeftAssociative(token)))&&!tokenIsleftBracket(stackTop(stack))){
						queuePush(postfix, stackTop(stack));
						stackPop(stack);
						if(stackEmpty(stack) || tokenIsleftBracket(stackTop(stack))) break;
					}
				}
			}
			stackPush(stack, token);
		}
		if (tokenIsleftBracket(token))
			stackPush(stack, token);
		if (tokenIsrightBracket(token)) {
			while (!tokenIsleftBracket(stackTop(stack))) {
				queuePush(postfix, stackTop(stack));
				stackPop(stack);
			}
			stackPop(stack);
		}
	}
	while(!stackEmpty(stack)){
		queuePush(postfix, stackTop(stack));
		stackPop(stack);
	}
	return postfix;
}




Queue *stringToTokenQueue(const char *expression){
	Queue *queue = createQueue();
	char *curpos = (char*) expression;
	int l;
	while(*curpos!='\0'){
		l=0;
		while(isSeparation(*curpos)) curpos++;
		if (isSymbole(*curpos)){
			Token *token = createTokenFromString(curpos, 1);
			queuePush(queue, token);
			curpos++;
		} else {
			while(*curpos+l != '\0' && !isSeparation(*(curpos+l)) && !isSymbole(*(curpos+l))){
				l++;
			}
			Token *token = createTokenFromString(curpos, 1);
			queuePush(queue, token);
			curpos +=l;
		}
	}

	return queue;
}

Token *evaluateOperator(Token *arg1, Token *op, Token *arg2){
	char op2 = tokenGetOperatorSymbol(op);
	float a = tokenGetValue(arg1);
	float b = tokenGetValue(arg2);
	float resultat;
	resultat=(op2=='+')?(a+b):((op2=='-')?(a-b):((op2=='*')?(a*b):((op2=='/')?(a/b):((op2=='^')?(pow(a,b)):0))));
	return createTokenFromValue(resultat);
}	

float evaluateExpression(Queue* postfix){
	unsigned int i;
	Stack *stack = createStack(queueSize(postfix));
	unsigned int len = queueSize(postfix);
	for(i=1;i<len;i++){
		if(tokenIsOperator(queueTop(postfix))){
			Token *ope_2 = stackTop(stack);
			stackPop(stack);
			Token *ope_1 = stackTop(stack);
			stackPop(stack);
			stackPush(stack, evaluateOperator(ope_1, queueTop(postfix), ope_2));
			queuePop(postfix);
		} else if (tokenIsNumber(queueTop(postfix))) {
			stackPush(stack, queueTop(postfix));
			queuePop(postfix);
		}
	}
	return tokenGetValue(stackTop(stack));
}











void printToken(FILE *f, void *e){
	tokenDump( f,  e);
}


void computeExpressions(FILE * input){
	char *buffer;
	size_t  linecapp= 32;
	buffer = malloc(linecapp * (sizeof(char)));
	Queue *queue = createQueue();
	Queue *postfix = createQueue();
	if(buffer==NULL){
		perror("Echec de l'allocation mémoire");
		exit(1);
	}
	while((getline(&buffer,&linecapp, input))>0){
		if (strcmp(buffer,"\n")){
			printf("Input: %s",buffer);
			queue = stringToTokenQueue(buffer);
			printf("Infix: ");
			queueDump(stdout, queue, printToken );
			printf("\n");
			postfix = shuntingYard(queue);
			printf("postfix: ");
			queueDump(stdout, postfix, printToken );
			printf("\n");
			printf("Evaluate: ");
			printf("%f",evaluateExpression(postfix));
			printf("\n\n");
		}
	}
	free(buffer);
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
 
