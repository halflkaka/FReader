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
    arbre middle_child;
    arbre right_child;
};

/*arbre assemble(enum State name, arbre * leftc, arbre * rightc){
    arbre newTree = malloc(sizeof(struct noeud));
    newTree->state = name;
    newTree->left_child = *leftc;
    newTree->right_child = *rightc;
    return newTree;
};*/

//Initialization
arbre initTree(enum State name){
    arbre node = malloc(sizeof(struct noeud));
    node->etiquette = NULL;
    node->left_child = NULL;
    node->right_child = NULL;
    node->middle_child = NULL;
    node->state = name;
    return node;
}

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
    print_tree(root->left_child);
    print_tree(root->middle_child);
    print_tree(root->right_child);
    if(root->state != Facteur && root->state != Term){
        printf("%s",root->etiquette);
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
    
    print_tree(node);
    printf("\n");
    
    regfree(&myregex);
    
    return 0;
}


//syntaxique
//If '~' is in the right node, have to move it to left
void adjust(arbre currentNode){
    if(currentNode->right_child->etiquette[0] == '~'){
        arbre temp = currentNode->left_child;
        currentNode->left_child = currentNode->right_child;
        currentNode->right_child = temp;
    }
}

//BNF
int Op(int d, arbre currentNode){
    int resultat = (result.token[d].type == op);
    if(resultat == 1){
        //Initialize OP node
        arbre Opnode = initTree(OP);
        Opnode->etiquette = result.token[d].valeur;
        //add in the tree
        currentNode->middle_child = Opnode;
    }
    return resultat;
}

int Vb(int d, int f, arbre currentNode){
    int resultat = (result.token[d].type == variable);
    if(resultat == 1){
        //Initialize Variable node
        arbre Varnode = initTree(Var);
        Varnode->etiquette = result.token[d].valeur;
        //add in the tree
        currentNode->left_child = Varnode;
    }
    return resultat;
}

int facteur(int d, int f, arbre currentNode){
    //Initialize Facteur node
    arbre Facnode = initTree(Facteur);
    int resultat = Vb(d,f,Facnode) && d == f;//facteur = variable
    if (!resultat){
        //Initialize Term node
        arbre Termnode = initTree(Term);
        if(result.token[d].type == par_ou && result.token[f].type == par_fe &&
           term(d+1, f-1, Termnode)){ //facteur = '('term')'
            //Connect facteur&term2
            Facnode->middle_child = Termnode;
            //Connect term1&facteur
            if (currentNode->left_child == NULL){
                currentNode->left_child = Facnode;
            }else if (currentNode->right_child == NULL){
                currentNode->right_child = Facnode;
            }
            //Add parenthese of Facteur node
            arbre leftc = initTree(Par);
            arbre rightc = initTree(Par);
            leftc->etiquette = result.token[d].valeur;
            rightc->etiquette = result.token[f].valeur;
            Facnode->left_child = leftc;
            Facnode->right_child = rightc;
            //It's a facteur!
            resultat = 1;
        }
    }else {
        if (currentNode->left_child == NULL){
            currentNode->left_child = Facnode;
        }else if (currentNode->right_child == NULL){
            currentNode->right_child = Facnode;
        }
    }
    return resultat;
}

int term(int d, int f, arbre currentNode){
    int i = f - 1, b = facteur(d, f, currentNode);
    while(b == 0 && i >= d){
        if(Op(i, currentNode) && facteur(d, i-1, currentNode) && facteur(i+1, f, currentNode)){
            //term = facteur OP facteur
            b = 1;
        }else if((result.token[d].valeur[0] == '~') && facteur(d+1, f, currentNode)){
            //term = '~'facteur
            arbre RevNode = initTree(Rev);
            RevNode->etiquette = "~";
            if(currentNode->left_child == NULL){
                currentNode->left_child = RevNode;
            }else{
                currentNode->right_child = RevNode;
            }
            adjust(currentNode);
            b = 1;
        }
        i --;
    }
    return b;
}


