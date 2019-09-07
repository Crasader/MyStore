#include "MainWnd.h"
#include "MySQL.h"

CDuiString MainWnd::GetSkinFolder() {
	/*_T ��Vs��װ��һ����,���������ǰ�ASCII���ַ�ת��ΪUnicode����ʽ*/
	return _T("");
}
CDuiString MainWnd::GetSkinFile() {
	return _T("MainWnd - ����.xml");
	//return _T("Cashier.xml");
}
LPCTSTR MainWnd::GetWindowClassName(void) const {
	return _T("MainWnd");
}

//�ַ�ת��
string UnicodeToAsc(const CDuiString& str) {
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,0,str,-1,NULL,0,NULL,NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,0,str,-1,pElementText,iTextLen,NULL,NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

CDuiString AscToUnicode(const string& str) {
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t *  pUnicode;
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	CDuiString  rt;
	rt = (wchar_t*)pUnicode;
	delete  pUnicode;

	return  rt;
}

/***********************************************Ա������***************************************************************/
//��ѯԱ����Ϣ
void MainWnd::SelectEmployeeInfo() {
	//����SQL��ѯ���
	string StrSQL("select * from employee ");
	//�ҵ�Combo�ؼ��õ��ؼ�����
	CComboBoxUI* pCombo = (CComboBoxUI*)m_PaintManager.FindControl(_T("COMOB_SELECT"));
	CDuiString StrStyle = pCombo->GetText();
	if (StrStyle == _T("��"))
		StrSQL += ";";
	else if (StrStyle == _T("����")) {
		StrSQL += "where name= '";
		CDuiString StrName = ((CEditUI*)m_PaintManager.FindControl(_T("username")))->GetText();
		if (StrName.IsEmpty()) {
			MessageBox(m_hWnd, _T("���������ѯ����"), _T("����"), IDOK);
			return;
		}
		StrSQL += UnicodeToAsc(StrName);
		StrSQL += "';";
	}
	else if (StrStyle == _T("�Ա�")) {
		StrSQL += "where gender='";
		CDuiString StrGender = ((CEditUI*)m_PaintManager.FindControl(_T("gender")))->GetText();
		if (StrGender.IsEmpty()) {
			MessageBox(m_hWnd, _T("�������������"), _T("����"), IDOK);
			return;
		}
		StrSQL += UnicodeToAsc(StrGender);
		StrSQL += "';";
	}
	else if (StrStyle == _T("н��")) {
		StrSQL += "where salary='";
		CDuiString StrSalary = ((CEditUI*)m_PaintManager.FindControl(_T("salary")))->GetText();
		if (StrSalary.IsEmpty()) {
			MessageBox(m_hWnd, _T("�������������"), _T("����"), IDOK);
			return;
		}
		StrSQL += UnicodeToAsc(StrSalary);
		StrSQL += "';";
	}
	else if (StrStyle == _T("�绰")) {
		StrSQL += "where telphone='";
		CDuiString StrTel = ((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->GetText();
		if (StrTel.IsEmpty()) {
			MessageBox(m_hWnd, _T("�������������"), _T("����"), IDOK);
			return;
		}
		StrSQL += UnicodeToAsc(StrTel);
		StrSQL += "';";
	}
	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
	if (vRet.empty())
		return;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListEmployeeInfo"));
	pList->RemoveAll();//����ϴβ�ѯ��List
	for (size_t i = 0; i < vRet.size(); ++i) {
		vector<string>& StrItem = vRet[i];
		CListTextElementUI* pDate = new CListTextElementUI;
		pDate->SetAttribute(_T("align"), _T("center"));
		pList->Add(pDate);

		pDate->SetText(0, AscToUnicode(StrItem[1]));
		pDate->SetText(1, AscToUnicode(StrItem[2]));
		pDate->SetText(2, AscToUnicode(StrItem[3]));
		pDate->SetText(3, AscToUnicode(StrItem[5]));
		pDate->SetText(4, AscToUnicode(StrItem[6]));
		pDate->SetText(5, AscToUnicode(StrItem[7]));
	}
}


//ɾ��Ա����Ϣ
void MainWnd::DeleteEmployeeInfo() {
	
	//��ȡ��ǰѡ��
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListEmployeeInfo"));

	//����
	int RowNum=pList->GetCurSel();
	CListTextElementUI*	pRow =(CListTextElementUI*) pList->GetItemAt(RowNum);

	//�����ݿ���ɾ��Ա����Ϣ

	//����SQLɾ������
	string StrSQL("delete from employee where name='");
	string StrId=UnicodeToAsc(pRow->GetText(0));//�õ���һ�е�
	StrSQL += StrId;
	StrSQL += "';";
	p_mysql->Delete(StrSQL);

	//CDuiString StrName = pRow->GetText(0);//ͨ���к��õ�List�е�����
	//��List�ؼ���ɾ��Ա����Ϣ
	pList->RemoveAt(RowNum);
}

void MainWnd::InsertEmployeeInfo() {

	//��������õ�Ա����Ϣ
	CDuiString StrName=((CEditUI*)m_PaintManager.FindControl(_T("username")))->GetText();
	CDuiString StrGender = ((CComboBoxUI*)m_PaintManager.FindControl(_T("gender")))->GetText();
	CDuiString StrBirthday= ((CEditUI*)m_PaintManager.FindControl(_T("birthday")))->GetText();
	CDuiString StrPosition = ((CComboBoxUI*)m_PaintManager.FindControl(_T("position")))->GetText();
	CDuiString StrTel = ((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->GetText();
	CDuiString StrSalay = ((CEditUI*)m_PaintManager.FindControl(_T("salary")))->GetText();


	//pList->GetCount();

	//char buf[32] = { 0 };
	//����ת��Ϊ�ַ�
	//_itoa(pList->GetCount()+1, buf, 10);
	//����SQL����
	string StrSQL("insert into employee(name,gender,birthday,password,position,telphone,salary) values('");
	//StrSQL += buf;
	//StrSQL += "','";
	StrSQL += UnicodeToAsc(StrName);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrGender);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrBirthday);
	StrSQL += "','123456','";
	StrSQL += UnicodeToAsc(StrPosition);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrTel);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrSalay);
	StrSQL += "');";
	//��ӦSQL����
	p_mysql->Insert(StrSQL);
	
	
	/*��List�ؼ�����Ӹ�Ա����Ϣ
	CListTextElementUI* pInfo = new CListTextElementUI;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListEmployeeInfo"));
	pList->Add(pInfo);
	pInfo->SetText(0, StrName);
	pInfo->SetText(1, StrGender);
	pInfo->SetText(2, StrBirthday);
	pInfo->SetText(3, StrPosition);
	pInfo->SetText(4, StrTel);
	pInfo->SetText(5, StrSalay);*/
	
	//����ͨ�����²�ѯ��List�ؼ�����ʾԱ����Ϣ
	SelectEmployeeInfo();

	//���༭�����
	((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("salary")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("birthday")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("username")))->SetText(_T(""));
}

void MainWnd::UpDateEmployeeInfo() {
	string StrSQL("update employee set ");
	//Ĭ�ϸ�����������Ա����Ϣ
	//Ĭ��ֻ�ܸ���Ȩ��/�绰/н��
	CDuiString StrName = ((CEditUI*)m_PaintManager.FindControl(_T("username")))->GetText();
	CDuiString StrPosition = ((CComboBoxUI*)m_PaintManager.FindControl(_T("position")))->GetText();
	CDuiString StrTel = ((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->GetText();
	CDuiString StrSalay = ((CEditUI*)m_PaintManager.FindControl(_T("salary")))->GetText();

	//����SQL���
	if (!StrPosition.IsEmpty()) {
		StrSQL += "position='";
		StrSQL += UnicodeToAsc(StrPosition);
		StrSQL += "'";
	}
	if (!StrTel.IsEmpty()&&StrTel!=_T("�绰")) {
		if (!StrPosition.IsEmpty()) {
			StrSQL += ",telphone='";
		}
		else {
			StrSQL += "telphone='";
		}
		StrSQL += UnicodeToAsc(StrTel);
		StrSQL += "'";
	}
	if (!StrSalay.IsEmpty()&&StrSalay!=_T("н��")) {
		if (!StrTel.IsEmpty() || !StrPosition.IsEmpty()) {
			StrSQL += ",salary='";
		}
		else {
			StrSQL += "salary='";
		}
		StrSQL += UnicodeToAsc(StrSalay);
		StrSQL += "'";
	}
	StrSQL += " where name='";
	StrSQL += UnicodeToAsc(StrName);
	StrSQL += "';";
	p_mysql->Update(StrSQL);
	SelectEmployeeInfo();

	//���༭�����
	((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->SetText(_T("�绰"));
	((CEditUI*)m_PaintManager.FindControl(_T("salary")))->SetText(_T("н��"));
	((CEditUI*)m_PaintManager.FindControl(_T("birthday")))->SetText(_T("����"));
	((CEditUI*)m_PaintManager.FindControl(_T("username")))->SetText(_T("����"));
}

/***************************************************��Ʒ����**************************************************************/

	//��ѯ��Ʒ��Ϣ
void MainWnd::SelectGoodsInfo() {
	//����SQL��ѯ���
	string StrSQL("select * from goods ");
	//�ҵ�Combo�ؼ��õ��ؼ�����
	CComboBoxUI* pCombo = (CComboBoxUI*)m_PaintManager.FindControl(_T("COMBO_SORT"));
	CDuiString StrStyle = pCombo->GetText();
	if (StrStyle == _T("ȫ��"))
		StrSQL += ";";
	else if (StrStyle == _T("�̾�")) {
		StrSQL += "where GoodsType= '�̾�';";
	}
	else if (StrStyle == _T("ˮ��")) {
		StrSQL += "where GoodsType= 'ˮ��';";
	}
	else if (StrStyle == _T("����Ʒ")) {
		StrSQL += "where GoodsType= '����Ʒ';";
	}
	else if (StrStyle == _T("��ʳ")) {
		StrSQL += "where GoodsType= '��ʳ';";
	}
	else if (StrStyle == _T("����")) {
		StrSQL += "where GoodsType= '����';";
	}
	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
	if (vRet.empty())
		return;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_GoodsInfo"));
	pList->RemoveAll();//����ϴβ�ѯ��List
	for (size_t i = 0; i < vRet.size(); ++i) {
		vector<string>& StrItem = vRet[i];
		CListTextElementUI* pDate = new CListTextElementUI;
		pDate->SetAttribute(_T("align"), _T("center"));
		pList->Add(pDate);

		pDate->SetText(0, AscToUnicode(StrItem[0]));
		pDate->SetText(1, AscToUnicode(StrItem[1]));
		pDate->SetText(2, AscToUnicode(StrItem[2]));
		pDate->SetText(3, AscToUnicode(StrItem[3]));
		pDate->SetText(4, AscToUnicode(StrItem[4]));
		pDate->SetText(5, AscToUnicode(StrItem[5]));
		pDate->SetText(6, AscToUnicode(StrItem[6]));
		pDate->SetText(7, AscToUnicode(StrItem[7]));
		pDate->SetText(8, AscToUnicode(StrItem[8]));
	}
}
//ɾ����Ʒ��Ϣ
void MainWnd::DeleteGoodsInfo() {
	//��ȡ��ǰѡ��
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_GoodsInfo"));

	//����
	int RowNum = pList->GetCurSel();
	CListTextElementUI*	pRow = (CListTextElementUI*)pList->GetItemAt(RowNum);

	//�����ݿ���ɾ����Ʒ��Ϣ

	//����SQLɾ������
	string StrSQL("delete from goods where GoodsId='");
	string StrId = UnicodeToAsc(pRow->GetText(0));//�õ���һ�еļ���ƷID��
	StrSQL += StrId;
	StrSQL += "';";
	p_mysql->Delete(StrSQL);

	//CDuiString StrName = pRow->GetText(0);//ͨ���к��õ�List�е�����
	//��List�ؼ���ɾ��Ա����Ϣ
	pList->RemoveAt(RowNum);
}
//������Ʒ��Ϣ
void MainWnd::InsertGoodsInfo() {
	//��������õ���Ʒ��Ϣ
	//CDuiString StrID = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSID")))->GetText();//������������������Ҫ�ṩ��ƷID
	CDuiString StrName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->GetText();
	CDuiString StrProDate = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRO_DATE")))->GetText();
	CDuiString StrSort = ((CComboBoxUI*)m_PaintManager.FindControl(_T("COMBO_SORT")))->GetText();
	CDuiString StrExpDate = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_EXP_DATE")))->GetText();
	CDuiString StrPrice = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRICE")))->GetText();
	CDuiString StrUnit = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_UNIT")))->GetText();
	CDuiString StrLeft = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->GetText();
	CDuiString StrPolice = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_POLICE")))->GetText();


	//pList->GetCount();

	//char buf[32] = { 0 };
	//����ת��Ϊ�ַ�
	//_itoa(pList->GetCount()+1, buf, 10);
	//����SQL����
	string StrSQL("insert into goods values(");
	//StrSQL += buf;
	//StrSQL += "','";
 	StrSQL += "NULL";
 	StrSQL += ",'";
	StrSQL += UnicodeToAsc(StrName);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrSort);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrProDate);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrExpDate);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrPrice);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrUnit);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrLeft);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrPolice);
	StrSQL += "');";
	//��ӦSQL����
	p_mysql->Insert(StrSQL);


	/*��List�ؼ�����Ӹ�Ա����Ϣ
	CListTextElementUI* pInfo = new CListTextElementUI;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListEmployeeInfo"));
	pList->Add(pInfo);
	pInfo->SetText(0, StrName);
	pInfo->SetText(1, StrGender);
	pInfo->SetText(2, StrBirthday);
	pInfo->SetText(3, StrPosition);
	pInfo->SetText(4, StrTel);
	pInfo->SetText(5, StrSalay);*/


	//���༭�����
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->SetText(_T("��Ʒ����"));
	((CComboBoxUI*)m_PaintManager.FindControl(_T("COMBO_SORT")))->SetText(_T("ȫ��"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRO_DATE")))->SetText(_T("��������"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_EXP_DATE")))->SetText(_T("��������"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRICE")))->SetText(_T("��Ʒ�۸�"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_UNIT")))->SetText(_T("��λ"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->SetText(_T("���"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_POLICE")))->SetText(_T("����ֵ"));
	
	
	//����ͨ�����²�ѯ��List�ؼ�����ʾԱ����Ϣ
	SelectGoodsInfo();
}
//������Ʒ��Ϣ
void MainWnd::UpDateGoodsInfo() {
	string StrSQL("update goods set ");
	//Ĭ�ϸ�����������Ա����Ϣ
	//Ĭ��ֻ�ܸ���Ȩ��/�绰/н��
	CDuiString StrName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->GetText();
	CDuiString StrPrice = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRICE")))->GetText();
	CDuiString StrLeft = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->GetText();
	CDuiString StrPolice = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_POLICE")))->GetText();
	//����SQL���
	if (!StrPrice.IsEmpty()&&StrPrice!=_T("��Ʒ�۸�")) {
		StrSQL += "Price='";
		StrSQL += UnicodeToAsc(StrPrice);
		StrSQL += "'";
	}
	if (!StrLeft.IsEmpty()&&StrLeft!=_T("���")) {
		if (!StrPrice.IsEmpty()) {
			StrSQL += ",Inventory='";
		}
		else {
			StrSQL += "Inventory='";
		}
		StrSQL += UnicodeToAsc(StrLeft);
		StrSQL += "'";
	}
	if (!StrPolice.IsEmpty()&&StrPolice!=_T("����ֵ")) {
		if (!StrPrice.IsEmpty() || !StrLeft.IsEmpty()) {
			StrSQL += ",AlarmValye='";
		}
		else {
			StrSQL += "AlarmValye='";
		}
		StrSQL += UnicodeToAsc(StrPolice);
		StrSQL += "'";
	}
	StrSQL += " where GoodsName='";
	StrSQL += UnicodeToAsc(StrName);
	StrSQL += "';";
	p_mysql->Update(StrSQL);
	SelectGoodsInfo();

	//���༭�����
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->SetText(_T("��Ʒ����"));
	((CComboBoxUI*)m_PaintManager.FindControl(_T("COMBO_SORT")))->SetText(_T("ȫ��"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRO_DATE")))->SetText(_T("��������"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_EXP_DATE")))->SetText(_T("��������"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRICE")))->SetText(_T("��Ʒ�۸�"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_UNIT")))->SetText(_T("��λ"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->SetText(_T("���"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_POLICE")))->SetText(_T("����ֵ"));
}


/***************************************************��Ϣ��Ӧ************************************************************/
void MainWnd::Notify(TNotifyUI& msg) {
	//��ȡ�ؼ�����
	CDuiString StrName = msg.pSender->GetName();
	//���ݿؼ����ͽ���ɸѡ
	if (msg.sType == _T("click")) {          //��ȡ�����Ϣ
		if (StrName == _T("BTN_CLOSE")) {	 //�������Ŀռ�����
			Close();
		}
		else if (StrName == _T("BTN_MIN")) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE,0);
		}
		else if (StrName == _T("BTN_SELECT")) {
			SelectEmployeeInfo();
		}
		else if (StrName == _T("BTN_DELETE")) {
			DeleteEmployeeInfo();
		}
		else if (StrName == _T("BTN_UPDATE")) {
			UpDateEmployeeInfo();
		}
		else if (StrName == _T("BTN_INSERT")) {
			InsertEmployeeInfo();
		}
		else if (StrName == _T("BTN_RECORD")) {
			MessageBox(NULL, _T("record"), _T("cashier"), IDOK);
		}
		else if (StrName == _T("BTN_GOODS_SELECT")) {
			SelectGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_DELETE")) {
			DeleteGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_INSERT")) {
			InsertGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_UPDATE")) {
			UpDateGoodsInfo();
		}
	}
	else if (msg.sType == _T("selectchanged")) {	//����
		//Ѱ�ҿؼ�
		CTabLayoutUI* pTab =(CTabLayoutUI*)m_PaintManager.FindControl(_T("tablayout"));
		if (StrName == _T("OPTION_EMPLOYEE")) {
			pTab->SelectItem(0);
		}
		else if (StrName == _T("OPTION_GOODS"))
			pTab->SelectItem(1);
		else
			pTab->SelectItem(2);
	}
}