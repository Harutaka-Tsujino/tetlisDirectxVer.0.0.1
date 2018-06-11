#ifndef TETLIS_DEIRECT_VER_0_0_1_RENDER_H
#define TETLIS_DEIRECT_VER_0_0_1_RENDER_H

#include <windows.h>
#include <d3dx9.h>

//描画に関する関数
VOID Render(VOID);

//総合的な視点の設定をする関数
VOID SetViewPointOverall(VOID);

//総合的な視野角の設定をする関数
VOID SetFocusOfViewOverall(VOID);

//画面を表示させる関数
VOID DisplayScreen(CustomVertex * cusV4Background, CustomVertex *cusV4Tetmino, CustomVertex *cusV4Frame, RECT *rectGameoverStr, RECT *rectScoreStr);

#endif
