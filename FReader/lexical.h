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

#define Start 0
#define Operator 1
#define Parenthese 2
#define Variable 3
#define End 4
#define Size 100

char operators[6] = {'~','|','&','=','(',')'};

int isOperator(char ch){
    for(int i = 0;i < 6;i++){
        if(ch == operators[i]){
            return 1;
        }
    }
    return 0;
}

int isVariableStart(char ch){
    return ch >= 'a' && ch <= 'z';
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
    rev,
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
    return lu.type == op || lu.type == par_fe || lu.type == par_ou || lu.type == rev;
}
int isError(lexeme lu){
    return lu.type == error;
}

//Definition of tree
typedef struct noeud * arbre;
//State of node
typedef enum State{
    Term,
    Facteur,
    OP,
    Var,
    Par,
    Rev,
    None
} State;

struct noeud{
    char * etiquette;
    enum State state;
    arbre left_child;
    arbre right_child;
};

//Initialization
arbre initTree(enum State name){
    arbre node = malloc(sizeof(struct noeud));
    node->etiquette = NULL;
    node->left_child = NULL;
    node->right_child = NULL;
    node->state = name;
    return node;
}

void rule1(arbre currentnode);
void rule2(arbre currentnode);
void rule5(arbre currentnode);
void rule6(arbre currentnode);
void rule7(arbre currentnode);
int term(int d, int f, arbre currentNode);
arbre node;

void print_formula(llexeme result){
    for(int i = 0;i < result.N;i ++){
        if(isVariableType(result.token[i])){
            printf("Variable: ");
        }else if(isOpType(result.token[i])){
            if(result.token[i].type == rev){
                printf("Uni Operator: ");
            }else if(result.token[i].type == par_ou){
                printf("Parenthese Ouverte: ");
            }else if (result.token[i].type == par_fe){
                printf("Parenthese Fermee: ");
            }else{
                printf("Bi Operator: ");
            }
        }else if(isError(result.token[i])){
            printf("Error happens on %c!\n",result.token[i].valeur[0]);
            break;
        }
        for(int j = 0;j < result.token[i].length;j++){
            printf("%c",result.token[i].valeur[j]);
        }
        printf("\n");
    }
}
//中序遍历打印
void print_tree(arbre root){
    if(root == NULL){return;}
    
    if(root->etiquette[0] == '~'){
        printf("(");
        printf("%s",root->etiquette);
        print_tree(root->left_child);
        print_tree(root->right_child);
        printf(")");
    }
    else{
        if(root->left_child != NULL){
            printf("(");
        }
        print_tree(root->left_child);
        if(root->etiquette != NULL){
            printf("%s",root->etiquette);
        }
        print_tree(root->right_child);
        if(root->right_child != NULL){
            printf(")");
        }
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
    
    node = initTree(Term);
    if(term(0, n-1, node) == 1){
        printf("syntaxique correct.\n");
    }else{
        printf("syntaxique error.\n");
    }
    
    arbre root = node->left_child;
    print_tree(root);
    printf("\n");
    
    regfree(&myregex);
    
    return 0;
}


//syntaxique

//BNF
int Op(int d, arbre currentNode){
    int resultat = (result.token[d].type == op);
    if(resultat == 1){
        //Initialize OP node
        currentNode->etiquette = result.token[d].valeur;
    }
    return resultat;
}

int Vb(int d, int f, arbre currentNode){
    int resultat = ((result.token[d].type == variable) && d == f);
    if(resultat == 1){
        //Initialize Variable node
        arbre Varnode = initTree(Var);
        Varnode->etiquette = result.token[d].valeur;
        //add in the tree
        if(currentNode->left_child == NULL){
            currentNode->left_child = Varnode;
        }else{
            currentNode->right_child = Varnode;
        }
    }
    return resultat;
}

int facteur(int d, int f, arbre currentNode){
    //Initialize Facteur node
    int resultat = Vb(d,f,currentNode);//facteur = variable
    if (!resultat){
        //Initialize Term node
        if(result.token[d].type == par_ou && result.token[f].type == par_fe &&
           term(d+1, f-1, currentNode)){ //facteur = '('term')'
            //It's a facteur!
            resultat = 1;
        }
    }
    return resultat;
}

int term(int d, int f, arbre currentNode){
    int i = f - 1, b = facteur(d, f, currentNode);
    arbre Facnode = initTree(Facteur);
    while(b == 0 && i >= d){
        if(Op(i, Facnode) && facteur(d, i-1, Facnode) && facteur(i+1, f, Facnode)){
            //term = facteur OP facteur
            if(currentNode->left_child == NULL){
                currentNode->left_child = Facnode;
            }else{
                currentNode->right_child = Facnode;
            }
            b = 1;
        }else if((result.token[d].valeur[0] == '~') && facteur(d+1, f, Facnode)){
            //term = '~'facteur
            Facnode->etiquette = "~";
            if(currentNode->left_child == NULL){
                currentNode->left_child = Facnode;
            }else{
                currentNode->right_child = Facnode;
            }
            b = 1;
        }
        i --;
    }
    return b;
}


//Apply rules
void rule1(arbre currentNode){
    currentNode->etiquette = "|";
    arbre leftbranch = currentNode->left_child;
    arbre Termnode = initTree(Term);
    Termnode->etiquette = "~";
    Termnode->left_child = leftbranch;
    currentNode->left_child = Termnode;
}

void rule2(arbre currentNode){
    currentNode->etiquette = "&";
    arbre leftbranch = currentNode->left_child;
    arbre rightbranch = currentNode->right_child;
    arbre r_leftbranch = rightbranch->left_child;
    arbre Facnode = initTree(Facteur);
    
    Facnode->etiquette = "|";
    Facnode->left_child = leftbranch;
    Facnode->right_child = r_leftbranch;
    
    currentNode->left_child = Facnode;
    
    rightbranch->etiquette = "|";
    rightbranch->left_child = leftbranch;
}

void rule5(arbre currentNode){
    currentNode->etiquette = "|";
    arbre leftbranch = currentNode->left_child->left_child;
    arbre rightbranch = currentNode->left_child->right_child;
    
    arbre TermnodeLeft = initTree(Term);
    arbre TermnodeRight = initTree(Term);
    TermnodeLeft->etiquette = "~";
    TermnodeRight->etiquette = "~";
    TermnodeLeft->left_child = leftbranch;
    TermnodeRight->left_child = rightbranch;
    
    currentNode->left_child = TermnodeLeft;
    currentNode->right_child = TermnodeRight;
}

void rule6(arbre currentNode){
    currentNode->etiquette = "&";
    arbre leftbranch = currentNode->left_child->left_child;
    arbre rightbranch = currentNode->left_child->right_child;
    
    arbre TermnodeLeft = initTree(Term);
    arbre TermnodeRight = initTree(Term);
    TermnodeLeft->etiquette = "~";
    TermnodeRight->etiquette = "~";
    TermnodeLeft->left_child = leftbranch;
    TermnodeRight->left_child = rightbranch;
    
    currentNode->left_child = TermnodeLeft;
    currentNode->right_child = TermnodeRight;
}

void rule7(arbre currentNode){
    arbre node = currentNode->left_child->left_child;
    currentNode->etiquette = node->etiquette;
    currentNode->left_child = node->left_child;
    currentNode->right_child = node->right_child;
    currentNode->state = node->state;
}
