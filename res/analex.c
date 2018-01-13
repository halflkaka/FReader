//
//  lexical.h
//  FReader
//
//  Created by 侍粲杰 on 2017/11/20.
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
    //Expression reguliere des variables
    char expr[] = "[a-z]{1}[0-9]*";
    result.N = 0;//Initialisation
    
    //Obtenir la longueur de la formule
    int length = 0;
    for(int i = 0;texte[i] != '\0' && texte[i] != '\n';i++){
        length++;
    }
    
    //Compile l'expression reguliere pour les variables
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
        lexeme lu = result.token[n];//Prendre la caractere
        while(state != End){
            switch (state) {
                case Start:
                    if(isVariableStart(texte[pText])){//Avoir une possibilite d'etre une variable
                        state = Variable;
                        lu.valeur[pToken++] = texte[pText];
                    }
                    else if(isOperator(texte[pText])){//Est un operateur
                        if(texte[pText] == '='){//Cas de "=>"
                            state = Operator;
                            lu.valeur[pToken++] = texte[pText];
                        }else{//D'autre cas
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
                    }else{//Erreur
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
                            lu.valeur[pToken++] = texte[pText];//i.e a120 est une variable correcte
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
                        //Mauvais lexeme
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
    
    //Print le resultat de l'analyse lexical
    print_formula(result);
    
    regfree(&myregex);
    
    return n;
}

//Analyse lexique pour les regles. Rentrer l'arbre de la condition et le resultat de la regle.
arbre ConditionScaner(char* formula, int length) {
    //expression reguliere des variables
    char expr[] = "[a-z]{1}[0-9]*", texte[100];
    condition.N = 0;
    lexeme* toke = condition.token;
    
    //Initialisation
    for(int i=0;i<50;i++){
        toke[i].length = 0;
        toke[i].type = variable;
        for(int j=0;j<20;j++){toke[i].valeur[j] = '\0';}
    }
    
    //Get formule
    for(int i = 0;i<length;i++){
        texte[i] = formula[i];
    }
    
    
    //compile l'expression reguliere pour les variables
    regex_t myregex;
    regmatch_t pmatch[100];
    if(regcomp(&myregex, expr, REG_EXTENDED|REG_NEWLINE)){
        printf("Mauvaise expression rationnelle\n");
        regfree(&myregex);
        return NULL;
    }
    
    int pText = 0;
    int n = 0;
    while(pText < length){
        int state = Start;
        int pToken = 0;
        lexeme lu = condition.token[n];//Prendre la caractere
        while(state != End){
            switch (state) {
                case Start:
                    if(isVariableStart(texte[pText])){//Avoir une possibilite d'etre une variable
                        state = Variable;
                        lu.valeur[pToken++] = texte[pText];
                    }
                    else if(isOperator(texte[pText])){//Est un operateur
                        if(texte[pText] == '='){//Cas de "=>"
                            state = Operator;
                            lu.valeur[pToken++] = texte[pText];
                        }else{//D'autre cas
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
                    }else{//Erreur
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
                            lu.valeur[pToken++] = texte[pText];//i.e a120 est une variable correcte
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
                        //Mauvais lexeme
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
        condition.token[n++] = lu;
    }
    condition.N = n;
    
    ConditionNode = initTree(Term);
    termC(0, n-1, ConditionNode);
    
    arbre ConditionRoot = ConditionNode->left_child;//Root of the condition of rule
    //    print_tree(ConditionRoot,1);
    //    printf("\n");
    
    regfree(&myregex);
    
    return ConditionRoot;
}

arbre ResultatScaner(char* formula, int length) {
    //expression reguliere des variables
    char expr[] = "[a-z]{1}[0-9]*", texte[100];
    regleresult.N = 0;
    lexeme* toke = regleresult.token;
    for(int i=0;i<50;i++){
        toke[i].length = 0;
        toke[i].type = variable;
        for(int j=0;j<20;j++){toke[i].valeur[j] = '\0';}
    }
    
    //Get formule
    for(int i = 0;i<length;i++){
        texte[i] = formula[i];
    }
    
    
    //compile l'expression reguliere pour les variables
    regex_t myregex;
    regmatch_t pmatch[100];
    if(regcomp(&myregex, expr, REG_EXTENDED|REG_NEWLINE)){
        printf("Mauvaise expression rationnelle\n");
        regfree(&myregex);
        return NULL;
    }
    
    int pText = 0;
    int n = 0;
    while(pText < length){
        int state = Start;
        int pToken = 0;
        lexeme lu = regleresult.token[n];//Prendre la caractere
        while(state != End){
            switch (state) {
                case Start:
                    if(isVariableStart(texte[pText])){//Avoir une possibilite d'etre une variable
                        state = Variable;
                        lu.valeur[pToken++] = texte[pText];
                    }
                    else if(isOperator(texte[pText])){//Est un operateur
                        if(texte[pText] == '='){//Cas de "=>"
                            state = Operator;
                            lu.valeur[pToken++] = texte[pText];
                        }else{//D'autre cas
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
                    }else{//Erreur
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
                            lu.valeur[pToken++] = texte[pText];//i.e a120 est une variable correcte
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
                        //Mauvais lexeme
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
        regleresult.token[n++] = lu;
    }
    regleresult.N = n;
    
    ResultNode = initTree(Term);
    termR(0,n-1,ResultNode);
    
    arbre ResultRoot = ResultNode->left_child;//Root of the result of rule
    //    print_tree(ResultRoot,1);
    //    printf("\n");
    
    regfree(&myregex);
    
    return ResultRoot;
}


