/**
 *  �ļ���:datastruct.h
 *  ����:���ݽṹ����
 *  ����:����
 **/

#ifndef _TCHAR_H_
#include "tchar.h"
#endif
#ifndef _WINDOWS_H_
#include "windows.h"
#endif

//���ݽṹ����
#define MAXPROVINCE 34
typedef struct dataPro    /*ʡ������*/
{
    TCHAR name[11];   //ʡ����
    TCHAR discipline[15];  //�ͼ���
    TCHAR telephone[15]; //��ϵ�绰
	
}PROVINCEDATA;
typedef struct dataCase  /*��������*/
{
    TCHAR id_case[12];  //�������
    TCHAR date[10];  //��������
    TCHAR province[15]; //����ʡ��
    TCHAR defendant[15]; //������
    TCHAR admin_level[15]; //��������
    TCHAR crime[15]; //��������
    TCHAR money[15]; //�永���
    TCHAR punish[15]; //���´���
}CASEDATA;
typedef struct dataNews  /*��������*/
{
    TCHAR id_case[12]; //�������
    TCHAR report_date[15]; //��������
    TCHAR medium_category[15]; //ý�����
    TCHAR medium_name[20]; //ý������
    TCHAR url[50]; //��������
}NEWSDATA;
typedef union DATA  //��������
{
    PROVINCEDATA province_data;  //ʡ������
    CASEDATA case_data;  //��������
    NEWSDATA news_data;  //��������
}DATA,*pDATA;
typedef struct OLNode   /*ʮ������*/
{
    int index; //����
    int data_flag;  //���ݱ�־ 0Ϊͷָ�� 1Ϊʡ�� 2Ϊ���� 3Ϊ����
    DATA *data;  //����
    struct OLNode *right; //�Һ������
    struct OLNode *down; //�º������
}OLNode, *OLink;

typedef struct stat1   //ͳ����1
{
	unsigned short year; //���
	int case_number; //������
	double total_amount; //�永�ܽ��
}StatisticsData1;
typedef struct stat2  //ͳ����2
{
	unsigned short year; //���
	int death_sentence; //����
	int life_imprisonment; //����
	int imprisonment1_5; //1-5������
	int imprisonment6_10;//6-10������
	int imprisonment11_more;//11�꼰11����������
}StatisticsData2;
typedef struct stat3  //ͳ����3
{
	TCHAR id_case[12]; //�������
	TCHAR admin_level[15]; //��������
	TCHAR crime[15]; //��������
	TCHAR money[15]; //�永���
	int report_number; //������
}StatisticsData3;
typedef struct stat4  //ͳ����4
{
	int year; //���
	int case_number; //������
	double total_amount;//�永�ܽ��
	int death_sentence;//����
	int life_imprisonment;//����
	int imprisonment1_5;//1-5������
	int imprisonment6_10;//6-10������
	int imprisonment11_more;//11�꼰11����������
}StatisticsData4;
