#pragma once

#define numqes 10   //��萔
#define GENRE  3    //�W��������

struct Choice_Size
{
    #define No_Choice 1.0f
    #define Now_Choice 1.3f
};

//�I����
enum Choice
{
    A = -1,
    B,
    C
};


struct Quiz_Data
{
     char question[500];       //�N�C�Y�̖��
     char choiceWord_A[200];     //�I����A
     char choiceWord_B[200];     //�I����B
     char choiceWord_C[200];     //�I����C

    Choice answer;                      //����
    const char* qread;                 //��ǂ�

};

struct Quiz_Data_Picture
{
    char questions[500];       //�N�C�Y�̖��
    char choiceWord_A[200];     //�I����A
    char choiceWord_B[200];     //�I����B
    char choiceWord_C[200];     //�I����C

    Choice answer;                      //����
    const char* qread;                 //��ǂ�

};


