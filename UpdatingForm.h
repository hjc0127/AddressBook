// UpdatingForm.h

#ifndef _UPDATINGFORM_H
#define _UPDATINGFORM_H
#include <Windows.h>
#define NUMBER 64
#define NAME 11
#define ADDRESS 64
#define TELEPHONENUMBER 12
#define EMAILADDRESS 64

BOOL CALLBACK UpdatingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL UpdatingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL UpdatingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL UpdatingForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL UpdatingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL UpdatingForm_OnRecordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL UpdatingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL UpdatingForm_OnCorrectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL UpdatingForm_OnEraseButtonClicked(HWND hWnd, WPARAM wParam, LPARAM laram);
BOOL UpdatingForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lparam);
BOOL UpdatingForm_OnListViewItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lparam);

#endif	// _UPDATINGFORM_H
