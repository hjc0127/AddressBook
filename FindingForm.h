// FindingForm.h

#ifndef _FINDINGFORM_H
#define _FINDINGFORM_H
#include <Windows.h>
#define NUMBER 64
#define NAME 11
#define ADDRESS 64
#define TELEPHONENUMBER 12
#define EMAILADDRESS 64

BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnListViewItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lparam);

#endif	// _FINDINGFORM_H
