/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WinMain
//
// DATE 2018.06.13
// VER.0.0.5
//
// Author Harutaka-Tsujino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tetlisDirectxVer.0.0.1Common.h"
#include "tetlisDirectxVer.0.0.1Initialize.h"
#include "tetlisDirectxVer.0.0.1Control.h"
#include "tetlisDirectxVer.0.0.1Render.h"
#include "tetlisDirectxVer.0.0.1WinMain.h"
#include <time.h>

Tetmino g_tetmino[7] =
{
	{ 0,'I',2,4,2,5,2,6,2,7 },
	{ 1,'J',2,5,3,5,3,6,3,7 },
	{ 2,'L',2,7,3,7,3,6,3,5 },
	{ 3,'S',2,7,2,6,3,6,3,5 },
	{ 4,'Z',2,5,2,6,3,6,3,7 },
	{ 5,'T',3,5,2,6,3,6,3,7 },
	{ 6,'O',2,5,3,5,2,6,3,6 }
};

MovMinoNumoOfArBuf g_movMinoNumOfArBuf;
MovMinoNumoOfArBuf g_targetMinoNumOfArBuf;

INT g_tetminoNum;
INT g_deletedLineCount = 0;
INT g_hold = -1, g_next, g_nextNext;
INT g_tetlisBoardSource[][TETLIS_WIDTH] =
{
	{ 9,110,110,110,-1,-1,110,121,110,110,-1,9 },
	{ 9,110,110,132,-1,110,110,110,110,110,110,9 },
	{ 9,110,-1,110,110,110,110,110,110,110,110,9 },

	{ 9,110,110,110,110,132,132,132,-1,110,110,9 },
	{ 9,110,110,-1,110,132,160,132,110,110,110,9 },
	{ 9,110,110,110,110,132,132,132,110,110,-1,9 },

	{ 9,110,110,110,110,110,110,110,110,110,-1,9 },
	{ 9,110,110,110,110,110,-1,-1,110,110,110,9 },
	{ 9,110,-1,110,110,110,110,-1,110,110,110,9 },

	{ 9,110,121,121,121,-1,110,-1,110,110,110,9 },
	{ 9,110,110,110,110,-1,110,110,132,110,-1,9 },
	{ 9,-1,-1,-1,132,110,110,110,110,110,110,9 }
};

INT g_tetlisBoard[TETLIS_HEIGHT][TETLIS_WIDTH];
INT g_tetlisBoardBuf[TETLIS_HEIGHT][TETLIS_WIDTH];
INT g_holdBoard[4][4];
INT g_nextBoard[4][4];
INT g_nextNextBoard[4][4];

BOOL g_showGameoverStr = false;
BOOL g_hardDropedTetmino = false;

CHAR g_scoreArray[8];
CHAR g_undergroundArray[5];

////////////////////////////////////
//ウィンドウの生成、エントリポイント
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	srand((unsigned int)time(NULL));
	//ウィンドウの生成////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HWND hWnd = NULL;
	MSG	msg;
	
	static CHAR szAppName[] = "TETLIS";
	WNDCLASSEX wndclass;
	
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	RegisterClassEx(&wndclass);
	
	hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, DISPLAY_WIDTH, DISPLAY_HEIGHT, NULL, NULL, hInst, NULL);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ウィンドウを見えるようにする
	ShowWindow(hWnd, SW_SHOW);

	//ウィンドウを更新するようにする
	UpdateWindow(hWnd);

	//ダイレクト3Dの初期化
	if (FAILED(InitDirect3D(hWnd)))
	{
		return 0;
	}

	//ダイレクトインプットの初期化
	if (FAILED(InitDinput(hWnd)))
	{
		return 0;
	}

	//ダイレクトフォントの初期化
	if (FAILED(InitDfont(hWnd)))
	{
		return 0;
	}

	//メッセージの中身の消去
	ZeroMemory(&msg, sizeof(msg));
	
	//時間の有効数字の設定
	timeBeginPeriod(1);

	//システム時間の取得
	DWORD sync_prev = timeGetTime();
	DWORD sync_current;

	//メッセージループ
	while (msg.message != WM_QUIT)
	{

		//メッセージを覗く
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			//メッセージの翻訳
			TranslateMessage(&msg);

			//メッセージの送信
			DispatchMessage(&msg);
		}
		else
		{
			sync_current = timeGetTime();

			//1秒間に60回この中に入る
			if (sync_current - sync_prev >= 1000 / 60) 
			{
				Control();
				Render();
				sync_prev = sync_current;
			}
		}
	}

	timeEndPeriod(1);

	//メモリ開放
	FreeDx();

	return(INT)msg.wParam;
}

//////////////////////////////
//ウィンメッセージの処理を行う
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)//////////////////////////////////////////////////////////////////////////////////
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}

		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

VOID FreeDx(VOID)//////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (g_pDKeyDevice)
	{
		g_pDKeyDevice->Unacquire();
	}

	SAFE_RELEASE(g_pDKeyDevice);
	SAFE_RELEASE(g_pDinput);

	for (int coordinateX = 0; coordinateX < g_texMax; coordinateX++)
	{
		SAFE_RELEASE(g_pTexture[coordinateX]);
	}

	for (int coordinateX = 0; coordinateX < g_fontMax; coordinateX++)
	{
		SAFE_RELEASE(g_pFont[coordinateX]);
	}

	SAFE_RELEASE(g_pD3dDevice);
	SAFE_RELEASE(g_pDirect3D);
}
