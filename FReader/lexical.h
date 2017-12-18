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
    int flag1;
    int flag2;
    int flag5;
    int flag6;
    int flag7;
};
typedef struct pile{
    void (*push)(arbre);
    int (*vide)(void);
    arbre (*pop)(void);
}pile;
arbre Root[100];
int hauteur = 0;
arbre pop_aux(void){
    if(hauteur == 0){
        exit(1);
    }else{
        arbre result = Root[--hauteur];
        Root[hauteur] = NULL;
        return result;
    }
}
void push_aux(arbre root){
    Root[hauteur++] = root;
}
int vide_aux(){
    return hauteur == 0;
}

//Initialization
arbre initTree(enum State name){
    arbre node = malloc(sizeof(struct noeud));
    node->etiquette = NULL;
    node->left_child = NULL;
    node->right_child = NULL;
    node->state = name;
    node->flag1 = 0;
    node->flag2 = 0;
    node->flag5 = 0;
    node->flag6 = 0;
    node->flag7 = 0;
    return node;
}


int rule1(arbre currentnode);
int rule2(arbre currentnode);
int rule5(arbre currentnode);
int rule6(arbre currentnode);
int rule7(arbre currentnode);
pile Pile;
int transform(arbre root, arbre currentnode, arbre newRoot, arbre newNode);
void TransformStart(arbre root);

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
void print_tree(arbre root, int flag){
    if(root == NULL){return;}
    
    if(root->etiquette[0] == '~'){
        if(flag != 1){printf("(");}
        printf("%s",root->etiquette);
        print_tree(root->left_child, 0);
        print_tree(root->right_child, 0);
        if(flag != 1){printf(")");}
    }
    else{
        if(root->left_child != NULL && flag != 1){
            printf("(");
        }
        print_tree(root->left_child, 0);
        if(root->etiquette != NULL){
            printf("%s",root->etiquette);
        }
        print_tree(root->right_child, 0);
        if(root->right_child != NULL && flag != 1){
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
    print_tree(root,1);
    printf("\n");
    
    regfree(&myregex);
    
    Pile.pop = pop_aux;
    Pile.push = push_aux;
    Pile.vide = vide_aux;
    
    TransformStart(root);
    
    
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
int rule1(arbre currentNode){
    if(currentNode->etiquette[0] == '=' && currentNode->flag1 != 1){
        currentNode->etiquette = "|";
        arbre leftbranch = currentNode->left_child;
        arbre Termnode = initTree(Term);
        Termnode->etiquette = "~";
        Termnode->left_child = leftbranch;
        currentNode->left_child = Termnode;
        return 1;
    }
    return 0;
}

int rule2(arbre currentNode){
    if(currentNode->etiquette[0] == '|' && currentNode->right_child->etiquette[0] == '&' && currentNode->flag2 != 1){
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
        return 1;
    }
    return 0;
}

int rule5(arbre currentNode){
    if(currentNode->etiquette[0] == '~' && currentNode->left_child->etiquette[0] == '&' && currentNode->flag5 != 1){
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
        return 1;
    }
    return 0;
}

int rule6(arbre currentNode){
    if(currentNode->etiquette[0] == '~' && currentNode->left_child->etiquette[0] == '|' && currentNode->flag6 != 1){
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
        return 1;
    }
    return 0;
}

int rule7(arbre currentNode){
    if(currentNode->etiquette[0] == '~' && currentNode->left_child->etiquette[0] == '~' && currentNode->flag7 != 1){
        arbre node = currentNode->left_child->left_child;
        currentNode->etiquette = node->etiquette;
        currentNode->left_child = node->left_child;
        currentNode->right_child = node->right_child;
        currentNode->state = node->state;
        return 1;
    }
    return 0;
}

//Copy a new Tree
arbre newTree(arbre root){
    if(root == NULL){return NULL;}
    arbre newRoot = initTree(Term);
    newRoot->etiquette = root->etiquette;
    newRoot->flag1 = root->flag1;
    newRoot->flag2 = root->flag2;
    newRoot->flag5 = root->flag5;
    newRoot->flag6 = root->flag6;
    newRoot->flag7 = root->flag7;
    newRoot->left_child = newTree(root->left_child);
    newRoot->right_child = newTree(root->right_child);
    return newRoot;
}

//Transform
int transform(arbre root, arbre currentnode, arbre newRoot, arbre newNode){
    if(currentnode == NULL && newNode == NULL){return 0;}
    if(rule1(currentnode)){
        //Mark the node so that the same rule will not be applied twice
        currentnode->flag1 = 1;
        newNode->flag1 = 1;
        
        Pile.push(newRoot);//Store the primary tree
        
        printf("regle 1: ");
        print_tree(root,1);
        printf("\n");
        
        Pile.push(root);//Store the new Tree
        return 1;
    }else if (rule2(currentnode)){
        currentnode->flag2 = 1;
        newNode->flag2 = 1;
        
        Pile.push(newRoot);
        
        printf("regle 2: ");
        print_tree(root,1);
        printf("\n");
        
        Pile.push(root);
        return 1;
    }else if (rule5(currentnode)){
        currentnode->flag5 = 1;
        newNode->flag5 = 1;
        
        Pile.push(newRoot);
        
        printf("regle 5: ");
        print_tree(root,1);
        printf("\n");
        
        Pile.push(root);
        return 1;
    }else if (rule6(currentnode)){
        currentnode->flag6 = 1;
        newNode->flag6 = 1;
        
        Pile.push(newRoot);
        
        printf("regle 6: ");
        print_tree(root,1);
        printf("\n");
        
        Pile.push(root);
        return 1;
    }else if (rule7(currentnode)){
        currentnode->flag7 = 1;
        newNode->flag7 = 1;
        
        Pile.push(newRoot);
        
        printf("regle 7: ");
        print_tree(root,1);
        printf("\n");
        
        Pile.push(root);
        return 1;
    }else if(transform(root, currentnode->left_child, newRoot, newNode->left_child)){//DFS to apply rules
        return 1;
    }else if(transform(root, currentnode->right_child, newRoot, newNode->right_child)){//DFS to apply rules
        return 1;
    }else{
        return 0;
    }
}

void TransformStart(arbre root){
    Pile.push(root);
    while (!Pile.vide()) {//When there are trees
        arbre currentRoot = Pile.pop();
        arbre newRoot = newTree(currentRoot);//Copy the tree so that the original information will not be changed
        transform(currentRoot, currentRoot, newRoot, newRoot);
    }
}
