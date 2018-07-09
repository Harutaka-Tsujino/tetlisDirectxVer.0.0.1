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
#include "tetlisDirectxVer.0.0.1Control.h"
#include <d3dx9.h>

ImageState g_tetminoState = { 0.f, 0.f, 30.f / 2, 30.f / 2 };
ImageState g_deletedLineEffectState = { 545.f,0.f,150.f,15.f };
ImageState g_gameoverStrState = { 990,790,800.f, 450.f };
ImageState g_scoreStrState = { 800.f,790.f,400.f, 225.f };
ImageState g_undergroundStrState = { 800.f,500.f,400.f, 225.f };

VOID Render(VOID)//////////////////////////////////////////////////////////////////////////////////
{
	static INT toDeleteTargetBlockCount = 0, prevDeletedLineState;

	//////////////////////////////////////////////
	//視点位置の設定、最後に絶対座標への変換を行う
	SetViewPointOverall();

	////////////////////////////////////////////
	//視野角の設定、最後に絶対座標への変換を行う
	SetFocusOfViewOverall();

	//画面の消去
	g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);

	//描画開始
	g_pD3dDevice->BeginScene();

	////////////
	//背景の描画
	RenderBackground();

	/////////////////////////////////////////////////////////////////
	//テトリスブロックの４頂点をtetlisBoardの配列番号を用いて設定する
	SetBlockVerticesAndRender();

	if (!prevDeletedLineState)
	{
		if (g_deletedLine)
		{
			toDeleteTargetBlockCount = 0;
		}
	}

	if (!g_deletedLine && !(0 < toDeleteTargetBlockCount&&toDeleteTargetBlockCount < 30))
	{
		if (!(g_itemData.useItem))
		{
			//////////////////////////////////////////
			//テトリミノターゲットの４頂点を設定、描画
			SetTetliminoTargetTextureAndRender();
		}
	}

	else
	{
		toDeleteTargetBlockCount++;

		if (toDeleteTargetBlockCount == 45)
		{
			toDeleteTargetBlockCount = 0;
		}
	}

	//////////////////////////////////////////
	//ハードドロップのエフェクトを描画する関数
	SetHardDropEffectTextureAndRender();

	//////////////////////////////////////////////
	//ラインが消された時のエフェクトを描画する関数
	SetDeletedLineEffectTextureAndRender();

	////////////
	//額縁の描画
	RnderFrame();

	////////////////////////////////////
	//ドリルアイテムに関係する描画をする
	//SetItemVerticesAndRender();

	////////////////////////////////////
	//アイテムの使用時に関する描画をする
	SetItemVerticiesAndRender();

	/*SetInventoryVerticesAndRender();*/
	{
		//ボタンが押されている時だけ描画したいので例外的にキーを取得している
		HRESULT hr = g_pDKeyDevice->Acquire();

		if ((hr == DI_OK) || (hr == S_FALSE))
		{
			//キー入力情報のバッファー
			BYTE diks[256];
			static BYTE prevDiks[256];

			//入力された情報を読み取る
			g_pDKeyDevice->GetDeviceState(sizeof(diks), &diks);

			ImageState inventoryTop = { 200.f,560.f,128.f/2,128.f/2 };
			ImageState inventoryBottom = { 200.f,560.f,128.f/2,128.f/2 };
			ImageState itemIconList = { 200.f,560.f,100.f/2,100.f/2 };

			CustomVertex cusV4ItemIconList[4] =
			{
				{ itemIconList.x - itemIconList.xScale,itemIconList.y - itemIconList.yScale,1.f,1.f,0xFFFFFFFF,50 * (g_itemData.currentItemNum) / 512.f,0.f },
				{ itemIconList.x + itemIconList.xScale,itemIconList.y - itemIconList.yScale,1.f,1.f,0xFFFFFFFF,50 * (g_itemData.currentItemNum+1)/512.f,0.f },
				{ itemIconList.x + itemIconList.xScale,itemIconList.y + itemIconList.yScale,1.f,1.f,0xFFFFFFFF,50 * (g_itemData.currentItemNum+1) / 512.f,50/64.f },
				{ itemIconList.x - itemIconList.xScale,itemIconList.y + itemIconList.yScale,1.f,1.f,0xFFFFFFFF,50 * (g_itemData.currentItemNum) / 512.f,50 / 64.f }
			};

			g_pD3dDevice->SetTexture(0, g_pTexture[g_itemIconListTex]);
			g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4ItemIconList, sizeof(CustomVertex));

			if (diks[DIK_V] & 0x80)
			{
				inventoryTop.y -= 50.f;
				inventoryBottom.y += 50.f;

				cusV4ItemIconList[0].tu = 0.f;
				cusV4ItemIconList[1].tu = 1.f;
				cusV4ItemIconList[2].tu = 1.f;
				cusV4ItemIconList[3].tu = 0.f;

				const FLOAT posX = 162;
				cusV4ItemIconList[0].x =posX+ itemIconList.x -512 + 100 * (g_itemMax-g_itemData.currentItemNum);
				cusV4ItemIconList[1].x =posX+ itemIconList.x+512+ 100 * (g_itemMax-g_itemData.currentItemNum);
				cusV4ItemIconList[2].x =posX+ itemIconList.x+512+ 100 * (g_itemMax-g_itemData.currentItemNum);
				cusV4ItemIconList[3].x =posX+ itemIconList.x-512+ 100 * (g_itemMax-g_itemData.currentItemNum);
													
				g_pD3dDevice->SetTexture(0, g_pTexture[g_itemIconListTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4ItemIconList, sizeof(CustomVertex));
			}

			CustomVertex CusV4InventoryTop[4] =
			{
				{ inventoryTop.x - inventoryTop.xScale,inventoryTop.y - inventoryTop.yScale,1.f,1.f,0xFFFFFFFF,0.f,0.f },
				{ inventoryTop.x + inventoryTop.xScale,inventoryTop.y - inventoryTop.yScale,1.f,1.f,0xFFFFFFFF,0.5f,0.f },
				{ inventoryTop.x + inventoryTop.xScale,inventoryTop.y + inventoryTop.yScale,1.f,1.f,0xFFFFFFFF,0.5f,1.f },
				{ inventoryTop.x - inventoryTop.xScale,inventoryTop.y + inventoryTop.yScale,1.f,1.f,0xFFFFFFFF,0.f,1.f }
			};

			CustomVertex CusV4InventoryBottom[4] =
			{
				{ inventoryBottom.x - inventoryBottom.xScale,inventoryBottom.y - inventoryBottom.yScale,1.f,1.f,0xFFFFFFFF,0.5f,0.f },
				{ inventoryBottom.x + inventoryBottom.xScale,inventoryBottom.y - inventoryBottom.yScale,1.f,1.f,0xFFFFFFFF,1.f,0.f },
				{ inventoryBottom.x + inventoryBottom.xScale,inventoryBottom.y + inventoryBottom.yScale,1.f,1.f,0xFFFFFFFF,1.f,1.f },
				{ inventoryBottom.x - inventoryBottom.xScale,inventoryBottom.y + inventoryBottom.yScale,1.f,1.f,0xFFFFFFFF,0.5f,1.f }
			};

			g_pD3dDevice->SetTexture(0, g_pTexture[g_inventoryTex]);
			g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, CusV4InventoryTop, sizeof(CustomVertex));
			g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, CusV4InventoryBottom, sizeof(CustomVertex));
		}
	}
	////////////////////////////////
	//ゲームオーバー時の文字列の描画
	RenderGameoverStr();

	////////////////////
	//スコア文字列の描画
	RenderScoreStr();

	//////////////////////////////////////
	//どのくらい掘り進んだかの文字列の描画
	RenderUndergroundStr();

	/////////////////////////////////////////////////////////
	//ホールド、ネクスト、ネクストネクストの4頂点を設定、描画
	SetHoldNextNextNextVerticesAndRender();

	//描画の終了
	g_pD3dDevice->EndScene();

	//画面表示が行われたことをバックバッファーに伝える
	g_pD3dDevice->Present(NULL, NULL, NULL, NULL);

	prevDeletedLineState = g_deletedLine;

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
	CustomVertex cusV4Background[4] =
	{
		{ 0.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ 1280.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ 1280.f, 720.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ 0.f, 720.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	g_pD3dDevice->SetTexture(0, g_pTexture[g_backgroundTex]);
	g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Background, sizeof(CustomVertex));

	return;
}

VOID SetItemVerticiesAndRender(VOID)
{
	if (g_itemData.useItem)
	{
		switch (g_itemData.currentItemNum)
		{
		case g_ultraDrillItem:
		{
			static ImageState drillItemState = { 0.f,0.f,90.f / 2,90.f / 2 };
			static INT additonalPosY = 0;

			if (g_itemData.count[g_ultraDrillItem] > 410)
			{
				additonalPosY += 200;
			}

			else
			{
				additonalPosY = 0;
			}

			CustomVertex cusV4drillItem[4] =
			{
				{ drillItemState.x - drillItemState.xScale, drillItemState.y - drillItemState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
				{ drillItemState.x + drillItemState.xScale, drillItemState.y - drillItemState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
				{ drillItemState.x + drillItemState.xScale, drillItemState.y + drillItemState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
				{ drillItemState.x - drillItemState.xScale, drillItemState.y + drillItemState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
			};

			if (g_itemData.count[g_ultraDrillItem] == 0)
			{
				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2;

				cusV4drillItem[0].tu = 200 / 512.f;
				cusV4drillItem[0].tv = 0 / 256.f;
				cusV4drillItem[1].tu = 250 / 512.f;
				cusV4drillItem[1].tv = 0 / 256.f;
				cusV4drillItem[2].tu = 250 / 512.f;
				cusV4drillItem[2].tv = 50 / 256.f;
				cusV4drillItem[3].tu = 200 / 512.f;
				cusV4drillItem[3].tv = 50 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));
			}

			if (g_itemData.count[g_ultraDrillItem] > 0)
			{
				FLOAT additionalScale = 0;

				if (g_itemData.count[g_ultraDrillItem] >= 180)
				{
					additionalScale = g_itemData.count[g_ultraDrillItem] - 180.f;
				}

				if (additionalScale > 360 - 180.f)
				{
					additionalScale = 360 - 180.f;
				}

				if (g_itemData.count[g_ultraDrillItem] < 180)
				{
					cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
					cusV4drillItem[0].y = 40.f;
					cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
					cusV4drillItem[1].y = 40.f;
					cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
					cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2;
					cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
					cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2;

					cusV4drillItem[0].tu = 200 / 512.f;
					cusV4drillItem[0].tv = 0 / 256.f;
					cusV4drillItem[1].tu = 250 / 512.f;
					cusV4drillItem[1].tv = 0 / 256.f;
					cusV4drillItem[2].tu = 250 / 512.f;
					cusV4drillItem[2].tv = 50 / 256.f;
					cusV4drillItem[3].tu = 200 / 512.f;
					cusV4drillItem[3].tv = 50 / 256.f;

					g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
					g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));
				}

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale - additionalScale;
				cusV4drillItem[0].y = 40.f - additionalScale + additonalPosY;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale + additionalScale;
				cusV4drillItem[1].y = 40.f - additionalScale + additonalPosY;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale + additionalScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 + additionalScale + additonalPosY;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale - additionalScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 + additionalScale + additonalPosY;

				switch (g_itemData.count[g_ultraDrillItem] / 60)
				{
				case 0:
					cusV4drillItem[0].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 11) % 4) / 512.f;
					cusV4drillItem[0].tv = 200 / 256.f;
					cusV4drillItem[1].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 11) % 4 + 1) / 512.f;
					cusV4drillItem[1].tv = 200 / 256.f;
					cusV4drillItem[2].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 11) % 4 + 1) / 512.f;
					cusV4drillItem[2].tv = 250 / 256.f;
					cusV4drillItem[3].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 11) % 4) / 512.f;
					cusV4drillItem[3].tv = 250 / 256.f;

					break;

				case 1:
					cusV4drillItem[0].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 9) % 4) / 512.f;
					cusV4drillItem[0].tv = 200 / 256.f;
					cusV4drillItem[1].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 9) % 4 + 1) / 512.f;
					cusV4drillItem[1].tv = 200 / 256.f;
					cusV4drillItem[2].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 9) % 4 + 1) / 512.f;
					cusV4drillItem[2].tv = 250 / 256.f;
					cusV4drillItem[3].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 9) % 4) / 512.f;
					cusV4drillItem[3].tv = 250 / 256.f;

					break;

				case 2:
					cusV4drillItem[0].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 5) % 4) / 512.f;
					cusV4drillItem[0].tv = 200 / 256.f;
					cusV4drillItem[1].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 5) % 4 + 1) / 512.f;
					cusV4drillItem[1].tv = 200 / 256.f;
					cusV4drillItem[2].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 5) % 4 + 1) / 512.f;
					cusV4drillItem[2].tv = 250 / 256.f;
					cusV4drillItem[3].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 5) % 4) / 512.f;
					cusV4drillItem[3].tv = 250 / 256.f;

					break;

				case 3:
					cusV4drillItem[0].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 3) % 4) / 512.f;
					cusV4drillItem[0].tv = 200 / 256.f;
					cusV4drillItem[1].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 3) % 4 + 1) / 512.f;
					cusV4drillItem[1].tv = 200 / 256.f;
					cusV4drillItem[2].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 3) % 4 + 1) / 512.f;
					cusV4drillItem[2].tv = 250 / 256.f;
					cusV4drillItem[3].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 3) % 4) / 512.f;
					cusV4drillItem[3].tv = 250 / 256.f;

					break;

				default:
					cusV4drillItem[0].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 1) % 4) / 512.f;
					cusV4drillItem[0].tv = 200 / 256.f;
					cusV4drillItem[1].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 1) % 4 + 1) / 512.f;
					cusV4drillItem[1].tv = 200 / 256.f;
					cusV4drillItem[2].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 1) % 4 + 1) / 512.f;
					cusV4drillItem[2].tv = 250 / 256.f;
					cusV4drillItem[3].tu = 50 * ((g_itemData.count[g_ultraDrillItem] / 1) % 4) / 512.f;
					cusV4drillItem[3].tv = 250 / 256.f;

					break;
				}

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f - additionalScale / 1.0f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f - additionalScale / 1.0f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 - additionalScale / 1.0f;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 - additionalScale / 1.0f;

				cusV4drillItem[0].tu = 0 / 512.f;
				cusV4drillItem[0].tv = 100 / 256.f;
				cusV4drillItem[1].tu = 50 / 512.f;
				cusV4drillItem[1].tv = 100 / 256.f;
				cusV4drillItem[2].tu = 50 / 512.f;
				cusV4drillItem[2].tv = 150 / 256.f;
				cusV4drillItem[3].tu = 0 / 512.f;
				cusV4drillItem[3].tv = 150 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f - additionalScale / 0.8f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f - additionalScale / 0.8f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 - additionalScale / 0.8f;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 - additionalScale / 0.8f;

				cusV4drillItem[0].tu = 0 / 512.f;
				cusV4drillItem[0].tv = 150 / 256.f;
				cusV4drillItem[1].tu = 50 / 512.f;
				cusV4drillItem[1].tv = 150 / 256.f;
				cusV4drillItem[2].tu = 50 / 512.f;
				cusV4drillItem[2].tv = 200 / 256.f;
				cusV4drillItem[3].tu = 0 / 512.f;
				cusV4drillItem[3].tv = 200 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f - additionalScale / 1.2f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f - additionalScale / 2.7f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 - additionalScale / 1.2f;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 - additionalScale / 1.2f;

				cusV4drillItem[0].tu = 50 / 512.f;
				cusV4drillItem[0].tv = 100 / 256.f;
				cusV4drillItem[1].tu = 100 / 512.f;
				cusV4drillItem[1].tv = 100 / 256.f;
				cusV4drillItem[2].tu = 100 / 512.f;
				cusV4drillItem[2].tv = 150 / 256.f;
				cusV4drillItem[3].tu = 50 / 512.f;
				cusV4drillItem[3].tv = 150 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f - additionalScale / 0.9f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f - additionalScale / 0.9f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 - additionalScale / 0.9f;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 - additionalScale / 0.9f;

				cusV4drillItem[0].tu = 50 / 512.f;
				cusV4drillItem[0].tv = 150 / 256.f;
				cusV4drillItem[1].tu = 100 / 512.f;
				cusV4drillItem[1].tv = 150 / 256.f;
				cusV4drillItem[2].tu = 100 / 512.f;
				cusV4drillItem[2].tv = 200 / 256.f;
				cusV4drillItem[3].tu = 50 / 512.f;
				cusV4drillItem[3].tv = 200 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f - additionalScale / 1.3f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f - additionalScale / 1.3f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 - additionalScale / 1.3f;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 - additionalScale / 1.3f;

				cusV4drillItem[0].tu = 100 / 512.f;
				cusV4drillItem[0].tv = 100 / 256.f;
				cusV4drillItem[1].tu = 150 / 512.f;
				cusV4drillItem[1].tv = 100 / 256.f;
				cusV4drillItem[2].tu = 150 / 512.f;
				cusV4drillItem[2].tv = 150 / 256.f;
				cusV4drillItem[3].tu = 100 / 512.f;
				cusV4drillItem[3].tv = 150 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f - additionalScale / 0.5f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f - additionalScale / 0.5f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 - additionalScale / 0.5f;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 - additionalScale / 0.5f;

				cusV4drillItem[0].tu = 100 / 512.f;
				cusV4drillItem[0].tv = 150 / 256.f;
				cusV4drillItem[1].tu = 150 / 512.f;
				cusV4drillItem[1].tv = 150 / 256.f;
				cusV4drillItem[2].tu = 150 / 512.f;
				cusV4drillItem[2].tv = 200 / 256.f;
				cusV4drillItem[3].tu = 100 / 512.f;
				cusV4drillItem[3].tv = 200 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f - additionalScale / 0.7f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f - additionalScale / 0.7f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 - additionalScale / 0.7f;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 - additionalScale / 0.7f;

				cusV4drillItem[0].tu = 150 / 512.f;
				cusV4drillItem[0].tv = 100 / 256.f;
				cusV4drillItem[1].tu = 200 / 512.f;
				cusV4drillItem[1].tv = 100 / 256.f;
				cusV4drillItem[2].tu = 200 / 512.f;
				cusV4drillItem[2].tv = 150 / 256.f;
				cusV4drillItem[3].tu = 150 / 512.f;
				cusV4drillItem[3].tv = 150 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[0].y = 40.f - additionalScale / 1.0f;
				cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[1].y = 40.f - additionalScale / 1.0f;
				cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 - additionalScale / 1.0f;
				cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 - additionalScale / 1.0f;

				cusV4drillItem[0].tu = 150 / 512.f;
				cusV4drillItem[0].tv = 150 / 256.f;
				cusV4drillItem[1].tu = 200 / 512.f;
				cusV4drillItem[1].tv = 150 / 256.f;
				cusV4drillItem[2].tu = 200 / 512.f;
				cusV4drillItem[2].tv = 200 / 256.f;
				cusV4drillItem[3].tu = 150 / 512.f;
				cusV4drillItem[3].tv = 200 / 256.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

				if (59 < g_itemData.count[g_ultraDrillItem] && g_itemData.count[g_ultraDrillItem] < 180)
				{
					cusV4drillItem[0].x = 410.f + g_itemData.posYX[1] * (15 * 2) - 10.f * ((g_itemData.count[g_ultraDrillItem] - 60) / 24);
					cusV4drillItem[0].y = 85.f - 10.f * ((g_itemData.count[g_ultraDrillItem] - 60) / 24);
					cusV4drillItem[1].x = 410.f + g_itemData.posYX[1] * (15 * 2) + 10.f * ((g_itemData.count[g_ultraDrillItem] - 60) / 24);
					cusV4drillItem[1].y = 85.f - 10.f * ((g_itemData.count[g_ultraDrillItem] - 60) / 24);
					cusV4drillItem[2].x = 410.f + g_itemData.posYX[1] * (15 * 2) + 10.f * ((g_itemData.count[g_ultraDrillItem] - 60) / 24);
					cusV4drillItem[2].y = 85.f + 10.f * ((g_itemData.count[g_ultraDrillItem] - 60) / 24);
					cusV4drillItem[3].x = 410.f + g_itemData.posYX[1] * (15 * 2) - 10.f * ((g_itemData.count[g_ultraDrillItem] - 60) / 24);
					cusV4drillItem[3].y = 85.f + 10.f * ((g_itemData.count[g_ultraDrillItem] - 60) / 24);

					cusV4drillItem[0].tu = (275 - 5 * ((g_itemData.count[g_ultraDrillItem] - 60) / 24)) / 512.f;
					cusV4drillItem[0].tv = (25 - 5 * ((g_itemData.count[g_ultraDrillItem] - 60) / 24)) / 256.f;
					cusV4drillItem[1].tu = (275 + 5 * ((g_itemData.count[g_ultraDrillItem] - 60) / 24)) / 512.f;
					cusV4drillItem[1].tv = (25 - 5 * ((g_itemData.count[g_ultraDrillItem] - 60) / 24)) / 256.f;
					cusV4drillItem[2].tu = (275 + 5 * ((g_itemData.count[g_ultraDrillItem] - 60) / 24)) / 512.f;
					cusV4drillItem[2].tv = (25 + 5 * ((g_itemData.count[g_ultraDrillItem] - 60) / 24)) / 256.f;
					cusV4drillItem[3].tu = (275 - 5 * ((g_itemData.count[g_ultraDrillItem] - 60) / 24)) / 512.f;
					cusV4drillItem[3].tv = (25 + 5 * ((g_itemData.count[g_ultraDrillItem] - 60) / 24)) / 256.f;

					g_pD3dDevice->SetTexture(0, g_pTexture[g_drillEffectTex]);
					g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));
				}

				INT flameScaleY = 10, flameScaleX = 30;

				cusV4drillItem[0].x = 408.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale - additionalScale - flameScaleX;
				cusV4drillItem[0].y = 40.f - additionalScale - flameScaleY;
				cusV4drillItem[1].x = 408.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale + additionalScale + flameScaleX;
				cusV4drillItem[1].y = 40.f - additionalScale - flameScaleY;
				cusV4drillItem[2].x = 408.f + g_itemData.posYX[1] * (15 * 2) + drillItemState.xScale + additionalScale + flameScaleX;
				cusV4drillItem[2].y = 40.f + drillItemState.yScale * 2 + additionalScale + flameScaleY + additonalPosY;
				cusV4drillItem[3].x = 408.f + g_itemData.posYX[1] * (15 * 2) - drillItemState.xScale - additionalScale - flameScaleX;
				cusV4drillItem[3].y = 40.f + drillItemState.yScale * 2 + additionalScale + flameScaleY + additonalPosY;

				if (419 < g_itemData.count[g_ultraDrillItem] && g_itemData.count[g_ultraDrillItem] < 480)
				{
					CustomVertex cusV4DrillBurnEffect[4] =
					{
						{410.f,70.f,1.f,1.f,0xBBFFFFFF,0.f,0.f},
						{710.f,70.f,1.f,1.f,0xBBFFFFFF,1.f,0.f},
						{710.f,670.f,1.f,1.f,0xBBFFFFFF,1.f,1.f},
						{410.f,670.f,1.f,1.f,0xBBFFFFFF,1.f,1.f}
					};

					cusV4DrillBurnEffect[0].tu = 300 * (((g_itemData.count[g_ultraDrillItem] - 420) / 4) % 15) / 4500.f;
					cusV4DrillBurnEffect[0].tv = 0 / 600.f;
					cusV4DrillBurnEffect[1].tu = 300 * (((g_itemData.count[g_ultraDrillItem] - 420) / 4) % 15 + 1) / 4500.f;
					cusV4DrillBurnEffect[1].tv = 0 / 600.f;
					cusV4DrillBurnEffect[2].tu = 300 * (((g_itemData.count[g_ultraDrillItem] - 420) / 4) % 15 + 1) / 4500.f;
					cusV4DrillBurnEffect[2].tv = 600 / 600.f;
					cusV4DrillBurnEffect[3].tu = 300 * (((g_itemData.count[g_ultraDrillItem] - 420) / 4) % 15) / 4500.f;
					cusV4DrillBurnEffect[3].tv = 600 / 600.f;

					g_pD3dDevice->SetTexture(0, g_pTexture[g_drillBurnEffectTex]);
					g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4DrillBurnEffect, sizeof(CustomVertex));
				}



				if (179 < g_itemData.count[g_ultraDrillItem] && g_itemData.count[g_ultraDrillItem] < 430)
				{
					cusV4drillItem[0].color = 0xBBFFFFFF;
					cusV4drillItem[1].color = 0xBBFFFFFF;
					cusV4drillItem[2].color = 0xBBFFFFFF;
					cusV4drillItem[3].color = 0xBBFFFFFF;

					if (g_itemData.count[g_ultraDrillItem] < 196)
					{
						cusV4drillItem[0].tu = 64 * (((g_itemData.count[g_ultraDrillItem] - 180) / 4) % 4) / 1280.f;
						cusV4drillItem[0].tv = 0 / 64.f;
						cusV4drillItem[1].tu = 64 * (((g_itemData.count[g_ultraDrillItem] - 180) / 4) % 4 + 1) / 1280.f;
						cusV4drillItem[1].tv = 0 / 64.f;
						cusV4drillItem[2].tu = 64 * (((g_itemData.count[g_ultraDrillItem] - 180) / 4) % 4 + 1) / 1280.f;
						cusV4drillItem[2].tv = 63 / 64.f;
						cusV4drillItem[3].tu = 64 * (((g_itemData.count[g_ultraDrillItem] - 180) / 4) % 4) / 1280.f;
						cusV4drillItem[3].tv = 63 / 64.f;
					}

					if (195 < g_itemData.count[g_ultraDrillItem] && g_itemData.count[g_ultraDrillItem] < 420)
					{
						cusV4drillItem[0].tu = 64 * ((g_itemData.count[g_ultraDrillItem] / 3) % 7 + 4) / 1280.f;
						cusV4drillItem[0].tv = 0 / 64.f;
						cusV4drillItem[1].tu = 64 * ((g_itemData.count[g_ultraDrillItem] / 3) % 7 + 5) / 1280.f;
						cusV4drillItem[1].tv = 0 / 64.f;
						cusV4drillItem[2].tu = 64 * ((g_itemData.count[g_ultraDrillItem] / 3) % 7 + 5) / 1280.f;
						cusV4drillItem[2].tv = 63 / 64.f;
						cusV4drillItem[3].tu = 64 * ((g_itemData.count[g_ultraDrillItem] / 3) % 7 + 4) / 1280.f;
						cusV4drillItem[3].tv = 63 / 64.f;
					}

					if (419 < g_itemData.count[g_ultraDrillItem])
					{
						cusV4drillItem[0].tu = 64 * ((g_itemData.count[g_ultraDrillItem] / 2) % 7 + 11) / 1280.f;
						cusV4drillItem[0].tv = 0 / 64.f;
						cusV4drillItem[1].tu = 64 * ((g_itemData.count[g_ultraDrillItem] / 2) % 7 + 12) / 1280.f;
						cusV4drillItem[1].tv = 0 / 64.f;
						cusV4drillItem[2].tu = 64 * ((g_itemData.count[g_ultraDrillItem] / 2) % 7 + 12) / 1280.f;
						cusV4drillItem[2].tv = 63 / 64.f;
						cusV4drillItem[3].tu = 64 * ((g_itemData.count[g_ultraDrillItem] / 2) % 7 + 11) / 1280.f;
						cusV4drillItem[3].tv = 63 / 64.f;
					}

					g_pD3dDevice->SetTexture(0, g_pTexture[g_drillFlameEffectTex]);
					g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4drillItem, sizeof(CustomVertex));

					cusV4drillItem[0].color = 0xFFFFFFFF;
					cusV4drillItem[1].color = 0xFFFFFFFF;
					cusV4drillItem[2].color = 0xFFFFFFFF;
					cusV4drillItem[3].color = 0xFFFFFFFF;
				}
			}

			static INT InitDeflecton = false;

			typedef struct
			{
				INT deflectionalPosX;
				INT deflectionalInitialVelocityY;
				INT deflectionalInitialVelocityX;
			}ParticleDeflection;

			static ParticleDeflection particleDeflection[12 * 4];

			if (g_itemData.count[g_ultraDrillItem] == 84 || g_itemData.count[g_ultraDrillItem] == 108 || g_itemData.count[g_ultraDrillItem] == 132 || g_itemData.count[g_ultraDrillItem] == 156)
			{
				InitDeflecton = true;
			}

			static FLOAT accelY = 0;

			if (InitDeflecton)
			{
				for (INT effect = 0; effect < 12; effect++)
				{
					(particleDeflection + effect)->deflectionalPosX = rand() % 90;
					(particleDeflection + effect)->deflectionalInitialVelocityY = rand() % 15 + 1;
					(particleDeflection + effect)->deflectionalInitialVelocityX = rand() % 15 - 7;
				}

				accelY = 0;
			}

			if ((83 < g_itemData.count[g_ultraDrillItem] && g_itemData.count[g_ultraDrillItem] < 180))
			{
				CustomVertex CusV3HardDropParticleEffect[3] =
				{
					{ 0.f,0.f,1.f,1.f,0xFFFFFFFF,0.5f,0.f },
					{ 0.f,0.f,1.f,1.f,0xFFFFFFFF,1.f,1.f },
					{ 0.f,0.f,1.f,1.f,0xFFFFFFFF,0.f,1.f }
				};

				InitDeflecton = false;

				for (INT effect = 0; effect < 12; effect++)
				{

					(CusV3HardDropParticleEffect)->x = 515.f + particleDeflection[effect].deflectionalPosX + 4 + (particleDeflection[effect].deflectionalInitialVelocityX*((g_itemData.count[g_ultraDrillItem] - 84) % 24));
					(CusV3HardDropParticleEffect)->y = 40.f + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*((g_itemData.count[g_ultraDrillItem] - 84) % 24));

					(CusV3HardDropParticleEffect + 1)->x = 515.f + particleDeflection[effect].deflectionalPosX + 8 + (particleDeflection[effect].deflectionalInitialVelocityX*((g_itemData.count[g_ultraDrillItem] - 84) % 24));
					(CusV3HardDropParticleEffect + 1)->y = 40.f + +7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*((g_itemData.count[g_ultraDrillItem] - 84) % 24));

					(CusV3HardDropParticleEffect + 2)->x = 515.f + particleDeflection[effect].deflectionalPosX + (particleDeflection[effect].deflectionalInitialVelocityX*((g_itemData.count[g_ultraDrillItem] - 84) % 24));
					(CusV3HardDropParticleEffect + 2)->y = 40.f + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*((g_itemData.count[g_ultraDrillItem] - 84) % 24));


					DWORD randomColor = (0xFF000000) + ((rand() % 256) * 0x10000) + ((rand() % 256) * 0x100) + (rand() % 256);

					CusV3HardDropParticleEffect->color = randomColor;
					(CusV3HardDropParticleEffect + 1)->color = randomColor;
					(CusV3HardDropParticleEffect + 2)->color = randomColor;

					g_pD3dDevice->SetTexture(0, g_pTexture[g_hardDropEffectTex]);
					g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, CusV3HardDropParticleEffect, sizeof(CustomVertex));
				}

				accelY += 0.7f;
			}
		}
		break;
		case g_laserCannonItem:
		{
			ImageState laserCannonState = { 395.f,0.f,15.f,15.f };
			laserCannonState.y = 70.f + 30 * (g_itemData.posYX[0] - g_deletedLineCount - 4) + laserCannonState.yScale;

			CustomVertex cusV4LaserCannon[4] =
			{
				{ laserCannonState.x - laserCannonState.xScale,laserCannonState.y - laserCannonState.yScale,1.f,1.f,0xFFFFFFFF,0.f,0.f },
				{ laserCannonState.x + laserCannonState.xScale,laserCannonState.y - laserCannonState.yScale,1.f,1.f,0xFFFFFFFF,1.f,0.f },
				{ laserCannonState.x + laserCannonState.xScale,laserCannonState.y + laserCannonState.yScale,1.f,1.f,0xFFFFFFFF,1.f,1.f },
				{ laserCannonState.x - laserCannonState.xScale,laserCannonState.y + laserCannonState.yScale,1.f,1.f,0xFFFFFFFF,0.f,1.f }
			};

			g_pD3dDevice->SetTexture(0, g_pTexture[g_laserCannonTex]);
			g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4LaserCannon, sizeof(CustomVertex));

			if (0 < g_itemData.count[g_laserCannonItem] && g_itemData.count[g_laserCannonItem] < 51)
			{
				ImageState laserCannonEffectState = { 410.f,0.f,15.f,15.f };

				laserCannonEffectState.y = 70.f + 30 * (g_itemData.posYX[0] - g_deletedLineCount - 4) + 15.f;

				if ((1 < g_itemData.count[g_laserCannonItem]) && (g_itemData.count[g_laserCannonItem] < 32))
				{
					laserCannonEffectState.yScale = 15.f + 2.8f*g_itemData.count[g_laserCannonItem];
				}

				if ((31 < g_itemData.count[g_laserCannonItem]) && (g_itemData.count[g_laserCannonItem] < 46))
				{
					laserCannonEffectState.yScale = 45.f - 3 * (g_itemData.count[g_laserCannonItem] - 30);
				}

				if (laserCannonEffectState.xScale < 150.f)
				{
					laserCannonEffectState.xScale = 5.f * g_itemData.count[g_laserCannonItem];
					laserCannonEffectState.x = 410.f + 5.f * g_itemData.count[g_laserCannonItem];
				}

				if (laserCannonEffectState.xScale >= 150.f)
				{
					laserCannonEffectState.x = 410.f + 150.f;
					laserCannonEffectState.xScale = 150.f;
				}

				CustomVertex cusV4LaserCannonEffect[4] =
				{
					{ laserCannonEffectState.x - laserCannonEffectState.xScale,laserCannonEffectState.y - laserCannonEffectState.yScale,1.f,1.f,0xFFFFFFFF,0.f,0.f },
					{ laserCannonEffectState.x + laserCannonEffectState.xScale,laserCannonEffectState.y - laserCannonEffectState.yScale,1.f,1.f,0xFFFFFFFF,1.f,0.f },
					{ laserCannonEffectState.x + laserCannonEffectState.xScale,laserCannonEffectState.y + laserCannonEffectState.yScale,1.f,1.f,0xFFFFFFFF,1.f,1.f },
					{ laserCannonEffectState.x - laserCannonEffectState.xScale,laserCannonEffectState.y + laserCannonEffectState.yScale,1.f,1.f,0xFFFFFFFF,0.f,1.f }
				};

				g_pD3dDevice->SetTexture(0, g_pTexture[g_effect_laserTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4LaserCannonEffect, sizeof(CustomVertex));
			}
			break;
		}
		case g_bulletItem:
		{
			static float accelY = 0;

			if (g_itemData.count[g_bulletItem] == 0)
			{
				ImageState scopeState = { 395.f,0.f,256.f / 2,256.f / 2 };

				scopeState.x = 395.f + 30 * (g_itemData.posYX[1]);
				scopeState.y = 85.f + 30 * (g_itemData.posYX[0] - g_deletedLineCount - 4);

				accelY = 0;

				CustomVertex cusV4Scope[4] =
				{
					{ scopeState.x - scopeState.xScale,scopeState.y - scopeState.yScale,1.f,1.f,0xFFFFFFFF,0.f / 512,50 / 512.f },
					{ scopeState.x + scopeState.xScale,scopeState.y - scopeState.yScale,1.f,1.f,0xFFFFFFFF,256 / 512.f,50 / 512.f },
					{ scopeState.x + scopeState.xScale,scopeState.y + scopeState.yScale,1.f,1.f,0xFFFFFFFF,256 / 512.f,306 / 512.f},
					{ scopeState.x - scopeState.xScale,scopeState.y + scopeState.yScale,1.f,1.f,0xFFFFFFFF,0.f / 512,306 / 512.f}
				};

				g_pD3dDevice->SetTexture(0, g_pTexture[g_bulletTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Scope, sizeof(CustomVertex));
			}

			if (0 < g_itemData.count[g_bulletItem] && g_itemData.count[g_bulletItem] < 181)
			{
				ImageState crackState = { 395.f,0.f,50.f / 2,50.f / 2 };

				int countBuf = (6 < g_itemData.count[g_bulletItem]) ? 6 : g_itemData.count[g_bulletItem];

				crackState.x = 395.f + 30 * (g_itemData.posYX[1]);
				crackState.y = 85.f + 30 * (g_itemData.posYX[0] - g_deletedLineCount - 4);

				CustomVertex cusV4crack[4] =
				{
					{ crackState.x - crackState.xScale,crackState.y - crackState.yScale,1.f,1.f,0xFFFFFFFF,75.f / 512,380 / 512.f },
					{ crackState.x + crackState.xScale,crackState.y - crackState.yScale,1.f,1.f,0xFFFFFFFF,125 / 512.f,380 / 512.f },
					{ crackState.x + crackState.xScale,crackState.y + crackState.yScale,1.f,1.f,0xFFFFFFFF,125 / 512.f,430 / 512.f },
					{ crackState.x - crackState.xScale,crackState.y + crackState.yScale,1.f,1.f,0xFFFFFFFF,75.f / 512,430 / 512.f }
				};

				cusV4crack[0].x += -25.f * ((countBuf) / 2);
				cusV4crack[0].y += -25.f * ((countBuf) / 2);
				cusV4crack[1].x += 25.f * ((countBuf) / 2);
				cusV4crack[1].y += -25.f * ((countBuf) / 2);
				cusV4crack[2].x += 25.f * ((countBuf) / 2);
				cusV4crack[2].y += 25.f * ((countBuf) / 2);
				cusV4crack[3].x += -25.f * ((countBuf) / 2);
				cusV4crack[3].y += 25.f * ((countBuf) / 2);

				cusV4crack[0].tu += -25 * (countBuf / 2) / 512.f;
				cusV4crack[0].tv += -25 * (countBuf / 2) / 512.f;
				cusV4crack[1].tu += 25 * (countBuf / 2) / 512.f;
				cusV4crack[1].tv += -25 * (countBuf / 2) / 512.f;
				cusV4crack[2].tu += 25 * (countBuf / 2) / 512.f;
				cusV4crack[2].tv += 25 * (countBuf / 2) / 512.f;
				cusV4crack[3].tu += -25 * (countBuf / 2) / 512.f;
				cusV4crack[3].tv += 25* (countBuf / 2) / 512.f;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_bulletTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4crack, sizeof(CustomVertex));
			}

			if (49<g_itemData.count[g_bulletItem])
			{
				ImageState cartridgeState = { 0.f,0.f,50.f / 2,50.f / 2 };

				CustomVertex cusV4Cartridge[4] =
				{
					{ cartridgeState.x - cartridgeState.xScale,cartridgeState.y - cartridgeState.yScale,1.f,1.f,0xFFFFFFFF,50.f / 512,0 / 512.f },
					{ cartridgeState.x + cartridgeState.xScale,cartridgeState.y - cartridgeState.yScale,1.f,1.f,0xFFFFFFFF,100 / 512.f,0 / 512.f },
					{ cartridgeState.x + cartridgeState.xScale,cartridgeState.y + cartridgeState.yScale,1.f,1.f,0xFFFFFFFF,100 / 512.f,50 / 512.f },
					{ cartridgeState.x - cartridgeState.xScale,cartridgeState.y + cartridgeState.yScale,1.f,1.f,0xFFFFFFFF,50.f / 512,50 / 512.f }
				};

				CustomVertex rotateCusV4Cartridge[4];

				const INT grand = 87;
				static FLOAT grandPos = 0;

				if (g_itemData.count[g_bulletItem] == grand)
				{
					accelY = 0;
				}

				if (g_itemData.count[g_bulletItem] >= grand)
				{
					RotateTexDeg(rotateCusV4Cartridge, cusV4Cartridge, -6.f * g_itemData.count[g_bulletItem], 600.f-(g_itemData.count[g_bulletItem]-grand), grandPos +(-6.f+accelY)*(g_itemData.count[g_bulletItem]-grand));
				}

				else
				{
					RotateTexDeg(rotateCusV4Cartridge, cusV4Cartridge, 6.f * g_itemData.count[g_bulletItem], 600.f, (4.f+accelY) * g_itemData.count[g_bulletItem]);

					grandPos = rotateCusV4Cartridge->y;
				}
				
				g_pD3dDevice->SetTexture(0, g_pTexture[g_bulletTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, rotateCusV4Cartridge, sizeof(CustomVertex));

				accelY += 0.1f;
			}
		}
		}
	}

	return;
}

VOID SetHardDropEffectTextureAndRender(VOID)
{
	static INT hardDropEffectCount = 0;
	static MovMinoNumoOfArBuf hardDropNumofArBuf;
	static INT InitDeflecton = false;
	static BOOL installatedBlock[4] = { 0,0,0,0 };

	typedef struct
	{
		INT deflectionalPosX;
		INT deflectionalInitialVelocityY;
		INT deflectionalInitialVelocityX;
	}ParticleDeflection;

	static ParticleDeflection particleDeflection[12 * 4];

	if (g_hardDropedTetmino)
	{
		InitDeflecton = true;
	}

	if (InitDeflecton&&hardDropEffectCount == 0)
	{
		memcpy(&hardDropNumofArBuf, &g_movMinoNumOfArBuf, sizeof(MovMinoNumoOfArBuf));

		for (INT effect = 0; effect < 12 * 4; effect++)
		{
			(particleDeflection + effect)->deflectionalPosX = rand() % 30;
			(particleDeflection + effect)->deflectionalInitialVelocityY = rand() % 15 + 1;
			(particleDeflection + effect)->deflectionalInitialVelocityX = rand() % 15 - 7;
		}

		if (g_tetlisBoardBuf[hardDropNumofArBuf.YX[0][0] + 1][hardDropNumofArBuf.YX[0][1]] != -1)
		{
			installatedBlock[0] = true;
		}

		if (g_tetlisBoardBuf[hardDropNumofArBuf.YX[1][0] + 1][hardDropNumofArBuf.YX[1][1]] != -1)
		{
			installatedBlock[1] = true;
		}

		if (g_tetlisBoardBuf[hardDropNumofArBuf.YX[2][0] + 1][hardDropNumofArBuf.YX[2][1]] != -1)
		{
			installatedBlock[2] = true;
		}

		if (g_tetlisBoardBuf[hardDropNumofArBuf.YX[3][0] + 1][hardDropNumofArBuf.YX[3][1]] != -1)
		{
			installatedBlock[3] = true;
		}

		hardDropEffectCount++;
	}

	if (hardDropEffectCount)
	{
		CustomVertex CusV3HardDropParticleEffect[3] =
		{
			{ 0.f,0.f,1.f,1.f,0xFFFFFFFF,0.5f,0.f },
			{ 0.f,0.f,1.f,1.f,0xFFFFFFFF,1.f,1.f },
			{ 0.f,0.f,1.f,1.f,0xFFFFFFFF,0.f,1.f }
		};

		static FLOAT accelY = 0;

		InitDeflecton = false;

		for (INT effect = 0; effect < 12; effect++)
		{
			if (installatedBlock[0] == 1)
			{
				(CusV3HardDropParticleEffect)->x = 395.f + hardDropNumofArBuf.YX[0][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 4 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect)->y = -35.f + (hardDropNumofArBuf.YX[0][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				(CusV3HardDropParticleEffect + 1)->x = 395.f + hardDropNumofArBuf.YX[0][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 8 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect + 1)->y = -35.f + (hardDropNumofArBuf.YX[0][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				(CusV3HardDropParticleEffect + 2)->x = 395.f + hardDropNumofArBuf.YX[0][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 0 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect + 2)->y = -35.f + (hardDropNumofArBuf.YX[0][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));


				DWORD randomColor = (0xFF000000) + ((rand() % 256) * 0x10000) + ((rand() % 256) * 0x100) + (rand() % 256);

				CusV3HardDropParticleEffect->color = randomColor;
				(CusV3HardDropParticleEffect + 1)->color = randomColor;
				(CusV3HardDropParticleEffect + 2)->color = randomColor;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_hardDropEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, CusV3HardDropParticleEffect, sizeof(CustomVertex));
			}
		}

		for (INT effect = 12; effect < 24; effect++)
		{
			if (installatedBlock[1] == 1)
			{
				(CusV3HardDropParticleEffect)->x = 380.f + hardDropNumofArBuf.YX[1][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 4 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect)->y = -35.f + (hardDropNumofArBuf.YX[1][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				(CusV3HardDropParticleEffect + 1)->x = 380.f + hardDropNumofArBuf.YX[1][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 8 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect + 1)->y = -35.f + (hardDropNumofArBuf.YX[1][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				(CusV3HardDropParticleEffect + 2)->x = 380.f + hardDropNumofArBuf.YX[1][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect + 2)->y = -35.f + (hardDropNumofArBuf.YX[1][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				DWORD randomColor = (0xFF000000) + ((rand() % 256) * 0x10000) + ((rand() % 256) * 0x100) + (rand() % 256);

				CusV3HardDropParticleEffect->color = randomColor;
				(CusV3HardDropParticleEffect + 1)->color = randomColor;
				(CusV3HardDropParticleEffect + 2)->color = randomColor;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_hardDropEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, CusV3HardDropParticleEffect, sizeof(CustomVertex));
			}
		}

		for (INT effect = 24; effect < 36; effect++)
		{
			if (installatedBlock[2] == 1)
			{
				(CusV3HardDropParticleEffect)->x = 380.f + hardDropNumofArBuf.YX[2][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 4 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect)->y = -35.f + (hardDropNumofArBuf.YX[2][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				(CusV3HardDropParticleEffect + 1)->x = 380.f + hardDropNumofArBuf.YX[2][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 8 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect + 1)->y = -35.f + (hardDropNumofArBuf.YX[2][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				(CusV3HardDropParticleEffect + 2)->x = 380.f + hardDropNumofArBuf.YX[2][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect + 2)->y = -35.f + (hardDropNumofArBuf.YX[2][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				DWORD randomColor = (0xFF000000) + ((rand() % 256) * 0x10000) + ((rand() % 256) * 0x100) + (rand() % 256);

				CusV3HardDropParticleEffect->color = randomColor;
				(CusV3HardDropParticleEffect + 1)->color = randomColor;
				(CusV3HardDropParticleEffect + 2)->color = randomColor;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_hardDropEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, CusV3HardDropParticleEffect, sizeof(CustomVertex));
			}
		}

		for (INT effect = 36; effect < 48; effect++)
		{
			if (installatedBlock[3] == 1)
			{
				(CusV3HardDropParticleEffect)->x = 380.f + hardDropNumofArBuf.YX[3][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 4 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect)->y = -35.f + (hardDropNumofArBuf.YX[3][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				(CusV3HardDropParticleEffect + 1)->x = 380.f + hardDropNumofArBuf.YX[3][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + 8 + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect + 1)->y = -35.f + (hardDropNumofArBuf.YX[3][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				(CusV3HardDropParticleEffect + 2)->x = 380.f + hardDropNumofArBuf.YX[3][1] * (g_tetminoState.xScale * 2) + particleDeflection[effect].deflectionalPosX + (particleDeflection[effect].deflectionalInitialVelocityX*(hardDropEffectCount - 1));
				(CusV3HardDropParticleEffect + 2)->y = -35.f + (hardDropNumofArBuf.YX[3][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + 7 + ((-particleDeflection[effect].deflectionalInitialVelocityY + accelY)*(hardDropEffectCount - 1));

				DWORD randomColor = (0xFF000000) + ((rand() % 256) * 0x10000) + ((rand() % 256) * 0x100) + (rand() % 256);

				CusV3HardDropParticleEffect->color = randomColor;
				(CusV3HardDropParticleEffect + 1)->color = randomColor;
				(CusV3HardDropParticleEffect + 2)->color = randomColor;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_hardDropEffectTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, CusV3HardDropParticleEffect, sizeof(CustomVertex));
			}
		}

		g_hardDropedTetmino = false;
		hardDropEffectCount++;
		accelY += 0.7f;

		if (hardDropEffectCount > 25)
		{
			hardDropEffectCount = 0;
			accelY = 0;
			installatedBlock[0] = false;
			installatedBlock[1] = false;
			installatedBlock[2] = false;
			installatedBlock[3] = false;
		}
	}

	return;
}

VOID SetBlockVerticesAndRender(VOID)
{
	static INT swellingUp = 0;
	static INT swellingUpCount = 0;
	static INT prevDeletedLineCount = 0;
	static INT differenceDeletedLineCount = 0;

	INT deletedLineCount = 0;

	for (INT column = 19; column < TETLIS_HEIGHT - 2; column++)
	{
		INT numOfDirt = 0;
		for (INT row = 1; row < TETLIS_WIDTH - 2; row++)
		{
			if (g_tetlisBoardBuf[column][row] % 100 == 10)
			{
				numOfDirt++;
			}
		}

		if (numOfDirt < 4)
		{
			deletedLineCount++;
		}
	}

	if (!(g_deletedLine))
	{
		prevDeletedLineCount = deletedLineCount;
	}

	if (g_deletedLine && (swellingUpCount == 0))
	{
		differenceDeletedLineCount = deletedLineCount - prevDeletedLineCount;
	}

	if (g_deletedLine)
	{
		swellingUpCount++;
		if ((swellingUpCount > 30) && (swellingUp < differenceDeletedLineCount * 30))
		{
			swellingUp += 15;
		}

		if (swellingUpCount == 60)
		{
			swellingUp = 0;
		}
	}

	else
	{
		swellingUpCount = 0;
	}

	static BOOL canRessetSwellingUp = false;

	if(g_itemData.count[g_laserCannonItem]>80)
	{
		swellingUp += 15;

		if (swellingUp>30*g_itemData.swellingUpCount[g_laserCannonItem])
		{
			swellingUp = 30 * g_itemData.swellingUpCount[g_laserCannonItem];
		}
	}

	if (g_itemData.count[g_ultraDrillItem]>450)
	{
		swellingUp += 15;

		if (swellingUp>30 * g_itemData.swellingUpCount[g_ultraDrillItem])
		{
			swellingUp = 30 * g_itemData.swellingUpCount[g_ultraDrillItem];
		}
	}

	if (canRessetSwellingUp)
	{
		swellingUp = 0; 
		canRessetSwellingUp = false;
	}
	
	if (g_itemData.count[g_laserCannonItem] == 111)
	{
		canRessetSwellingUp = true;
		g_itemData.swellingUpCount[g_laserCannonItem] = 0;
	}

	if (g_itemData.count[g_ultraDrillItem] == 480)
	{
		canRessetSwellingUp = true;
		g_itemData.swellingUpCount[g_ultraDrillItem] = 0;
	}

	for (int column = 4; column < TETLIS_HEIGHT; column++)
	{
		for (int row = 0; row < TETLIS_WIDTH; row++)
		{
			CustomVertex cusV4Tetmino[4] =
			{
				{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
				{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
				{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
				{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
			};

			if ((g_tetlisBoard[column][row] != -1) && (g_tetlisBoard[column][row] != 9))
			{
				cusV4Tetmino[0].x = 395.f + row * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[0].y = -35.f - swellingUp + (column - g_deletedLineCount) * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[1].x = 395.f + row * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[1].y = -35.f - swellingUp + (column - g_deletedLineCount) * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[2].x = 395.f + row * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[2].y = -35.f - swellingUp + (column - g_deletedLineCount) * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				cusV4Tetmino[3].x = 395.f + row * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[3].y = -35.f - swellingUp + (column - g_deletedLineCount) * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

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
					cusV4Tetmino[0].tu = 101.f / 512;
					cusV4Tetmino[0].tv = 51.f / 256;
					cusV4Tetmino[1].tu = 150.f / 512;
					cusV4Tetmino[1].tv = 51.f / 256;
					cusV4Tetmino[2].tu = 150.f / 512;
					cusV4Tetmino[2].tv = 97.f / 256;
					cusV4Tetmino[3].tu = 101.f / 512;
					cusV4Tetmino[3].tv = 97.f / 256;
					break;
				case 21:
					cusV4Tetmino[0].tu = 51.f / 512;
					cusV4Tetmino[0].tv = 51.f / 256;
					cusV4Tetmino[1].tu = 100.f / 512;
					cusV4Tetmino[1].tv = 51.f / 256;
					cusV4Tetmino[2].tu = 100.f / 512;
					cusV4Tetmino[2].tv = 97.f / 256;
					cusV4Tetmino[3].tu = 51.f / 512;
					cusV4Tetmino[3].tv = 97.f / 256;
					break;
				case 20:
					cusV4Tetmino[0].tu = 51.f / 512;
					cusV4Tetmino[0].tv = 51.f / 256;
					cusV4Tetmino[1].tu = 100.f / 512;
					cusV4Tetmino[1].tv = 51.f / 256;
					cusV4Tetmino[2].tu = 100.f / 512;
					cusV4Tetmino[2].tv = 97.f / 256;
					cusV4Tetmino[3].tu = 51.f / 512;
					cusV4Tetmino[3].tv = 97.f / 256;
					break;
				case 32:
					cusV4Tetmino[0].tu = 0.f;
					cusV4Tetmino[0].tv = 51.f / 256;
					cusV4Tetmino[1].tu = 50.f / 512;
					cusV4Tetmino[1].tv = 51.f / 256;
					cusV4Tetmino[2].tu = 50.f / 512;
					cusV4Tetmino[2].tv = 97.f / 256;
					cusV4Tetmino[3].tu = 0.f;
					cusV4Tetmino[3].tv = 97.f / 256;
					break;
				case 31:
					cusV4Tetmino[0].tu = 0.f;
					cusV4Tetmino[0].tv = 51.f / 256;
					cusV4Tetmino[1].tu = 50.f / 512;
					cusV4Tetmino[1].tv = 51.f / 256;
					cusV4Tetmino[2].tu = 50.f / 512;
					cusV4Tetmino[2].tv = 97.f / 256;
					cusV4Tetmino[3].tu = 0.f;
					cusV4Tetmino[3].tv = 97.f / 256;
					break;
				case 30:
					cusV4Tetmino[0].tu = 0.f;
					cusV4Tetmino[0].tv = 51.f / 256;
					cusV4Tetmino[1].tu = 50.f / 512;
					cusV4Tetmino[1].tv = 51.f / 256;
					cusV4Tetmino[2].tu = 50.f / 512;
					cusV4Tetmino[2].tv = 97.f / 256;
					cusV4Tetmino[3].tu = 0.f;
					cusV4Tetmino[3].tv = 97.f / 256;
					break;
				case 60:
					cusV4Tetmino[0].tu = 151.f / 512;
					cusV4Tetmino[0].tv = 45.f / 256;
					cusV4Tetmino[1].tu = 201.f / 512;
					cusV4Tetmino[1].tv = 45.f / 256;
					cusV4Tetmino[2].tu = 201.f / 512;
					cusV4Tetmino[2].tv = 96.f / 256;
					cusV4Tetmino[3].tu = 151.f / 512;
					cusV4Tetmino[3].tv = 96.f / 256;
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
	CustomVertex cusV4Tetmino[4] =
	{
		{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 2 / 512.f, 152 / 256.f },
		{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y - g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 50 / 512.f, 152 / 256.f },
		{ g_tetminoState.x + g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 50 / 512.f, 199 / 256.f },
		{ g_tetminoState.x - g_tetminoState.xScale, g_tetminoState.y + g_tetminoState.yScale, 1.f, 1.f, 0xFFFFFFFF, 2 / 512.f, 199 / 256.f }
	};

	////////////////////////////////////////////////////
	//flameCountとマスクを用いアルファ値を書き換えている
	UnderGoChangeTarAlpha(cusV4Tetmino);

	for (INT block = 0; block < 4; block++)
	{
		cusV4Tetmino[0].x = 2 + 395.f + g_targetMinoNumOfArBuf.YX[block][1] * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
		cusV4Tetmino[0].y = 2 + -35.f + (g_targetMinoNumOfArBuf.YX[block][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
		cusV4Tetmino[1].x = -2 + 395.f + g_targetMinoNumOfArBuf.YX[block][1] * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
		cusV4Tetmino[1].y = 2 + -35.f + (g_targetMinoNumOfArBuf.YX[block][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
		cusV4Tetmino[2].x = -2 + 395.f + g_targetMinoNumOfArBuf.YX[block][1] * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
		cusV4Tetmino[2].y = -2 + -35.f + (g_targetMinoNumOfArBuf.YX[block][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
		cusV4Tetmino[3].x = 2 + 395.f + g_targetMinoNumOfArBuf.YX[block][1] * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
		cusV4Tetmino[3].y = -2 + -35.f + (g_targetMinoNumOfArBuf.YX[block][0] - g_deletedLineCount) * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

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

			(cusV4Tetmino + coordinate)->color &= 0x00ffffff;
			(cusV4Tetmino + coordinate)->color += alpha * 0x1000000;
		}
	}

	else
	{
		for (INT coordinate = 0; coordinate < 4; coordinate++)
		{
			DWORD alpha = 4 * frameCount;

			(cusV4Tetmino + coordinate)->color &= 0x00ffffff;
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

VOID SetDeletedLineEffectTextureAndRender(VOID)
{
	static INT renderEffectCount = 0;

	if (g_deletedLine && (renderEffectCount < 30))
	{
		INT additionalDeletedEffectScale = 0;
		static INT deletedLineEffectCount = 0;

		CustomVertex cusV4DeletedLineEffect[4] =
		{

			{ g_deletedLineEffectState.x - g_deletedLineEffectState.xScale, g_deletedLineEffectState.y - g_deletedLineEffectState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
			{ g_deletedLineEffectState.x + g_deletedLineEffectState.xScale, g_deletedLineEffectState.y - g_deletedLineEffectState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
			{ g_deletedLineEffectState.x + g_deletedLineEffectState.xScale, g_deletedLineEffectState.y + g_deletedLineEffectState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
			{ g_deletedLineEffectState.x - g_deletedLineEffectState.xScale, g_deletedLineEffectState.y + g_deletedLineEffectState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
		};

		///////////////////////////////////////////////////
		//カウントを用いアニメーションを行うための設定を行う
		CountToAnimation(&additionalDeletedEffectScale, &deletedLineEffectCount, cusV4DeletedLineEffect);

		for (INT column = 0; column < TETLIS_HEIGHT; column++)
		{
			if (g_reduceBlockDurPosition[column][1] ||
				g_reduceBlockDurPosition[column][2] ||
				g_reduceBlockDurPosition[column][3] ||
				g_reduceBlockDurPosition[column][4] ||
				g_reduceBlockDurPosition[column][5] ||
				g_reduceBlockDurPosition[column][6] ||
				g_reduceBlockDurPosition[column][7] ||
				g_reduceBlockDurPosition[column][8] ||
				g_reduceBlockDurPosition[column][9] ||
				g_reduceBlockDurPosition[column][10])
			{
				g_deletedLineEffectState.xScale = 15.f + additionalDeletedEffectScale;

				cusV4DeletedLineEffect[0].x = 560.f - g_deletedLineEffectState.xScale;
				cusV4DeletedLineEffect[0].y = -35.f + (column - g_deletedLineCount) * (g_deletedLineEffectState.yScale * 2) - g_deletedLineEffectState.yScale;
				cusV4DeletedLineEffect[1].x = 560.f + g_deletedLineEffectState.xScale;
				cusV4DeletedLineEffect[1].y = -35.f + (column - g_deletedLineCount) * (g_deletedLineEffectState.yScale * 2) - g_deletedLineEffectState.yScale;
				cusV4DeletedLineEffect[2].x = 560.f + g_deletedLineEffectState.xScale;
				cusV4DeletedLineEffect[2].y = -35.f + (column - g_deletedLineCount) * (g_deletedLineEffectState.yScale * 2) + g_deletedLineEffectState.yScale;
				cusV4DeletedLineEffect[3].x = 560.f - g_deletedLineEffectState.xScale;
				cusV4DeletedLineEffect[3].y = -35.f + (column - g_deletedLineCount) * (g_deletedLineEffectState.yScale * 2) + g_deletedLineEffectState.yScale;

				g_pD3dDevice->SetTexture(0, g_pTexture[g_reduceBlockDurTex]);
				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4DeletedLineEffect, sizeof(CustomVertex));
			}
		}

		CustomVertex cusV4DeletedCirculeEffect[4] =
		{
			{ g_deletedLineEffectState.x - g_deletedLineEffectState.xScale, g_deletedLineEffectState.y - g_deletedLineEffectState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
			{ g_deletedLineEffectState.x + g_deletedLineEffectState.xScale, g_deletedLineEffectState.y - g_deletedLineEffectState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
			{ g_deletedLineEffectState.x + g_deletedLineEffectState.xScale, g_deletedLineEffectState.y + g_deletedLineEffectState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
			{ g_deletedLineEffectState.x - g_deletedLineEffectState.xScale, g_deletedLineEffectState.y + g_deletedLineEffectState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
		};

		typedef struct
		{
			INT effectDeflectionalScale[16];
			INT effectDeflectionalPosX[16];
			INT effectDeflectionalPosY[16];
			INT effectDeflectionalDegree[16];
			INT effectDeflectionalAppearanceTime[16];
		}EffectDeflection;

		static EffectDeflection effectDeflection[11];

		static INT isFirstFrame, circuleEffectCount = 0;

		if (!circuleEffectCount)
		{
			isFirstFrame = true;
		}

		else
		{
			isFirstFrame = false;
		}

		if (isFirstFrame)
		{
			for (INT line = 0; line < 11; line++)
			{
				for (INT effect = 0; effect < 16; effect++)
				{
					(effectDeflection + line)->effectDeflectionalScale[effect] = rand() % 7 + 10;
					(effectDeflection + line)->effectDeflectionalPosX[effect] = rand() % 290;
					(effectDeflection + line)->effectDeflectionalPosY[effect] = rand() % 20;
					(effectDeflection + line)->effectDeflectionalDegree[effect] = rand() % 180;
					(effectDeflection + line)->effectDeflectionalAppearanceTime[effect] = rand() % 12;
				}
			}

			isFirstFrame = false;
		}

		INT line = 0;

		for (INT column = 0; column < TETLIS_HEIGHT; column++)
		{
			if (g_reduceBlockDurPosition[column][1] ||
				g_reduceBlockDurPosition[column][2] ||
				g_reduceBlockDurPosition[column][3] ||
				g_reduceBlockDurPosition[column][4] ||
				g_reduceBlockDurPosition[column][5] ||
				g_reduceBlockDurPosition[column][6] ||
				g_reduceBlockDurPosition[column][7] ||
				g_reduceBlockDurPosition[column][8] ||
				g_reduceBlockDurPosition[column][9] ||
				g_reduceBlockDurPosition[column][10])
			{
				for (INT effect = 0; effect < 16; effect++)
				{
					cusV4DeletedCirculeEffect->x = 410 + 5.f + (effectDeflection + line)->effectDeflectionalPosX[effect] + (-(effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)cos((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180) - (-(effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)sin((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180);
					cusV4DeletedCirculeEffect->y = -50 + 5.f + (column - g_deletedLineCount) * (g_deletedLineEffectState.yScale * 2) + (effectDeflection + line)->effectDeflectionalPosY[effect] + (-(effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)sin((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180) + (-(effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)cos((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180);
					(cusV4DeletedCirculeEffect + 1)->x = 410 + 5.f + (effectDeflection + line)->effectDeflectionalPosX[effect] + ((effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)cos((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180) - (-(effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)sin((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180);
					(cusV4DeletedCirculeEffect + 1)->y = -50 + 5.f + (column - g_deletedLineCount) * (g_deletedLineEffectState.yScale * 2) + (effectDeflection + line)->effectDeflectionalPosY[effect] + ((effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)sin((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180) + (-(effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)cos((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180);
					(cusV4DeletedCirculeEffect + 2)->x = 410 + 5.f + (effectDeflection + line)->effectDeflectionalPosX[effect] + ((effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)cos((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180) - ((effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)sin((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180);
					(cusV4DeletedCirculeEffect + 2)->y = -50 + 5.f + (column - g_deletedLineCount) * (g_deletedLineEffectState.yScale * 2) + (effectDeflection + line)->effectDeflectionalPosY[effect] + ((effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)sin((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180) + ((effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)cos((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180);
					(cusV4DeletedCirculeEffect + 3)->x = 410 + 5.f + (effectDeflection + line)->effectDeflectionalPosX[effect] + (-(effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)cos((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180) - ((effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)sin((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180);
					(cusV4DeletedCirculeEffect + 3)->y = -50 + 5.f + (column - g_deletedLineCount) * (g_deletedLineEffectState.yScale * 2) + (effectDeflection + line)->effectDeflectionalPosY[effect] + (-(effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)sin((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180) + ((effectDeflection + line)->effectDeflectionalScale[effect])*(FLOAT)cos((effectDeflection + line)->effectDeflectionalDegree[effect] * PI / 180);

					DWORD randomColor = (0xFF000000) + ((rand() % 256) * 0x10000) + ((rand() % 256) * 0x100) + (rand() % 256);

					cusV4DeletedCirculeEffect->color = randomColor;
					(cusV4DeletedCirculeEffect + 1)->color = randomColor;
					(cusV4DeletedCirculeEffect + 2)->color = randomColor;
					(cusV4DeletedCirculeEffect + 3)->color = randomColor;

					INT timeFromAppearance = (circuleEffectCount - (effectDeflection + line)->effectDeflectionalAppearanceTime[effect]);

					if ((0 <= timeFromAppearance) && (timeFromAppearance <= 17))
					{
						cusV4DeletedCirculeEffect->tu = ((timeFromAppearance / 3) * 64) / 512.f;
						(cusV4DeletedCirculeEffect + 1)->tu = (((timeFromAppearance / 3 + 1) * 64) - 1) / 512.f;
						(cusV4DeletedCirculeEffect + 2)->tu = (((timeFromAppearance / 3 + 1) * 64) - 1) / 512.f;
						(cusV4DeletedCirculeEffect + 3)->tu = ((timeFromAppearance / 3) * 64) / 512.f;
						g_pD3dDevice->SetTexture(0, g_pTexture[g_circuleEffectTex]);
						g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4DeletedCirculeEffect, sizeof(CustomVertex));
					}
				}
				line++;
			}
		}

		circuleEffectCount++;
		if (circuleEffectCount == 30)
		{
			circuleEffectCount = 0;
			isFirstFrame = true;
		}
	}

	if (g_deletedLine)
	{
		renderEffectCount++;
	}

	else
	{
		renderEffectCount = 0;
	}

	return;
}

VOID CountToAnimation(INT *additionalDeletedEffectScale, INT *deletedLineEffectCount, CustomVertex *cusV4DeletedLineEffect)
{
	if (g_deletedLine)
	{
		*additionalDeletedEffectScale = 10 * (*deletedLineEffectCount);

		if ((*additionalDeletedEffectScale) > 150)
		{
			*additionalDeletedEffectScale = 150;
		}

		for (INT coordinate = 0; coordinate < 4; coordinate++)
		{
			(cusV4DeletedLineEffect + coordinate)->color &= 0x00FFFFFF;
			(cusV4DeletedLineEffect + coordinate)->color += (0xFF - 8 * (*deletedLineEffectCount)) << 24;
		}

		*deletedLineEffectCount += 1;
		if (*deletedLineEffectCount == 30)
		{
			*additionalDeletedEffectScale = 0;
			*deletedLineEffectCount = 0;
		}
	}

	return;
}

VOID RnderFrame(VOID)
{
	CustomVertex cusV4Frame[4] =
	{
		{ 0.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ 1280.f,   0.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ 1280.f, 720.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ 0.f, 720.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	g_pD3dDevice->SetTexture(0, g_pTexture[g_frameTex]);
	g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Frame, sizeof(CustomVertex));

	return;
}

VOID RenderGameoverStr(VOID)
{
	RECT rectGameoverStr
	{
		(LONG)(g_gameoverStrState.x - g_gameoverStrState.xScale), (LONG)(g_gameoverStrState.y - g_gameoverStrState.yScale), (LONG)(g_gameoverStrState.x + g_gameoverStrState.xScale), (LONG)(g_gameoverStrState.y + g_gameoverStrState.yScale)
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

VOID RenderUndergroundStr(VOID)
{
	RECT rectUndergroundStr
	{
		(LONG)(g_undergroundStrState.x - g_undergroundStrState.xScale), (LONG)(g_undergroundStrState.y - g_undergroundStrState.yScale), (LONG)(g_undergroundStrState.x + g_undergroundStrState.xScale), (LONG)(g_undergroundStrState.y + g_undergroundStrState.yScale)
	};

	sprintf(g_undergroundArray, "%d", g_deletedLineOverall);
	g_pFont[g_undergroundFont]->DrawText(NULL, g_undergroundArray, -1, &rectUndergroundStr, DT_RIGHT, GAMEOVER_COLOR);

	return;
}

VOID SetHoldNextNextNextVerticesAndRender(VOID)
{
	CustomVertex cusV4Tetmino[4] =
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
				cusV4Tetmino[0].x = 235.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[0].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[1].x = 235.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[1].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[2].x = 235.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[2].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				cusV4Tetmino[3].x = 235.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[3].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

				switch (g_holdBoard[coordinateY][coordinateX] % 100)
				{
				case 0:
					cusV4Tetmino[0].x = 250.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
					cusV4Tetmino[0].y = 85.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
					cusV4Tetmino[1].x = 250.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
					cusV4Tetmino[1].y = 85.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
					cusV4Tetmino[2].x = 250.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
					cusV4Tetmino[2].y = 85.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
					cusV4Tetmino[3].x = 250.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
					cusV4Tetmino[3].y = 85.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

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
					cusV4Tetmino[0].x = 250.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
					cusV4Tetmino[0].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
					cusV4Tetmino[1].x = 250.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
					cusV4Tetmino[1].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
					cusV4Tetmino[2].x = 250.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
					cusV4Tetmino[2].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
					cusV4Tetmino[3].x = 250.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
					cusV4Tetmino[3].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoOTex]);
					break;
				}

				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}

			if (g_nextBoard[coordinateY][coordinateX] != -1)
			{
				cusV4Tetmino[0].x = 790.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[0].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[1].x = 790.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[1].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
				cusV4Tetmino[2].x = 790.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
				cusV4Tetmino[2].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
				cusV4Tetmino[3].x = 790.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
				cusV4Tetmino[3].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

				switch (g_nextBoard[coordinateY][coordinateX] % 100)
				{
				case 0:
					cusV4Tetmino[0].x = 805.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
					cusV4Tetmino[0].y = 85.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
					cusV4Tetmino[1].x = 805.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
					cusV4Tetmino[1].y = 85.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
					cusV4Tetmino[2].x = 805.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
					cusV4Tetmino[2].y = 85.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
					cusV4Tetmino[3].x = 805.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
					cusV4Tetmino[3].y = 85.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

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
					cusV4Tetmino[0].x = 805.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
					cusV4Tetmino[0].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
					cusV4Tetmino[1].x = 805.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
					cusV4Tetmino[1].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) - g_tetminoState.yScale;
					cusV4Tetmino[2].x = 805.f + coordinateX * (g_tetminoState.xScale * 2) + g_tetminoState.xScale;
					cusV4Tetmino[2].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;
					cusV4Tetmino[3].x = 805.f + coordinateX * (g_tetminoState.xScale * 2) - g_tetminoState.xScale;
					cusV4Tetmino[3].y = 70.f + coordinateY * (g_tetminoState.yScale * 2) + g_tetminoState.yScale;

					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoOTex]);
					break;
				}

				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Tetmino, sizeof(CustomVertex));
			}

			ImageState tetminoNextNextState = { 0.f, 0.f, 26.f / 2, 26.f / 2 };

			CustomVertex cusV4TetminoNextNext[4]
			{
				{ tetminoNextNextState.x - tetminoNextNextState.xScale, tetminoNextNextState.y - tetminoNextNextState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
				{ tetminoNextNextState.x + tetminoNextNextState.xScale, tetminoNextNextState.y - tetminoNextNextState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
				{ tetminoNextNextState.x + tetminoNextNextState.xScale, tetminoNextNextState.y + tetminoNextNextState.yScale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
				{ tetminoNextNextState.x - tetminoNextNextState.xScale, tetminoNextNextState.y + tetminoNextNextState.yScale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
			};

			if (g_nextNextBoard[coordinateY][coordinateX] != -1)
			{
				cusV4TetminoNextNext[0].x = 800.f + coordinateX * (tetminoNextNextState.xScale * 2) - tetminoNextNextState.xScale;
				cusV4TetminoNextNext[0].y = 285.f + coordinateY * (tetminoNextNextState.yScale * 2) - tetminoNextNextState.yScale;
				cusV4TetminoNextNext[1].x = 800.f + coordinateX * (tetminoNextNextState.xScale * 2) + tetminoNextNextState.xScale;
				cusV4TetminoNextNext[1].y = 285.f + coordinateY * (tetminoNextNextState.yScale * 2) - tetminoNextNextState.yScale;
				cusV4TetminoNextNext[2].x = 800.f + coordinateX * (tetminoNextNextState.xScale * 2) + tetminoNextNextState.xScale;
				cusV4TetminoNextNext[2].y = 285.f + coordinateY * (tetminoNextNextState.yScale * 2) + tetminoNextNextState.yScale;
				cusV4TetminoNextNext[3].x = 800.f + coordinateX * (tetminoNextNextState.xScale * 2) - tetminoNextNextState.xScale;
				cusV4TetminoNextNext[3].y = 285.f + coordinateY * (tetminoNextNextState.yScale * 2) + tetminoNextNextState.yScale;

				switch (g_nextNextBoard[coordinateY][coordinateX] % 10)
				{
				case 0:
					cusV4TetminoNextNext[0].x = 810.f + coordinateX * (tetminoNextNextState.xScale * 2) - tetminoNextNextState.xScale;
					cusV4TetminoNextNext[0].y = 302.f + coordinateY * (tetminoNextNextState.yScale * 2) - tetminoNextNextState.yScale;
					cusV4TetminoNextNext[1].x = 810.f + coordinateX * (tetminoNextNextState.xScale * 2) + tetminoNextNextState.xScale;
					cusV4TetminoNextNext[1].y = 302.f + coordinateY * (tetminoNextNextState.yScale * 2) - tetminoNextNextState.yScale;
					cusV4TetminoNextNext[2].x = 810.f + coordinateX * (tetminoNextNextState.xScale * 2) + tetminoNextNextState.xScale;
					cusV4TetminoNextNext[2].y = 302.f + coordinateY * (tetminoNextNextState.yScale * 2) + tetminoNextNextState.yScale;
					cusV4TetminoNextNext[3].x = 810.f + coordinateX * (tetminoNextNextState.xScale * 2) - tetminoNextNextState.xScale;
					cusV4TetminoNextNext[3].y = 302.f + coordinateY * (tetminoNextNextState.yScale * 2) + tetminoNextNextState.yScale;

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
					cusV4TetminoNextNext[0].x = 813.f + coordinateX * (tetminoNextNextState.xScale * 2) - tetminoNextNextState.xScale;
					cusV4TetminoNextNext[0].y = 285.f + coordinateY * (tetminoNextNextState.yScale * 2) - tetminoNextNextState.yScale;
					cusV4TetminoNextNext[1].x = 813.f + coordinateX * (tetminoNextNextState.xScale * 2) + tetminoNextNextState.xScale;
					cusV4TetminoNextNext[1].y = 285.f + coordinateY * (tetminoNextNextState.yScale * 2) - tetminoNextNextState.yScale;
					cusV4TetminoNextNext[2].x = 813.f + coordinateX * (tetminoNextNextState.xScale * 2) + tetminoNextNextState.xScale;
					cusV4TetminoNextNext[2].y = 285.f + coordinateY * (tetminoNextNextState.yScale * 2) + tetminoNextNextState.yScale;
					cusV4TetminoNextNext[3].x = 813.f + coordinateX * (tetminoNextNextState.xScale * 2) - tetminoNextNextState.xScale;
					cusV4TetminoNextNext[3].y = 285.f + coordinateY * (tetminoNextNextState.yScale * 2) + tetminoNextNextState.yScale;

					g_pD3dDevice->SetTexture(0, g_pTexture[g_tetminoOTex]);
					break;
				}

				g_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4TetminoNextNext, sizeof(CustomVertex));
			}
		}
	}

	return;
}

//////////////////////////////
//度数法によるテクスチャの回転
VOID RotateTexDeg(CustomVertex *dest, CustomVertex *src, FLOAT degree, FLOAT shiftX, FLOAT shiftY)
{
	memcpy(dest, src, sizeof(CustomVertex) * 4);

	double rad = (double)(degree * PI / 180);

	for (INT vertex = 0; vertex < 4; vertex++)
	{
		dest[vertex].x = src[vertex].x*(FLOAT)cos(rad) - src[vertex].y*(FLOAT)sin(rad) + shiftX;
		dest[vertex].y = src[vertex].x*(FLOAT)sin(rad) + src[vertex].y*(FLOAT)cos(rad) + shiftY;
	}

	return;
}

