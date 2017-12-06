//
//  lexical.h
//  FReader
//
//  Created by 侍粲杰 on 2017/12/2.
//  Copyright © 2017年 侍粲杰. All rights reserved.
//

#include <stdio.h>
#include <regex.h>

#define Start 0
#define Operator 1
#define Parenthese 2
#define Variable 3
#define End 4

char operators[6] = {'~','|','&','=','(',')'};

int isOperator(char ch){
    for(int i = 0;i < 6;i++){
        if(ch == operators[i]){
            return 1;
        }
    }
    return 0;
}

int isVariable(char ch){
    if((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')){
        return 1;
    }else return 0;
}

typedef enum lexeme_t{
    variable,
    par_ou,
    par_fe,
    op,
    error
} lexeme_t;

typedef struct lexeme{
    lexeme_t type;
    char valeur[20];
    int length;
} lexeme;

typedef struct llexeme{
    lexeme token[50];
    int N;
} llexeme;
llexeme result;

int isVariableType(lexeme lu){
    return lu.type == variable;
}
int isOpType(lexeme lu){
    return lu.type == op || lu.type == par_fe || lu.type == par_ou;
}
int isError(lexeme lu){
    return lu.type == error;
}

void print_formula(llexeme result){
    for(int i = 0;i < result.N;i ++){
        if(isVariableType(result.token[i])){
            printf("Variable: ");
        }else if(isOpType(result.token[i])){
            printf("Operator: ");
        }else if(isError(result.token[i])){
            printf("Error happens!\n");
            break;
        }
        for(int j = 0;j < result.token[i].length;j++){
            printf("%c",result.token[i].valeur[j]);
        }
        printf("\n");
    }
}

int FScaner() {
    //expression reguliere des variables
    char expr[] = "[a-z]{1}[0-9]*", texte[100];
    result.N = 0;
    
    //Get formule
    printf("donner le contexte: ");
    scanf("%99[^\n]", texte);
    char c = getchar();
    if(c != '\n'){
        printf("Erreur, texte trop long\n");
        return 1;
    }
    
    //Get length of formule
    int length = 0;
    for(int i = 0;texte[i] != '\0';i++){
        length++;
    }
    
    //compile l'expression reguliere pour les variables
    regex_t myregex;
    regmatch_t pmatch[100];
    if(regcomp(&myregex, expr, REG_EXTENDED|REG_NEWLINE)){
        printf("Mauvaise expression rationnelle\n");
        regfree(&myregex);
        return 2;
    }
    
    //AFD
    int pText = 0;
    int n = 0;
    while(pText < length){
        int state = Start;
        int pToken = 0;
        lexeme lu = result.token[n];
        while(state != End){
            switch (state) {
                case Start:
                    if(isVariable(texte[pText])){
                        state = Variable;
                        lu.valeur[pToken++] = texte[pText];
                    }
                    else if(isOperator(texte[pText])){
                        if(texte[pText] == '='){
                            state = Operator;
                            lu.valeur[pToken++] = texte[pText];
                        }else{
                            state = End;
                            lu.valeur[pToken++] = texte[pText];
                            lu.type = op;
                        }
                    }else{
                        state = End;
                        //TODO erreur!
                    }
                    break;
                case Variable:
                    if(isVariable(texte[pText])){
                        if(texte[pText] >= 'a' && texte[pText <= 'z']){
                            state = End;
                            pText --;
                        }else{
                            lu.valeur[pToken++] = texte[pText];
                        }
                    }else if(isOperator(texte[pText]) || texte[pText] == '\0'){
                        state = End;
                        pText --;
                    }else{
                        lu.type = error;
                    }
                    break;
                case Operator:
                    if(texte[pText] == '>'){
                        state = End;
                        lu.type = op;
                        lu.valeur[pToken++] = texte[pText];
                    }
                    else{
                        //Wrong lexical
                        state = End;
                        lu.type = error;
                    }
                    break;
                default:
                    break;
            }
            pText ++;
        }
        lu.length = pToken;
        
        //regex execution
        char* p = lu.valeur;
        int res = regexec(&myregex,p, 1, pmatch, 0);
        if(res == 0){
            lu.type = variable;
        }
        result.token[n++] = lu;
    }
    result.N = n;
    
    print_formula(result);
    
    regfree(&myregex);
    
    return 0;
}
