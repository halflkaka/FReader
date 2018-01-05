//
//  lexical.h
//  FReader
//
//  Created by 侍粲杰 on 2017/12/2.
//  Copyright © 2017年 侍粲杰. All rights reserved.
//

#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include "anasynt.c"
#include "moteur.c"

#define Start 0
#define Operator 1
#define Parenthese 2
#define Variable 3
#define End 4
#define Size 100



int FScaner(char* texte, int* flag) {
    //expression reguliere des variables
    char expr[] = "[a-z]{1}[0-9]*";
    result.N = 0;
    
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
                    if(isVariableStart(texte[pText])){
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
                            if(texte[pText] == '('){
                                lu.type = par_ou;
                            }else if (texte[pText] == ')'){
                                lu.type = par_fe;
                            }else if (texte[pText] == '~'){
                                lu.type = rev;
                            }else{
                                lu.type = op;
                            }
                        }
                    }else{
                        state = End;
                        lu.type = error;
                        lu.valeur[pToken++] = texte[pText];
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
                        state = End;
                        pText --;
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
    
//    anasyn(n);
    
//    moteur(flag);
    
    regfree(&myregex);
    
    return n;
}


//syntaxique






