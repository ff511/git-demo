#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#define bool int
#define true 1
#define false 0
void ltrim(char *str, const char ch);
void rtrim(char *str, const char ch);
int caculate(const char *input, int *result);
char get_operant(char *data, int* op1, int* op2);
int skip_work(char *data);
jmp_buf jmp;

int main()
{
    int result;
    char input[1024] = {0};
    puts("***********self made english caculator*****************");
    puts("input exit to go out!");
    while(1)
    {
        setjmp(jmp);
        puts("please input an expression:");
        if(gets(input) == NULL)
        {
            puts("get input data failed!");
            return -1;
        }

        ltrim(input, ' ');
        rtrim(input, ' ');
        if(strcmp(input, "exit") == 0)
            break;
        if(skip_work(input) < 0)
            puts("skip failed!");
        if(caculate(input, &result) == 0)
        {
            printf("The result is:%s = %d\n", input, result);
        }
        else
        {
            printf("cannot caculate!\n");
        }

    }
    puts("out of the caculator!");
    return 0;
}

void ltrim(char *str, const char ch)
{
    char *ptr;
    for (ptr = str; *ptr; ptr ++)
    {
        if(*ptr != ch)
            break;
    }
    strcpy(str, ptr);
}
void rtrim(char *str, const char ch)
{
    int i;
    int len = strlen(str);
    for (i = len - 1; i >= 0; i--)
    {
        if(str[i] != ch)
            break;
    }
    str[i + 1] = '\0';
}
int caculate(const char *str, int *result)
{
    char *input = (char*)malloc(strlen(str) + 1);
    strcpy(input, str);
    int op1, op2;
    char op;
    float div;
    op = get_operant(input, &op1, &op2);

    free(input);
    input = NULL;
    switch(op)
    {
    case '+':
        *result = op1 + op2;
        break;
    case '-':
        *result = op1 - op2;
        break;
    case '*':
        *result = op1 * op2;
        break;
    case '/':
        if(op2 == 0)
        {
            puts("divisor illegal!");
            return -1;
        }
        else
            div = (float)op1 / op2;
        printf("The result is:%s = %0.3f\n", str, div);
        longjmp(jmp, 1);
        break;
    default:
        return -1;
    }
    return 0;
}
char get_operant(char *data, int* op1, int* op2)
{
    static const char *op_list[]={"+", "-", "*", "/"};
    int i;
    char *temp = data;
    char *change;
    char *tok = NULL;
    char *steptok = NULL;
    while(*temp != '\0')
    {
        if(*temp == ' ')
        {
            change = temp;
            while(*change != '\0')
            {
                *change = *(change + 1);
                change ++;
            }
        }
        temp++;
    }
    char data_mp[100] = {0};
    //char data_mp[100] = {0};
    int sum = 0;
    strcpy(data_mp, data);
    char tok_one[50] = {0};
    char tok_two[50] = {0};
    char tok_one_temp[50] = {0};
    char tok_two_temp[50] = {0};
    for (i = 0; i < 4; i++)
    {
        tok = strtok(data, op_list[i]);
        if (strcmp(data, data_mp) != 0)
            break;
    }
    strcpy(tok_one, tok);
    strcpy(tok_one_temp, tok);
    tok = strtok(NULL, op_list[i]);
    strcpy(tok_two, tok);
    strcpy(tok_two_temp, tok);
    //printf("tok_one:%s\n", tok_one);
    steptok = strtok(tok_one, "and");

    if(strcmp(tok_one_temp, steptok) != 0)
    {
        sum = atoi(steptok);
        steptok = strtok(NULL, "and");
        sum += atoi(steptok);
        *op1 = sum;
    }
    else
        *op1 = atoi(tok_one);
    steptok = strtok(tok_two, "and");
    if(strcmp(tok_two_temp, steptok) != 0)
    {
        sum = atoi(steptok);
        steptok = strtok(NULL, "and");
        sum += atoi(steptok);
        *op2 = sum;
    }
    else
        *op2 = atoi(tok_two);
    //printf("%d,%d,%c\n",*op1,*op2,*op_list[i]);
    return *op_list[i];
}
int skip_work(char *data)
{
    static const char *symbol[8]={
        "plus", "minus", "multiply", "divide", "+", "-", "*", "/"};
    static const char *link[8]={
        "hundred", "thousand", "million", "billion", "00", "000", "000000, 000000000"};
    static const char *decade[16] ={
        "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety",
        "20", "30", "40", "50", "60", "70", "80", "90"};
    static const char *digit[20]={
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
        "ten", "eleven", "twelve", "thirteen", "fourteen","fifteen", "sixteen",
        "seventeen", "eighteen", "nineteen"};
    bool flag = false;
    char *toks = " ";
    char *tok = strtok(data, toks);
    int i, j, m ,n;
    char temp_tok[20] = {0};
    char data_temp[50]= {0};
    while(tok)
    {
        for(i = 0; i < 20; i++)
            if(!strcmp(tok, digit[i]))
            {
                if(flag == true)
                {
                    i += atoi(decade[m + 8]);
                }
                sprintf(temp_tok, "%d", i);
                break;
            }
        for(j = 0; j < 4; j++)
        {
            if(!strcmp(tok, link[j]))
            {
                strcat(data_temp, link[j + 4]);
                //sprintf(temp_tok, i*atoi(j+4));
                break;
            }
        }
        if(j != 4)
        {
            tok = strtok(NULL, toks);
            continue;
        }
        for(m = 0; m < 8; m++)
        {
            if(!strcmp(tok, decade[m]))
            {
                flag = true;
                //strcat(data_temp, decade[m + 8]);
                break;
            }
        }
        if (m != 8)
        {
            tok = strtok(NULL, toks);
            continue;
        }
        for(n = 0; n < 4;n++)
            if(!strcmp(tok, symbol[n]))
            {
                strcpy(temp_tok, symbol[n+4]);
                break;
            }
        if(i == 20 && j == 4 && m == 8 && n == 4)
            strcpy(temp_tok, tok);
        //if (!strcmp(temp_tok, data))
        //        strcpy(data, temp_tok);
        //else
        //  printf("dddd11:%s\n", temp_tok);
        strcat(data_temp, temp_tok);
        tok = strtok(NULL, toks);
        flag = false;
    }

    strcpy(data, data_temp);
    return 0;
}
