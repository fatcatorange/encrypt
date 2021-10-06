#include <stdio.h>
#include <corecrt_malloc.h>
#include <string.h>
#include<Windows.h>
#include <string>
#include <iostream>
using namespace std;
void encrypt(char* fileName, int cut)
{
    FILE* fp = NULL;
    int size = 0;
    fp = fopen(fileName, "rb");
    if (NULL == fp)
    {
        printf("failed!\n");
        return;
    }
    printf("open file %s sucess!\n", fileName);
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    printf("this file has %d words!\n", size);

    char* tmp;
    int read_size;
    //tmp = (char*)malloc((size ) * sizeof(char));
    //read_size = fread(tmp,sizeof(char),size,fp);
    //tmp[size-1] = '\0';
    //printf("output string:\n%s\n",tmp);
    char ch;
    char code = 'a';
    char temp[256];
    for (int i = 0; i < cut; i++)
    {
        temp[i] = fileName[i];

    }
    temp[cut] = '\0';
    strcat(temp, "test");
    FILE* fpw = fopen(temp, "wb");
    while (!feof(fp))
    {
        ch = fgetc(fp);
        fputc(ch, fpw);
        fputc(code, fpw);;
    }

    fclose(fpw);
    fclose(fp);
    char del[256] = "del ";
    strcat(del, fileName);
    system(del);
    rename(temp, fileName);
}
void getCurrentDirectory(int size, char buff[256])
{
    char newBuff[256] = { 0 };
    LPWSTR buff2 = (LPWSTR)(LPCWSTR)buff;
    GetCurrentDirectory(256, buff2);
    strcpy(buff, (const char*)buff2);
    for (int i = 0; i < 256; i += 2)
    {

        if (buff[i] == '\0')
        {
            newBuff[i / 2] = '\0';
            break;
        }
        newBuff[i / 2] = buff[i];

    }
    strcpy(buff, newBuff);

}
void findFile(char* pathName)
{
    char findFileName[256];
    memset(findFileName, 0, 256);
    sprintf(findFileName, "%s\\*.*", pathName);
    printf("the filename that you want to find:%s\n", findFileName);

    WIN32_FIND_DATA findData;
    HANDLE hFile = FindFirstFile((LPWSTR)(LPCWSTR)findFileName, &findData);
    printf("%s", (LPWSTR)(LPCWSTR)findFileName);
    //printf("%s", strcat(findFileName, "\\out.txt"));
    if (INVALID_HANDLE_VALUE == hFile)
    {
        // printf("%d,%s",INVALID_HANDLE_VALUE);
        printf("failed!\n");
        return;
    }

}
char* change(LPCTSTR buff)
{
    char newBuff[256];
    for (int i = 0; i < 256; i++)
    {

        if (buff[i] == '\0')
        {
            newBuff[i] = '\0';
            break;
        }
        newBuff[i] = buff[i];

    }
    return newBuff;
}
void findFolder(LPCTSTR folder, char fol[256])
{
    wchar_t Lfol[256];
    for (int i = 0; i < 256; i++)
    {
        Lfol[i] = fol[i];
        if (fol[i] == '\0')
            break;
    }
    folder = Lfol;
    for (int i = 0; i < 256; i++)
    {
        printf("%c", folder[i]);
        if (folder[i] == '\0')
            break;
    }
    printf("\n");
    WIN32_FIND_DATA findData;
    LPCTSTR lpFileName = folder;
    HANDLE hFile = FindFirstFile(folder, &findData);
    int cut = 0;
    int cut2 = 0;
    // fopen("C:\\test\\gg.txt","w");

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("there are no files in this folder\n");
        return;
    }
    while (FindNextFile(hFile, &findData))
    {
        if (findData.cFileName[0] == '.')
            continue;
        if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
        {
            for (int i = 0; i < 256; i++)
            {
                if (fol[i] == '*')
                {
                    cut = i;
                    fol[i] = '\0';
                    break;
                }

            }
            printf("find folder:");
            //strcat(fol,"");
            strcat(fol, change(findData.cFileName));
            strcat(fol, "\\*.*");
            findFolder(((LPCWSTR)(fol)), fol);
            fol[cut] = '*';
            fol[cut + 1] = '.';
            fol[cut + 2] = '*';
            fol[cut + 3] = '\0';
        }
        else {
            printf("find file:");
            for (int i = 0; i < 256; i++)
            {
                if (fol[i] == '*')
                {
                    cut2 = i;
                    break;
                }
            }
            int counter = 0;
            for (int i = cut2; i < 256; i++)
            {
                fol[i] = findData.cFileName[counter];
                if (findData.cFileName[counter] == '\0')
                    break;
                counter++;
            }
            printf("%s\n", fol);
            encrypt(fol, cut2);
            fol[cut2] = '*';
            fol[cut2 + 1] = '.';
            fol[cut2 + 2] = '*';
            fol[cut2 + 3] = '\0';

        }
    }
}
int main()
{
    //jiami((char*)"C:\\test\\example.txt");

    char buff[256] = { 0 };
    char fol[256] = "C:\\test\\*.*";
    LPCTSTR folder = L"C:\\test\\*.*";
    findFolder(folder, fol);
    getCurrentDirectory(256, buff);
    printf("current directory %s\n", buff);
    //findFile(buff);
    //fopen("");
}