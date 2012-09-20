/**
 *  �ļ���:model.h
 *  ����:����ģ��ͷ�ļ�
 *  ����:����
 **/
#include "datastruct.h"

//ȫ�����
int emptyAll (OLNode *head);

//��ʼ��ʮ������
int initOLNode (OLNode **head);

//ʡ��֧����غ���
int insertProvince (OLNode *head,PROVINCEDATA data);
PROVINCEDATA* fetchProvinceByIndex (OLNode *head,int province_index);
PROVINCEDATA** fetchALLProvince (OLNode *head);
int emptyProvinceByIndex (OLNode *head,int province_index);
int modifyProvinceByIndex (OLNode *head,int province_index,PROVINCEDATA newData);

//����֧����غ���
int deleteCase (OLNode *nodeProvince);
int insertCaseByIndex (OLNode *head,int province_index,CASEDATA data);
CASEDATA* fetchCaseByIndex (OLNode *head,int province_index,int case_index);
CASEDATA** fetchAllCase (OLNode *head,int province_index);
int CountCase (OLNode *head,int province_index);
int deleteCaseByIndex (OLNode *head,int province_index,int case_index);
int modifyCaseByIndex (OLNode *head,int province_index,int case_index,CASEDATA newData);

//����֧����غ���
int deleteNews (OLNode *nodeCase);
int insertNewsByIndex (OLNode *head,int province_index,int case_index,NEWSDATA data);
NEWSDATA* fetchNewsByIndex (OLNode *head,int province_index,int case_index,int news_index);
NEWSDATA** fetchALLNews (OLNode *head,int province_index,int case_index);
int CountNews (OLNode *head,int province_index,int case_index);
int deleteNewsByIndex (OLNode *head,int province_index,int case_index,int news_index);
int modifyNewsByIndex (OLNode *head,int province_index,int case_index,int news_index,NEWSDATA newData);


//����ͳ��
StatisticsData1** StatisticsFunction1(OLNode *head,LPWSTR key,int type);
StatisticsData2** StatisticsFunction2(OLNode *head,LPWSTR key);
StatisticsData3** StatisticsFunction3(OLNode *head);
StatisticsData4** StatisticsFunction4(OLNode *head);

//����ת������
int provinceTranslate(LPWSTR data);
int crimeTranslate(LPWSTR data);
int admin_LevelTranslate(LPWSTR data);
int mediumTranslate(LPWSTR data);
LPWSTR timeTranslateToString(SYSTEMTIME *time,LPWSTR string);
SYSTEMTIME* StringTranslateToTime(LPWSTR string,SYSTEMTIME *time);

//�ַ�������
LPWSTR lstrpbrk(LPCWSTR cs,LPCWSTR ct);  //�ַ���ƥ��

//������
void quicksort1(StatisticsData1 **data,int low,int high);
void quicksort2(StatisticsData2 **data,int low,int high);
void quicksort3(StatisticsData3 **data,int low,int high);
void quicksort4(StatisticsData4 **data,int low,int high);

//���뵼���ļ�
int importFile(OLNode *head,LPWSTR path);
int exportFile(OLNode *head,LPWSTR path);
