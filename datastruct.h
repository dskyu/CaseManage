/**
 *  文件名:datastruct.h
 *  功能:数据结构定义
 *  作者:杜宇
 **/

#ifndef _TCHAR_H_
#include "tchar.h"
#endif
#ifndef _WINDOWS_H_
#include "windows.h"
#endif

//数据结构声明
#define MAXPROVINCE 34
typedef struct dataPro    /*省份数据*/
{
    TCHAR name[11];   //省份名
    TCHAR discipline[15];  //纪检人
    TCHAR telephone[15]; //联系电话
	
}PROVINCEDATA;
typedef struct dataCase  /*案件数据*/
{
    TCHAR id_case[12];  //案件编号
    TCHAR date[10];  //立案日期
    TCHAR province[15]; //所属省份
    TCHAR defendant[15]; //被告人
    TCHAR admin_level[15]; //行政级别
    TCHAR crime[15]; //犯罪类型
    TCHAR money[15]; //涉案金额
    TCHAR punish[15]; //刑事处罚
}CASEDATA;
typedef struct dataNews  /*新闻数据*/
{
    TCHAR id_case[12]; //案件编号
    TCHAR report_date[15]; //报告日期
    TCHAR medium_category[15]; //媒体类别
    TCHAR medium_name[20]; //媒体名称
    TCHAR url[50]; //内容索引
}NEWSDATA;
typedef union DATA  //数据联合
{
    PROVINCEDATA province_data;  //省份数据
    CASEDATA case_data;  //案件数据
    NEWSDATA news_data;  //新闻数据
}DATA,*pDATA;
typedef struct OLNode   /*十字链表*/
{
    int index; //索引
    int data_flag;  //数据标志 0为头指针 1为省份 2为案件 3为新闻
    DATA *data;  //数据
    struct OLNode *right; //右后继链域
    struct OLNode *down; //下后继链域
}OLNode, *OLink;

typedef struct stat1   //统计项1
{
	unsigned short year; //年份
	int case_number; //案件数
	double total_amount; //涉案总金额
}StatisticsData1;
typedef struct stat2  //统计项2
{
	unsigned short year; //年份
	int death_sentence; //死刑
	int life_imprisonment; //无期
	int imprisonment1_5; //1-5年有期
	int imprisonment6_10;//6-10年有期
	int imprisonment11_more;//11年及11年以上有期
}StatisticsData2;
typedef struct stat3  //统计项3
{
	TCHAR id_case[12]; //案件编号
	TCHAR admin_level[15]; //行政级别
	TCHAR crime[15]; //犯罪类型
	TCHAR money[15]; //涉案金额
	int report_number; //报道数
}StatisticsData3;
typedef struct stat4  //统计项4
{
	int year; //年份
	int case_number; //案件数
	double total_amount;//涉案总金额
	int death_sentence;//死刑
	int life_imprisonment;//无期
	int imprisonment1_5;//1-5年有期
	int imprisonment6_10;//6-10年有期
	int imprisonment11_more;//11年及11年以上有期
}StatisticsData4;
