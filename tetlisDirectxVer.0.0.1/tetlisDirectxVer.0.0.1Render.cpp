/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render
//
// DATE 2018.06.13
// VER.0.0.5
//
// Author Harutaka-Tsujino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tetlisDirectxVer.0.0.1Common.h"
#include "tetlisDirectxVer.0.0.1Initialize.h"
#include "tetlisDirectxVer.0.0.1Render.h"
#include <d3dx9.h>

ImageState g_tetminoState = { 0.f, 0.f, 30.f / 2, 30.f / 2 };
ImageState g_GameoverStrState = { 990,790,800.f, 450.f };
ImageState g_scoreStrState = { 800.f,790.f,400.f, 225.f };


VOID Render(VOID)//////////////////////////////////////////////////////////////////////////////////
{
	//////////////////////////////////////////////
	//���_�ʒu�̐ݒ�A�Ō�ɐ�΍��W�ւ̕ϊ����s��
	SetViewPointOverall();

	////////////////////////////////////////////
	//����p�̐ݒ�A�Ō�ɐ�΍��W�ւ̕ϊ����s��
	SetFocusOfViewOverall();
	
	//��ʂ̏���
	g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);

	//�`��J�n
	g_pD3dDevice->BeginScene();

	////////////
	//�w�i�̕`��
	RenderBackground();

	/////////////////////////////////////////////////////////////////
	//�e�g���X�u���b�N�̂S���_��tetlisBoard�̔z��ԍ���p���Đݒ肷��
	SetBlockVerticesAndRender();

	//////////////////////////////////////////
	//�e�g���~�m�^�[�Q�b�g�̂S���_��ݒ�A�`��
	SetTetliminoTargetTextureAndRender();

	////////////
	//�z���̕`��
	RnderFrame();

	////////////////////////////////
	//�Q�[���I�[�o�[���̕�����̕`��
	RenderGameoverStr();

	////////////////////
	//�X�R�A������̕`��
	RenderScoreStr();

	/////////////////////////////////////////////////////////
	//�z�[���h�A�l�N�X�g�A�l�N�X�g�l�N�X�g��4���_��ݒ�A�`��
	SetHoldNextNextNextVerticesAndRender();

	//�`��̏I��
	g_pD3dDevice->EndScene();

	//��ʕ\�����s��ꂽ���Ƃ��o�b�N�o�b�t�@�[�ɓ`����
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

VOID RenderBackground(VOID)
{
	CustomVertex cusV4Background[4]
	{
		{ 0.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ 1600.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ 1600.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ 0.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	g_pD3dDevice->SetTexture(0, g_pTexture[g_backgroundTex]);
	g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Background, sizeof(CustomVertex));

	return;
}

VOID SetBlockVerticesAndRender(VOID)
{
	

	for (int column = 0; column < TETLIS_HEIGHT; column++)
	{
		for (int row = 0; row < TETLIS_HEIGHT; row++)
		{
			CustomVertex cusV4Tetmino[4]
			{
				{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
				{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
				{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
				{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
			};

			if ((g_tetlisBoard[column][row] != -1) && (g_tetlisBoard[column][row] != 9))
			{
				cusV4Tetmino[0].x = 624.f + row * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[0].y = 52.f + (column - g_deletedLineCount) * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[1].x = 624.f + row * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[1].y = 52.f + (column - g_deletedLineCount) * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[2].x = 624.f + row * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[2].y = 52.f + (column - g_deletedLineCount) * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				cusV4Tetmino[3].x = 624.f + row * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[3].y = 52.f + (column - g_deletedLineCount) * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
			
				g_pD3dDevice->SetTexture(0, g_pTexture[g_integratedBlockTex]);
				
				switch (g_tetlisBoard[column][row] % 100)
				{
				case 0:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoITex]);
					break;
				case 1:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoJTex]);
					break;
				case 2:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoLTex]);
					break;
				case 3:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoSTex]);
					break;
				case 4:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoZTex]);
					break;
				case 5:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoTTex]);
					break;
				case 6:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoOTex]);
					break;
				case 10:
					cusV4Tetmino[0].tu = 100.f / 512;
					cusV4Tetmino[0].tv = 50.f / 256;
					cusV4Tetmino[1].tu = 150.f / 512;
					cusV4Tetmino[1].tv = 50.f / 256;
					cusV4Tetmino[2].tu = 150.f / 512;
					cusV4Tetmino[2].tv = 100.f / 256;
					cusV4Tetmino[3].tu = 100.f / 512;
					cusV4Tetmino[3].tv = 100.f / 256;
					break;
				case 21:
					cusV4Tetmino[0].tu = 50.f / 512;
					cusV4Tetmino[0].tv = 50.f / 256;
					cusV4Tetmino[1].tu = 100.f / 512;
					cusV4Tetmino[1].tv = 50.f / 256;
					cusV4Tetmino[2].tu = 100.f / 512;
					cusV4Tetmino[2].tv = 100.f / 256;
					cusV4Tetmino[3].tu = 50.f / 512;
					cusV4Tetmino[3].tv = 100.f / 256;
					break;
				case 20:
					cusV4Tetmino[0].tu = 50.f / 512;
					cusV4Tetmino[0].tv = 50.f / 256;
					cusV4Tetmino[1].tu = 100.f / 512;
					cusV4Tetmino[1].tv = 50.f / 256;
					cusV4Tetmino[2].tu = 100.f / 512;
					cusV4Tetmino[2].tv = 100.f / 256;
					cusV4Tetmino[3].tu = 50.f / 512;
					cusV4Tetmino[3].tv = 100.f / 256;
					break;
				case 32:
					cusV4Tetmino[0].tu = 0.f;
					cusV4Tetmino[0].tv = 50.f / 256;
					cusV4Tetmino[1].tu = 50.f / 512;
					cusV4Tetmino[1].tv = 50.f / 256;
					cusV4Tetmino[2].tu = 50.f / 512;
					cusV4Tetmino[2].tv = 100.f / 256;
					cusV4Tetmino[3].tu = 0.f;
					cusV4Tetmino[3].tv = 100.f / 256;
					break;
				case 31:
					cusV4Tetmino[0].tu = 0.f;
					cusV4Tetmino[0].tv = 50.f / 256;
					cusV4Tetmino[1].tu = 50.f / 512;
					cusV4Tetmino[1].tv = 50.f / 256;
					cusV4Tetmino[2].tu = 50.f / 512;
					cusV4Tetmino[2].tv = 100.f / 256;
					cusV4Tetmino[3].tu = 0.f;
					cusV4Tetmino[3].tv = 100.f / 256;
					break;
				case 30:
					cusV4Tetmino[0].tu = 0.f;
					cusV4Tetmino[0].tv = 50.f / 256;
					cusV4Tetmino[1].tu = 50.f / 512;
					cusV4Tetmino[1].tv = 50.f / 256;
					cusV4Tetmino[2].tu = 50.f / 512;
					cusV4Tetmino[2].tv = 100.f / 256;
					cusV4Tetmino[3].tu = 0.f;
					cusV4Tetmino[3].tv = 100.f / 256;
					break;
				}

				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}
		}
	}

	return;
}

VOID SetTetliminoTargetTextureAndRender(VOID)
{
	CustomVertex cusV4Tetmino[4]
	{
		{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1/512.f, 147/256.f },
		{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 50/512.f, 147/256.f },
		{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 50/512.f, 196/256.f },
		{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1/512.f, 196/256.f }
	};

	////////////////////////////////////////////////////
	//flameCount�ƃ}�X�N��p���A���t�@�l�����������Ă���
	UnderGoChangeTarAlpha(cusV4Tetmino);

	for (INT block = 0; block < 4; block++)
	{
		cusV4Tetmino[0].x =2+ 624.f + g_targetMinoNumOfArBuf.YX[block][1] * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
		cusV4Tetmino[0].y =2+ 52.f + (g_targetMinoNumOfArBuf.YX[block][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
		cusV4Tetmino[1].x =-2+ 624.f + g_targetMinoNumOfArBuf.YX[block][1] * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
		cusV4Tetmino[1].y =2+ 52.f + (g_targetMinoNumOfArBuf.YX[block][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
		cusV4Tetmino[2].x =-2+ 624.f + g_targetMinoNumOfArBuf.YX[block][1] * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
		cusV4Tetmino[2].y =-2+ 52.f + (g_targetMinoNumOfArBuf.YX[block][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
		cusV4Tetmino[3].x =2+ 624.f + g_targetMinoNumOfArBuf.YX[block][1] * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
		cusV4Tetmino[3].y =-2+ 52.f + (g_targetMinoNumOfArBuf.YX[block][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

		g_pD3dDevice->SetTexture(0, g_pTexture[g_integratedBlockTex]);
		g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
	}
	
	return;
}

VOID UnderGoChangeTarAlpha(CustomVertex *cusV4Tetmino)
{
	static INT frameCount = 0;

	if (frameCount < 64)
	{
		for (INT coordinate = 0; coordinate < 4; coordinate++)
		{	
			DWORD alpha = 255 - 4 * frameCount;

			(cusV4Tetmino + coordinate)->color &= 0xffffff;
			(cusV4Tetmino + coordinate)->color += alpha * 0x1000000;
		}
	}

	else
	{
		for (INT coordinate = 0; coordinate < 4; coordinate++)
		{
			DWORD alpha = 4 * frameCount;
			
			(cusV4Tetmino + coordinate)->color &= 0xffffff;
			(cusV4Tetmino + coordinate)->color += alpha * 0x1000000;
		}

		if (frameCount == 127)
		{
			frameCount = 0;
		}
	}

	frameCount++;

	return;
}

VOID RnderFrame(VOID)
{
	CustomVertex cusV4Frame[4]
	{
		{ 0.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ 1600.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ 1600.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ 0.f, 900.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	g_pD3dDevice->SetTexture(0, g_pTexture[g_frameTex]);
	g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Frame, sizeof(CustomVertex));

	return;
}

VOID RenderGameoverStr(VOID)
{
	RECT rectGameoverStr
	{
		(LONG)(g_GameoverStrState.x - g_GameoverStrState.xScale), (LONG)(g_GameoverStrState.y - g_GameoverStrState.yScale), (LONG)(g_GameoverStrState.x + g_GameoverStrState.xScale), (LONG)(g_GameoverStrState.y + g_GameoverStrState.yScale)
	};

	if (g_showGameoverStr)
	{
		g_pFont[g_gameoverFont]->DrawText(NULL, GAMEOVER, -1, &rectGameoverStr, NULL, GAMEOVER_COLOR);
	}

	return;
}

VOID RenderScoreStr(VOID)
{
	RECT rectScoreStr
	{
		(LONG)(g_scoreStrState.x - g_scoreStrState.xScale + 700), (LONG)(g_scoreStrState.y - g_scoreStrState.yScale), (LONG)(g_scoreStrState.x + g_scoreStrState.xScale), (LONG)(g_scoreStrState.y + g_scoreStrState.yScale)
	};

	g_pFont[g_scoreFont]->DrawText(NULL, g_scoreArray, -1, &rectScoreStr, DT_RIGHT, GAMEOVER_COLOR);

	return;
}

VOID SetHoldNextNextNextVerticesAndRender(VOID)
{
	CustomVertex cusV4Tetmino[4]
	{
		{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	for (int coordinateY = 0; coordinateY < 4; coordinateY++)
	{
		for (int coordinateX = 0; coordinateX < 4; coordinateX++)
		{
			if (g_holdBoard[coordinateY][coordinateX] != -1)
			{
				cusV4Tetmino[0].x= 100.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[0].y= 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[1].x = 100.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[1].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[2].x = 100.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[2].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				cusV4Tetmino[3].x = 100.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[3].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				
				switch (g_holdBoard[coordinateY][coordinateX] % 100)
				{
				case 0:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoITex]);
					break;
				case 1:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoJTex]);
					break;
				case 2:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoLTex]);
					break;
				case 3:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoSTex]);
					break;
				case 4:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoZTex]);
					break;
				case 5:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoTTex]);
					break;
				case 6:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoOTex]);
					break;
				}

				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}

			if (g_nextBoard[coordinateY][coordinateX] != -1)
			{
				cusV4Tetmino[0].x = 252.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[0].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[1].x = 252.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[1].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[2].x = 252.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[2].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				cusV4Tetmino[3].x = 252.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[3].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				
				switch (g_nextBoard[coordinateY][coordinateX] % 100)
				{
				case 0:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoITex]);
					break;
				case 1:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoJTex]);
					break;
				case 2:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoLTex]);
					break;
				case 3:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoSTex]);
					break;
				case 4:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoZTex]);
					break;
				case 5:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoTTex]);
					break;
				case 6:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoOTex]);
					break;
				}

				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}

			if (g_nextNextBoard[coordinateY][coordinateX] != -1)
			{
				cusV4Tetmino[0].x = 402.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[0].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[1].x = 402.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[1].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[2].x = 402.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[2].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				cusV4Tetmino[3].x = 402.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[3].y = 52.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				
				switch (g_nextNextBoard[coordinateY][coordinateX] % 10)
				{
				case 0:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoITex]);
					break;
				case 1:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoJTex]);
					break;
				case 2:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoLTex]);
					break;
				case 3:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoSTex]);
					break;
				case 4:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoZTex]);
					break;
				case 5:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoTTex]);
					break;
				case 6:
					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoOTex]);
					break;
				}

				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}
		}
	}

	return;
}