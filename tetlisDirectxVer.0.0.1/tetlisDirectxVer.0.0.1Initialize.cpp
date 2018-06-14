/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize
//
// DATE 2018.06.13
// VER.0.0.5
//
// Author Harutaka-Tsujino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tetlisDirectxVer.0.0.1Common.h"
#include "tetlisDirectxVer.0.0.1Initialize.h"

LPDIRECT3D9 g_pDirect3D;
LPDIRECT3DDEVICE9 g_pD3dDevice;
LPDIRECTINPUT8 g_pDinput = NULL;
LPDIRECT3DTEXTURE9 g_pTexture[g_texMax];
LPDIRECTINPUTDEVICE8 g_pDKeyDevice = NULL;
LPD3DXFONT g_pFont[g_fontMax];
D3DPRESENT_PARAMETERS g_D3dPresentParameters;

//////////////////////////////////
//ダイレクト3Dの初期化に関する関数
HRESULT InitDirect3D(HWND hWnd)
{
	//ダイレクト3Dの作成
	if (NULL == (g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	//g_D3dPresentParametersの中身の消去
	ZeroMemory(&g_D3dPresentParameters, sizeof(g_D3dPresentParameters));

	//////////////////////////////////////
	//バッファー形式、枚数、切り替えの設定
	SetBuckBufferOverall();

	//ウィンドウモードで動いていることの提示
	g_D3dPresentParameters.Windowed = TRUE;

	//ダイレクト3Dデバイスの作成
	if (FAILED(g_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&g_D3dPresentParameters, &g_pD3dDevice)))
	{
		//描画モードの設定
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(g_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&g_D3dPresentParameters, &g_pD3dDevice)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}

	////////////////
	//描画の詳細設定
	SetRenderStateOverall();

	//////////////////////////
	//テクスチャ階層の詳細設定
	SetTextureStageStateOverall();

	//頂点情報の登録
	g_pD3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	//////////////////
	//テクスチャの生成
	CreateTextureFromFile();
	
	return S_OK;
}

VOID SetBuckBufferOverall(VOID)
{
	//バッファーの形式設定
	g_D3dPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	//バッファーの枚数設定
	g_D3dPresentParameters.BackBufferCount = 1;
	//バッファーの切り替え方法の設定
	g_D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	return;
}

VOID SetRenderStateOverall(VOID)
{
	g_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return;
}

VOID SetTextureStageStateOverall(VOID)
{
	g_pD3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return;
}

VOID CreateTextureFromFile(VOID)
{
	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"background.png",
		&g_pTexture[g_backgroundTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"frame.png",
		&g_pTexture[g_frameTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"tetlimino.png",
		&g_pTexture[g_tetminoITex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"blockBrightStone.jpg",
		&g_pTexture[g_tetminoJTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"blockDarkstone.jpg",
		&g_pTexture[g_tetminoLTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"blockDirt.jpg",
		&g_pTexture[g_tetminoSTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"blockPsychedelia.png",
		&g_pTexture[g_tetminoZTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"blockStoneHT.png",
		&g_pTexture[g_tetminoTTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"blockMoss.png",
		&g_pTexture[g_tetminoOTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"blockTarget.png",
		&g_pTexture[g_tetminoTarTex]);

	D3DXCreateTextureFromFile(
		g_pD3dDevice,
		"block_integrated_illust_HT.png",
		&g_pTexture[g_integratedBlockTex]);

	return;
}

//////////////////////////////////////////
//ダイレクトインプットの初期化に関する関数
HRESULT InitDinput(HWND hWnd)
{
	HRESULT hr;

	//ダイレクトインプットの生成
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDinput, NULL)))
	{
		return hr;
	}

	//ダイレクトインプットデバイスの生成
	if (FAILED(hr = g_pDinput->CreateDevice(GUID_SysKeyboard,
		&g_pDKeyDevice, NULL)))
	{
		return hr;
	}

	//入力形式をキーボードに設定
	if (FAILED(hr = g_pDKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	//ダイレクトインプットデバイスの権限取得の優先度設定
	if (FAILED(hr = g_pDKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	return S_OK;
}

HRESULT InitDfont(HWND hWnd)
{
	////////////////
	//フォントの生成
	CreateFont();

	return S_OK;
}

VOID CreateFont(VOID)
{
	D3DXCreateFont(g_pD3dDevice, 160, 80, 0, 0, 0, 0, 0, 0, 0, "Times New Roman", &g_pFont[g_gameoverFont]);
	D3DXCreateFont(g_pD3dDevice, 40, 20, 0, 0, 0, 0, 0, 0, 0, "Times New Roman", &g_pFont[g_scoreFont]);

	return;
}
