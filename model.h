/**
 *  文件名:model.h
 *  功能:数据模型头文件
 *  作者:杜宇
 **/
#include "datastruct.h"

//全部清空
int emptyAll (OLNode *head);

//初始化十字链表
int initOLNode (OLNode **head);

//省份支链相关函数
int insertProvince (OLNode *head,PROVINCEDATA data);
PROVINCEDATA* fetchProvinceByIndex (OLNode *head,int province_index);
PROVINCEDATA** fetchALLProvince (OLNode *head);
int emptyProvinceByIndex (OLNode *head,int province_index);
int modifyProvinceByIndex (OLNode *head,int province_index,PROVINCEDATA newData);

//案件支链相关函数
int deleteCase (OLNode *nodeProvince);
int insertCaseByIndex (OLNode *head,int province_index,CASEDATA data);
CASEDATA* fetchCaseByIndex (OLNode *head,int province_index,int case_index);
CASEDATA** fetchAllCase (OLNode *head,int province_index);
int CountCase (OLNode *head,int province_index);
int deleteCaseByIndex (OLNode *head,int province_index,int case_index);
int modifyCaseByIndex (OLNode *head,int province_index,int case_index,CASEDATA newData);

//新闻支链相关函数
int deleteNews (OLNode *nodeCase);
int insertNewsByIndex (OLNode *head,int province_index,int case_index,NEWSDATA data);
NEWSDATA* fetchNewsByIndex (OLNode *head,int province_index,int case_index,int news_index);
NEWSDATA** fetchALLNews (OLNode *head,int province_index,int case_index);
int CountNews (OLNode *head,int province_index,int case_index);
int deleteNewsByIndex (OLNode *head,int province_index,int case_index,int news_index);
int modifyNewsByIndex (OLNode *head,int province_index,int case_index,int news_index,NEWSDATA newData);


//数据统计
StatisticsData1** StatisticsFunction1(OLNode *head,LPWSTR key,int type);
StatisticsData2** StatisticsFunction2(OLNode *head,LPWSTR key);
StatisticsData3** StatisticsFunction3(OLNode *head);
StatisticsData4** StatisticsFunction4(OLNode *head);

//类型转换函数
int provinceTranslate(LPWSTR data);
int crimeTranslate(LPWSTR data);
int admin_LevelTranslate(LPWSTR data);
int mediumTranslate(LPWSTR data);
LPWSTR timeTranslateToString(SYSTEMTIME *time,LPWSTR string);
SYSTEMTIME* StringTranslateToTime(LPWSTR string,SYSTEMTIME *time);

//字符处理函数
LPWSTR lstrpbrk(LPCWSTR cs,LPCWSTR ct);  //字符串匹配

//排序函数
void quicksort1(StatisticsData1 **data,int low,int high);
void quicksort2(StatisticsData2 **data,int low,int high);
void quicksort3(StatisticsData3 **data,int low,int high);
void quicksort4(StatisticsData4 **data,int low,int high);

//导入导出文件
int importFile(OLNode *head,LPWSTR path);
int exportFile(OLNode *head,LPWSTR path);
