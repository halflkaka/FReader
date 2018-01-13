//
//  anasynt.c
//  FReader
//
//  Created by 侍粲杰 on 2017/11/25.
//  Copyright © 2018年 侍粲杰. All rights reserved.
//

#include <stdio.h>
#include "source.h"

llexeme result;
arbre node;

//Definir la fonction de l'analyse syntaxique
void anasyn(int n){
    node = initTree(Term);
    if(term(0, n-1, node) == 1){//Si on bien construit l'arbre de la formule, ca veut dire que c'est syntaxiquement correcte.
        printf("syntaxiquement correcte.\n");
    }else{
        printf("syntaxiquement erreur.\n");
    }
    printf("formule initial avec parenthese bien placee:\n");
    print_tree_initial(node->left_child, 1);
    printf("\n");
}
//BNF
int Op(int d, arbre currentNode){
    int resultat = (result.token[d].type == op);
    if(resultat == 1){
        //Initialiser OP node
        currentNode->etiquette = result.token[d].valeur;
    }
    return resultat;
}
int OpC(int d, arbre currentNode){
    int resultat = (condition.token[d].type == op);
    if(resultat == 1){
        //Initialize OP node
        currentNode->etiquette = condition.token[d].valeur;
    }
    return resultat;
}
int OpR(int d, arbre currentNode){
    int resultat = (regleresult.token[d].type == op);
    if(resultat == 1){
        //Initialize OP node
        currentNode->etiquette = regleresult.token[d].valeur;
    }
    return resultat;
}

int Vb(int d, int f, arbre currentNode){
    int resultat = ((result.token[d].type == variable) && d == f);
    if(resultat == 1){
        //Initialiser Variable node
        arbre Varnode = initTree(Var);
        Varnode->etiquette = result.token[d].valeur;
        //Ajouter dans l'arbre
        if(currentNode->left_child == NULL){
            currentNode->left_child = Varnode;
        }else{
            currentNode->right_child = Varnode;
        }
    }
    return resultat;
}
int VbC(int d, int f, arbre currentNode){
    int resultat = ((condition.token[d].type == variable) && d == f);
    if(resultat == 1){
        //Initialize Variable node
        arbre Varnode = initTree(Var);
        Varnode->etiquette = condition.token[d].valeur;
        //add in the tree
        if(currentNode->left_child == NULL){
            currentNode->left_child = Varnode;
        }else{
            currentNode->right_child = Varnode;
        }
    }
    return resultat;
}
int VbR(int d, int f, arbre currentNode){
    int resultat = ((regleresult.token[d].type == variable) && d == f);
    if(resultat == 1){
        //Initialize Variable node
        arbre Varnode = initTree(Var);
        Varnode->etiquette = regleresult.token[d].valeur;
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
    //Initialiser Facteur node
    int resultat = Vb(d,f,currentNode);//facteur = variable
    if (!resultat){
        //Initialiser Term node
        if(result.token[d].type == par_ou && result.token[f].type == par_fe &&
           term(d+1, f-1, currentNode)){ //facteur = '('term')'
            //C'est un facteur!
            resultat = 1;
        }
    }
    return resultat;
}
int facteurC(int d, int f, arbre currentNode){
    //Initialize Facteur node
    int resultat = VbC(d,f,currentNode);//facteur = variable
    if (!resultat){
        //Initialize Term node
        if(condition.token[d].type == par_ou && condition.token[f].type == par_fe &&
           termC(d+1, f-1, currentNode)){ //facteur = '('term')'
            //It's a facteur!
            resultat = 1;
        }
    }
    return resultat;
}
int facteurR(int d, int f, arbre currentNode){
    //Initialize Facteur node
    int resultat = VbR(d,f,currentNode);//facteur = variable
    if (!resultat){
        //Initialize Term node
        if(regleresult.token[d].type == par_ou && regleresult.token[f].type == par_fe &&
           termR(d+1, f-1, currentNode)){ //facteur = '('term')'
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
int termC(int d, int f, arbre currentNode){
    int i = f - 1, b = facteurC(d, f, currentNode);
    arbre Facnode = initTree(Facteur);
    while(b == 0 && i >= d){
        if(OpC(i, Facnode) && facteurC(d, i-1, Facnode) && facteurC(i+1, f, Facnode)){
            //term = facteur OP facteur
            if(currentNode->left_child == NULL){
                currentNode->left_child = Facnode;
            }else{
                currentNode->right_child = Facnode;
            }
            b = 1;
        }else if((condition.token[d].valeur[0] == '~') && facteurC(d+1, f, Facnode)){
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
int termR(int d, int f, arbre currentNode){
    int i = f - 1, b = facteurR(d, f, currentNode);
    arbre Facnode = initTree(Facteur);
    while(b == 0 && i >= d){
        if(OpR(i, Facnode) && facteurR(d, i-1, Facnode) && facteurR(i+1, f, Facnode)){
            //term = facteur OP facteur
            if(currentNode->left_child == NULL){
                currentNode->left_child = Facnode;
            }else{
                currentNode->right_child = Facnode;
            }
            b = 1;
        }else if((regleresult.token[d].valeur[0] == '~') && facteurR(d+1, f, Facnode)){
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
//Copier un nouveau arbre
arbre newTree(arbre root){
    if(root == NULL){return NULL;}
    arbre newRoot = initTree(root->state);
    newRoot->etiquette = root->etiquette;
    newRoot->flag1 = root->flag1;
    newRoot->flag2 = root->flag2;
    newRoot->flag3 = root->flag3;
    newRoot->flag4 = root->flag4;
    newRoot->flag5 = root->flag5;
    newRoot->flag6 = root->flag6;
    newRoot->flag7 = root->flag7;
    newRoot->left_child = newTree(root->left_child);
    newRoot->right_child = newTree(root->right_child);
    return newRoot;
}
