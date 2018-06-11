/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render
//
// DATE 2018.06.09
// VER.0.0.1
//
// Author Harutaka-Tsujino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tetlisDirectxVer.0.0.1.h"
#include "tetlisDirectxVer.0.0.1Initialize.h"
#include "tetlisDirectxVer.0.0.1Render.h"

VOID Render(VOID)//////////////////////////////////////////////////////////////////////////////////
{
	//////////////////////////////////////////////
	//視点位置の設定、最後に絶対座標への変換を行う
	SetViewPointOverall();

	////////////////////////////////////////////
	//視野角の設定、最後に絶対座標への変換を行う
	SetFocusOfViewOverall();
	
	//カスタムバーテックスの生成///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**/CustomVertex cusV4Background[4]
	/**/	{
	/**/	{ 0.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	/**/	{ 1600.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	/**/	{ 1600.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	/**/	{ 0.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	/**/};
	/**/
	/**/CustomVertex cusV4Frame[4]
	/**/{
	/**/	{ 0.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	/**/	{ 1600.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	/**/	{ 1600.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	/**/	{ 0.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	/**/};
	/**/
	/**/CustomVertex cusV4Tetmino[4]
	/**/{
	/**/	{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
	/**/	{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
	/**/	{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
	/**/	{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	/**/};
	/**/
	/**/RECT rectGameoverStr
	/**/{
	/**/	(LONG)(g_GameoverStrState.x - g_GameoverStrState.xScale),(LONG)(g_GameoverStrState.y - g_GameoverStrState.yScale),(LONG)(g_GameoverStrState.x + g_GameoverStrState.xScale),(LONG)(g_GameoverStrState.y + g_GameoverStrState.yScale)
	/**/};
	/**/
	/**/RECT rectScoreStr
	/**/{
	/**/	(LONG)(g_scoreStrState.x - g_scoreStrState.xScale + 700),(LONG)(g_scoreStrState.y - g_scoreStrState.yScale),(LONG)(g_scoreStrState.x + g_scoreStrState.xScale),(LONG)(g_scoreStrState.y + g_scoreStrState.yScale)
	/**/};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//画面表示関係の処理、カスタムバーテックスをg_tetlisBoardの配列番号を用いて、書き換えテトリスを表示している
	DisplayScreen(cusV4Background, cusV4Tetmino, cusV4Frame, &rectGameoverStr, &rectScoreStr);

	//画面表示が行われたことをバックバッファーに伝える
	g_pD3dDevice->Present(NULL, NULL, NULL, NULL);

	return;
}

VOID SetViewPointOverall(VOID)
{
	D3DXVECTOR3 vecEyePt(0.0f, 1.0f, -3.0f);
	D3DXVECTOR3 vecLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 100.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vecEyePt, &vecLookatPt, &vecUpVec);
	g_pD3dDevice->SetTransform(D3DTS_VIEW, &matView);

	return;
}

VOID SetFocusOfViewOverall(VOID)
{
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pD3dDevice->SetTransform(D3DTS_VIEW, &matProj);

	return;
}

VOID CreateCustomVertex(VOID)
{
	static CustomVertex cusV4Background[4]
	{
		{ 0.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 1600.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 1600.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 0.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	static CustomVertex cusV4Frame[4]
	{
		{ 0.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 1600.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 1600.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 0.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	static CustomVertex cusV4Tetmino[4]
	{
		{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	static RECT rectGameoverStr
	{
		(LONG)(g_GameoverStrState.x - g_GameoverStrState.xScale),(LONG)(g_GameoverStrState.y - g_GameoverStrState.yScale),(LONG)(g_GameoverStrState.x + g_GameoverStrState.xScale),(LONG)(g_GameoverStrState.y + g_GameoverStrState.yScale)
	};

	static RECT rectScoreStr
	{
		(LONG)(g_scoreStrState.x - g_scoreStrState.xScale + 700),(LONG)(g_scoreStrState.y - g_scoreStrState.yScale),(LONG)(g_scoreStrState.x + g_scoreStrState.xScale),(LONG)(g_scoreStrState.y + g_scoreStrState.yScale)
	};

	return;
}

VOID DisplayScreen(CustomVertex *cusV4Background, CustomVertex *cusV4Tetmino, CustomVertex *cusV4Frame, RECT *rectGameoverStr, RECT *rectScoreStr)
{
	g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);

	g_pD3dDevice->BeginScene();

	g_pD3dDevice->SetTexture(0, g_pTexture[g_backgroundTex]);
	g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Background, sizeof(CustomVertex));

	for (int column = 0; column < TETLIS_HEIGHT; column++)
	{
		for (int row = 0; row < TETLIS_HEIGHT; row++)
		{
			if ((g_tetlisBoard[column][row] != -1) && (g_tetlisBoard[column][row] != 9))
			{
				cusV4Tetmino->x = 624.f + row * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino->y = 52.f + column * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				(cusV4Tetmino+1)->x = 624.f + row * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				(cusV4Tetmino+1)->y = 52.f + column * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				(cusV4Tetmino+2)->x = 624.f + row * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				(cusV4Tetmino+2)->y = 52.f + column * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				(cusV4Tetmino+3)->x = 624.f + row * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				(cusV4Tetmino+3)->y = 52.f + column * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}
		}
	}

	g_pD3dDevice->SetTexture(0, g_pTexture[g_frameTex]);
	g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Frame, sizeof(CustomVertex));

	if (g_showGameoverStr)
	{
		g_pFont[g_gameoverFont]->DrawText(NULL, GAMEOVER, -1, rectGameoverStr, NULL, GAMEOVER_COLOR);
	}

	g_pFont[g_scoreFont]->DrawText(NULL, g_scoreArray, -1, rectScoreStr, NULL, GAMEOVER_COLOR);

	for (int coordinateY = 0; coordinateY < 4; coordinateY++)
	{
		for (int coordinateX = 0; coordinateX < 4; coordinateX++)
		{
			if (g_holdBoard[coordinateY][coordinateX] != -1)
			{
				cusV4Tetmino->x = 100.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				(cusV4Tetmino+1)->x = 100.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				(cusV4Tetmino+1)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				(cusV4Tetmino+2)->x = 100.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				(cusV4Tetmino+2)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				(cusV4Tetmino+3)->x = 100.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				(cusV4Tetmino+3)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}

			if (g_nextBoard[coordinateY][coordinateX] != -1)
			{
				cusV4Tetmino->x = 252.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				(cusV4Tetmino + 1)->x = 252.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				(cusV4Tetmino + 1)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				(cusV4Tetmino + 2)->x = 252.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				(cusV4Tetmino + 2)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				(cusV4Tetmino + 3)->x = 252.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				(cusV4Tetmino + 3)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}

			if (g_nextNextBoard[coordinateY][coordinateX] != -1)
			{
				cusV4Tetmino->x = 402.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				(cusV4Tetmino + 1)->x = 402.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				(cusV4Tetmino + 1)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				(cusV4Tetmino + 2)->x = 402.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				(cusV4Tetmino + 2)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				(cusV4Tetmino + 3)->x = 402.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				(cusV4Tetmino + 3)->y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}
		}
	}

	g_pD3dDevice->EndScene();

	return;
}
