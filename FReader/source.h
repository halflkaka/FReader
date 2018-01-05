//
//  source.h
//  FReader
//
//  Created by 侍粲杰 on 2018/1/5.
//  Copyright © 2018年 侍粲杰. All rights reserved.
//

//Definir un arbre
typedef struct noeud * arbre;

//Statut d'un node, il s'agit de mon BNF
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
    int flag3;
    int flag4;
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
    node->flag3 = 0;
    node->flag4 = 0;
    node->flag5 = 0;
    node->flag6 = 0;
    node->flag7 = 0;
    return node;
}

//中序遍历打印
void print_tree(arbre root, int flag, FILE* Fp){
    if(root == NULL){return;}
    
    if(root->etiquette[0] == '~'){
        if(flag != 1){printf("(");fputs("(", Fp);}
        printf("%s",root->etiquette);
        fputs(root->etiquette, Fp);
        print_tree(root->left_child, 0, Fp);
        print_tree(root->right_child, 0, Fp);
        if(flag != 1){printf(")");fputs(")", Fp);}
    }
    else{
        if(root->left_child != NULL && flag != 1){
            printf("(");
            fputs("(", Fp);
        }
        print_tree(root->left_child, 0, Fp);
        if(root->etiquette != NULL){
            printf("%s",root->etiquette);
            fputs(root->etiquette, Fp);
        }
        print_tree(root->right_child, 0, Fp);
        if(root->right_child != NULL && flag != 1){
            printf(")");
            fputs(")", Fp);
        }
    }
}
int rule1(arbre currentnode);
int rule2(arbre currentnode);
int rule3(arbre currentnode);
int rule4(arbre currentnode);
int rule5(arbre currentnode);
int rule6(arbre currentnode);
int rule7(arbre currentnode);
pile Pile;
int transform(arbre root, arbre currentnode, arbre newRoot, arbre newNode, int* flag, FILE* Fp);
void TransformStart(arbre root, int* flag, FILE* Fp);

int term(int d, int f, arbre currentNode);
arbre node;

char operators[6] = {'~','|','&','=','(',')'}; //Six types d'operateurs

//Distinguer si char est un operateur
int isOperator(char ch){
    for(int i = 0;i < 6;i++){
        if(ch == operators[i]){
            return 1;
        }
    }
    return 0;
}
//Distinguer si char a la possibilite d'etre une variable
int isVariableStart(char ch){
    return ch >= 'a' && ch <= 'z';
}
//Distinguer les caracteres d'une variable. i.e a120 est une variable
int isVariable(char ch){
    if((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')){
        return 1;
    }else return 0;
}
//Definir les types lexique
typedef enum lexeme_t{
    variable,
    par_ou,
    par_fe,
    op,
    rev,//'~'
    error
} lexeme_t;
//Definir un lexeme
typedef struct lexeme{
    lexeme_t type;
    char valeur[20];
    int length;
} lexeme;
//Definir la liste des lexemes
typedef struct llexeme{
    lexeme token[50];
    int N;
} llexeme;
llexeme result;

//Determiner le type d'un lexeme
int isVariableType(lexeme lu){
    return lu.type == variable;
}
int isOpType(lexeme lu){
    return lu.type == op || lu.type == par_fe || lu.type == par_ou || lu.type == rev;
}
int isError(lexeme lu){
    return lu.type == error;
}

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
