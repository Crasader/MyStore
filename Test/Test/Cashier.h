#pragma once
#include "Common.h"
#include "MySQL.h"
#include "MainWnd.h"
class CashierWnd :public WindowImplBase {
protected:
	//��¼����ͬ���̳�  �������������, Ҳ��Ҫʵ�����������麯��
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const;
public:
	MySQL* p_mysql;
	//�̳��ڻ������Ӧ��Ϣ�ĺ���
	void Notify(TNotifyUI& msg);
	void SelectGoods();
	void AddGoodsCount();
	void SubGoodsCount();
	void InsertGoodsList();
	void CommiOrder();
	void CancelGoods();
};
