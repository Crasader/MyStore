#pragma once

#include "Common.h"
#include "MySQL.h"
//��һ��������,�����̳���DUiLib����Ļ���



class MainWnd : public WindowImplBase {
private:
	//�����е��������麯��,�Ѵ��麯��ʵ��

	//�õ�xml�ļ�Ŀ¼����
	CDuiString GetSkinFolder();
	//�õ�xml�ļ����ֺ���
	CDuiString GetSkinFile();
	//�����������
	LPCTSTR GetWindowClassName(void) const;//const���ε����Ա����,���ں��������б�֮��,������֮ǰ,��ʾ���thisָ����һ������,�����޸���ĳ�Ա����
	//��Ϣ��Ӧ
	void Notify(TNotifyUI& msg);
	//��ѯԱ����Ϣ
	void SelectEmployeeInfo();
	//ɾ��Ա����Ϣ
	void DeleteEmployeeInfo();
	//����Ա����Ϣ
	void InsertEmployeeInfo();
	//����Ա����Ϣ
	void UpDateEmployeeInfo();
	//��ѯ��Ʒ��Ϣ
	void SelectGoodsInfo();
	//ɾ����Ʒ��Ϣ
	void DeleteGoodsInfo();
	//������Ʒ��Ϣ
	void InsertGoodsInfo();
	//������Ʒ��Ϣ
	void UpDateGoodsInfo();
public:
	MySQL* p_mysql;

};

//�ַ�ת��
string UnicodeToAsc(const CDuiString& str);

CDuiString AscToUnicode(const string& str);