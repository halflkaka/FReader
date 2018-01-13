//
//  moteur.c
//  FReader
//
//  Created by 侍粲杰 on 2017/12/5.
//  Copyright © 2018年 侍粲杰. All rights reserved.
//

#include <stdio.h>

//Appliquer les regles
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

//rule3 et rule4 sont pour modifier l'ordre de la formule
int rule3(arbre currentNode){
    if((currentNode->etiquette[0] == '|') && currentNode->flag3 != 1){
        arbre CopyLeft = newTree(currentNode->left_child);
        arbre CopyRight = newTree(currentNode->right_child);
        
        while (CopyLeft->state != Var && CopyLeft != NULL) {
            CopyLeft = CopyLeft->left_child;
        }
        while (CopyRight->state != Var && CopyRight != NULL) {
            CopyRight = CopyRight->left_child;
        }
        
        if(CopyLeft->etiquette[0] > CopyRight->etiquette[0]){
            arbre tempNode = currentNode->left_child;
            currentNode->left_child = currentNode->right_child;
            currentNode->right_child = tempNode;
            return 1;
        }else{return 0;}
    }else{return 0;}
}

int rule4(arbre currentNode){
    if((currentNode->etiquette[0] == '&') && currentNode->flag4 != 1){
        arbre CopyLeft = newTree(currentNode->left_child);
        arbre CopyRight = newTree(currentNode->right_child);
        
        while (CopyLeft->state != Var) {
            if(CopyLeft == NULL){break;}
            CopyLeft = CopyLeft->left_child;
        }
        while (CopyRight->state != Var) {
            if(CopyRight == NULL){break;}
            CopyRight = CopyRight->left_child;
        }
        
        if(CopyLeft->etiquette[0] > CopyRight->etiquette[0]){
            arbre tempNode = currentNode->left_child;
            currentNode->left_child = currentNode->right_child;
            currentNode->right_child = tempNode;
            return 1;
        }else{return 0;}
        
    }else{return 0;}
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

//Transformation. Rentrer 1 si une regle peut etre applique. Sinon rentrer 0.
int transform(arbre root, arbre currentnode, arbre newRoot, arbre newNode, int* flag, FILE *Fp){
    if(currentnode == NULL && newNode == NULL){return 0;}
    if(flag[1] == 1 && rule1(currentnode)){
        //Remarquer la node telle que la meme regle ne va pas etre applique deux fois
        currentnode->flag1 = 1;
        newNode->flag1 = 1;
        
        Pile.push(newRoot);//Enregistrer le premier arbre
        
        fputs("regle 1: ", Fp);
        
        print_tree(root,1, Fp);//Print le resultat
        
        fputs("\n", Fp);
        
        Pile.push(root);//Store the new Tree
        return 1;
    }else if(flag[2] == 1 && rule2(currentnode)){
        //Remarquer la node telle que la meme regle ne va pas etre applique deux fois
        currentnode->flag2 = 1;
        newNode->flag2 = 1;
        
        Pile.push(newRoot);
        
        fputs("regle 2: ", Fp);
        
        print_tree(root,1, Fp);//Print le resultat
        
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[3] == 1 && rule3(currentnode)){
        //Remarquer la node telle que la meme regle ne va pas etre applique deux fois
        currentnode->flag3 = 1;
        newNode->flag3 = 1;
        
        Pile.push(newRoot);
        
        fputs("regle 3: ", Fp);
        
        print_tree(root, 1, Fp);//Print le resultat
        
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[4] == 1 && rule4(currentnode)){
        //Remarquer la node telle que la meme regle ne va pas etre applique deux fois
        currentnode->flag4 = 1;
        newNode->flag4 = 1;
        
        Pile.push(newRoot);
        
        fputs("regle 4: ", Fp);
        
        print_tree(root, 1, Fp);//Print le resultat
        
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[5] == 1 && rule5(currentnode)){
        //Remarquer la node telle que la meme regle ne va pas etre applique deux fois
        currentnode->flag5 = 1;
        newNode->flag5 = 1;
        
        Pile.push(newRoot);
        
        fputs("regle 5: ", Fp);
        
        print_tree(root,1, Fp);//Print le resultat

        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[6] == 1 && rule6(currentnode)){
        //Remarquer la node telle que la meme regle ne va pas etre applique deux fois
        currentnode->flag6 = 1;
        newNode->flag6 = 1;
        
        Pile.push(newRoot);
        
        fputs("regle 6: ", Fp);
        
        print_tree(root,1, Fp);//Print le resultat

        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[7] == 1 && rule7(currentnode)){
        //Remarquer la node telle que la meme regle ne va pas etre applique deux fois
        currentnode->flag7 = 1;
        newNode->flag7 = 1;
        
        Pile.push(newRoot);
        
        fputs("regle 7: ", Fp);
        
        print_tree(root,1, Fp);//Print le resultat

        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if(transform(root, currentnode->left_child, newRoot, newNode->left_child, flag, Fp)){//DFS dans la branche gauche pour appliquer les regles
        return 1;
    }else if(transform(root, currentnode->right_child, newRoot, newNode->right_child, flag, Fp)){//DFS dans la branche droite pour appliquer les regles
        return 1;
    }else{
        return 0;
    }
}

//Commencer la transformation a chaque arbre dans la liste
void TransformStart(arbre root, int* flag, FILE* Fp){
    Pile.push(root);
    while (!Pile.vide()) {//Quand il y a des arbres
        arbre currentRoot = Pile.pop();
        arbre newRoot = newTree(currentRoot);//Copier l'arbre tel que l'information de l'arbre original ne va pas changer a cause de l'operation du pointeur
        transform(currentRoot, currentRoot, newRoot, newRoot,flag, Fp);
    }
}

//La fonction pour appliquer le moteur
void moteur(int* flag, int type){
    FILE *Fp;
    if(type == 0){Fp = fopen("result.txt", "w");}
    else{Fp = fopen("result.txt", "a");fputs("\n", Fp);}
    
    arbre root = node->left_child;//Obtenir le root de l'arbre de la formule
    print_tree(root,1,Fp);
    fputs("\n", Fp);
    
    //Definition du Pile
    Pile.pop = pop_aux;
    Pile.push = push_aux;
    Pile.vide = vide_aux;
    
    TransformStart(root, flag, Fp);
}



