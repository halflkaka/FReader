//
//  syntaxique.h
//  FReader
//
//  Created by 侍粲杰 on 2017/12/6.
//  Copyright © 2017年 侍粲杰. All rights reserved.
//

//#include <stdio.h>
#include "lexical.h"


int Number(char ch){
    return (ch >= '0' && ch <= '9');
}

int Op(char ch1, char ch2){
    return (ch1 == '&' || ch1 == '|' || ch1 == '~' || (ch1 == '=' && ch2 == '>'));
}

int Character(char ch){
    return ch >= 'a' && ch <= 'z';
}

int Vb(char* ch){
    
}
