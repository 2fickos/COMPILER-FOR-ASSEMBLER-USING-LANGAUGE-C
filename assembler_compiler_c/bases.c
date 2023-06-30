#include "bases.h"

char base32[32] = { '!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v' };

char* decToBase32(int num)
{

    int j, count = 0, NewBase = 0, pos = 1, i = 0;
    unsigned int abs = num;
    char temp;
    char bin[10];
    char* base_32 = calloc(3 , sizeof(char));

    while (i < 10)
    {
        bin[i] = abs % 2 + '0';
        abs = abs / 2;
        i++;
    }
    for (j = 0; j < 5; j++)
    {
        temp = bin[9 - j];
        bin[9 - j] = bin[j];
        bin[j] = temp;
    }

    if (!base_32)
        return "`Dynamic allocatio error.\0";
    for (i = 9; i > -1; i--)
    {
        NewBase += pow(2, count) * (bin[i] - '0');
        count++;
        if (count == 5)
        {
            base_32[pos] = base32[NewBase];
            pos--;
            count = 0;

            NewBase = 0;
        }
    }
    base_32[2] = '\0';
    return base_32;

}

int binToDec(char* bin)
{
    int i;
    int num = 0;

    for (i = 9; i > -1; i--)
    {
        num += pow(2, 9 - i) * (bin[i] - '0');
    }
    if (bin[0] == '1')
        return 0 - (pow(2, 10) - num);
    return num;
}


char* decToBin(int num)
{
    unsigned int i = 0, abs = num;
    int j;
    char temp;
    char* bin = (char*)calloc(11 , sizeof(char));
    if (!bin)
        return "`Dynamic allocatio error.\0";
    while (i < 10)
    {
        bin[i] = abs % 2 + '0';
        abs = abs / 2;
        i++;
    }
    for (j = 0; j < 5; j++)
    {
        temp = bin[9 - j];
        bin[9 - j] = bin[j];
        bin[j] = temp;
    }

    bin[10] = '\0';
    return bin;

}


char* binToBase32(char* bin)
{
    int count = 0;
    int num = 0;
    int i = 0;
    int pos = 1;
    char* base_32 = calloc(3 , sizeof(char));
    if (!base_32)
        return "`Dynamic allocatio error.\0";
    for (i = 9; i > -1; i--)
    {
        num += pow(2, count) * (bin[i] - '0');
        count++;
        if (count == 5)
        {
            base_32[pos] = base32[num];
            pos--;
            count = 0;

            num = 0;
        }
    }
    base_32[2] = '\0';
    return base_32;
}

char* connectchars(const char* s1, const char* s2)
{
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result)
    {
        strcpy(result, s1);
        strcat(result, s2);
        return result;
    }
    else
    {
        return NULL;
    }
}

int textToNum(char* text, int from, int end)
{
    int i = 0;
    int result = 0, start = from;
    if (text[start] == '-' || text[start] == '+')
        start++;
    while (end - i >= start)
    {
        result += pow(10, i) * ((int)(text[end - i] - '0'));
        i++;
    }
    return (text[from] == '-') ? -result : result;
}

