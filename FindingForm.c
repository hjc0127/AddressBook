// FindingForm.c

#include "FindingForm.h"
#include "AddressBook.h"
#include "resource.h"
#include <stdlib.h>
#include <commctrl.h>
#include <stdio.h>
#pragma warning(disable : 4996)

//FindingFormProc: 
//	FindingForm_OnInitDialog
//	FindingForm_OnCommand
//		FindingForm_OnFinddButtonClicked
//	FindingForm_OnNotify
//		FindingForm_OnListViewItemDoubleClicked
//	FindingForm_OnClose

BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG: ret = FindingForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_COMMAND: ret = FindingForm_OnCommand(hWnd, wParam, lParam); break;
	case WM_NOTIFY: ret = FindingForm_OnNotify(hWnd, wParam, lParam); break;
	case WM_CLOSE: ret = FindingForm_OnClose(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}
	return ret;
}

// 1. 찾기 윈도우가 생성되었을때 
BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN column = { 0, };
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETEXTENDEDLISTVIEWSTYLE, (WPARAM)0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 1.1 리스트뷰의 헤더를 만든다.
	// LVM_INSERTCOLUMN,
	// wParam: Index of the new column.
	// lParam: Pointer to an LVCOLUMN structure that contains the attributes of the new column.
	column.mask = LVCF_WIDTH | LVCF_TEXT;

	column.cx = 50;
	column.pszText = "번호";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&column);

	column.cx = 70;
	column.pszText = "성명";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&column);

	column.cx = 120;
	column.pszText = "주소";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)2, (LPARAM)&column);

	column.cx = 120;
	column.pszText = "전화번호";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)3, (LPARAM)&column);

	column.cx = 120;
	column.pszText = "이메일주소";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)4, (LPARAM)&column);

	return FALSE;
}

BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_FIND: ret = FindingForm_OnFindButtonClicked(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}
	return ret;
}

BOOL FindingForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (wParam) {
	case IDC_LIST_PERSONALS: ret = FindingForm_OnListViewItemDoubleClicked(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}
	return ret;
}

// 4. 닫기 버튼을 클릭했을때
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long(*indexes);
	indexes = (Long*)GetWindowLong(hWnd, GWL_USERDATA);
	if (indexes != NULL) {
		free(indexes);
	}
	// 4.1 찾기 윈도우를 닫는다.
	EndDialog(hWnd, 0);
	return FALSE;
}

//2. 찾기 버튼을 클릭했을때
BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook *addressBook;
	LVITEM item = { 0, };
	HWND hUpdatingForm;
	Long i = 0;
	Long(*indexes);
	Long count;
	TCHAR name[NAME];
	TCHAR number[NUMBER];

	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 리스트뷰의 모든항목을 지운다.
		// LVM_DELETEALLITEMS
		// wParam: Must be zero.
		// lParam: Must be zero.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, (WPARAM)0, (LPARAM)0);

		//2.2 성명을 읽는다.
		// WM_GETTEXT: 
		// wParam: The maximum number of characters to be copied
		// lParam: A pointer to the buffer that is to receive the text.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)NAME, (LPARAM)name);

		//2.3 주소록 윈도우를 찾는다.
		hUpdatingForm = FindWindow("#32770", "주소록");

		//2.4 주소록 윈도우 주소록에서 개인을 찾는다.
		addressBook = (AddressBook*)GetWindowLong(hUpdatingForm, GWL_USERDATA);
		indexes = (Long*)GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		Find(addressBook, name, &indexes, &count);
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);

		//2.5 찾은 개인들을 리스트뷰에 출력한다.
		// LVM_INSERTITEM
		// wParam: Must be zero.
		// lParam: Pointer to an LVITEM structure
		// LVM_SETITEMTEXT
		// wParam: Zero-based index of the list-view item
		// lParam: Pointer to an LVITEM structure. 
		item.mask = LVIF_TEXT;

		while (i < count) {
			sprintf(number, "%d", i + 1);
			item.pszText = number;
			item.iItem = i;
			item.iSubItem = 0;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, (WPARAM)0, (LPARAM)&item);

			item.pszText = addressBook->personals[indexes[i]].name;
			item.iSubItem = 1;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

			item.pszText = addressBook->personals[indexes[i]].address;
			item.iSubItem = 2;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

			item.pszText = addressBook->personals[indexes[i]].telephoneNumber;
			item.iSubItem = 3;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

			item.pszText = addressBook->personals[indexes[i]].emailAddress;
			item.iSubItem = 4;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
			i++;
		}
	}
	return FALSE;
}

// 3. 찾기 위도우 리스트뷰에서 항목을 더블클릭했을때
BOOL FindingForm_OnListViewItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hUpdatingForm;
	Long index;
	Long(*indexes);
	LVITEM item = { 0, };
	TCHAR name[NAME];
	TCHAR address[ADDRESS];
	TCHAR telephoneNumber[TELEPHONENUMBER];
	TCHAR emailAddress[EMAILADDRESS];

	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//3.1 선택한 항목의 위치를 읽는다.
		index = (Long)SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0);

		//3.2 해당 위치의 항목을 읽는다.
		// LVM_GETITEMTEXT
		item.mask = LVIF_TEXT; // LVIF_TEXT: pszText member
		item.iItem = index;

		item.iSubItem = 1;
		item.cchTextMax = NAME;
		item.pszText = name;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 2;
		item.cchTextMax = ADDRESS;
		item.pszText = address;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 3;
		item.cchTextMax = TELEPHONENUMBER;
		item.pszText = telephoneNumber;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 4;
		item.cchTextMax = EMAILADDRESS;
		item.pszText = emailAddress;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		//3.3 주소록 윈도우를 찾는다.
		hUpdatingForm = FindWindow("#32770", "주소록");

		//3.4 읽은 항목을 주소록 윈도우의 리스트뷰에 표시한다.
		// LVM_SETSELECTIONMARK: Sets the selection mark in a list-view control.
		// wParam: Must be zero.
		// lParam: Zero-based index of the new selection mark.
		// GetWindowLong(hWnd,GWL_USERDATA);
		indexes = (Long*)GetWindowLong(hWnd, GWL_USERDATA);
		SendMessage(GetDlgItem(hUpdatingForm, IDC_LIST_PERSONALS), LVM_SETSELECTIONMARK, (WPARAM)0, (LPARAM)indexes[index]);

		//3.5 읽은 항목을 주소록 윈도우 개인에 개인을 출력한다.
		// GetDlgItem: Retrieves a handle to a control in the specified dialog box.
		// WM_SETTEXT: Sets the text of a window.
		// wParam: This parameter is not used.
		// lParam: A pointer to a null-terminated string that is the window text.	
		SendMessage(GetDlgItem(hUpdatingForm, IDC_EDIT_NAME), WM_SETTEXT, (WPARAM)NAME, (LPARAM)name);
		SendMessage(GetDlgItem(hUpdatingForm, IDC_EDIT_ADDRESS), WM_SETTEXT, (WPARAM)ADDRESS, (LPARAM)address);
		SendMessage(GetDlgItem(hUpdatingForm, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)TELEPHONENUMBER, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hUpdatingForm, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, (WPARAM)EMAILADDRESS, (LPARAM)emailAddress);

		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		//3.6 찾기 윈도우를 닫는다.
		EndDialog(hWnd, 0);
	}
	return FALSE;
}