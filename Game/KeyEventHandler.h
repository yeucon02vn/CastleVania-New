#pragma once
#include <dinput.h>

int InitDirectInput();
int InitKeyboard(HWND);
void PollKeyboard();

int IsKeyDown(int key);	// ki?m tra tr?ng thái phím có ?ang ? d??i hay không

int IsKeyRelease(int key);	// ki?m tra xem phím có ?ang ? tr?ng thái trên hay không
int IsKeyPress(int key);	// ki?m tra xem phím có v?a chuy?n tr?ng thái t? up xu?ng down không

void KillKeyboard();
