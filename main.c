/**
 *  文件名: main.c
 *  功能:主窗体的创建及各控件逻辑处理
 *  作者:杜宇
 **/
#include "Windows.h"
#include "stdio.h"
#include "tchar.h"
#include "locale.h"
#include "winuser.h"
#include "commctrl.h"
#include "shlwapi.h"
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"comdlg32.lib")
#include "resource.h"
#include "model.h"


#define MAXSIZE   256
#define OUTPUTSIZE 512

//全局实例句柄
HINSTANCE g_hInstance; 

//标志
int isInit = 1;  
int updateWinFlag = 0;
int winFlag = 1;

//默认常量
TCHAR ClassName[] = _T("DLGCLASS");
TCHAR MenuName[] = _T("MyMenu");
TCHAR MyIcon[] = _T("Icon");
TCHAR MainDlgName[] = _T("MyDialog");
TCHAR AboutDlgName[] = _T("AboutDialog");
TCHAR StatisticsDlgName[] = _T("StatisticsDialog");
TCHAR FindDlgName[] = _T("FindDialog");
TCHAR ProvinceDlgName[] = _T("ProvinceDialog");
TCHAR CaseDlgName[] = _T("AddCaseDialog");
TCHAR NewsDlgName[] = _T("AddNewsDialog");
TCHAR OptionDlgName[] = _T("OptionDialog");
TCHAR ListViewClassName[] = _T("SysListView32");
TCHAR ConfigFileName[] = _T("config.ini");
TCHAR DefaultDataFileName[] = _T("default.dat");

TCHAR buffer[MAXSIZE];
TCHAR configFilePath[MAXSIZE];  //配置文件路径
TCHAR defaultDataFilePath[MAXSIZE]; //默认数据文件路径

//窗口句柄
HWND hMainDialog;
HWND hAboutDialog;
HWND hFindDialog;
HWND hProvinceDialog;
HWND hStatisticsDialog;
HWND hCaseDialog;
HWND hNewsDialog;
HWND hOptionDialog;

//列表框句柄
HWND hList1;
HWND hList2[3];
HWND hList3[5];

//通用对话框
OPENFILENAME g_ofn;
TCHAR FilterString[] = _T("Data Files\0*.dat\0All Files\0*.*\0");  //过滤器
TCHAR OurTitle[] = _T("请选择文件路径:");  //标题
TCHAR FilePath[OUTPUTSIZE] ;  //文件路径

//数据
OLink head;
int news_index = 1;

//初始化函数
int InitApp(OLNode **head)  //初始化应用
{
	TCHAR value[5];
	//数据化数据
	if (initOLNode(head) != 0) return -1;
	//设置配置文件路径
	GetCurrentDirectory(MAXSIZE,buffer);
	lstrcat(buffer,TEXT("\\"));
	lstrcpy(configFilePath,buffer);
	lstrcat(configFilePath,ConfigFileName);
	//设置默认数据文件路径
	lstrcpy(defaultDataFilePath,buffer);
	lstrcat(defaultDataFilePath,DefaultDataFileName);

	//加载配置文件
	if (PathFileExists(configFilePath) == 0)
	{
		//写配置文件
		WritePrivateProfileString(TEXT("AutoData"),TEXT("Import"),TEXT("1"),configFilePath);
		WritePrivateProfileString(TEXT("AutoData"),TEXT("Export"),TEXT("1"),configFilePath);
	}
	//读配置文件
	GetPrivateProfileString(TEXT("AutoData"),TEXT("Import"),NULL,value,5,configFilePath);
	if (lstrcmp(value,TEXT("1")) == 0) importFile(*head,defaultDataFilePath);

	return 0;
}

void InitComboBox_Province(HWND hComboBox)
{
	int i;
    TCHAR* tp[34] = {TEXT("安微"),TEXT("澳门"),TEXT("北京"),TEXT("重庆"),TEXT("福建"),TEXT("广东"),TEXT("广西"),TEXT("贵州"),TEXT("甘肃"),TEXT("黑龙江"),TEXT("河北"),
                    TEXT("河南"),TEXT("湖南"),TEXT("湖北"),TEXT("海南"),TEXT("吉林"),TEXT("江西"),TEXT("江苏"),TEXT("辽宁"),TEXT("宁夏"),TEXT("内蒙"),TEXT("青海"),
                    TEXT("上海"),TEXT("山东"),TEXT("山西"),TEXT("陕西"),TEXT("四川"),TEXT("天津"),TEXT("台湾"),TEXT("西藏"),TEXT("香港"),TEXT("新疆"),TEXT("云南"),TEXT("浙江")};
    
	for (i=0;i<34;i++)
    {
        SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)(tp[i]));
    }
}
void InitComboBox_Crime(HWND hComboBox)
{
	int i;
	TCHAR* tp[6] = {TEXT("受贿"),TEXT("贪污"),TEXT("挪用公款"),TEXT("渎职"),TEXT("滥用职权"),TEXT("其他")};
    
	for (i=0;i<6;i++)
    {
        SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)(tp[i]));
    }
}
void InitComboBox_Level(HWND hComboBox)
{
	int i;
	TCHAR* tp[8] = {TEXT("副科级"),TEXT("正科级"),TEXT("副处级"),TEXT("正处级"),TEXT("副厅级"),TEXT("正厅级"),TEXT("副部级"),TEXT("正部级及以上")};
    
	for (i=0;i<8;i++)
    {
        SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)(tp[i]));
    }
}
void InitComboBox_Medium(HWND hComboBox)
{
	int i;
	TCHAR* tp[4] = {TEXT("报纸"),TEXT("广播"),TEXT("电视"),TEXT("互联网")};
    
	for (i=0;i<4;i++)
    {
        SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)(tp[i]));
    }
}

void InitEdit_News(HWND hWnd)
{
	SetDlgItemText(hWnd,IDC_EDIT1,TEXT("请先选择具体省份"));
	SetDlgItemText(hWnd,IDC_EDIT2,TEXT("无"));
	SetDlgItemText(hWnd,IDC_EDIT3,TEXT("无"));
	SetDlgItemText(hWnd,IDC_EDIT4,TEXT("无"));
	SetDlgItemText(hWnd,IDC_EDIT5,TEXT("无"));
}
void InitEdit_Province(HWND hWnd)
{
	SetDlgItemText(hWnd,IDC_EDIT6,TEXT("无"));
	SetDlgItemText(hWnd,IDC_EDIT7,TEXT("无"));
}

void InitColumn_MainDlg(HWND hList)
{
    LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.pszText =  _T("案件编号");
    lvc.cx = 100;
    SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&lvc);
    lvc.mask |= LVCF_FMT;
    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = _T("立案日期");
    lvc.cx = 80;
    SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&lvc);
    lvc.pszText = _T("所属省份");
    SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&lvc);
    lvc.pszText = _T("被告人姓名");
    SendMessage(hList,LVM_INSERTCOLUMN,3,(LPARAM)&lvc);
    lvc.pszText = _T("行政级别");
    SendMessage(hList,LVM_INSERTCOLUMN,4,(LPARAM)&lvc);
    lvc.pszText = _T("主要罪名");
    SendMessage(hList,LVM_INSERTCOLUMN,5,(LPARAM)&lvc);
    lvc.pszText = _T("涉案金额");
    SendMessage(hList,LVM_INSERTCOLUMN,6,(LPARAM)&lvc);
    lvc.pszText = _T("刑事处罚");
    SendMessage(hList,LVM_INSERTCOLUMN,7,(LPARAM)&lvc);
}
void InitColumn_Statistics1(HWND hList)
{
	LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.pszText =  _T("年份");
    lvc.cx = 50;
    SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&lvc);
    lvc.mask |= LVCF_FMT;
    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = _T("案发数");
    lvc.cx = 50;
    SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&lvc);
	lvc.cx = 90;
	lvc.pszText = _T("涉案金额");
    SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&lvc);
}
void InitColumn_Statistics2(HWND hList)
{
	LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.pszText =  _T("案件编号");
    lvc.cx = 90;
    SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&lvc);
    lvc.mask |= LVCF_FMT;
    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = _T("被告行政级别");
    lvc.cx = 90;
    SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&lvc);
	lvc.pszText = _T("主要罪名");
	lvc.cx = 60;
    SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&lvc);
	lvc.pszText = _T("涉案金额");
	lvc.cx = 90;
    SendMessage(hList,LVM_INSERTCOLUMN,3,(LPARAM)&lvc);
	lvc.pszText = _T("媒体报道次数");
	lvc.cx = 90;
    SendMessage(hList,LVM_INSERTCOLUMN,4,(LPARAM)&lvc);
}
void InitColumn_Statistics3(HWND hList)
{
	LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.pszText =  _T("年份");
    lvc.cx = 50;
    SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&lvc);
    lvc.mask |= LVCF_FMT;
    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = _T("死刑");
    lvc.cx = 40;
    SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&lvc);
	lvc.pszText = _T("无期徒刑");
	lvc.cx = 75;
    SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&lvc);
	lvc.pszText = _T("有期1-5年");
    SendMessage(hList,LVM_INSERTCOLUMN,3,(LPARAM)&lvc);
	lvc.pszText = _T("有期6-10");
    SendMessage(hList,LVM_INSERTCOLUMN,4,(LPARAM)&lvc);
	lvc.pszText = _T("有期11年以上");
    SendMessage(hList,LVM_INSERTCOLUMN,5,(LPARAM)&lvc);
}
void InitColumn_Statistics4(HWND hList)
{
	LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.pszText =  _T("年份");
    lvc.cx = 60;
    SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&lvc);
    lvc.mask |= LVCF_FMT;
    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = _T("案发数");
    lvc.cx = 90;
    SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&lvc);
	lvc.pszText = _T("涉案金额");
	SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&lvc);
	lvc.pszText = _T("死刑");
	SendMessage(hList,LVM_INSERTCOLUMN,3,(LPARAM)&lvc);
	lvc.pszText = _T("无期徒刑");
    SendMessage(hList,LVM_INSERTCOLUMN,4,(LPARAM)&lvc);
	lvc.pszText = _T("有期1-5年");
    SendMessage(hList,LVM_INSERTCOLUMN,5,(LPARAM)&lvc);
	lvc.pszText = _T("有期6-10");
    SendMessage(hList,LVM_INSERTCOLUMN,6,(LPARAM)&lvc);
	lvc.pszText = _T("有期11年以上");
    SendMessage(hList,LVM_INSERTCOLUMN,7,(LPARAM)&lvc);
}
void InitColumn_Find1(HWND hList)
{
	LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.pszText =  _T("省份");
    lvc.cx = 60;
    SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&lvc);
    lvc.mask |= LVCF_FMT;
    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = _T("纪检人姓名");
    lvc.cx = 80;
    SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&lvc);
	lvc.pszText = _T("联系电话");
	SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&lvc);
}
void InitColumn_Find2(HWND hList)
{
	LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.pszText =  _T("案件编号");
    lvc.cx = 60;
    SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&lvc);
    lvc.mask |= LVCF_FMT;
    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = _T("报道日期");
    lvc.cx = 60;
    SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&lvc);
	lvc.pszText = _T("媒体类别");
	SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&lvc);
	lvc.pszText = _T("媒体名称");
	SendMessage(hList,LVM_INSERTCOLUMN,3,(LPARAM)&lvc);
	lvc.pszText = _T("内容索引");
	lvc.cx = 120;
	SendMessage(hList,LVM_INSERTCOLUMN,4,(LPARAM)&lvc);
}
void InitListBox_Province(HWND hListBox)
{
	int i;
    TCHAR* tp[34] = {TEXT("安微"),TEXT("澳门"),TEXT("北京"),TEXT("重庆"),TEXT("福建"),TEXT("广东"),TEXT("广西"),TEXT("贵州"),TEXT("甘肃"),TEXT("黑龙江"),TEXT("河北"),
                    TEXT("河南"),TEXT("湖南"),TEXT("湖北"),TEXT("海南"),TEXT("吉林"),TEXT("江西"),TEXT("江苏"),TEXT("辽宁"),TEXT("宁夏"),TEXT("内蒙"),TEXT("青海"),
                    TEXT("上海"),TEXT("山东"),TEXT("山西"),TEXT("陕西"),TEXT("四川"),TEXT("天津"),TEXT("台湾"),TEXT("西藏"),TEXT("香港"),TEXT("新疆"),TEXT("云南"),TEXT("浙江")};
    
	for (i=0;i<34;i++)
    {
        SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)(tp[i]));
    }
}

//主窗口处理函数
void InsertItem_Empty(HWND hList)  //置空信息
{
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
    lvi.iSubItem = 0;
	lvi.pszText = TEXT("无信息");
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
}
void InsertItem_Case(HWND hList,DWORD row,CASEDATA *data)  //置案件信息
{
	int punish;
	TCHAR buf[20];
	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = row;
    lvi.iSubItem = 0;
	lvi.pszText = data->id_case;
    lvi.lParam = row;
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
	lvi.mask = LVIF_TEXT;
    lvi.iSubItem ++;
	lvi.pszText = data->date;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data->province;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data->defendant;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data->admin_level;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data->crime;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	wsprintf(buf,TEXT("%s万"),data->money);
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	punish = _wtoi(data->punish);
	if (punish <= 0) wsprintf(buf,TEXT("%s"),TEXT("死刑"));
	else if (punish >= 100) wsprintf(buf,TEXT("%s"),TEXT("无期徒刑"));
	else wsprintf(buf,TEXT("%d年"),punish);
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
}
void InitList_All_Province(HWND hList)  //获取全部省份信息
{
	CASEDATA **case_data;
	int i = 0,j = 0,sum = 0;
	SendMessage(hList,LVM_DELETEALLITEMS,0,0);
	for (j = 0;j < 34;j++)
	{
		if (NULL == (case_data = fetchAllCase(head,j))) continue;
		i = 0;
		while (case_data[i])
		{
			InsertItem_Case(hList,sum,case_data[i]);
			i++;
			sum++;
		}
		free(case_data);
	}
	if (sum == 0) InsertItem_Empty(hList);
}
void ClearList(HWND hList)  //清空列表框
{
	SendMessage(hList,LVM_DELETEALLITEMS,0,0);
}
void SetEdit1 (HWND hWnd,NEWSDATA *data)  //设置主窗口新闻
{
	 SetDlgItemText(hWnd,IDC_EDIT1,data->id_case);
	 SetDlgItemText(hWnd,IDC_EDIT2,data->report_date);
	 SetDlgItemText(hWnd,IDC_EDIT3,data->medium_category);
	 SetDlgItemText(hWnd,IDC_EDIT4,data->medium_name);
	 SetDlgItemText(hWnd,IDC_EDIT5,data->url);
}
int UpdateMainList(HWND hWnd)   //更新主窗口列表框
{
	DWORD i;
	PROVINCEDATA *province_data;
	CASEDATA **case_data;
	int ComboBoxIndex;

	ComboBoxIndex = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_PROVINCE),CB_GETCURSEL,0,0);
	InitEdit_News(hWnd); 
	InitEdit_Province(hWnd); 
	if (ComboBoxIndex == 0)
	{
		SetDlgItemText(hWnd,IDC_EDIT1,TEXT("请先选择具体省份"));
		InitList_All_Province(hList1);
		return -1;
	}
	SetDlgItemText(hWnd,IDC_EDIT1,TEXT("无"));
	if ((province_data = fetchProvinceByIndex(head,ComboBoxIndex - 1)) == NULL) return -1;
	SetDlgItemText(hWnd,IDC_EDIT6,province_data->discipline);
	SetDlgItemText(hWnd,IDC_EDIT7,province_data->telephone);
	ClearList(hList1);
	if (NULL == (case_data = fetchAllCase(head,ComboBoxIndex - 1)))
	{
		InsertItem_Empty(hList1);
		return -1;
	}
	else
	{
		i = 0;
		while (case_data[i])
		{
			InsertItem_Case (hList1,i,case_data[i]);
			i++;
		}
		free (case_data);
	}
	return 0;
}
int GetNews(HWND hWnd)  //获取新闻信息
{
	NEWSDATA *news_data;
	int news_conut;
	int province_index,case_index;
	province_index = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_PROVINCE),CB_GETCURSEL,0,0);
	case_index = SendMessage(hList1,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
	if (province_index < 1 || case_index < 0) return -1;
	if (NULL == (news_data = fetchNewsByIndex(head,province_index-1,case_index,0)))
	{
		InitEdit_News(hWnd);
		SetDlgItemText(hWnd,IDC_EDIT1,TEXT("无"));
		SetDlgItemText(hWnd,IDC_NEWS_COUNT,TEXT("共0"));
		return -1;
	}
	else
	{
		news_index = 0;
		news_conut = CountNews(head,province_index-1,case_index);
		wsprintf(buffer,TEXT("第1共%d"),news_conut);
		SetDlgItemText(hWnd,IDC_NEWS_COUNT,buffer);
		SetEdit1(hWnd,news_data);
	}
	return 0;
}
int ShowOtherNews(HWND hWnd,int flag)  //显示其它新闻
{
	NEWSDATA *news_data;
	int news_conut;
	int province_index,case_index;
	province_index = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_PROVINCE),CB_GETCURSEL,0,0);
	case_index = SendMessage(hList1,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
	if (province_index <= 0) 
	{
		MessageBox (hWnd,TEXT("请先选择具体省份!"),TEXT("提示"),MB_OK);
		return -1;
	}
	if (province_index < 1 || case_index < 0) 
	{
		MessageBox (hWnd,TEXT("还没选择呢!"),TEXT("提示"),MB_OK);
		return -1;
	}
	if (NULL != (news_data = fetchNewsByIndex(head,province_index-1,case_index,news_index+flag)))
	{
		news_index += flag;
		news_conut = CountNews(head,province_index-1,case_index);
		wsprintf(buffer,TEXT("第%d共%d"),news_index+1,news_conut);
		SetDlgItemText(hWnd,IDC_NEWS_COUNT,buffer);
		SetEdit1(hWnd,news_data);
		return 0;
	}
	else
	{
		MessageBox (hWnd,TEXT("没有新闻啦!"),TEXT("提示"),MB_OK);
		return -1;
	}
}
int CheckPath(LPWSTR path)  //检查通用对话框获取的路径
{
	int len;
	LPWSTR temp = path;
	len = lstrlen(path);
	temp = temp + (len - 4);
	if (lstrcmp(temp,_T(".dat")) == 0) return 1;
	return 0;
}

//案件管理窗口函数
void SetListBox_Case(HWND hListBox,CASEDATA *data)
{
	SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)data->id_case);
}
void SetEdit_Case(HWND hWnd,CASEDATA *data)
{
	SYSTEMTIME spTime;
	DateTime_GetSystemtime(GetDlgItem(hWnd,IDC_DATETIMEPICKER1),&spTime);
	SetDlgItemText(hWnd,IDC_EDIT51,data->id_case);
	StringTranslateToTime(data->date,&spTime);
	DateTime_SetSystemtime(GetDlgItem(hWnd,IDC_DATETIMEPICKER1),GDT_VALID,&spTime);
	SendMessage(GetDlgItem(hWnd,IDC_COMBO51),CB_SETCURSEL,(WPARAM)provinceTranslate(data->province),0);
	SetDlgItemText(hWnd,IDC_EDIT54,data->defendant);
	SendMessage(GetDlgItem(hWnd,IDC_COMBO52),CB_SETCURSEL,(WPARAM)crimeTranslate(data->crime),0);
	SendMessage(GetDlgItem(hWnd,IDC_COMBO53),CB_SETCURSEL,(WPARAM)admin_LevelTranslate(data->admin_level),0);
	SetDlgItemText(hWnd,IDC_EDIT55,data->money);
	SetDlgItemText(hWnd,IDC_EDIT56,data->punish);
}
void SetEditEmpty_Case(HWND hWnd)
{
	SetDlgItemText(hWnd,IDC_EDIT51,TEXT(""));
	SetDlgItemText(hWnd,IDC_EDIT52,TEXT(""));
	SendMessage(GetDlgItem(hWnd,IDC_COMBO51),CB_SETCURSEL,-1,0);
	SetDlgItemText(hWnd,IDC_EDIT54,TEXT(""));
	SendMessage(GetDlgItem(hWnd,IDC_COMBO52),CB_SETCURSEL,-1,0);
	SendMessage(GetDlgItem(hWnd,IDC_COMBO53),CB_SETCURSEL,-1,0);
	SetDlgItemText(hWnd,IDC_EDIT55,TEXT(""));
	SetDlgItemText(hWnd,IDC_EDIT56,TEXT(""));
}
void SetListBox_Case_Radio1(HWND hWnd)
{
	SetEditEmpty_Case(hWnd);
	SetDlgItemText(hWnd,IDC_BUTTON51,TEXT("添加"));
}
void SetListBox_Case_Radio2(HWND hWnd)
{
	SetDlgItemText(hWnd,IDC_BUTTON51,TEXT("修改"));
}
void SetListBox_Case_Radio3(HWND hWnd)
{
	SetDlgItemText(hWnd,IDC_BUTTON51,TEXT("删除"));
}
void UpdataListBox_Case(HWND hWnd,int province_index)
{
	CASEDATA **case_data;
	int i;
	SendMessage(GetDlgItem(hWnd,IDC_LIST52),LB_RESETCONTENT,0,0);
	if (NULL == (case_data = fetchAllCase(head,province_index))) return;
	i = 0;
	while (case_data[i])
	{
		SetListBox_Case(GetDlgItem(hWnd,IDC_LIST52),case_data[i]);
	i++;
	}
	free(case_data);
}
int GetCurCASEDATA (HWND hWnd,CASEDATA *case_data)
{
	int index;
	SYSTEMTIME spTime;
	GetDlgItemText(hWnd,IDC_EDIT51,case_data->id_case,30);
	DateTime_GetSystemtime(GetDlgItem(hWnd,IDC_DATETIMEPICKER1),&spTime);
	timeTranslateToString(&spTime,case_data->date);
	index = SendMessage(GetDlgItem(hWnd,IDC_LIST51),LB_GETCURSEL,0,0);
	SendMessage(GetDlgItem(hWnd,IDC_LIST51),LB_GETTEXT,index,(LPARAM)case_data->province);
	GetDlgItemText(hWnd,IDC_EDIT54,case_data->defendant,30);
	GetDlgItemText(hWnd,IDC_COMBO52,case_data->crime,30);
	GetDlgItemText(hWnd,IDC_COMBO53,case_data->admin_level,30);
	GetDlgItemText(hWnd,IDC_EDIT55,case_data->money,30);
	GetDlgItemText(hWnd,IDC_EDIT56,case_data->punish,30);
	if (lstrlen(case_data->id_case) == 0) return -1;
	if (lstrlen(case_data->date) == 0) return -1;
	if (lstrlen(case_data->province) == 0) return -1;
	if (lstrlen(case_data->defendant) == 0) return -1;
	if (lstrlen(case_data->crime) == 0) return -1;
	if (lstrlen(case_data->admin_level) == 0) return -1;
	if (lstrlen(case_data->money) == 0) return -1;
	if (lstrlen(case_data->punish) == 0) return -1;
	return 0;
}
void Add_Case(HWND hWnd)
{
	CASEDATA data,*case_data = &data;
	int province_index;
	province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST51),LB_GETCURSEL,0,0);
	if (province_index < 0) 
	{
		MessageBox(hWnd,TEXT("请选择一个省份"),TEXT("提示"),MB_OK);
		return;
	}
	if (GetCurCASEDATA (hWnd,case_data) == -1) 
	{
		MessageBox(hWnd,TEXT("内容不能为空"),TEXT("错误"),MB_OK);
		return;
	}
	insertCaseByIndex(head,province_index,data);
	UpdataListBox_Case(hWnd,province_index);
	MessageBox(hWnd,TEXT("添加成功"),TEXT("提示"),MB_OK);
}
void Modify_Case(HWND hWnd)
{
	CASEDATA data,*case_data = &data;
	int province_index,case_index;
	province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST51),LB_GETCURSEL,0,0);
	case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST52),LB_GETCURSEL,0,0);
	if (province_index < 0 || case_index < 0) 
	{
		MessageBox(hWnd,TEXT("请选择一个省份和案件"),TEXT("提示"),MB_OK);
		return;
	}
	if (GetCurCASEDATA (hWnd,case_data) == -1) 
	{
		MessageBox(hWnd,TEXT("内容不能为空"),TEXT("错误"),MB_OK);
		return;
	}
	modifyCaseByIndex(head,province_index,case_index,data);
	UpdataListBox_Case(hWnd,province_index);
	MessageBox(hWnd,TEXT("修改成功"),TEXT("提示"),MB_OK);
}
void Delete_Case(HWND hWnd)
{
	int province_index,case_index;
	province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST51),LB_GETCURSEL,0,0);
	case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST52),LB_GETCURSEL,0,0);
	if (province_index < 0 || case_index < 0) 
	{
		MessageBox(hWnd,TEXT("请选择一个省份和案件"),TEXT("提示"),MB_OK);
		return;
	}
	deleteCaseByIndex (head,province_index,case_index);
	SetEditEmpty_Case(hWnd);
	UpdataListBox_Case(hWnd,province_index);
	MessageBox(hWnd,TEXT("删除成功"),TEXT("提示"),MB_OK);
}

//新闻管理窗口函数
void SetListBox_News(HWND hListBox,NEWSDATA *data)
{
	SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)data->report_date);
}
void SetEditEmpty_News(HWND hWnd)
{
	SetDlgItemText(hWnd,IDC_EDIT61,TEXT(""));
	SetDlgItemText(hWnd,IDC_EDIT62,TEXT(""));
	SendMessage(GetDlgItem(hWnd,IDC_COMBO61),CB_SETCURSEL,-1,0);
	SetDlgItemText(hWnd,IDC_EDIT63,TEXT(""));
}
void SetListBox_News_Radio1(HWND hWnd)
{
	SetEditEmpty_News(hWnd);
	SetDlgItemText(hWnd,IDC_BUTTON61,TEXT("添加"));
}
void SetListBox_News_Radio2(HWND hWnd)
{
	SetDlgItemText(hWnd,IDC_BUTTON61,TEXT("修改"));
}
void SetListBox_News_Radio3(HWND hWnd)
{
	SetDlgItemText(hWnd,IDC_BUTTON61,TEXT("删除"));
}
void SetEdit_News(HWND hWnd,NEWSDATA *data)
{
	SYSTEMTIME spTime;
	DateTime_GetSystemtime(GetDlgItem(hWnd,IDC_DATETIMEPICKER2),&spTime);
	StringTranslateToTime(data->report_date,&spTime);
	DateTime_SetSystemtime(GetDlgItem(hWnd,IDC_DATETIMEPICKER2),GDT_VALID,&spTime);

	SetDlgItemText(hWnd,IDC_EDIT62,data->medium_name);
	SendMessage(GetDlgItem(hWnd,IDC_COMBO61),CB_SETCURSEL,(WPARAM)mediumTranslate(data->medium_category),0);
	SetDlgItemText(hWnd,IDC_EDIT63,data->url);
}
void UpdataListBox_News(HWND hWnd,int province_index,int case_index)
{
	NEWSDATA **news_data;
	int i;
	SendMessage(GetDlgItem(hWnd,IDC_LIST63),LB_RESETCONTENT,0,0);
	if (NULL == (news_data = fetchALLNews(head,province_index,case_index))) return;
	i = 0;
	while (news_data[i])
	{
		SetListBox_News(GetDlgItem(hWnd,IDC_LIST63),news_data[i]);
	i++;
	}
	free(news_data);
}
int GetCurNEWSDATA (HWND hWnd,NEWSDATA *news_data)
{
	SYSTEMTIME spTime;
	int case_index;

	case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_GETCURSEL,0,0);
	if (case_index < 0) return -1;
	SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_GETTEXT,case_index,(LPARAM)news_data->id_case);
	DateTime_GetSystemtime(GetDlgItem(hWnd,IDC_DATETIMEPICKER2),&spTime);
	timeTranslateToString(&spTime,news_data->report_date);
	GetDlgItemText(hWnd,IDC_EDIT62,news_data->medium_name,30);
	GetDlgItemText(hWnd,IDC_COMBO61,news_data->medium_category,30);
	GetDlgItemText(hWnd,IDC_EDIT63,news_data->url,30);
	if (lstrlen(news_data->report_date) == 0) return -1;
	if (lstrlen(news_data->medium_name) == 0) return -1;
	if (lstrlen(news_data->medium_category) == 0) return -1;
	if (lstrlen(news_data->url) == 0) return -1;
	return 0;
}
void Add_News(HWND hWnd)
{
	NEWSDATA data,*news_data = &data;
	int province_index,case_index;
	province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST61),LB_GETCURSEL,0,0);
	case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_GETCURSEL,0,0);

	if (province_index < 0 || case_index < 0)
	{
		MessageBox(hWnd,TEXT("请选择一个省份和案件"),TEXT("提示"),MB_OK);
		return;
	}
	if (GetCurNEWSDATA (hWnd,news_data) == -1)
	{
		MessageBox(hWnd,TEXT("内容不能为空"),TEXT("错误"),MB_OK);
		return;
	}
	insertNewsByIndex(head,province_index,case_index,data);
	UpdataListBox_News(hWnd,province_index,case_index);
	MessageBox(hWnd,TEXT("添加成功"),TEXT("提示"),MB_OK);
}
void Modify_News(HWND hWnd)
{
	NEWSDATA data,*news_data = &data;
	int province_index,case_index,news_index;
	
	province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST61),LB_GETCURSEL,0,0);
	case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_GETCURSEL,0,0);
	news_index = SendMessage(GetDlgItem(hWnd,IDC_LIST63),LB_GETCURSEL,0,0);
	if (province_index < 0 || case_index < 0 || news_index < 0)
	{
		MessageBox(hWnd,TEXT("请选择一个省份和案件及新闻"),TEXT("提示"),MB_OK);
		return;
	}
	if (GetCurNEWSDATA (hWnd,news_data) == -1)
	{
		MessageBox(hWnd,TEXT("内容不能为空"),TEXT("错误"),MB_OK);
		return;
	}
	modifyNewsByIndex(head,province_index,case_index,news_index,data);
	UpdataListBox_News(hWnd,province_index,case_index);
	MessageBox(hWnd,TEXT("修改成功"),TEXT("提示"),MB_OK);
}
void Delete_News(HWND hWnd)
{
	int province_index,case_index,news_index;
	province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST61),LB_GETCURSEL,0,0);
	case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_GETCURSEL,0,0);
	news_index = SendMessage(GetDlgItem(hWnd,IDC_LIST63),LB_GETCURSEL,0,0);
	if (province_index < 0 || case_index < 0 || news_index < 0)
	{
		MessageBox(hWnd,TEXT("请选择一个省份和案件及新闻"),TEXT("提示"),MB_OK);
		return;
	}
	deleteNewsByIndex (head,province_index,case_index,news_index);
	SetEditEmpty_News(hWnd);
	UpdataListBox_News(hWnd,province_index,case_index);
	MessageBox(hWnd,TEXT("删除成功"),TEXT("提示"),MB_OK);
}

//查找窗口函数
void SetList_Find1(HWND hList,DWORD row,PROVINCEDATA *data)
{
	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.iItem = row;
    lvi.iSubItem = 0;
	lvi.pszText = data ? data->name : TEXT("未找到");
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
    lvi.iSubItem ++;
	lvi.pszText = data ? data->discipline : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->telephone : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
}
void FindProvinceByName(HWND hWnd,LPWSTR key)
{
	PROVINCEDATA **province_data;
	int i = 0,sum = 0;
	SendMessage(hList2[0],LVM_DELETEALLITEMS,0,0);
	province_data = fetchALLProvince(head);
	i = 0;
	while (province_data[i])
	{
		if (lstrcmp(province_data[i]->name,key) == 0)
		{
			SetList_Find1(hList2[0],sum,province_data[i]);
			sum++;
		}
		i++;
	}
	if (sum == 0) SetList_Find1(hList2[0],0,NULL);
}
void FindProvinceBydiscipline(HWND hWnd,LPWSTR key)
{
	PROVINCEDATA **province_data;
	int i = 0,sum = 0;
	SendMessage(hList2[0],LVM_DELETEALLITEMS,0,0);
	province_data = fetchALLProvince(head);
	i = 0;
	while (province_data[i])
	{
		if (lstrcmp(province_data[i]->discipline,key) == 0)
		{
			SetList_Find1(hList2[0],sum,province_data[i]);
			sum++;
		}
		i++;
	}
	free (province_data);
	if (sum == 0) SetList_Find1(hList2[0],0,NULL);
}
void SetList_Find2(HWND hList,DWORD row,CASEDATA *data)
{
	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.iItem = row;
    lvi.iSubItem = 0;
	lvi.pszText = data ? data->id_case : TEXT("未找到");
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
    lvi.iSubItem ++;
	lvi.pszText = data ? data->date : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->province : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->defendant : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->admin_level : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->crime : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->money : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->punish : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
}
void FindCaseById(HWND hWnd,LPWSTR key)
{
	CASEDATA **case_data;
	int i = 0,j = 0,sum = 0;
	SendMessage(hList2[1],LVM_DELETEALLITEMS,0,0);
	for (j = 0;j < 34;j++)
	{
		if (NULL == (case_data = fetchAllCase(head,j))) continue;
		i = 0;
		while (case_data[i])
		{
			if (lstrcmp(case_data[i]->id_case,key) == 0)
			{
				SetList_Find2(hList2[1],sum,case_data[i]);
				sum++;
			}
			i++;
		}
		free(case_data);
	}
	if (sum == 0) SetList_Find1(hList2[1],0,NULL);
}
void FindCaseByDefendantAndLevel(HWND hWnd,LPWSTR key1,LPWSTR key2)
{
	CASEDATA **case_data;
	int i = 0,j = 0,sum = 0;
	SendMessage(hList2[1],LVM_DELETEALLITEMS,0,0);
	
	for (j = 0;j < 34;j++)
	{
		if (NULL == (case_data = fetchAllCase(head,j))) continue;
		i = 0;
		while (case_data[i])
		{
			if (lstrcmp(case_data[i]->defendant,key1) == 0 && lstrcmp(case_data[i]->admin_level,key2) == 0)
			{
				SetList_Find2(hList2[1],sum,case_data[i]);
				sum++;
			}
			i++;
		}
		free(case_data);
	}
	if (sum == 0) SetList_Find2(hList2[1],0,NULL);
}
void SetList_Find3(HWND hList,DWORD row,NEWSDATA *data)
{
	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.iItem = row;
    lvi.iSubItem = 0;
	lvi.pszText = data ? data->id_case : TEXT("未找到");
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
    lvi.iSubItem ++;
	lvi.pszText = data ? data->report_date : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->medium_category : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->medium_name : TEXT("");
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	lvi.pszText = data ? data->url : TEXT("");
	SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
}
void FindNewsByIdAndMedium(HWND hWnd,LPWSTR key1,LPWSTR key2)
{
	NEWSDATA **news_data;
	int i = 0,j = 0,k = 0,case_count,sum = 0;
	SendMessage(hList2[2],LVM_DELETEALLITEMS,0,0);
	
	for (j = 0;j < 34;j++)
	{
		case_count = CountCase(head,j);
		for (k = 0;k < case_count;k++)
		{
			if (NULL == (news_data = fetchALLNews(head,j,k))) continue;
			i = 0;
			while (news_data[i])
			{
				if (lstrcmp(news_data[i]->id_case,key1) == 0 && lstrcmp(news_data[i]->medium_name,key2) == 0)
				{
					SetList_Find3(hList2[2],sum,news_data[i]);
					sum++;
				}
				i++;
			}
			free(news_data);
		}
	}
	if (sum == 0) SetList_Find3(hList2[2],0,NULL);
}
void FindNewsByDateAndPartOfMedium(HWND hWnd,LPWSTR key1,LPWSTR key2)
{
	NEWSDATA **news_data;
	int i = 0,j = 0,k = 0,case_count,sum = 0;
	SendMessage(hList2[2],LVM_DELETEALLITEMS,0,0);
	
	for (j = 0;j < 34;j++)
	{
		case_count = CountCase(head,j);
		for (k = 0;k < case_count;k++)
		{
			if (NULL == (news_data = fetchALLNews(head,j,k))) continue;
			i = 0;
			while (news_data[i])
			{
				if (lstrcmp(news_data[i]->report_date,key1) == 0 && lstrpbrk(news_data[i]->medium_name,key2) > 0)
				{
					SetList_Find3(hList2[2],sum,news_data[i]);
					sum++;
				}
				i++;
			}
			free(news_data);
		}
	}
	if (sum == 0) SetList_Find3(hList2[2],0,NULL);
}

//统计窗口函数
void SetStatisticsList1(HWND hList,int row,StatisticsData1* data)
{
	TCHAR string[50],*buf = string;
	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.iItem = row;
    lvi.iSubItem = 0;
	wmemset(buf,L'\0',50);
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->year);
	else buf = TEXT("无信息");
	lvi.pszText = buf;
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
    lvi.iSubItem ++;
	if (NULL != data)  wsprintf(buf,TEXT("%d"),data->case_number);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) _stprintf(buf,TEXT("%.2lf万"),data->total_amount);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
}
void ComboBoxSelect1(HWND hList,int type,LPWSTR key)
{
	int i;
	StatisticsData1** data;
	ClearList(hList);
	data = StatisticsFunction1(head,key,type);

	i = 0;
	if (data[i] == NULL) 
	{
		SetStatisticsList1(hList,i,data[i]);
		return;
	}
	while (data[i]) i++;
	quicksort1(data,0,i-1);

	i = 0;
	while (data[i])
	{
		SetStatisticsList1(hList,i,data[i]);
		i++;
	}
	i = 0;
	while(data[i])
	{
		free(data[i]);
		i++;
	}
	free(data);
}
void SetStatisticsList2(HWND hList,int row,StatisticsData2* data)
{
	TCHAR string[50],*buf = string;
	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.iItem = row;
    lvi.iSubItem = 0;
	wmemset(buf,L'\0',50);
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->year);
	else buf = TEXT("无信息");
	lvi.pszText = buf;
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
    lvi.iSubItem ++;
	if (NULL != data)  wsprintf(buf,TEXT("%d"),data->death_sentence);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->life_imprisonment);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->imprisonment1_5);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->imprisonment6_10);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->imprisonment11_more);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
}
void ComboBoxSelect2(HWND hList,LPWSTR key)
{
	int i;
	StatisticsData2** data;
	ClearList(hList);

	data = StatisticsFunction2(head,key);
	i = 0;
	if (data[i] == NULL) 
	{
		SetStatisticsList2(hList,i,data[i]);
		return;
	}
	while (data[i]) i++;
	quicksort2(data,0,i-1);

	i = 0;
	while (data[i])
	{
		SetStatisticsList2(hList,i,data[i]);
		i++;
	}
	i = 0;
	while(data[i])
	{
		free(data[i]);
		i++;
	}
	free(data);
}
void SetStatisticsList3(HWND hList,int row,StatisticsData3* data)
{
	TCHAR string[50],*buf = string;
	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.iItem = row;
    lvi.iSubItem = 0;
	wmemset(buf,L'\0',50);
	if (NULL != data) wsprintf(buf,TEXT("%s"),data->id_case);
	else buf = TEXT("无信息");
	lvi.pszText = buf;
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
    lvi.iSubItem ++;
	if (NULL != data)  wsprintf(buf,TEXT("%s"),data->admin_level);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%s"),data->crime);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%s万"),data->money);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->report_number);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
}
void ComboBoxSelect3(HWND hList)
{
	int i;
	StatisticsData3** data;
	ClearList(hList);

	data = StatisticsFunction3(head);
	i = 0;
	if (data[i] == NULL) 
	{
		SetStatisticsList3(hList,i,data[i]);
		return;
	}
	while (data[i]) i++;
	quicksort3(data,0,i-1);

	i = 0;
	while (data[i])
	{
		SetStatisticsList3(hList,i,data[i]);
		i++;
		if (i == 10) break;
	}
	i = 0;
	while(data[i])
	{
		free(data[i]);
		i++;
	}
	free(data);
}
void SetStatisticsList4(HWND hList,int row,StatisticsData4* data)
{
	TCHAR string[50],*buf = string;
	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.iItem = row;
    lvi.iSubItem = 0;
	wmemset(buf,L'\0',50);
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->year);
	else buf = TEXT("无信息");
	lvi.pszText = buf;
	SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
    lvi.iSubItem ++;
	if (NULL != data)  wsprintf(buf,TEXT("%d"),data->case_number);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) _stprintf(buf,TEXT("%.2lf万"),data->total_amount);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data)  wsprintf(buf,TEXT("%d"),data->death_sentence);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->life_imprisonment);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->imprisonment1_5);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->imprisonment6_10);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	lvi.iSubItem ++;
	if (NULL != data) wsprintf(buf,TEXT("%d"),data->imprisonment11_more);
	else buf = TEXT("");
	lvi.pszText = buf;
    SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);

}
void ComboBoxSelect4(HWND hList)
{
	int i;
	StatisticsData4** data;
	ClearList(hList);

	data = StatisticsFunction4(head);
	i = 0;
	if (data[i] == NULL) 
	{
		SetStatisticsList4(hList,i,data[i]);
		return;
	}
	while (data[i]) i++;

	quicksort4(data,0,i-1);

	i = 0;
	while (data[i])
	{
		SetStatisticsList4(hList,i,data[i]);
		i++;
	}
	i = 0;
	while(data[i])
	{
		free(data[i]);
		i++;
	}
	free(data);
}

//选项窗口函数
void SetConfigFile(HWND hWnd) //写配置文件
{
	if (IsDlgButtonChecked(hWnd,IDC_CHECK1) ==  BST_CHECKED)
	{
		WritePrivateProfileString(TEXT("AutoData"),TEXT("Import"),TEXT("1"),configFilePath);
	}
	else
	{
		WritePrivateProfileString(TEXT("AutoData"),TEXT("Import"),TEXT("0"),configFilePath);
	}
	if (IsDlgButtonChecked(hWnd,IDC_CHECK2) ==  BST_CHECKED)
	{
		WritePrivateProfileString(TEXT("AutoData"),TEXT("Export"),TEXT("1"),configFilePath);
	}
	else
	{
		WritePrivateProfileString(TEXT("AutoData"),TEXT("Export"),TEXT("0"),configFilePath);
	}
}
void GetConfigFile(HWND hWnd) //读配置文件
{
	TCHAR value[5];
	GetPrivateProfileString(TEXT("AutoData"),TEXT("Import"),NULL,value,5,configFilePath);
	if (lstrcmp(value,TEXT("1")) == 0)
	{
		CheckDlgButton(hWnd,IDC_CHECK1,BST_CHECKED);
	}
	else
	{
		CheckDlgButton(hWnd,IDC_CHECK1,BST_UNCHECKED);
	}
	GetPrivateProfileString(TEXT("AutoData"),TEXT("Export"),NULL,value,5,configFilePath);
	if (lstrcmp(value,TEXT("1")) == 0)
	{
		CheckDlgButton(hWnd,IDC_CHECK2,BST_CHECKED);
	}
	else
	{
		CheckDlgButton(hWnd,IDC_CHECK2,BST_UNCHECKED);
	}
}

//窗口过程回调函数
BOOL CALLBACK ProcWinOption(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
    {
		case WM_INITDIALOG:
		{
			GetConfigFile(hWnd);
		}
		break;

        case WM_COMMAND:
        if (lParam != 0)
        {
            if(HIWORD(wParam) == BN_CLICKED)
            {
                switch(LOWORD(wParam))
                {
                    case IDC_OPTION_OK:
						SetConfigFile(hWnd);
						updateWinFlag = 1;  //更新内容 
                        EndDialog(hWnd,0);
                        break;
                }
            }
        }
        break;
		case WM_CLOSE:
		{
			SetConfigFile(hWnd);
			updateWinFlag = 1;
			EndDialog(hWnd,0);
		}
		break;
    }
    return 0;
}
BOOL CALLBACK ProcWinStatistics(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	HWND hComboBox[3];
    TCHAR key[12];
	int index;
	switch(Msg)
    {
		case WM_INITDIALOG:
		{
			hList3[0] = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
            32,50,356,140,hWnd,NULL,g_hInstance,NULL);
			InitColumn_Statistics1(hList3[0]);
			hList3[1] = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
            456,50,328,140,hWnd,NULL,g_hInstance,NULL);
			InitColumn_Statistics1(hList3[1]);
			hList3[2] = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
            32,235,356,130,hWnd,NULL,g_hInstance,NULL);
			InitColumn_Statistics3(hList3[2]);
			hList3[3] = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
            456,235,328,130,hWnd,NULL,g_hInstance,NULL);
			InitColumn_Statistics2(hList3[3]);
			hList3[4] = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
            50,395,698,160,hWnd,NULL,g_hInstance,NULL);
			InitColumn_Statistics4(hList3[4]);

			hComboBox[0] = GetDlgItem(hWnd,IDC_COMBO_PROVINCE2);
			InitComboBox_Province(hComboBox[0]);
			hComboBox[1] = GetDlgItem(hWnd,IDC_COMBO_CRIME);
			InitComboBox_Crime(hComboBox[1]);
			hComboBox[2] = GetDlgItem(hWnd,IDC_COMBO_LEVEL);
			InitComboBox_Level(hComboBox[2]);

			ComboBoxSelect3(hList3[3]);
			ComboBoxSelect4(hList3[4]);
		}
		break;

        case WM_COMMAND:
		{
			if (lParam != 0)
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					switch(LOWORD(wParam))
					{
						case IDC_BU_STABACK:
							updateWinFlag = 1;
							EndDialog(hWnd,0);
							break;
					}
				}
				if (HIWORD(wParam) == CBN_CLOSEUP)
				{
					switch(LOWORD(wParam))
					{
						case IDC_COMBO_CRIME:
							index = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_CRIME),CB_GETCURSEL,0,0);
							SendMessage(GetDlgItem(hWnd,IDC_COMBO_CRIME),CB_GETLBTEXT,index,(LPARAM)key);
							ComboBoxSelect1(hList3[0],1,key);
							break;
						case IDC_COMBO_PROVINCE2:
							index = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_PROVINCE2),CB_GETCURSEL,0,0);
							SendMessage(GetDlgItem(hWnd,IDC_COMBO_PROVINCE2),CB_GETLBTEXT,index,(LPARAM)key);
							ComboBoxSelect1(hList3[1],2,key);
							break;
						case IDC_COMBO_LEVEL:
							index = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_LEVEL),CB_GETCURSEL,0,0);
							SendMessage(GetDlgItem(hWnd,IDC_COMBO_LEVEL),CB_GETLBTEXT,index,(LPARAM)key);
							ComboBoxSelect2(hList3[2],key);
							break;
					}
				}
			}
		}
        break;

		case WM_CLOSE:
		{
			updateWinFlag = 1;
			EndDialog(hWnd,0);
		}
		break;
    }
    return 0;
}
BOOL CALLBACK ProcWinFind(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	TCHAR key[30];
	TCHAR key2[30];
    switch(Msg)
    {
		case WM_INITDIALOG:
		{
			 hList2[0] = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
             40,75,396,75,hWnd,NULL,g_hInstance,NULL);
			 InitColumn_Find1(hList2[0]);
			 hList2[1] = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
             40,212,396,150,hWnd,NULL,g_hInstance,NULL);
			 InitColumn_MainDlg(hList2[1]);
			 hList2[2] = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
             40,460,396,120,hWnd,NULL,g_hInstance,NULL);
			 InitColumn_Find2(hList2[2]);
			 CheckDlgButton(hWnd,IDC_RADIO1,BST_CHECKED);
			 CheckDlgButton(hWnd,IDC_RADIO3,BST_CHECKED);
			 CheckDlgButton(hWnd,IDC_RADIO5,BST_CHECKED);
			 InitComboBox_Level(GetDlgItem(hWnd,IDC_COMBO_ADMIN_LEVEL));
		}
		break;

        case WM_COMMAND:
		{
			if (lParam != 0)
			{
				switch (LOWORD(wParam))
				{
					case IDC_BUTTON_FIND1:
						GetDlgItemText(hWnd,IDC_EDIT11,key,30);
						if (IsDlgButtonChecked(hWnd,IDC_RADIO1) == BST_CHECKED) FindProvinceByName(hWnd,key);
						if (IsDlgButtonChecked(hWnd,IDC_RADIO2) == BST_CHECKED) FindProvinceBydiscipline(hWnd,key);
						break;
					case IDC_BUTTON_FIND2:
						GetDlgItemText(hWnd,IDC_EDIT12,key,30);
						GetDlgItemText(hWnd,IDC_COMBO_ADMIN_LEVEL,key2,30);
						if (IsDlgButtonChecked(hWnd,IDC_RADIO3) == BST_CHECKED) FindCaseById(hWnd,key);
						if (IsDlgButtonChecked(hWnd,IDC_RADIO4) == BST_CHECKED) FindCaseByDefendantAndLevel(hWnd,key,key2);
						break;
					case IDC_BUTTON_FIND3:
						GetDlgItemText(hWnd,IDC_EDIT13,key,30);
						GetDlgItemText(hWnd,IDC_EDIT14,key2,30);
						if (IsDlgButtonChecked(hWnd,IDC_RADIO5) == BST_CHECKED) FindNewsByIdAndMedium(hWnd,key,key2);
						if (IsDlgButtonChecked(hWnd,IDC_RADIO6) == BST_CHECKED) FindNewsByDateAndPartOfMedium(hWnd,key,key2);
						break;
					case IDC_RADIO1:
						SetDlgItemText(hWnd,IDC_STATIC1,TEXT("省份:"));
						break;
					case IDC_RADIO2:
						SetDlgItemText(hWnd,IDC_STATIC1,TEXT("纪检人:"));
						break;
					case IDC_RADIO3:
						SetDlgItemText(hWnd,IDC_STATIC2,TEXT("编号:"));
						SendMessage(GetDlgItem(hWnd,IDC_COMBO_ADMIN_LEVEL),CB_SETCURSEL,-1,0);
						break;
					case IDC_RADIO4:
						SetDlgItemText(hWnd,IDC_STATIC2,TEXT("被告:"));
						SendMessage(GetDlgItem(hWnd,IDC_COMBO_ADMIN_LEVEL),CB_SETCURSEL,0,0);
						break;
					case IDC_RADIO5:
						SetDlgItemText(hWnd,IDC_STATIC3,TEXT("编号:"));
						SetDlgItemText(hWnd,IDC_STATIC4,TEXT("媒体名:"));
						break;
					case IDC_RADIO6:
						SetDlgItemText(hWnd,IDC_STATIC3,TEXT("报道日:"));
						SetDlgItemText(hWnd,IDC_STATIC4,TEXT("媒体名:"));
						break;
					case IDC_BU_FINDBACK:
						updateWinFlag = 1;
						EndDialog(hWnd,0);
						break;
				}
			}
		}
        break;

		case WM_CLOSE:
		{
			updateWinFlag = 1;
			EndDialog(hWnd,0);
		}
		break;
    }
    return 0;
}
BOOL CALLBACK ProcWinCase(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	CASEDATA **case_data;
	CASEDATA *data;
	int province_index,case_index,i;
    switch(Msg)
    {
		case WM_INITDIALOG:
		{
			InitComboBox_Crime(GetDlgItem(hWnd,IDC_COMBO52));
			InitComboBox_Level(GetDlgItem(hWnd,IDC_COMBO53));
			if (winFlag == 1)
			{
				CheckDlgButton(hWnd,IDC_RADIO51,BST_CHECKED);
				CheckDlgButton(hWnd,IDC_RADIO52,BST_UNCHECKED);
				CheckDlgButton(hWnd,IDC_RADIO53,BST_UNCHECKED);
				SetDlgItemText(hWnd,IDC_BUTTON51,TEXT("添加"));
			}
			else if (winFlag == 2)
			{
				CheckDlgButton(hWnd,IDC_RADIO51,BST_UNCHECKED);
				CheckDlgButton(hWnd,IDC_RADIO52,BST_CHECKED);
				CheckDlgButton(hWnd,IDC_RADIO53,BST_UNCHECKED);
				SetDlgItemText(hWnd,IDC_BUTTON51,TEXT("修改"));
			}
			else if (winFlag == 3)
			{
				CheckDlgButton(hWnd,IDC_RADIO51,BST_UNCHECKED);
				CheckDlgButton(hWnd,IDC_RADIO52,BST_UNCHECKED);
				CheckDlgButton(hWnd,IDC_RADIO53,BST_CHECKED);
				SetDlgItemText(hWnd,IDC_BUTTON51,TEXT("删除"));
			}
			InitListBox_Province(GetDlgItem(hWnd,IDC_LIST51));
			
		}
		break;

        case WM_COMMAND:
		{
			if (lParam != 0)
			{
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					switch (LOWORD(wParam))
					{
						case IDC_LIST51:
							SendMessage(GetDlgItem(hWnd,IDC_LIST52),LB_RESETCONTENT,0,0);
							province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST51),LB_GETCURSEL,0,0L);
							if (province_index < 0) break;
							if (NULL == (case_data = fetchAllCase(head,province_index)))
							{
								SendMessage(GetDlgItem(hWnd,IDC_LIST52),LB_ADDSTRING,0,(LPARAM)TEXT("无信息"));
								break;
							}
							i = 0;
							while (case_data[i])
							{
								SetListBox_Case(GetDlgItem(hWnd,IDC_LIST52),case_data[i]);
								i++;
							}
							free(case_data);
							break;
						case IDC_LIST52:
							{
								province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST51),LB_GETCURSEL,0,0L);
								case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST52),LB_GETCURSEL,0,0L);
								if (province_index < 0 || case_index < 0) break;
								if (NULL == (data = fetchCaseByIndex(head,province_index,case_index))) break;
								SetEdit_Case(hWnd,data);
							}
							break;
					}
				}
				else
				{
					switch (LOWORD(wParam))
					{
						case IDC_RADIO51:
							SetListBox_Case_Radio1(hWnd);
							break;
						case IDC_RADIO52:
							SetListBox_Case_Radio2(hWnd);
							break;
						case IDC_RADIO53:
							SetListBox_Case_Radio3(hWnd);
							break;
						case IDC_BUTTON52:
							updateWinFlag = 1;
							EndDialog(hWnd,0);
							break;
						case IDC_BUTTON51:
							if (IsDlgButtonChecked(hWnd,IDC_RADIO51) == BST_CHECKED) Add_Case(hWnd);
							if (IsDlgButtonChecked(hWnd,IDC_RADIO52) == BST_CHECKED) Modify_Case(hWnd);
							if (IsDlgButtonChecked(hWnd,IDC_RADIO53) == BST_CHECKED) Delete_Case(hWnd);
							break;
					}
				}
			}
		}
		break;

		case WM_CLOSE:
		{
			updateWinFlag = 1;
			EndDialog(hWnd,0);
		}
		break;
    }
    return 0;
}
BOOL CALLBACK ProcWinNews(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	CASEDATA **case_data;
	NEWSDATA **news_data;
	NEWSDATA *data;
	int province_index,case_index,news_index,i;
    switch(Msg)
    {
		case WM_INITDIALOG:
		{
			InitComboBox_Medium(GetDlgItem(hWnd,IDC_COMBO61));
			if (winFlag == 1)
			{
				CheckDlgButton(hWnd,IDC_RADIO61,BST_CHECKED);
				CheckDlgButton(hWnd,IDC_RADIO62,BST_UNCHECKED);
				CheckDlgButton(hWnd,IDC_RADIO63,BST_UNCHECKED);
			}
			else if (winFlag == 2)
			{
				CheckDlgButton(hWnd,IDC_RADIO61,BST_UNCHECKED);
				CheckDlgButton(hWnd,IDC_RADIO62,BST_CHECKED);
				CheckDlgButton(hWnd,IDC_RADIO63,BST_UNCHECKED);
			}
			else if (winFlag == 3)
			{
				CheckDlgButton(hWnd,IDC_RADIO61,BST_UNCHECKED);
				CheckDlgButton(hWnd,IDC_RADIO62,BST_UNCHECKED);
				CheckDlgButton(hWnd,IDC_RADIO63,BST_CHECKED);
			}
			InitListBox_Province(GetDlgItem(hWnd,IDC_LIST61));

		}
		break;

        case WM_COMMAND:
		{
			if (lParam != 0)
			{
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					switch (LOWORD(wParam))
					{
						case IDC_LIST61:
							SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_RESETCONTENT,0,0);
							SendMessage(GetDlgItem(hWnd,IDC_LIST63),LB_RESETCONTENT,0,0);
							province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST61),LB_GETCURSEL,0,0L);
							if (province_index < 0) break;
							if (NULL == (case_data = fetchAllCase(head,province_index)))
							{
								SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_ADDSTRING,0,(LPARAM)TEXT("无信息"));
								break;
							}
							i = 0;
							while (case_data[i])
							{
								SetListBox_Case(GetDlgItem(hWnd,IDC_LIST62),case_data[i]);
								i++;
							}
							free(case_data);
							break;
						case IDC_LIST62:
							{
								SendMessage(GetDlgItem(hWnd,IDC_LIST63),LB_RESETCONTENT,0,0);
								province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST61),LB_GETCURSEL,0,0L);
								case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_GETCURSEL,0,0L);
								if (province_index < 0 || case_index < 0) break;
								if (NULL == (news_data = fetchALLNews(head,province_index,case_index)))
								{
									SendMessage(GetDlgItem(hWnd,IDC_LIST63),LB_ADDSTRING,0,(LPARAM)TEXT("无信息"));
									break;
								}
								i = 0;
								while (news_data[i])
								{
									SetListBox_News(GetDlgItem(hWnd,IDC_LIST63),news_data[i]);
									i++;
								}
								free(news_data);
							}
							break;
						case IDC_LIST63:
							{
								province_index = SendMessage(GetDlgItem(hWnd,IDC_LIST61),LB_GETCURSEL,0,0L);
								case_index = SendMessage(GetDlgItem(hWnd,IDC_LIST62),LB_GETCURSEL,0,0L);
								news_index = SendMessage(GetDlgItem(hWnd,IDC_LIST63),LB_GETCURSEL,0,0L);
								if (province_index < 0 || case_index < 0 || news_index <0) break;
								if (NULL == (data = fetchNewsByIndex(head,province_index,case_index,news_index))) break;
								SetEdit_News(hWnd,data);
							}
							break;
					}
				}
				else
				{
					switch (LOWORD(wParam))
					{
						case IDC_RADIO61:
							SetListBox_News_Radio1(hWnd);
							break;
						case IDC_RADIO62:
							SetListBox_News_Radio2(hWnd);
							break;
						case IDC_RADIO63:
							SetListBox_News_Radio3(hWnd);
							break;
						case IDC_BUTTON62:
							updateWinFlag = 1;
							EndDialog(hWnd,0);
							break;
						case IDC_BUTTON61:
							if (IsDlgButtonChecked(hWnd,IDC_RADIO61) == BST_CHECKED) Add_News(hWnd);
							if (IsDlgButtonChecked(hWnd,IDC_RADIO62) == BST_CHECKED) Modify_News(hWnd);
							if (IsDlgButtonChecked(hWnd,IDC_RADIO63) == BST_CHECKED) Delete_News(hWnd);
							break;
					}
				}
			}
		}
        break;

		case WM_CLOSE:
		{
			updateWinFlag = 1;
			EndDialog(hWnd,0);
		}
		break;
    }
    return 0;
}
BOOL CALLBACK ProcWinProvince(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	HWND hComboBox;
	PROVINCEDATA data,*province_data=&data;
	int province_index;
    switch(Msg)
    {
		case WM_INITDIALOG:
		{
			hComboBox = GetDlgItem(hWnd,IDC_COMBO_PROVINCE3);
			InitComboBox_Province(hComboBox);
			SendMessage(hComboBox, CB_SETCURSEL , 0, 0L); 
			province_index = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_PROVINCE3),CB_GETCURSEL,0,0);
			if (NULL != (province_data = fetchProvinceByIndex(head,province_index))) 
			{
				SetDlgItemText (hWnd,IDC_EDIT21,province_data->discipline);
				SetDlgItemText (hWnd,IDC_EDIT22,province_data->telephone);
			}
		}
		break;

        case WM_COMMAND:
		{
			if  (lParam != 0)
            {
				switch (LOWORD(wParam))
				{
					case IDC_BUTTON21:
						province_index = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_PROVINCE3),CB_GETCURSEL,0,0);
						GetDlgItemText(hWnd,IDC_COMBO_PROVINCE3,province_data->name,30);
						GetDlgItemText (hWnd,IDC_EDIT21,province_data->discipline,30);
						GetDlgItemText (hWnd,IDC_EDIT22,province_data->telephone,30);
						if (modifyProvinceByIndex (head,province_index,data) == 0)
						{
							MessageBox(hWnd,TEXT("修改成功"),TEXT("提示"),MB_OK);
						}
						break;
					case IDC_BUTTON22:
						updateWinFlag = 1;
						EndDialog(hWnd,0);
						break;
					default:
						break;
				}
				if (HIWORD(wParam) == CBN_CLOSEUP)
				{
					switch (LOWORD(wParam))
					{
						case IDC_COMBO_PROVINCE3:
						province_index = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO_PROVINCE3),CB_GETCURSEL,0,0);
						if (NULL == (province_data = fetchProvinceByIndex(head,province_index)) && province_index > 0) 
						{
							SetDlgItemText (hWnd,IDC_EDIT21,TEXT("无信息"));
							SetDlgItemText (hWnd,IDC_EDIT22,TEXT("无信息"));
						}
						else
						{
							SetDlgItemText (hWnd,IDC_EDIT21,province_data->discipline);
							SetDlgItemText (hWnd,IDC_EDIT22,province_data->telephone);
						}
						break;
					}
				}
			}
		}
        break;
		case WM_CLOSE:
		{
			updateWinFlag = 1;
			EndDialog(hWnd,0);
		}
		break;
    }
    return 0;
}
BOOL CALLBACK ProcWinAbout(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
    switch(Msg)
    {
		case WM_INITDIALOG:
		{
			return TRUE ;
		}
		break;

        case WM_COMMAND:
        if (lParam != 0)
        {
            if(HIWORD(wParam) == BN_CLICKED)
            {
                switch(LOWORD(wParam))
                {
                    case IDC_BUTTON_BACK:
						updateWinFlag = 1;
                        EndDialog(hWnd,IDC_BUTTON_BACK);
                        break;
                }
            }
        }
        break;
 
    }
    return 0;
}
LRESULT CALLBACK ProcWinMain(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
    HWND hComboBox;
	PAINTSTRUCT stPs;
    HDC hDC;
	int index;
	TCHAR value[5];

    switch(Msg)
    {
        case WM_CREATE:
        {
            hList1 = CreateWindowEx(0,ListViewClassName,NULL,LVS_REPORT | WS_CHILD |WS_VISIBLE,
            20,85,550,250,hWnd,NULL,g_hInstance,NULL);
            InitColumn_MainDlg(hList1);
        }
        break;

		case WM_PAINT:
		{
			hDC = BeginPaint(hWnd,&stPs);
			hComboBox = GetDlgItem(hWnd,IDC_COMBO_PROVINCE);
			if (isInit == 1)
			{
				SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)TEXT("所有"));
				InitComboBox_Province(hComboBox);
				InitEdit_News(hWnd);
				InitEdit_Province(hWnd);
				SendMessage(hComboBox, CB_SETCURSEL , 0, 0L); 
				InitList_All_Province(hList1);
				isInit = 0;
			}

			EndPaint(hWnd,&stPs);
		}
		break;

		case WM_ACTIVATE:
		{
			 if(updateWinFlag) 
			 {
				 UpdateMainList(hWnd);
				 updateWinFlag = 0;
			 } 
		}
		break;

        case WM_COMMAND:
        {
            if  (lParam == 0)
            {
                switch(LOWORD(wParam))
                {
                    case ID_APP_EXIT:
                        DestroyWindow(hWnd);
                        break;
                    case ID_APP_ABOUT:
                        hAboutDialog = (HWND)DialogBox(g_hInstance,AboutDlgName,hMainDialog,(DLGPROC)ProcWinAbout);
                        break;
					case ID_OPTION:
                        hOptionDialog = (HWND)DialogBox(g_hInstance,OptionDlgName,hMainDialog,(DLGPROC)ProcWinOption);
                        break;
					case ID_EMPTY:
						index = MessageBox(hWnd,TEXT("确定要清空?"),TEXT("警告"),MB_OKCANCEL | MB_ICONWARNING);
						if (index == IDOK)
						{
							if (emptyAll(head) == 0)
							{
								MessageBox(hWnd,TEXT("清除成功!"),TEXT("成功"),MB_OK);
								SetDlgItemText(hWnd,IDC_NEWS_COUNT,TEXT(""));
								UpdateMainList(hWnd);
							}
							else MessageBox(hWnd,TEXT("清除失败!"),TEXT("失败"),MB_OK);
						}
						break;
                    case ID_FILE_IMPORT:
						RtlZeroMemory(buffer,MAXSIZE);
                        g_ofn.lStructSize = sizeof(g_ofn);
                        g_ofn.hInstance = g_hInstance;
                        g_ofn.lpstrFilter = FilterString;
                        g_ofn.lpstrFile = buffer;
                        g_ofn.nMaxFile = MAXSIZE;
                        g_ofn.Flags = OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST |OFN_LONGNAMES|OFN_EXPLORER|OFN_HIDEREADONLY;
                        g_ofn.lpstrFileTitle = OurTitle;
                        if(GetOpenFileName(&g_ofn))
                        {
							RtlZeroMemory(FilePath,OUTPUTSIZE);
							wsprintf(FilePath,TEXT("%s"),g_ofn.lpstrFile);
							if (importFile(head,FilePath) == 0)
							{
								SetDlgItemText(hWnd,IDC_NEWS_COUNT,TEXT(""));
								UpdateMainList(hWnd);
								wsprintf(buffer,TEXT("%s已被成功导入本系统!"),FilePath);
								MessageBox(hWnd,buffer,TEXT("导入成功!"),MB_OK);
							}
							else
							{
								wsprintf(buffer,TEXT("%s导入失败!"),FilePath);
								MessageBox(hWnd,buffer,TEXT("导入失败!"),MB_OK);
							}
                        }
                    break;
                    case ID_FILE_EXPORT:
						RtlZeroMemory(buffer,MAXSIZE);
                        g_ofn.lStructSize = sizeof(g_ofn);
                        g_ofn.hInstance = g_hInstance;
                        g_ofn.lpstrFilter = FilterString;
                        g_ofn.lpstrFile = buffer;
                        g_ofn.nMaxFile = MAXSIZE;
                        g_ofn.Flags = OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST |OFN_LONGNAMES|OFN_EXPLORER|OFN_HIDEREADONLY;
                        g_ofn.lpstrFileTitle = OurTitle;
                        if(GetSaveFileName(&g_ofn))
                        {
							RtlZeroMemory(FilePath,OUTPUTSIZE);

							if (CheckPath(g_ofn.lpstrFile) == 0)
							{
								wsprintf(FilePath,TEXT("%s.dat"),g_ofn.lpstrFile);
							}
							else
							{
								wsprintf(FilePath,TEXT("%s"),g_ofn.lpstrFile);
							}
							
							if (exportFile(head,FilePath) == 0)
							{
								wsprintf(buffer,TEXT("导出至:%s"),FilePath);
								MessageBox(hWnd,buffer,TEXT("导出成功!"),MB_OK);
							}
							else
							{
								wsprintf(buffer,TEXT("%s导出失败!"),FilePath);
								MessageBox(hWnd,buffer,TEXT("导出失败!"),MB_OK);
							}
                        }
                    break;

					case ID_PROVINCE_INFO:
						hProvinceDialog = (HWND)DialogBox(g_hInstance,ProvinceDlgName,hWnd,(DLGPROC)ProcWinProvince);
						break;
					case ID_APP_STATISTICS:
						hStatisticsDialog = (HWND)DialogBox(g_hInstance,StatisticsDlgName,hWnd,(DLGPROC)ProcWinStatistics);
						break;
					case ID_FIND:
						hFindDialog = (HWND)DialogBox(g_hInstance,FindDlgName,hWnd,(DLGPROC)ProcWinFind);
						break;
					case ID_CASE_ADD:
						winFlag = 1;
						hCaseDialog = (HWND)DialogBox(g_hInstance,CaseDlgName,hWnd,(DLGPROC)ProcWinCase);
						break;
					case ID_CASE_ALTER:
						winFlag = 2;
						hCaseDialog = (HWND)DialogBox(g_hInstance,CaseDlgName,hWnd,(DLGPROC)ProcWinCase);
						break;
					case ID_CASE_DEL:
						winFlag = 3;
						hCaseDialog = (HWND)DialogBox(g_hInstance,CaseDlgName,hWnd,(DLGPROC)ProcWinCase);
						break;
					case ID_NEWS_ADD:
						winFlag = 1;
						hNewsDialog = (HWND)DialogBox(g_hInstance,NewsDlgName,hWnd,(DLGPROC)ProcWinNews);
						break;
					case ID_NEWS_ALTER:
						winFlag = 2;
						hNewsDialog = (HWND)DialogBox(g_hInstance,NewsDlgName,hWnd,(DLGPROC)ProcWinNews);
						break;
					case ID_NEWS_DEL:
						winFlag = 3;
						hNewsDialog = (HWND)DialogBox(g_hInstance,NewsDlgName,hWnd,(DLGPROC)ProcWinNews);
						break;
                }
            }
			else 
			{
				if(HIWORD(wParam) == BN_CLICKED)
				{
					switch(LOWORD(wParam))
					{
						case IDC_BUTTON_BEFORE:
							ShowOtherNews(hWnd,-1);
							break;
						case IDC_BUTTON_NEXT:
							ShowOtherNews(hWnd,1);
							break;
					}
				}
				if (HIWORD(wParam) == CBN_CLOSEUP)
				{
					SetDlgItemText(hWnd,IDC_NEWS_COUNT,TEXT(""));
					switch(LOWORD(wParam))
					{
						case IDC_COMBO_PROVINCE:
							 UpdateMainList(hWnd);
							 break;
					}
				}
			}
        }
        break;

		case WM_NOTIFY:
        {
            NMHDR *pNm = (NMHDR *)lParam;
            if(pNm->hwndFrom == hList1)
            {
                if(pNm->code == NM_DBLCLK)
				{
					GetNews(hWnd);
				}
            }
        }
		break;

        case WM_DESTROY:
        {
			GetPrivateProfileString(TEXT("AutoData"),TEXT("Export"),NULL,value,5,configFilePath);
			if (lstrcmp(value,TEXT("1")) == 0) exportFile(head,defaultDataFilePath);
            PostQuitMessage(0);
        }
        break;

        default:
            return DefWindowProc(hWnd, Msg, wParam, lParam );
    }
    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    HICON hIcon;  //图标资源句柄
    HCURSOR hCursor; //光标资源句柄
	HACCEL hAccelerator; //快捷键资源句柄
    MSG stMsg;  //系统消息
    WNDCLASSEX stdWndClass; //窗口类
    g_hInstance = hInstance; //实例句柄
	
	setlocale(LC_ALL, "chs");//中文文本输出设置

	if (InitApp(&head) != 0)  //初始化应用
	{
		MessageBox(hMainDialog,TEXT("初始化数据失败"),TEXT("错误"),MB_RETRYCANCEL);
	}

	hAccelerator = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR));
	hIcon = LoadIcon(hInstance,MyIcon);
	hCursor = LoadCursor(NULL,IDC_ARROW);

	//注册窗口类
    RtlZeroMemory(&stdWndClass, sizeof(stdWndClass));
    stdWndClass.hCursor = LoadCursor(0,IDC_ARROW);
    stdWndClass.cbSize = sizeof(stdWndClass);
    stdWndClass.style = CS_HREDRAW|CS_VREDRAW;
    stdWndClass.lpfnWndProc = ProcWinMain;
    stdWndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    stdWndClass.lpszClassName = ClassName;
    stdWndClass.hInstance = hInstance;
    stdWndClass.lpszMenuName = MenuName;
    stdWndClass.cbClsExtra = 0;
    stdWndClass.cbWndExtra = DLGWINDOWEXTRA;
    stdWndClass.hIcon = hIcon;
    stdWndClass.hIconSm = hIcon;
    stdWndClass.hCursor = hCursor;
	
	
    RegisterClassEx(&stdWndClass);
    if ((hMainDialog = (HWND)CreateDialogParam(hInstance,MainDlgName,NULL,NULL,0)) == 0)
		return 0;
	
    ShowWindow(hMainDialog,SW_SHOWNORMAL);
    UpdateWindow(hMainDialog);

	//消息循环
    while(GetMessage(&stMsg,NULL,0,0))
    {
        if(!TranslateAccelerator(hMainDialog,hAccelerator,&stMsg))
        {
            TranslateMessage(&stMsg);
            DispatchMessage(&stMsg);
        }
    }

    return stMsg.wParam;
}
