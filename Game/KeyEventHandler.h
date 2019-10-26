#pragma once
#include <dinput.h>

int InitDirectInput();
int InitKeyboard(HWND);
void PollKeyboard();

int IsKeyDown(int key);	// ki?m tra tr?ng th�i ph�m c� ?ang ? d??i hay kh�ng

int IsKeyRelease(int key);	// ki?m tra xem ph�m c� ?ang ? tr?ng th�i tr�n hay kh�ng
int IsKeyPress(int key);	// ki?m tra xem ph�m c� v?a chuy?n tr?ng th�i t? up xu?ng down kh�ng

void KillKeyboard();
