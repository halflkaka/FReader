//
//  main.c
//  FReader
//
//  Created by 侍粲杰 on 2017/12/2.
//  Copyright © 2017年 侍粲杰. All rights reserved.
//

#include <stdio.h>
#include <regex.h>
#include "analex.c"
//#include "syntaxique.h"


int main(int argc, const char * argv[]) {
    char regle[100], condition[100], resultat[100], formula[100];
    int flag[8];
    memset(flag, 0, sizeof(flag));
    
    FILE *Fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/formula1.txt", "r");
    if(NULL == Fp){
        printf("failed to open formula.txt\n");
        return 1;
    }
    while (!feof(Fp)) {
        memset(formula, 0, sizeof(formula));
        fgets(formula, sizeof(formula)-1, Fp);
        //Get length
        int length = 0;
        for(int i = 0;formula[i] != '\n' && formula[i] != '\0';i++){
            length++;
        }
        if(formula[length] != '\n'){printf("\n");}
        
        FILE *fp = fopen("/Users/shicanjie/SJTU/TPLA/FReader/FReader/regle.txt", "r");
        if(NULL == fp)
        {
            printf("failed to open regle.txt\n");
            return 1;
        }
        
        while(!feof(fp))
        {
            memset(regle, 0, sizeof(regle));
            fgets(regle, sizeof(regle) - 1, fp); // 包含了换行符
            //        printf("%s", regle);
            //Get length
            int length = 0;
            for(int i = 0;regle[i] != '\n' && regle[i] != '\0';i++){
                length++;
            }
            if(regle[length] != '\n'){printf("\n");}
            //Get position of :
            int pos = 0;
            for(int i = 0;i<length;i++){
                if(regle[i] == ':'){pos = i;break;}
            }
            //Get condition
            for(int i = 4;i<pos;i++){condition[i] = regle[i];}
            //Get resultat
            for(int i = 0;i<length-pos-1;i++){
                resultat[i] = regle[pos+1+i];
            }
            //Get Flag
            if(regle[1] == '1'){flag[1] = 1;}
            else if (regle[1] == '2'){flag[2] = 1;}
            else if (regle[1] == '3'){flag[3] = 1;}
            else if (regle[1] == '4'){flag[4] = 1;}
            else if (regle[1] == '5'){flag[5] = 1;}
            else if (regle[1] == '6'){flag[6] = 1;}
            else if (regle[1] == '7'){flag[7] = 1;}
            
            //Get nodes of regles
            //arbre ConRoot = ConditionScaner(condition, pos);
            //arbre ResRoot = ResultatScaner(resultat, length-pos-1);
        }
        
        fclose(fp);
        int n = FScaner(formula, flag);
        anasyn(n);
        moteur(flag);
    }
    fclose(Fp);
    
    return 0;
}
