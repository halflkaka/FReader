//
//  main.c
//  FReader
//
//  Created by 侍粲杰 on 2017/11/20.
//  Copyright © 2017年 侍粲杰. All rights reserved.
//

#include <stdio.h>
#include <regex.h>
#include "analex.c"



int main(int argc, const char * argv[]) {
    char regle[100], condition[100], resultat[100], formula[100];
    int flag[8];//Pour determiner quelles regles sont disponibles
    memset(flag, 0, sizeof(flag));
    int fois = 0;
    
    FILE *Fp = fopen("formula.txt", "r");
    if(NULL == Fp){
        printf("failed to open formula.txt\n");
        return 1;
    }
    while (!feof(Fp)) {
        memset(formula, 0, sizeof(formula));//Initialisation
        fgets(formula, sizeof(formula)-1, Fp);
        printf("La formule est: %s\n", formula);
        
        //Ouvrier le fichier des regles
        FILE *fp = fopen("regle.txt", "r");
        if(NULL == fp)
        {
            printf("failed to open regle.txt\n");
            return 1;
        }
        while(!feof(fp))
        {
            memset(regle, 0, sizeof(regle));
            fgets(regle, sizeof(regle) - 1, fp); //Obtenir une regle

            //Obtenir la longueur de la regle
            int length = 0;
            for(int i = 0;regle[i] != '\n' && regle[i] != '\0';i++){
                length++;
            }
            
            //Obtenir la position de ":"
            int pos = 0;
            for(int i = 0;i<length;i++){
                if(regle[i] == ':'){pos = i;break;}
            }
            
            //Obtenir la condition (avant ":")
            for(int i = 4;i<pos;i++){condition[i] = regle[i];}
            
            //Obtenir le resultat (apres ":")
            for(int i = 0;i<length-pos-1;i++){
                resultat[i] = regle[pos+1+i];
            }
            
            //Trouver les regles disponibles
            if(regle[1] == '1'){flag[1] = 1;}
            else if (regle[1] == '2'){flag[2] = 1;}
            else if (regle[1] == '3'){flag[3] = 1;}
            else if (regle[1] == '4'){flag[4] = 1;}
            else if (regle[1] == '5'){flag[5] = 1;}
            else if (regle[1] == '6'){flag[6] = 1;}
            else if (regle[1] == '7'){flag[7] = 1;}
            
            //Obtenir deux arbres de la regle
            //arbre ConRoot = ConditionScaner(condition, pos);
            //arbre ResRoot = ResultatScaner(resultat, length-pos-1);
        }
        
        fclose(fp);
        
        int n = FScaner(formula, flag);//Analyse lexical
        
        anasyn(n);//Analyse syntaxique
        
        moteur(flag, fois);//Transformation selon la regle
        
        fois ++;
        printf("\n");
    }
    fclose(Fp);
    
    return 0;
}
