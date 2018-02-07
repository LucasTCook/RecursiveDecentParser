#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Define the types of chars
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

//Define the types of operators
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define EOF -1

//Initialize the arrays to hold input, lexemes, and the ID and Integers detected
char expr[100];
char ids[100][100];
char integers[100][100];
char lexeme [100];

//Initialize the variable to hold the next char of the input
char nextChar;

//Initialize the variables that hold the defines terms above
int charClass;
int nextToken;

//Initialize the variable to increment within the arrays during processing
int lexLen;
int count = -1;
int numOfIds = 0;
int numOfIntegers = 0;


//Initialize the variables that keep track of the number of each special char
int numOfParen = 0;
int numOfAddOp = 0;
int numOfMulOp = 0;
int numOfDivOp = 0;
int numOfSubOp = 0;

//Implicitly declare the functions that will be used
void addChar();
void getChar();
void getNonBlank();
int lex();
void expression();
void term();
void factor();
void assign();
void id();


//main driver
int main(){
    //Prompt the user and receive their input
    printf("Enter the expression: ");
    fgets(expr, 100, stdin);
    //Eliminate the '\n' at the end of the array by making it null
    expr[strlen(expr) -1] = '\0';
    
    //Get the first character of the input
    getChar();
    lex();
    assign();
    //Perform lex() while the char isnt the end of the input
    /*do{
     if (numOfParen < 0){
     printf("!ERROR: No Left Parenthesis for the Right Parenthesis");
     exit(0);
     }
     lex();
     }while(charClass != EOF);*/
    
    //Detects missing parenthesis and prints an error accordingly
    /*if (numOfLParen != numOfRParen) {
     if (numOfRParen < numOfLParen)
     printf("! ERROR: MISSING RIGHT PARENTHESIS\n");
     if (numOfRParen > numOfLParen)
     printf("! ERROR: MISSING LEFT PARENTHESIS\n");
     }*/
    
    //Prints all IDs after the entire input is processed
    printf("\nIDs: ");
    for(int i = 0; i<numOfIds; i++) {
        printf("%s", ids[i]);
        if(i != (numOfIds-1))
            printf(", ");
    }
    
    //Prints all INTEGERs after the entire input is processed
    printf("\nINTEGERs: ");
    for(int i = 0; i<numOfIntegers; i++) {
        printf("%s", integers[i]);
        if(i != (numOfIntegers-1))
            printf(", ");
    }
    
    //Prints the number of each special character of the input
    // printf("\nNumber of Left Parenthesis: %d\n", numOfLParen);
    //printf("Number of Right Parenthesis: %d\n", numOfRParen);
    printf("Number of Addition Operations: %d\n", numOfAddOp);
    printf("Number of Subtraction Operations: %d\n", numOfSubOp);
    printf("Number of Multiplication Operations: %d\n", numOfMulOp);
    printf("Number of Division Operations: %d\n", numOfDivOp);
    
    
}

//Gets the next char and decides its type
void getChar(){
    //Increment count to travel through the input char by char
    count++;
    
    //If the next char isnt NULL then decide if it is a LETTER or DIGIT or UNKNOWN and assign the charClass accordingly
    if (expr[count] != '\0'){
        nextChar = expr[count];
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
    //If the next char is NULL, then make charClass EOF (Which tells the main driver to stop processing the input)
    else charClass = EOF;
}

//Adds the current char to the lexeme after being identified and processed
void addChar(){
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
}

//Decides the type of special char and return the appropriate token
//Once a decision is made, the counter of that char is incremented
int lookup(char ch){
    switch(ch){
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            numOfParen++;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            numOfParen--;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            numOfAddOp++;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            numOfSubOp++;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            numOfMulOp++;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            numOfDivOp++;
            break;
        case '=':
            addChar();
            nextToken = ASSIGN_OP;
            break;
        default:
            printf("! ERROR: Character %c is unable to be processed\n", ch);
    }
    return nextToken;
}

//Ignores the blank spaces of the input
void getNonBlank() {
    while (isspace(nextChar)){
        getChar();
    }
}

//Forms the lexemes of the input
//Anything starting with a Letter and followed by a Letter OR Digit is classified as an ID
//Anything staring with a Digit and followed by a Digit is classified as an INTEGER
int lex(){
    //lexLen=0 shows the start of a new lexeme
    lexLen = 0;
    
    //ignore any blank space
    getNonBlank();
    
    //Parse through the input based of the first letter of the new lexeme
    switch (charClass){
            //Letter followed by another letter or a digit
        case LETTER:
            addChar();
            getChar();
            while(charClass == LETTER || charClass == DIGIT){
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
            
            //Digit followed by another digit
        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
            //Neither Letter or Digit, so perform lookup on that char
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
            
            //The end of the input has been reached
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    
    //input the completed ID lexeme into the ID array and increment the numOfIds
    if (nextToken == IDENT) {
        strcpy(ids[numOfIds], lexeme);
        numOfIds++;
    }
    //input the completed INTEGER lexeme into the INTEGER array and increment the numOfIntegers
    else if (nextToken == INT_LIT) {
        strcpy(integers[numOfIntegers], lexeme);
        numOfIntegers++;
    }
    return nextToken;
}

void expression(int indent) {
    for(int i = 0; i<indent; i++)
        printf("\t");
    printf("[expr\n");
    term(indent+1);
    while(nextToken == ADD_OP || nextToken == SUB_OP){
        for(int i = 0; i<=indent; i++)
            printf("\t");
        if (nextToken == ADD_OP)
            printf("[+]\n");
        if (nextToken == SUB_OP)
            printf("[-]\n");
        lex();
        term(indent+1);
    }
    for(int i = 0; i<indent; i++)
        printf("\t");
    printf("]\n");
}

void term(int indent) {
    for(int i = 0; i<indent; i++)
        printf("\t");
    printf("[term\n");
    factor(indent+1);
    while (nextToken == MULT_OP || nextToken == DIV_OP){
        for(int i = 0; i<=indent; i++)
            printf("\t");
        if (nextToken == MULT_OP)
            printf("[*]\n");
        if (nextToken == DIV_OP)
            printf("[/]\n");
        lex();
        factor(indent+1);
    }
    for(int i = 0; i<indent; i++)
        printf("\t");
    printf("]\n");
}

void factor(int indent) {
    for(int i = 0; i<indent; i++)
        printf("\t");
    printf("[factor\n");
    if(nextToken == IDENT) {
        id(indent+1);
        lex();
    }
    else {
        if (nextToken == LEFT_PAREN) {
            for(int i = 0; i<=indent; i++)
                printf("\t");
            printf("[(]\n");
            lex();
            expression(indent+1);
            if (nextToken == RIGHT_PAREN){
                for(int i = 0; i<=indent; i++)
                    printf("\t");
                printf("[)]\n");
                lex();
            }
            else{
                printf("MISSING PARENTHESIS\nINVALID EXPRESSION\n");
                exit(0);
            }
        } else{
            printf("MISSING PARENTHESIS\nINVALID EXPRESSION\n");
            exit(0);
        }
    }
    for(int i = 0; i<indent; i++)
        printf("\t");
    printf("]\n");
}

void assign(int indent){
    printf("[assign\n");
    if(nextToken == IDENT) {
        id(1);
        lex();
        if (nextToken == ASSIGN_OP) {
            printf("\t[=]\n");
            lex();
            expression(1);
        }
        else {
            printf("Not a Valid Expression\n");
            exit(0);
        }
    }
    else {
        printf("Not a Valid Expression\n");
        exit(0);
    }
    printf("]");
}

void id(int indent){
    for(int i = 0; i< indent; i++)
        printf("\t");
    printf("[id [%s]]\n", ids[numOfIds-1]);
}
