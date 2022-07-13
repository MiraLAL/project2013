#pragma once

#define numqes 10   //問題数

struct Choice_Size
{
    #define No_Choice 1.0f
    #define Now_Choice 1.3f
};

//選択肢
enum Choice
{
    A = -1,
    B,
    C
};


struct Quiz_Data
{
     char question[500];       //クイズの問題
     char choiceWord_A[200];     //選択肢A
     char choiceWord_B[200];     //選択肢B
     char choiceWord_C[200];     //選択肢C

    Choice answer;                      //答え
    const char* qread;                 //問読み
};


