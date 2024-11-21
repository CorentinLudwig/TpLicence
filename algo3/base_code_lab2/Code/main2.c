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
				int p =tokenGetOperatorPriority(token);
				while(tokenIsleftBracket(stackTop(stack))){
					queuePush(postfix, stackTop(stack));
					stackPop(stack);
				}
				if(!stackEmpty(stack)){
					while((tokenGetOperatorPriority(stackTop(stack))>p||(tokenGetOperatorPriority(stackTop(stack))==p 
					&&tokenOperatorIsLeftAssociative(token)))){
						queuePush(postfix, stackTop(stack));
						stackPop(stack);
					}
				}
			}
			stackPush(stack, token);
		}
		if (tokenIsleftBracket(token))
			stackPush(stack, token);
		if (tokenIsrightBracket(token)) {
			while (!tokenIsleftBracket(stackTop(stack))) {
				stackPush(stack, stackTop(stack));
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
		printf("Input: %s",buffer);
		queue = stringToTokenQueue(buffer);
		printf("Infix: ");
		queueDump(stdout, queue, printToken );
		printf("\n");
		postfix = shuntingYard(queue);
		printf("postfix: ");
		queueDump(stdout, postfix, printToken );
		printf("\n\n");
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
 