#include "UpdatingForm.h"
#include "FindingForm.h"
#include "AddressBook.h"
#include "resource.h"
#include <stdlib.h>
#include <commctrl.h>
#include <stdio.h>
#pragma warning(disable : 4996)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	int response;
	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_UPDATINGFORM), NULL, UpdatingFormProc);
	return response;
}

BOOL CALLBACK UpdatingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG: ret = UpdatingForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_COMMAND: ret = UpdatingForm_OnCommand(hWnd, wParam, lParam); break;
	case WM_NOTIFY: ret = UpdatingForm_OnNotify(hWnd, wParam, lParam); break;
	case WM_CLOSE: ret = UpdatingForm_OnClose(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}
	return ret;
}

// 1. 윈도우가 생성되었을때ㅤ 
BOOL UpdatingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// 1.1 주소록을 준비한다.
	AddressBook *addressBook;
	Long i = 0;
	LVCOLUMN column = { 0, };
	LVITEM item = { 0, };
	TCHAR number[NUMBER];
	addressBook = (AddressBook(*))malloc(sizeof(AddressBook));
	Create(addressBook, 60000);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)addressBook);
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETEXTENDEDLISTVIEWSTYLE, (WPARAM)0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 1.2 리스트뷰의 헤더를 만든다.
	// LVM_INSERTCOLUMN
	// wParam = index of the new column
	// lParam = pointer to an LVCOLUMN structure
	// mask: Variable specifying which members contain valid information
	// LVCF_WIDTH: cx(Width of the column, in pixels.)
	// LVCF_TEXT: pszText(the column header text.)
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

	// 1.3 주소록을 적재한다.
	Load(addressBook);

	// 1.4 주소록의 개수만큼 리스트뷰에 출력한다.
	item.mask = LVIF_TEXT;
	while (i < addressBook->length) {
		item.iItem = i;
		sprintf(number, "%d", i + 1);
		item.pszText = number;
		item.iSubItem = 0;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, (WPARAM)0, (LPARAM)&item);
		item.pszText = addressBook->personals[i].name;
		item.iSubItem = 1;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
		item.pszText = addressBook->personals[i].address;
		item.iSubItem = 2;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
		item.pszText = addressBook->personals[i].telephoneNumber;
		item.iSubItem = 3;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
		item.pszText = addressBook->personals[i].emailAddress;
		item.iSubItem = 4;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
		i++;
	}
	return FALSE;
}

// 기재하기, 찾기, 고치기, 지우기, 정리하기 관련
BOOL UpdatingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	// LOWORD : 지정한 32비트 데이터에서 하위 16비트 데이터 값을 얻는데 사용하는 함수이다.
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_RECORD: ret = UpdatingForm_OnRecordButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_FIND: ret = UpdatingForm_OnFindButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_CORRECT: ret = UpdatingForm_OnCorrectButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_ERASE: ret = UpdatingForm_OnEraseButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_ARRANGE: ret = UpdatingForm_OnArrangeButtonClicked(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}
	return ret;
}

// 리스트뷰 더블클릭
BOOL UpdatingForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (wParam) {
	case IDC_LIST_PERSONALS: ret = UpdatingForm_OnListViewItemDoubleClicked(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}
	return ret;
}

// 8. 윈도우 닫기 버튼을 클릭했을때 
BOOL UpdatingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook *addressBook;
	addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);

	if (addressBook != NULL) {
		// 8.1 주소록을 저장한다.
		Save(addressBook);
		// 8.2 주소록을 지운다.
		Destroy(addressBook);
		free(addressBook);
	}
	// 8.3 주소록을 닫는다. 
	EndDialog(hWnd, 0);
	return FALSE;
}

// 2. 기재하기 버튼을 클릭했을때
BOOL UpdatingForm_OnRecordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook *addressBook;
	LVITEM item = { 0, };
	Long index;
	TCHAR number[NUMBER];
	TCHAR name[NAME];
	TCHAR address[ADDRESS];
	TCHAR telephoneNumber[TELEPHONENUMBER];
	TCHAR emailAddress[EMAILADDRESS];

	// notification code
	if (HIWORD(wParam) == BN_CLICKED) {
		// 2.1 개인을 읽는다.
		// GetDlgItem: Retrieves a handle to a control in the specified dialog box.
		// WM_GETTEXT: This message is sent by an application to copy the text that corresponds to a window into a buffer provided by the caller.
		// wParam: maximum number of characters to be copied
		// lParam: pointer to the buffer that is to receive the text.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)NAME, (LPARAM)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)ADDRESS, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)TELEPHONENUMBER, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)EMAILADDRESS, (LPARAM)emailAddress);

		// 2.2 주소록에 읽은 개인을 적는다.
		// GetWindowLong: Retrieves information about the specified window. 
		// GetWindowLong(_In_ HWND hWnd,_In_ int  nIndex)
		// GWL_USERDATA: Retrieves the user data associated with the window.
		addressBook = (AddressBook *)GetWindowLong(hWnd, GWL_USERDATA);
		index = Record(addressBook, name, address, telephoneNumber, emailAddress);

		// 2.3 리스트뷰에 항목을 추가한다. 
		// LVM_INSERTITEM
		// wParam: Must be zero.
		// lParam: Pointer to an LVITEM structure
		// LVM_SETITEMTEXT
		// wParam: Zero-based index of the list-view item
		// lParam: Pointer to an LVITEM structure. 
		// LVITEM structure: The pszText member is valid or must be set.
		// iItem: Zero-based index of the item to which this structure refers.
		// iSubItem: One-based index of the subitem to which this structure refers, or zero if this structure refers to an item rather than a subitem.
		// pszText: string containing the item text.

		item.mask = LVIF_TEXT; // LVIF_TEXT: pszText member
		item.iItem = index;
		sprintf(number, "%d", index + 1);
		item.pszText = number;
		item.iSubItem = 0;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, (WPARAM)0, (LPARAM)&item);

		item.pszText = addressBook->personals[index].name;
		item.iSubItem = 1;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.pszText = addressBook->personals[index].address;
		item.iSubItem = 2;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.pszText = addressBook->personals[index].telephoneNumber;
		item.iSubItem = 3;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.pszText = addressBook->personals[index].emailAddress;
		item.iSubItem = 4;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
	}
	return FALSE;
}

// 3. 찾기 버튼을 클릭했을때
BOOL UpdatingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// 3.1 찾기 윈도우를 출력한다. 
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), hWnd, FindingFormProc);
	}
	return FALSE;
}

// 4. 고치기 버튼을 클릭했을때
BOOL UpdatingForm_OnCorrectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook *addressBook;
	LVITEM item = { 0, };
	Long index;
	TCHAR address[ADDRESS];
	TCHAR telephoneNumber[TELEPHONENUMBER];
	TCHAR emailAddress[EMAILADDRESS];

	if (HIWORD(wParam) == BN_CLICKED) {
		// 4.1 리스트뷰에서 선택된 위치를 읽는다.
		// LVM_GETSELECTIONMARK: This message sets the selection mark in a list-view control.
		// wParam: Must be zero.
		// lParam: Must be zero.
		// GetWindowLong: Retrieves information about the specified window. 
		index = (Long)SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0);

		// 4.2 개인을 읽는다. (주소, 전화번호, 이메일주소)
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)ADDRESS, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)TELEPHONENUMBER, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)EMAILADDRESS, (LPARAM)emailAddress);

		// 4.3 주소록에 해당 개인을 고친다.
		addressBook = (AddressBook *)GetWindowLong(hWnd, GWL_USERDATA);
		index = Correct(addressBook, index, address, telephoneNumber, emailAddress);

		// 4.4 리스트뷰에 해당 위치의 항목을 고친다.
		// LVM_SETITEMTEXT
		// wParam: Zero-based index of the list-view item
		// lParam: Pointer to an LVITEM structure. 
		// LVITEM structure: The pszText member is valid or must be set.
		// iItem: Zero-based index of the item to which this structure refers.
		// iSubItem: One-based index of the subitem to which this structure refers, or zero if this structure refers to an item rather than a subitem.
		// pszText: string containing the item text.
		item.mask = LVIF_TEXT; // LVIF_TEXT: pszText member
		item.iItem = index;

		item.pszText = addressBook->personals[index].address;
		item.iSubItem = 2;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.pszText = addressBook->personals[index].telephoneNumber;
		item.iSubItem = 3;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.pszText = addressBook->personals[index].emailAddress;
		item.iSubItem = 4;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
	}
	return FALSE;
}

// 5. 지우기 버튼을 클릭했을때
BOOL UpdatingForm_OnEraseButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook *addressBook;
	LVITEM item = { 0, };
	TCHAR number[NUMBER];
	Long index;
	Long i;

	if (HIWORD(wParam) == BN_CLICKED) {
		// 5.1 리스트뷰에서 선택된 위치를 읽는다.
		// LVM_GETSELECTIONMARK: This message sets the selection mark in a list-view control.
		// wParam: Must be zero.
		// lParam: Must be zero.
		// GetWindowLong: Retrieves information about the specified window. 
		index = (Long)SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0);

		// 5.2 주소록에서 해당 개인을 지운다.
		addressBook = (AddressBook *)GetWindowLong(hWnd, GWL_USERDATA);
		Erase(addressBook, index);

		// 5.3 리스트뷰에서 해당 항목을 지운다.
		// LVM_DELETEITEM
		// wParam = The index of the list-view item to delete.
		// lParam = Must be zero.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEITEM, (WPARAM)index, (LPARAM)0);

		// 5.4 지운 다음 항목부터 번호를 수정한다.
		// LVM_SETITEMTEXT
		// wParam: Zero-based index of the list-view item
		// lParam: Pointer to an LVITEM structure. 
		// LVITEM structure: The pszText member is valid or must be set.
		// iItem: Zero-based index of the item to which this structure refers.
		// iSubItem: One-based index of the subitem to which this structure refers, or zero if this structure refers to an item rather than a subitem.
		item.mask = LVIF_TEXT; // LVIF_TEXT: pszText member
		item.iItem = index;
		item.iSubItem = 0;
		for (i = index; i < addressBook->length; i++) {
			sprintf(number, "%d", i + 1);
			item.pszText = number;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
		}
	}
	return FALSE;
}
// 6. 정리하기 버튼을 클릭했을때
BOOL UpdatingForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook *addressBook;
	LVITEM item = { 0, };
	Long index;
	TCHAR number[NUMBER];

	if (HIWORD(wParam) == BN_CLICKED) {
		// 6.1 주소록에서 개인들을 정렬한다.
		addressBook = (AddressBook *)GetWindowLong(hWnd, GWL_USERDATA);
		Arrange(addressBook);

		// 6.2 리스트뷰에 모든 항목을 지운다.
		// LVM_DELETEALLITEMS
		// wParam: Must be zero.
		// lParam: Must be zero.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, (WPARAM)0, (LPARAM)0);

		// 6.3 리스트뷰에 주소록을 출력한다.
		// LVM_INSERTITEM
		// wParam: Must be zero.
		// lParam: Pointer to an LVITEM structure
		// LVM_SETITEMTEXT
		// wParam: Zero-based index of the list-view item
		// lParam: Pointer to an LVITEM structure. 
		// LVITEM structure: The pszText member is valid or must be set.
		// iItem: Zero-based index of the item to which this structure refers.
		// iSubItem: One-based index of the subitem to which this structure refers, or zero if this structure refers to an item rather than a subitem.
		// pszText: string containing the item text.
		item.mask = LVIF_TEXT; // LVIF_TEXT: pszText member
		for (index = 0; index < addressBook->length; index++) {
			sprintf(number, "%d", index + 1);
			item.pszText = number;
			item.iItem = index;
			item.iSubItem = 0;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, (WPARAM)0, (LPARAM)&item);

			item.pszText = addressBook->personals[index].name;
			item.iSubItem = 1;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

			item.pszText = addressBook->personals[index].address;
			item.iSubItem = 2;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

			item.pszText = addressBook->personals[index].telephoneNumber;
			item.iSubItem = 3;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

			item.pszText = addressBook->personals[index].emailAddress;
			item.iSubItem = 4;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		}
	}
	return FALSE;
}

// 7. 리스트뷰에서 항목을 더블클릭했을때
BOOL UpdatingForm_OnListViewItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LVITEM item = { 0, };
	Long index;
	TCHAR name[NAME];
	TCHAR address[ADDRESS];
	TCHAR telephoneNumber[TELEPHONENUMBER];
	TCHAR emailAddress[EMAILADDRESS];

	// NM_DBLCLK: Sent by a list-view control when the user double-clicks an item with the left mouse button. 
	//            This notification code is sent in the form of a WM_NOTIFY message.
	// NM_DBLCLK lpnmh = (LPNMHDR) lParam
	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		// 7.1 클릭한 항목의 위치를 읽는다.
		// LVM_GETSELECTIONMARK: This message sets the selection mark in a list-view control.
		// wParam: Must be zero.
		// lParam: Must be zero.
		index = (Long)SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0);

		// 7.2 해당 위치의 항목을 읽는다.
		// LVM_GETITEMTEXT
		// wParam: Index of the list - view item.
		// lParam: Pointer to an LVITEM structure.
		// mask: Variable specifying which members contain valid information
		// iItem: Zero-based index of the item to which this structure refers.
		// iSubItem: One-based index of the subitem to which this structure refers, or zero if this structure refers to an item rather than a subitem.
		// pszText: string containing the item text.
		// cchTextMax: Number of TCHARs in the buffer pointed to by pszText
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

		// 7.3 읽은 항목을 개인에 출력한다.
		// GetDlgItem: Retrieves a handle to a control in the specified dialog box.
		// WM_SETTEXT: Sets the text of a window.
		// wParam: This parameter is not used.
		// lParam: A pointer to a null-terminated string that is the window text.	
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, (WPARAM)NAME, (LPARAM)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, (WPARAM)ADDRESS, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)TELEPHONENUMBER, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, (WPARAM)EMAILADDRESS, (LPARAM)emailAddress);
	}
	return FALSE;
}