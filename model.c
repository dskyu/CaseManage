/**
 *  文件名:model.c
 *  功能:十字链表操作
 *  作者:杜宇
 **/
#include "model.h"

int emptyAll (OLNode *head)
{
	int i;
	for (i = 0;i < 34;i++)
	{
		if (emptyProvinceByIndex(head,i) == -1) return -1;
	}
	return 0;
}
int initOLNode (OLNode **head)
{
    int i;
	TCHAR* tp[34] = {TEXT("安微"),TEXT("澳门"),TEXT("北京"),TEXT("重庆"),TEXT("福建"),TEXT("广东"),TEXT("广西"),TEXT("贵州"),TEXT("甘肃"),TEXT("黑龙江"),TEXT("河北"),
                    TEXT("河南"),TEXT("湖南"),TEXT("湖北"),TEXT("海南"),TEXT("吉林"),TEXT("江西"),TEXT("江苏"),TEXT("辽宁"),TEXT("宁夏"),TEXT("内蒙"),TEXT("青海"),
                    TEXT("上海"),TEXT("山东"),TEXT("山西"),TEXT("陕西"),TEXT("四川"),TEXT("天津"),TEXT("台湾"),TEXT("西藏"),TEXT("香港"),TEXT("新疆"),TEXT("云南"),TEXT("浙江")};
	PROVINCEDATA province;
    (*head) = (OLink) malloc (sizeof(OLNode));
    (*head)->data_flag = 0;
    (*head)->right = NULL;
    (*head)->down = NULL;
    (*head)->index = -1;
    (*head)->data = NULL;
	
    for (i = 0;i < MAXPROVINCE;i++)
    {
		lstrcpy(province.name,tp[i]);
		lstrcpy(province.discipline,TEXT("未录入"));
		lstrcpy(province.telephone,TEXT("未录入"));
        insertProvince(*head,province);
    }
    return 0;
}
int insertProvince (OLNode *head,PROVINCEDATA data)
{
    int index = 0;
    OLink newProvince,p = head;
    newProvince = (OLink) malloc (sizeof(OLNode));
    while (p->right)
    {
        p = p->right;
        index++;
    }
    newProvince->data = (pDATA) malloc (sizeof(DATA));
    newProvince->index = index;
    newProvince->data_flag = 1;
    newProvince->right = NULL;
    newProvince->down = NULL;
    newProvince->data->province_data = data;
    p->right = newProvince;
    return 0;
}
PROVINCEDATA* fetchProvinceByIndex (OLNode *head,int province_index)
{
    int x = 0;
    OLink p = head;
    if (province_index < 0) return NULL;
    while (province_index+1 != x)
    {
        if (!p->right) return NULL;
        p = p->right;
        x++;
    }
    return &(p->data->province_data);
}
PROVINCEDATA** fetchALLProvince (OLNode *head)
{
    int x = 0;
    OLink p = head;
    PROVINCEDATA **data = NULL;
    data = (PROVINCEDATA **) malloc ((MAXPROVINCE+1)*sizeof(PROVINCEDATA *));
    while (p->right)
    {
        p = p->right;
        if (p->data == NULL) data[x] = NULL;
        else data[x] = &(p->data->province_data);
        x++;
    }
    data[x] = NULL;
    return data;
}
int emptyProvinceByIndex (OLNode *head,int province_index)
{
    int x = 0;
    OLink p = head;
    if (province_index < 0) return -1;
    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }
    deleteCase(p);
	lstrcpy(p->data->province_data.telephone,TEXT("未录入"));
	lstrcpy(p->data->province_data.discipline,TEXT("未录入"));
    p->down = NULL;
    return 0;
}
int deleteCase (OLNode *nodeProvince)
{
    OLink p,q;
    p = nodeProvince->down;
    while (p)
    {
        q = p->down;
        deleteNews(p);
        free(p);
        p = q;
    }
    return 0;
}
int modifyProvinceByIndex (OLNode *head,int province_index,PROVINCEDATA newData)
{
    int x = 0;
    OLink p = head;
    if (province_index < 0) return -1;
    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }
	p->data->province_data = newData;
    return 0;
}
int insertCaseByIndex (OLNode *head,int province_index,CASEDATA data)
{
    int x = 0;
    int index = -1;
    OLink p = head,q,newCase;
    if (province_index < 0) return -1;
    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }

    newCase = (OLink) malloc (sizeof(OLNode));
    q = p;
    if (q->down == NULL)
    {
        q->down = newCase;
        index++;
    }
    else
    {
        while (q->down)
        {
             q = q->down;
             index++;
        }
        index++;
        q->down = newCase;
    }

    newCase->data = (pDATA) malloc (sizeof(DATA));
    newCase->index = index;
    newCase->data_flag = 2;
    newCase->down = NULL;
    newCase->right = NULL;
    newCase->data->case_data = data;
    return 0;
}
int CountCase (OLNode *head,int province_index)
{
    int x = 0;
    OLink p = head;
    int sum = 0;

    if (province_index < 0) return -1;

    while (province_index+1 != x)
    {
        if (!p->right) return 0;
        p = p->right;
        x++;
    }

    while (p->down)
    {
        p = p->down;
        sum++;
    }
    return sum;
}
CASEDATA* fetchCaseByIndex (OLNode *head,int province_index,int case_index)
{
    int x = 0;
    OLink p = head;

    if (province_index < 0) return NULL;
    if (case_index < 0) return NULL;

    while (province_index+1 != x)
    {
        if (!p->right) return NULL;
        p = p->right;
        x++;
    }
    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return NULL;
        p = p->down;
        x++;
    }

    return &(p->data->case_data);
}
CASEDATA** fetchAllCase (OLNode *head,int province_index)

{
    int x = 0;
    int count = 0;
    OLink p = head;
    CASEDATA **data = NULL;
    if (province_index < 0) return NULL;
    while (province_index+1 != x)
    {
        if (!p->right) return NULL;
        p = p->right;
        x++;
    }
    if (!p->down) return NULL;
    if ((count = CountCase(head,province_index)) == 0) return NULL;

    data = (CASEDATA **) malloc ((count+1)*sizeof(CASEDATA *));
    x = 0;
    while (p->down)
    {
        p = p->down;
        data[x] = &(p->data->case_data);
        x++;
    }
    data[x] = NULL;
    return data;
}
int modifyCaseByIndex (OLNode *head,int province_index,int case_index,CASEDATA newData)
{
    int x = 0;
    OLink p = head;

    if (province_index < 0) return -1;
    if (case_index < 0) return -1;

    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }
    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return -1;
        p = p->down;
        x++;
    }
    p->data->case_data = newData;
    return 0;
}
int deleteCaseByIndex (OLNode *head,int province_index,int case_index)
{
    int x = 0;
    OLink p = head,q;

    if (province_index < 0) return -1;
    if (case_index < 0) return -1;

    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }
    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return -1;
        q = p;
        p = p->down;
        x++;
    }
    q->down = p->down;
    deleteNews (p);
    free(p);
    return 0;
}
int deleteNews (OLNode *nodeCase)
{
    OLink p,q;
    p = nodeCase->right;
    while (p)
    {
        q = p->right;
        free(p);
        p = q;
    }
    return 0;
}
int insertNewsByIndex (OLNode *head,int province_index,int case_index,NEWSDATA data)
{
    int x = 0;
    int index = -1;
    OLink p = head,q,newNews;
    if (province_index < 0) return -1;
    if (case_index<0) return -1;
    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }

    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return -1;
        p = p->down;
        x++;
    }

    newNews = (OLink) malloc (sizeof(OLNode));
    q = p;
    if (q->right == NULL)
    {
        q->right = newNews;
        index++;
    }
    else
    {
        while (q->right)
        {
             q = q->right;
             index++;
        }
        index++;
        q->right = newNews;
    }

    newNews->data = (pDATA) malloc (sizeof(DATA));
    newNews->index = index;
    newNews->data_flag = 3;
    newNews->down = NULL;
    newNews->right = NULL;
    newNews->data->news_data = data;
    return 0;
}
NEWSDATA* fetchNewsByIndex (OLNode *head,int province_index,int case_index,int news_index)
{
    int x = 0;
    OLink p = head;

    if (province_index < 0) return NULL;
    if (case_index < 0) return NULL;
    if (news_index < 0) return NULL;

    while (province_index+1 != x)
    {
        if (!p->right) return NULL;
        p = p->right;
        x++;
    }
    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return NULL;
        p = p->down;
        x++;
    }
    x = 0;
    while (news_index+1 != x)
    {
        if (!p->right) return NULL;
        p = p->right;
        x++;
    }
    return &(p->data->news_data);
}
NEWSDATA** fetchALLNews (OLNode *head,int province_index,int case_index)
{
    int x = 0;
    int count = 0;
    OLink p = head;
    NEWSDATA **data = NULL;
    if (province_index < 0) return NULL;
    if (case_index < 0) return NULL;
    while (province_index+1 != x)
    {
        if (!p->right) return NULL;
        p = p->right;
        x++;
    }
    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return NULL;
        p = p->down;
        x++;
    }

    if (!p->right) return NULL;
    if ((count = CountNews(head,province_index,case_index)) == 0) return NULL;

    data = (NEWSDATA **) malloc ((count+1)*sizeof(NEWSDATA *));
    x = 0;
    while (p->right)
    {
        p = p->right;
        data[x] = &(p->data->news_data);
        x++;
    }
    data[x] = NULL;
    return data;
}
int CountNews (OLNode *head,int province_index,int case_index)
{
    int x = 0;
    int sum = 0;
    OLink p = head;

    if (province_index < 0) return -1;
    if (case_index < 0) return -1;

    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }
    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return -1;
        p = p->down;
        x++;
    }

    while(p->right)
    {
        p = p->right;
        sum++;
    }
    return sum;
}
int modifyNewsByIndex (OLNode *head,int province_index,int case_index,int news_index,NEWSDATA newData)
{
    int x = 0;
    OLink p = head;

    if (province_index < 0) return -1;
    if (case_index < 0) return -1;
    if (news_index < 0) return -1;

    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }
    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return -1;
        p = p->down;
        x++;
    }
    x = 0;
    while (news_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }
    p->data->news_data = newData;
    return 0;
}
int deleteNewsByIndex (OLNode *head,int province_index,int case_index,int news_index)
{
    int x = 0;
    OLink p = head,q;

    if (province_index < 0) return -1;
    if (case_index < 0) return -1;
    if (news_index < 0) return -1;

    while (province_index+1 != x)
    {
        if (!p->right) return -1;
        p = p->right;
        x++;
    }
    x = 0;
    while (case_index+1 != x)
    {
        if (!p->down) return -1;
        p = p->down;
        x++;
    }
    x = 0;
    while (news_index+1 != x)
    {
        if (!p->right) return -1;
        q = p;
        p = p->right;
        x++;
    }
    q->right = p->right;
    free(p);
    return 0;
}

StatisticsData1** StatisticsFunction1(OLNode *head,LPWSTR key,int type)
{
	int i = 0,j;
    OLink p = head,q;
	SYSTEMTIME time;
	StatisticsData1 **data;
	data = (StatisticsData1**) malloc (sizeof(StatisticsData1*)*50);
	
    while (p->right)
    {
        p = p->right;
		q = p;
		while(q->down)
		{
			q = q->down;
			if(lstrcmp(key,type == 1 ? q->data->case_data.crime : q->data->case_data.province) == 0)
			{
				StringTranslateToTime(q->data->case_data.date,&time);
				for (j = 0;j < i;j++)
				{
					if(data[j]->year == time.wYear) break;
				}
				if (j >= i)
				{
					data[i] = (StatisticsData1*) malloc (sizeof(StatisticsData1));
					data[j]->year = time.wYear;
					data[j]->case_number = 1;
					data[j]->total_amount = _wtof(q->data->case_data.money);
					i++;
				}
				else
				{
					data[j]->case_number++;
					data[j]->total_amount += _wtof(q->data->case_data.money);
				}
			}
		}
    }
	data[i] = NULL;
	return data;
}
StatisticsData2** StatisticsFunction2(OLNode *head,LPWSTR key)
{
	int i = 0,j;
	int punish;
    OLink p = head,q;
	SYSTEMTIME time;
	StatisticsData2 **data;
	data = (StatisticsData2**) malloc (sizeof(StatisticsData2*)*50);
	
    while (p->right)
    {
        p = p->right;
		q = p;
		while(q->down)
		{
			q = q->down;
			if(lstrcmp(key,q->data->case_data.admin_level) == 0)
			{
				StringTranslateToTime(q->data->case_data.date,&time);
				for (j = 0;j < i;j++)
				{
					if(data[j]->year == time.wYear) break;
				}
				punish = _wtoi(q->data->case_data.punish);
				if (j >= i)
				{
					data[i] = (StatisticsData2*) malloc (sizeof(StatisticsData2));
					data[j]->year = time.wYear;
					data[j]->life_imprisonment = 0;
					data[j]->death_sentence = 0;
					data[j]->imprisonment1_5 = 0;
					data[j]->imprisonment6_10 = 0;
					data[j]->imprisonment11_more = 0;
					if (punish >= 100) data[j]->life_imprisonment = 1;
					else if (punish <= 0) data[j]->death_sentence = 1;
					else if (punish >= 1 && punish <= 5) data[j]->imprisonment1_5 = 1;
					else if (punish >= 6 && punish <= 10) data[j]->imprisonment6_10 = 1;
					else if (punish >= 11 && punish < 100) data[j]->imprisonment11_more = 1;
					i++;
				}
				else
				{
					if (punish >= 100) data[j]->life_imprisonment++;
					else if (punish <= 0) data[j]->death_sentence++;
					else if (punish >= 1 && punish <= 5) data[j]->imprisonment1_5++;
					else if (punish >= 6 && punish <= 10) data[j]->imprisonment6_10++;
					else if (punish >= 11 && punish < 100) data[j]->imprisonment11_more++;
				}
			}
		}
    }
	data[i] = NULL;
	return data;
}
StatisticsData3** StatisticsFunction3(OLNode *head)
{
	int i = 0,count;
    int province_index = 0,case_index = 0;
	OLink p = head,q;

	StatisticsData3 **data;
	data = (StatisticsData3**) malloc (sizeof(StatisticsData3*)*50);

	province_index = 0;
    while (p->right)
    {
        p = p->right;
		q = p;
		case_index = 0;
		while(q->down)
		{
			q = q->down;
			count = CountNews(head,province_index,case_index);
			case_index++;
			if (count <= 0) continue;
			data[i] = (StatisticsData3*) malloc (sizeof(StatisticsData3));
			wsprintf(data[i]->id_case,TEXT("%s"),q->data->case_data.id_case);
			wsprintf(data[i]->admin_level,TEXT("%s"),q->data->case_data.admin_level);
			wsprintf(data[i]->crime,TEXT("%s"),q->data->case_data.crime);
			wsprintf(data[i]->money,TEXT("%s"),q->data->case_data.money);
			data[i]->report_number = count;	
			i++;
		}
		province_index++;
    }
	data[i] = NULL;
	return data;
}
StatisticsData4** StatisticsFunction4(OLNode *head)
{
	int i = 0,j,punish;
    OLink p = head,q;
	SYSTEMTIME time;
	StatisticsData4 **data;
	data = (StatisticsData4**) malloc (sizeof(StatisticsData4*)*50);
	
    while (p->right)
    {
        p = p->right;
		q = p;
		while(q->down)
		{
			q = q->down;
			StringTranslateToTime(q->data->case_data.date,&time);
			for (j = 0;j < i;j++)
			{
				if(data[j]->year == time.wYear) break;
			}
			punish = _wtoi(q->data->case_data.punish);
			if (j >= i)
			{
				data[i] = (StatisticsData4*) malloc (sizeof(StatisticsData4));
				data[j]->year = time.wYear;
				data[j]->case_number = 1;
				data[j]->total_amount = _wtof(q->data->case_data.money);
				data[j]->life_imprisonment = 0;
				data[j]->death_sentence = 0;
				data[j]->imprisonment1_5 = 0;
				data[j]->imprisonment6_10 = 0;
				data[j]->imprisonment11_more = 0;
				if (punish >= 100) data[j]->life_imprisonment = 1;
				else if (punish <= 0) data[j]->death_sentence = 1;
				else if (punish >= 1 && punish <= 5) data[j]->imprisonment1_5 = 1;
				else if (punish >= 6 && punish <= 10) data[j]->imprisonment6_10 = 1;
				else if (punish >= 11 && punish < 100) data[j]->imprisonment11_more = 1;
				i++;
			}
			else
			{
				data[j]->case_number++;
				data[j]->total_amount += _wtof(q->data->case_data.money);
				if (punish >= 100) data[j]->life_imprisonment++;
				else if (punish <= 0) data[j]->death_sentence++;
				else if (punish >= 1 && punish <= 5) data[j]->imprisonment1_5++;
				else if (punish >= 6 && punish <= 10) data[j]->imprisonment6_10++;
				else if (punish >= 11 && punish < 100) data[j]->imprisonment11_more++;
			}
		}
    }
	data[i] = NULL;
	return data;
}

int provinceTranslate(LPWSTR data)
{
	int i;
    TCHAR* tp[34] = {TEXT("安微"),TEXT("澳门"),TEXT("北京"),TEXT("重庆"),TEXT("福建"),TEXT("广东"),TEXT("广西"),TEXT("贵州"),TEXT("甘肃"),TEXT("黑龙江"),TEXT("河北"),
                    TEXT("河南"),TEXT("湖南"),TEXT("湖北"),TEXT("海南"),TEXT("吉林"),TEXT("江西"),TEXT("江苏"),TEXT("辽宁"),TEXT("宁夏"),TEXT("内蒙"),TEXT("青海"),
                    TEXT("上海"),TEXT("山东"),TEXT("山西"),TEXT("陕西"),TEXT("四川"),TEXT("天津"),TEXT("台湾"),TEXT("西藏"),TEXT("香港"),TEXT("新疆"),TEXT("云南"),TEXT("浙江")};
    
	for (i=0;i<34;i++)
    {
        if (lstrcmp(data,tp[i]) == 0) return i;
    }
	return -1;
}
int crimeTranslate(LPWSTR data)
{
	int i;
    TCHAR* tp[6] = {TEXT("受贿"),TEXT("贪污"),TEXT("挪用公款"),TEXT("渎职"),TEXT("滥用职权"),TEXT("其他")};
    
	for (i=0;i<6;i++)
    {
        if (lstrcmp(data,tp[i]) == 0) return i;
    }
	return -1;
}
int admin_LevelTranslate(LPWSTR data)
{
	int i;
    TCHAR* tp[8] = {TEXT("副科级"),TEXT("正科级"),TEXT("副处级"),TEXT("正处级"),TEXT("副厅级"),TEXT("正厅级"),TEXT("副部级"),TEXT("正部级及以上")};
    
	for (i=0;i<6;i++)
    {
        if (lstrcmp(data,tp[i]) == 0) return i;
    }
	return -1;
}
int mediumTranslate(LPWSTR data)
{
	int i;
    TCHAR* tp[4] = {TEXT("报纸"),TEXT("广播"),TEXT("电视"),TEXT("互联网")};
    
	for (i=0;i<4;i++)
    {
        if (lstrcmp(data,tp[i]) == 0) return i;
    }
	return -1;
}

LPWSTR timeTranslateToString(SYSTEMTIME *time,LPWSTR string)
{
	if (time->wMonth < 10 && time->wDay < 10) 
		wsprintf(string,TEXT("%d0%d0%d"),time->wYear,time->wMonth,time->wDay);
	else if (time->wMonth < 10 && time->wDay >= 10) 
		wsprintf(string,TEXT("%d0%d%d"),time->wYear,time->wMonth,time->wDay);
	else if (time->wMonth >= 10 && time->wDay < 10) 
		wsprintf(string,TEXT("%d%d0%d"),time->wYear,time->wMonth,time->wDay);
	else if (time->wMonth >= 10 && time->wDay >= 10) 
		wsprintf(string,TEXT("%d%d%d"),time->wYear,time->wMonth,time->wDay);
	return string;
}
SYSTEMTIME* StringTranslateToTime(LPWSTR string,SYSTEMTIME *time)
{
	TCHAR s[10];
	int i,k=0;
	for (i = 0;i<4;i++) s[i] = string[k++];
	s[i] = L'\0';
	time->wYear = (unsigned short)_wtoi(s); 
	for (i = 0;i<2;i++) s[i] = string[k++];
	s[i] = L'\0';
	time->wMonth = (unsigned short)_wtoi(s); 
	for (i = 0;i<2;i++) s[i] = string[k++];
	s[i] = L'\0';
	time->wDay = (unsigned short)_wtoi(s); 
	return time;
}
LPWSTR lstrpbrk(LPCWSTR cs,LPCWSTR ct)
{
	const TCHAR *sc1,*sc2;
	for( sc1 = cs;*sc1 != L'\0';++sc1)
	{
		for( sc2 = ct;*sc2 != L'\0'; ++sc2)
		{
			if (*sc1 == *sc2) return (TCHAR *)sc1;
		}
	}
	return NULL;
}

void quicksort1(StatisticsData1 **data,int low,int high)
{  
	int i,j;
	StatisticsData1 *x;
    i=low;
    j=high;
    x=data[low];

    while (i<j)
    {
		while (data[j]->case_number < x->case_number && j>i) j--;
        if (j>i)
        {
            data[i]=data[j];
            i++;
        }
        while (data[j]->case_number > x->case_number && i<j) i++;
        if (i<j)
        {
            data[j]=data[i];
            j--;
        }
    }
    data[i]=x;

    if (low<(i-1)) quicksort1(data,low,i-1);
    if ((i+1)<high) quicksort1(data,i+1,high);
}
void quicksort2(StatisticsData2 **data,int low,int high)
{  
	int i,j;
	StatisticsData2 *x;
    i=low;
    j=high;
    x=data[low];

    while (i<j)
    {
		while (data[j]->year < x->year && j>i) j--;
        if (j>i)
        {
            data[i]=data[j];
            i++;
        }
		while (data[j]->year > x->year && i<j) i++;
        if (i<j)
        {
            data[j]=data[i];
            j--;
        }
    }
    data[i]=x;

    if (low<(i-1)) quicksort2(data,low,i-1);
    if ((i+1)<high) quicksort2(data,i+1,high);
}
void quicksort3(StatisticsData3 **data,int low,int high)
{  
	int i,j;
	StatisticsData3 *x;
    i=low;
    j=high;
    x=data[low];

    while (i<j)
    {
		while (data[j]->report_number < x->report_number && j>i) j--;
        if (j>i)
        {
            data[i]=data[j];
            i++;
        }
		while (data[j]->report_number > x->report_number && i<j) i++;
        if (i<j)
        {
            data[j]=data[i];
            j--;
        }
    }
    data[i]=x;

    if (low<(i-1)) quicksort3(data,low,i-1);
    if ((i+1)<high) quicksort3(data,i+1,high);
}
void quicksort4(StatisticsData4 **data,int low,int high)
{  
	int i,j;
	StatisticsData4 *x;
    i=low;
    j=high;
    x=data[low];

    while (i<j)
    {
		while (data[j]->year < x->year && j>i) j--;
        if (j>i)
        {
            data[i]=data[j];
            i++;
        }
		while (data[j]->year > x->year && i<j) i++;
        if (i<j)
        {
            data[j]=data[i];
            j--;
        }
    }
    data[i]=x;

    if (low<(i-1)) quicksort4(data,low,i-1);
    if ((i+1)<high) quicksort4(data,i+1,high);
}

int importFile(OLNode *head,LPWSTR path)
{
	int i = 0,j = 0;
	FILE *fp = NULL;
	OLink p = head;
	PROVINCEDATA province_data,*pprovince = &province_data;
	CASEDATA case_data, *pcase = &case_data;
	NEWSDATA news_data,*pnews = &news_data;
	TCHAR flag[10];

	if ((fp = _wfreopen(path,TEXT("r"),stdin)) == NULL ) return -1;
	if (emptyAll(head) == -1) return -1;

	i = 0;
	while(p->right)
	{
		p = p->right;
		pprovince = &p->data->province_data;
		wscanf(TEXT("%s"),flag);
		if (lstrcmp(flag,TEXT("#")) == 0) break;
		wscanf(TEXT("%s %s %s"),pprovince->name,pprovince->discipline,pprovince->telephone);

		j = 0;
		while(1)
		{
			wscanf(TEXT("%s"),flag);
			if (lstrcmp(flag,TEXT("#")) == 0) break;
			
			wscanf(TEXT("%s %s %s %s %s %s %s %s"),pcase->id_case,pcase->date,pcase->province,
					pcase->defendant,pcase->admin_level,pcase->crime,pcase->money,pcase->punish);

			insertCaseByIndex(head,i,*pcase);

			while(1)
			{
				wscanf(TEXT("%s"),flag);
				if (lstrcmp(flag,TEXT("#")) == 0) break;

				wscanf(TEXT("%s %s %s %s %s"),pnews->id_case,pnews->report_date,pnews->medium_category,
					pnews->medium_name,pnews->url);

				insertNewsByIndex(head,i,j,*pnews);
			}
			j++;
		}
		i++;
	}
	fclose(fp);
	return 0;
}
int exportFile(OLNode *head,LPWSTR path)
{
	FILE *fp = NULL;
	OLink p = head,q,r;
	PROVINCEDATA *pprovince;
	CASEDATA *pcase;
	NEWSDATA *pnews;

	if ((fp = _wfreopen(path,TEXT("w"),stdout)) == NULL ) return -1;

	while(p->right)
	{
		p = p->right;
		pprovince = &p->data->province_data;
		wprintf(TEXT("province %s %s %s\n"),pprovince->name,pprovince->discipline,pprovince->telephone);
		q = p;
		while (q->down)
		{
			q = q->down;
			pcase = &q->data->case_data;
			wprintf(TEXT("\tcase %s %s %s %s %s %s %s %s\n"),pcase->id_case,pcase->date,pcase->province,
				pcase->defendant,pcase->admin_level,pcase->crime,pcase->money,pcase->punish);
			r = q;
			while(r->right)
			{
				r = r->right;
				pnews = &r->data->news_data;
				wprintf(TEXT("\t\tnews %s %s %s %s %s\n"),pnews->id_case,pnews->report_date,pnews->medium_category,
					pnews->medium_name,pnews->url);
			}
			wprintf(TEXT("\t\t#\n"));
		}
		wprintf(TEXT("\t#\n"));
	}
	wprintf(TEXT("#\n"));
	fclose(fp);
	return 0;
}