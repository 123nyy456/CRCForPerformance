#include "stdafx.h"
#include <string>
#include <fstream>
#include <conio.h>

#include <windows.h> 

unsigned char* GenerateTestdata(int N, int W, int H)
{
    unsigned char* testdata = new unsigned char[N * W * H * 4];
    for (int i = 0; i < N; i++)
    {
        unsigned char a = 0;
        unsigned char r = 1;
        unsigned char g = 2;
        unsigned char b = 3;
        for (int j = 0; j < W * H; j++)
        {
            testdata[i * W * H * 4 + j * 4] = a++;
            testdata[i * W * H * 4 + j * 4 + 1] = r++;
            testdata[i * W * H * 4 + j * 4 + 2] = g++;
            testdata[i * W * H * 4 + j * 4 + 3] = b++;
        }
    }
    return testdata;
}

unsigned char* GenerateResultdata(int N, int W, int H)
{
    unsigned char* resultdata = new unsigned char[N * W * H * 4];
    for (int i = 0; i < N; i++)
    {
        unsigned char a = 0;
        unsigned char r = 1;
        unsigned char g = 2;
        unsigned char b = 3;
        for (int j = 0; j < W * H; j++)
        {
            resultdata[i * W * H * 4 + j] = a++;
            resultdata[i * W * H * 4 + W * H + j] = r++;
            resultdata[i * W * H * 4 + W * H * 2 + j] = g++;
            resultdata[i * W * H * 4 + W * H * 3 + j] = b++;
        }
    }
    return resultdata;
}

/// ��Ҫ���ȵķ��������Բ�������������Ƿ���Խ����쳣���
/// ʾ����
/// ����N=3 W=2 H=2 X=1
/// ԭʼ�����ļ����¸�ʽ����
///                image0                          image1                          image2
///                |                               |                               |
///                pixel0  pixel1  pixel2  pixel3  pixel0  pixel1  pixel2  pixel3  pixel0  pixel1  pixel2  pixel3
///                |       |       |       |       |       |       |       |       |       |       |       |
/// �������ļ�ֵΪ 0 1 2 3 1 2 3 4 2 3 4 5 3 4 5 6 0 1 2 3 0 1 2 3 0 1 2 3 0 1 2 3 0 1 2 3 1 2 3 4 2 3 4 5 3 4 5 6
/// 
/// ������Ϊ
///                image0                          image1                          image2
///                |                               |                               |
///                pixel0  pixel1  pixel2  pixel3  pixel0  pixel1  pixel2  pixel3  pixel0  pixel1  pixel2  pixel3
///                |       |       |       |       |       |       |       |       |       |       |       |
///                0 1 2 3 1 2 3 4 2 3 4 5 3 4 5 6 0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3 0 1 2 3 1 2 3 4 2 3 4 5 3 4 5 6

char* Process(char* buffer, int W, int H, char* newbuffer)
{
    int pixelCount = W * H;
    int fileLength = pixelCount * 4;
    int aIndex = 0;
    int rIndex = pixelCount + aIndex;
    int gIndex = pixelCount + rIndex;
    int bIndex = pixelCount + gIndex;
    for (int j = 0; j < pixelCount; j++)
    {
        int j4 = 4 * j;
        newbuffer[aIndex++] = buffer[j4];
        newbuffer[rIndex++] = buffer[j4 + 1];
        newbuffer[gIndex++] = buffer[j4 + 2];
        newbuffer[bIndex++] = buffer[j4 + 3];
    }
    return newbuffer;
}


int main(int argc, char* argv[])
{
    int N = 50; // �������ļ�������ͼ����
    int W = 1920;// ͼ����
    int H = 1080; // ͼ��߶�


    unsigned char* pSourceData = GenerateTestdata(N, W, H);
            
    // ���ȴ�����ʼ
    LARGE_INTEGER BeginTime; 
    LARGE_INTEGER Frequency; 
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);

    char* pResultData = new char[N * W * H * 4];
    for (int i = 0; i < N; i++)
    {
        int offset = i * W * H * 4;
        Process((char*)(pSourceData + offset), W, H, pResultData + offset);
    }

    LARGE_INTEGER EndTime; 
    QueryPerformanceCounter(&EndTime); 
    double dTime = (double)(EndTime.QuadPart - BeginTime.QuadPart)/Frequency.QuadPart; 
    printf("\nARGB Cost [%.2f] ms\n", dTime*1000);

    // ���ȴ������

    unsigned char* pResultStandard = GenerateResultdata(N, W, H);
    if (memcmp(pResultData, pResultStandard, N * W * H * 4) == 0)
    {
        printf("True.\n");
    }
    else
    {
        printf("False. \n");
    }
    
    return 0;
}

