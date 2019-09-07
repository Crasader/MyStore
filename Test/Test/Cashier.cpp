#include "Cashier.h"




CDuiString CashierWnd::GetSkinFolder() {
	return _T("");
}
CDuiString CashierWnd::GetSkinFile() {
	return _T("Cashier.xml");
}
LPCTSTR CashierWnd::GetWindowClassName(void) const {
	return _T("CashierWnd");
}

  void CashierWnd::SelectGoods() {
 	//�ӿؼ��ҵ���Ʒ��
 	CDuiString StrGoodName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->GetText();
 	//����SQL���
 	string StrSQL("select * from goods where GoodsName='");
 	StrSQL += UnicodeToAsc(StrGoodName);
 	StrSQL += "';";
 	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
 	if (vRet.empty()) {
 		MessageBox(m_hWnd, _T("û�и���Ʒ"), _T("����"), IDOK);
 		return;
 	}


 	//����Ʒ��ʾ
	string str = vRet[0][7];
	CDuiString StrLeft = AscToUnicode(str);
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->SetText(StrLeft);
 
 }
 void CashierWnd::AddGoodsCount() {
	//���----��
 	CEditUI* pGoodsLeft = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT"));
 	CDuiString StrLeft = pGoodsLeft->GetText();
 	if (StrLeft == _T("0")) {
 		MessageBox(m_hWnd, _T("��治��"), _T("����"), IDOK);
 		return;
 	}
 	int count = atoi(UnicodeToAsc(StrLeft).c_str());
 	--count;
 	StrLeft.Format(_T("%d"), count);
 	pGoodsLeft->SetText(StrLeft);
 
 	//��Ʒ++++��
 	CEditUI* pGoodsCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_COUNT"));
 	CDuiString StrCount = pGoodsCount->GetText();
 	count = atoi(UnicodeToAsc(StrCount).c_str());
 	++count;
 	StrCount.Format(_T("%d"), count);
 	pGoodsCount->SetText(StrCount);
 }
 void CashierWnd::SubGoodsCount() {
 	//��Ʒ��һ
 	CEditUI* pGoodsCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_COUNT"));
 	CDuiString StrCount = pGoodsCount->GetText();
 	if (StrCount == _T("0")) {
 		MessageBox(m_hWnd, _T("��Ʒ�����Ѿ�Ϊ0"), _T("����"), IDOK);
 		return;
 	}
 	int count = atoi(UnicodeToAsc(StrCount).c_str());
 	--count;
 	StrCount.Format(_T("%d"), count);
	pGoodsCount->SetText(StrCount);
 	//����һ
 	CEditUI* pGoodsLeft = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT"));
 	CDuiString StrLeft = pGoodsLeft->GetText();
 	count = atoi(UnicodeToAsc(StrLeft).c_str());
 	++count;
 	StrLeft.Format(_T("%d"), count);
 	pGoodsLeft->SetText(StrLeft);
 }
 void CashierWnd::InsertGoodsList() {
 	//��ȡ��Ʒ�����Լ���������
 	CDuiString StrGoodName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->GetText();
 	CEditUI* pGoodCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_COUNT"));
 	CDuiString StrCount = pGoodCount->GetText();
 
 	//�����ݿ��ȡ��Ʒ��Ϣ
 	string StrSQL("select * from goods where GoodsName='");
 	StrSQL += UnicodeToAsc(StrGoodName);
 	StrSQL += "';";
 	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
 
 	//�����ܼ۸�
 	int count = atoi(UnicodeToAsc(StrCount).c_str());
 	double Price = atof(vRet[0][5].c_str());
 	Price = count * Price;
 	CDuiString StrPrice;
 	StrPrice.Format(_T("%lf"), Price);

 	//��List�ؼ����и���
 	CListTextElementUI* pItem = new CListTextElementUI;
 	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListGoodsInfo"));
 	pList->Add(pItem);
 	pItem->SetText(0, StrGoodName);
 	pItem->SetText(1, AscToUnicode(vRet[0][5]));
 	pItem->SetText(2, StrCount);
 	pItem->SetText(3, AscToUnicode(vRet[0][6]));
 	pItem->SetText(4, StrPrice);
	//���༭�����
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_COUNT")))->SetText(_T(""));
 }
 void CashierWnd::CommiOrder() {
	//�ϼƼ۸�
 	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListGoodsInfo"));
 	int count = pList->GetCount();
 
 	double Price = 0;
 	for (int i = 0; i < count; ++i) {
		CListTextElementUI* pItem = (CListTextElementUI*)pList->GetItemAt(i);
 		CDuiString StrPrice = pItem->GetText(4);
 		Price += atof(UnicodeToAsc(StrPrice).c_str());
 	}
 	CDuiString StrPrice;
 	StrPrice.Format(_T("%.02lf"), Price);
 	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_MONEY")))->SetText(StrPrice);

	//������Ʒ���ݿ�
 	for (int i = 0; i < count; ++i) {
		CListTextElementUI* pItem = (CListTextElementUI*)pList->GetItemAt(i);
 		CDuiString StrCount = pItem->GetText(2);
		CDuiString StrName = pItem->GetText(0);
		//CDuiString StrLeft = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->GetText();

		//��ȡ���ݿ�����Ʒ�Ŀ��
		string StrSQLLeft("select * from goods where GoodsName='");
		StrSQLLeft += UnicodeToAsc(StrName);
		StrSQLLeft += "';";
		vector<vector<string>> vRet = p_mysql->Select(StrSQLLeft);
		string StrLeft = vRet[0][7];
		//�Կ����и���,��ȥ�����ߵ���Ʒ����
		const char* str = StrLeft.c_str();
		int temp = atoi(str);
		int count = atoi((UnicodeToAsc(StrCount)).c_str());
		temp -= count;
		char buf[32] = { 0 };
		_itoa(temp, buf, 10);
		StrLeft = buf;
		//���¿����½����ݿ�
 		string StrSQLUpdate("update goods set Inventory=");
		StrSQLUpdate += StrLeft;
		StrSQLUpdate += " where GoodsName='";
		StrSQLUpdate += UnicodeToAsc(StrName);
		StrSQLUpdate += "';";
 		p_mysql->Update(StrSQLUpdate);

 	}
	pList->RemoveAll();
 	//���뱾�����ۼ�¼
 }

 //ȡ����List�ؼ��е���Ʒ

 void CashierWnd::CancelGoods() {

	 CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListGoodsInfo"));
	 //����
	 int RowNum = pList->GetCurSel();
	 //CListTextElementUI* pRow = (CListTextElementUI*)pList->GetItemAt(RowNum);
	 pList->RemoveAt(RowNum);
 }


 void CashierWnd::Notify(TNotifyUI& msg) {
	 //��ȡ�ؼ�����
	 CDuiString StrName = msg.pSender->GetName();

	 //���ݿؼ�������Ӧ��Ϣ
	 if (msg.sType == _T("click")) {
		 if (StrName == _T("BTN_CLOSE")) {
			 Close();
		 }
		 else if (StrName == _T("BTN_MIN")) {
			 SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		 }
		 else if (StrName == _T("BTN_SELECT")) {
			 SelectGoods();
		 }
		 else if (StrName == _T("BTN_ADD")) {
			 AddGoodsCount();
		 }
		 else if (StrName == _T("BTN_SUB")) {
			 SubGoodsCount();
		 }
		 else if (StrName == _T("BTN_ADDTO")) {
			 InsertGoodsList();
		 }
		 else if (StrName == _T("BTN_COMMIT")) {
			 CommiOrder();
		 }
		 else if (StrName == _T("BTN_CANCEL")) {
			 CancelGoods();
		 }
	 }
 }