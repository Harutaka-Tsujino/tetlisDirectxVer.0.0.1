/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WinMain
//
// DATE 2018.06.10
// VER.0.0.1
//
// Author Harutaka-Tsujino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tetlisDirectxVer.0.0.1.h"
#include "tetlisDirectxVer.0.0.1Initialize.h"
#include "tetlisDirectxVer.0.0.1Control.h"
#include "tetlisDirectxVer.0.0.1Render.h"
#include "tetlisDirectxVer.0.0.1WinMain.h"

Tetmino g_tetmino[7] =
{
	{ 0,'I',1,4,1,5,1,6,1,7 },
	{ 1,'J',1,5,2,5,2,6,2,7 },
	{ 2,'L',1,7,2,7,2,6,2,5 },
	{ 3,'S',1,7,1,6,2,6,2,5 },
	{ 4,'Z',1,5,1,6,2,6,2,7 },
	{ 5,'T',2,5,1,6,2,6,2,7 },
	{ 6,'O',1,5,2,5,1,6,2,6 }
};

MovMinoNumoOfArBuf g_movMinoNumOfArBuf;
MovMinoNumoOfArBuf g_targetMinoNumOfArBuf;

INT g_tetminoNum;
INT g_hold = -1, g_next, g_nextNext;
INT g_tetlisBoard[TETLIS_HEIGHT][TETLIS_WIDTH];
INT g_tetlisBoardBuf[TETLIS_HEIGHT][TETLIS_WIDTH];
INT g_holdBoard[4][4];
INT g_nextBoard[4][4];
INT g_nextNextBoard[4][4];

BOOL g_showGameoverStr = false;

CHAR g_scoreArray[8];

////////////////////////////////////
//�E�B���h�E�̐����A�G���g���|�C���g
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	srand((unsigned int)time(NULL));
	//�E�B���h�E�̐���////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**/HWND hWnd = NULL;
	/**/MSG	msg;
	/**/
	/**/static CHAR szAppName[] = "TETLIS";
	/**/WNDCLASSEX wndclass;
	/**/
	/**/wndclass.cbSize = sizeof(wndclass);
	/**/wndclass.style = CS_HREDRAW | CS_VREDRAW;
	/**/wndclass.lpfnWndProc = WndProc;
	/**/wndclass.cbClsExtra = 0;
	/**/wndclass.cbWndExtra = 0;
	/**/wndclass.hInstance = hInst;
	/**/wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	/**/wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	/**/wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	/**/wndclass.lpszMenuName = NULL;
	/**/wndclass.lpszClassName = szAppName;
	/**/wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	/**/
	/**/RegisterClassEx(&wndclass);
	/**/
	/**/hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, DISPLAY_WIDTH, DISPLAY_HEIGHT, NULL, NULL, hInst, NULL);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//�E�B���h�E��������悤�ɂ���
	ShowWindow(hWnd, SW_SHOW);

	//�E�B���h�E���X�V����悤�ɂ���
	UpdateWindow(hWnd);

	//�_�C���N�g3D�̏�����
	if (FAILED(InitDirect3D(hWnd)))
	{
		return 0;
	}

	//�_�C���N�g�C���v�b�g�̏�����
	if (FAILED(InitDinput(hWnd)))
	{
		return 0;
	}

	//�_�C���N�g�t�H���g�̏�����
	if (FAILED(InitDfont(hWnd)))
	{
		return 0;
	}

	//���b�Z�[�W�̒��g�̏���
	ZeroMemory(&msg, sizeof(msg));
	
	//���Ԃ̗L�������̐ݒ�
	timeBeginPeriod(1);

	//�V�X�e�����Ԃ̎擾
	DWORD sync_prev = timeGetTime();
	DWORD sync_current;

	//���b�Z�[�W���[�v
	while (msg.message != WM_QUIT)
	{

		//���b�Z�[�W��`��
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			//���b�Z�[�W�̖|��
			TranslateMessage(&msg);

			//���b�Z�[�W�̑��M
			DispatchMessage(&msg);
		}
		else
		{
			sync_current = timeGetTime();

			//1�b�Ԃ�60�񂱂̒��ɓ���
			if (sync_current - sync_prev >= 1000 / 60) 
			{
				Control();
				Render();
				sync_prev = sync_current;
			}
		}
	}

	timeEndPeriod(1);

	//�������J��
	FreeDx();

	return(INT)msg.wParam;
}

//////////////////////////////
//�E�B�����b�Z�[�W�̏������s��
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
