//
//  moteur.c
//  FReader
//
//  Created by 侍粲杰 on 2018/1/5.
//  Copyright © 2018年 侍粲杰. All rights reserved.
//

#include <stdio.h>

//Ordre
int depth(arbre root){
    int LeftDepth = 0, RightDepth = 0;
    arbre newRoot1 = newTree(root);
    while(newRoot1 != NULL){
        LeftDepth ++;
        newRoot1 = newRoot1->left_child;
    }
    arbre newRoot2 = newTree(root);
    while (newRoot2 != NULL) {
        RightDepth++;
        newRoot2 = newRoot2->right_child;
    }
    if(LeftDepth >= RightDepth){
        return LeftDepth;
    }else{
        return RightDepth;
    }
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

int rule3(arbre currentNode){
    if((currentNode->etiquette[0] == '|') &&
       (depth(currentNode->left_child) > depth(currentNode->right_child)) && currentNode->flag3 != 1){
        arbre tempNode = currentNode->left_child;
        currentNode->left_child = currentNode->right_child;
        currentNode->right_child = tempNode;
        return 1;
    }else if((currentNode->etiquette[0] == '|') &&
             (depth(currentNode->left_child) == depth(currentNode->right_child)) && currentNode->flag3 != 1){
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
    if((currentNode->etiquette[0] == '&') &&
       (depth(currentNode->left_child) > depth(currentNode->right_child)) && currentNode->flag4 != 1){
        arbre tempNode = currentNode->left_child;
        currentNode->left_child = currentNode->right_child;
        currentNode->right_child = tempNode;
        return 1;
    }else if((currentNode->etiquette[0] == '&') &&
             (depth(currentNode->left_child) == depth(currentNode->right_child)) && currentNode->flag4 != 1){
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

//Transform
int transform(arbre root, arbre currentnode, arbre newRoot, arbre newNode, int* flag, FILE *Fp){
    if(currentnode == NULL && newNode == NULL){return 0;}
    if(flag[1] == 1 && rule1(currentnode)){
        //Mark the node so that the same rule will not be applied twice
        currentnode->flag1 = 1;
        newNode->flag1 = 1;
        
        Pile.push(newRoot);//Store the primary tree
        
        //        FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/result.txt", "a");
        printf("regle 1: ");
        fputs("regle 1: ", Fp);
        
        print_tree(root,1, Fp);
        
        printf("\n");
        fputs("\n", Fp);
        
        Pile.push(root);//Store the new Tree
        return 1;
    }else if(flag[2] == 1 && rule2(currentnode)){
        currentnode->flag2 = 1;
        newNode->flag2 = 1;
        
        Pile.push(newRoot);
        
        //        FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/result.txt", "a");
        printf("regle 2: ");
        fputs("regle 2: ", Fp);
        
        print_tree(root,1, Fp);
        
        printf("\n");
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[3] == 1 && rule3(currentnode)){
        currentnode->flag3 = 1;
        newNode->flag3 = 1;
        
        Pile.push(newRoot);
        
        //        FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/result.txt", "a");
        printf("regle 3: ");
        fputs("regle 3: ", Fp);
        
        print_tree(root, 1, Fp);
        
        printf("\n");
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[4] == 1 && rule4(currentnode)){
        currentnode->flag4 = 1;
        newNode->flag4 = 1;
        
        Pile.push(newRoot);
        
        //        FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/result.txt", "a");
        printf("regle 4: ");
        fputs("regle 4: ", Fp);
        
        print_tree(root, 1, Fp);
        
        printf("\n");
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[5] == 1 && rule5(currentnode)){
        currentnode->flag5 = 1;
        newNode->flag5 = 1;
        
        Pile.push(newRoot);
        
        //        FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/result.txt", "a");
        printf("regle 5: ");
        fputs("regle 5: ", Fp);
        
        print_tree(root,1, Fp);
        printf("\n");
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[6] == 1 && rule6(currentnode)){
        currentnode->flag6 = 1;
        newNode->flag6 = 1;
        
        Pile.push(newRoot);
        
        //        FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/result.txt", "a");
        printf("regle 6: ");
        fputs("regle 6: ", Fp);
        
        print_tree(root,1, Fp);
        printf("\n");
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if (flag[7] == 1 && rule7(currentnode)){
        currentnode->flag7 = 1;
        newNode->flag7 = 1;
        
        Pile.push(newRoot);
        
        //        FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/result.txt", "a");
        printf("regle 7: ");
        fputs("regle 7: ", Fp);
        
        print_tree(root,1, Fp);
        printf("\n");
        fputs("\n", Fp);
        
        Pile.push(root);
        return 1;
    }else if(transform(root, currentnode->left_child, newRoot, newNode->left_child, flag, Fp)){//DFS to apply rules
        return 1;
    }else if(transform(root, currentnode->right_child, newRoot, newNode->right_child, flag, Fp)){//DFS to apply rules
        return 1;
    }else{
        return 0;
    }
}

void TransformStart(arbre root, int* flag, FILE* Fp){
    Pile.push(root);
    while (!Pile.vide()) {//When there are trees
        arbre currentRoot = Pile.pop();
        arbre newRoot = newTree(currentRoot);//Copy the tree so that the original information will not be changed
        transform(currentRoot, currentRoot, newRoot, newRoot,flag, Fp);
    }
}

void moteur(int* flag){
    FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/result.txt", "w");
    
    arbre root = node->left_child;
    print_tree(root,1,Fp);
    printf("\n");
    fputs("\n", Fp);
    
    
    Pile.pop = pop_aux;
    Pile.push = push_aux;
    Pile.vide = vide_aux;
    
    TransformStart(root, flag, Fp);
}



