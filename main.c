#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define LITERAL_SIZE 8
#define KEYWORD_SIZE 16


typedef enum {
    INTEGER
} TypeLiteral;

typedef enum {
    EXIT,
} TypeKeyword;

typedef enum {
    SEMI_COLON,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    WHITESPACE
} TypeSeparator;

typedef struct {
    TypeLiteral type;
    int value;
} Literal;

typedef struct {
    TypeKeyword type;
} Keyword;

typedef struct {
    TypeSeparator type;
} Separator;

void print_keyword(Keyword *keyword){
    if(keyword->type == EXIT) printf("Token: KEYWORD, Type: EXIT \n");
    else printf("Token: KEYWORD, Type: UNIDENTIFIED");
}

void print_literal(Literal *literal){
    if(literal->type == INTEGER) printf("Token: LITERAL, Type: INTEGER, Value: %d \n", literal->value);
    else printf("Token: LITERAL, Type: UNIDENTIFIED");
}

void print_separator(Separator *seperator){
    if(seperator->type == SEMI_COLON) printf("Token: SEPERATOR, Type: SEMI COLON \n");
    else if(seperator->type == OPEN_PARENTHESIS) printf("Token: SEPERATOR, Type: OPEN PARENTHESIS \n");
    else if(seperator->type == CLOSE_PARENTHESIS) printf("Token: SEPERATOR, Type: CLOSE PARENTHESIS \n");
    else if(seperator->type == WHITESPACE) printf("Token: SEPERATOR, Type: WHITESPACE \n");
    else printf("Token: SEPARATOR, Type: UNIDENTIFIED");
}

Literal* read_integer(FILE *fptr){
    Literal *l_int = malloc(sizeof(Literal));
    char* read_str = malloc(sizeof(char) * LITERAL_SIZE);
    int write_index = 0;
    char ch = fgetc(fptr);
    while(ch != EOF){
        if(isdigit(ch)) {
            read_str[write_index++] = ch;
        }
        else {
            ungetc(ch, fptr);
            break;
        }
        ch = fgetc(fptr);
    }
    l_int->type = INTEGER;
    l_int->value = atoi(read_str);
    free(read_str);
    return l_int;
}

Keyword* read_keyword(FILE *fptr){
    Keyword *k_keyword = malloc(sizeof(Keyword));
    char* read_str = malloc(sizeof(char) * KEYWORD_SIZE);
    int write_index = 0;
    char ch = fgetc(fptr);
    while(ch != EOF){
        if(isalpha(ch) && ch != '(' && ch != ')' && ch != ';' && ch != ' ') {
            read_str[write_index++] = ch;
        }
        else {
            ungetc(ch, fptr);
            break;
        }
        ch = fgetc(fptr);
    }
    if(strcmp(read_str, "exit")){
        k_keyword->type = EXIT;
    }
    free(read_str);
    return k_keyword;
}

void lexer(char* path){
    FILE *fptr = fopen(path, "r");
    char ch = fgetc(fptr);
    while (ch != EOF){
        if(ch == '('){
            Separator *sep = malloc(sizeof(Separator));
            sep->type = OPEN_PARENTHESIS;
            print_separator(sep);
        }
        else if(ch == ')'){
            Separator *sep = malloc(sizeof(Separator));
            sep->type = CLOSE_PARENTHESIS;
            print_separator(sep);
        }
        else if(ch == ';'){
            Separator *sep = malloc(sizeof(Separator));
            sep->type = SEMI_COLON;
            print_separator(sep);
        }
        else if(ch == ' '){
            Separator *sep = malloc(sizeof(Separator));
            sep->type = WHITESPACE;
            print_separator(sep);
        }
        else if(isdigit(ch)){
            ungetc(ch, fptr);
            Literal *num = read_integer(fptr);
            print_literal(num);
        }
        else if(isalpha(ch)){
            ungetc(ch, fptr);
            Keyword *keyword = read_keyword(fptr);
            print_keyword(keyword);
        }
        else printf("%c\n", ch);
        ch = fgetc(fptr);
    }
    fclose(fptr);
} 


int main(){
    lexer("program.tbat");
    return 0;
}