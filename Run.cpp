#include "DxLib.h"
#include <stdlib.h>
#include <time.h>
enum { RED, YELLOW, BLUE, TRUCK };
const int CAR_MAX = 100;
const int CAR_W[] = { 32,26,26,40 };
const int CAR_H[] = { 48,48,48,100 };
int imgCar[4];
const int COM_MAX = CAR_MAX;
int computerX[COM_MAX];
int computerY[COM_MAX];
int computerType[COM_MAX];
// ポイント
int pointX = 0;
int pointY = -100;
int pointFlag = 0;
// 金
int goldX = 0;
int goldY = -100;
int goldFlag = 0;
// エメラルド
int EmerarudoX = 0;
int EmerarudoY = -100;
int EmerarudoFlag = 0;
// アメジスト
int amejisutoX = 0;
int amejisutoY = -100;
int amejisutoFlag = 0;
// ダイヤ
int diamondX = 0;
int diamondY = -100;
int diamondFlag = 0;
// Void
int VoidX = 0;
int VoidY = -100;
int VoidFlag = 0;
void drawCar(int x, int y, int type)
{
	DrawGraph(x - CAR_W[type] / 2,y - CAR_H[type] / 2,imgCar[type],TRUE);
}

// 文字描画
void drawText(int x,int y,int col,const char* txt,int val,int siz)
{
	SetFontSize(siz);
	DrawFormatString(x + 2,y + 2,0x000000,txt,val);
	DrawFormatString(x,y,col,txt,val);
}
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	srand((unsigned)time(NULL));
	const int WIDTH = 720;
	const int HEIGHT = 640;
	SetWindowText("Run");
	SetGraphMode(WIDTH, HEIGHT, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
		return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);
	// 画像読み込み
	int imgBG = LoadGraph("image/bg.png");
	imgCar[RED] = LoadGraph("image/car_red.png");
	imgCar[YELLOW] = LoadGraph("image/car_yellow.png");
	imgCar[BLUE] = LoadGraph("image/car_blue.png");
	imgCar[TRUCK] = LoadGraph("image/truck.png");
	int imgPlayer = LoadGraph("image/CAT.PNG");
	int imgPoint = LoadGraph("image/point.png");
	int imgGold = LoadGraph("image/Gold.png");
	int imgAmejisuto = LoadGraph("image/Amejisuto.png");
	int imgDiamond = LoadGraph("image/Diamond.png");
	int imgEmerarudo = LoadGraph("image/Emerarudo.png");
	int imgVoid = LoadGraph("image/Void.png");
	int playerW;
	int playerH;
	GetGraphSize(imgPlayer,&playerW,&playerH);
	// サウンド
	int bgm = LoadSoundMem("sound/bgm.mp3");
	int jin = LoadSoundMem("sound/gameover.mp3");
	int sePoint = LoadSoundMem("sound/fuel.mp3");
	ChangeVolumeSoundMem(128, bgm);
	ChangeVolumeSoundMem(128, jin);
	// プレイヤー
	int playerX = WIDTH / 2;
	int playerY = HEIGHT / 2;
	for (int i = 0; i < COM_MAX; i++)
	{
		computerX[i] = rand() % 180 + 270;
		computerY[i] = -(rand() % 800);
		computerType[i] = YELLOW + rand() % 3;
	}
	int Money = 0;
	int MostMoney = 45000;
	int bgY = 0;
	enum { TITLE, PLAY, OVER };
	int scene = TITLE;
	int timer = 0;
	while (1)
	{
		// Moneyが10000増えるごとに車を1台追加
		int carCount = 5 + Money / 10000;
				// 最大台数を超えないようにする
		if (carCount > COM_MAX)
		{
			carCount = COM_MAX;
		}

		ClearDrawScreen();
		if (scene == PLAY)
		{
			bgY = (bgY + 10) % HEIGHT;
		}

		DrawGraph(0, bgY - HEIGHT, imgBG, FALSE);
		DrawGraph(0, bgY, imgBG, FALSE);
		if (scene == PLAY)
		{
			GetMousePoint(&playerX, &playerY);
			if (playerX < 260) playerX = 260;
			if (playerX > 460) playerX = 460;
			if (playerY < 40) playerY = 40;
			if (playerY > 600) playerY = 600;
		}

		DrawExtendGraph(playerX - playerW / 6,playerY - playerH / 6,playerX + playerW / 6,playerY + playerH / 6,imgPlayer,TRUE);
		// 敵車
		for (int i = 0; i < carCount; i++)
		{
			if (scene == PLAY)
			{
				// 下へ移動
				computerY[i] += 6;
				// 画面外へ出たら再配置
				if (computerY[i] > HEIGHT + 100)
				{
					computerX[i] = rand() % 180 + 270;
					computerY[i] = -(rand() % 500 + 100);
					computerType[i] = YELLOW + rand() % 3;
				}

				// 当たり判定
				int dx = abs(computerX[i] - playerX);
				int dy = abs(computerY[i] - playerY);
				const int PLAYER_W = playerW / 5;
				const int PLAYER_H = playerH / 5;
				int hitW = PLAYER_W / 2 + CAR_W[computerType[i]] / 2 - 8;
				int hitH = PLAYER_H / 2 + CAR_H[computerType[i]] / 2 - 8;
				if (dx < hitW && dy < hitH)
				{
					Money -= 1000;
					computerX[i] = rand() % 180 + 270;
					computerY[i] = -(rand() % 500 + 100);
					computerType[i] = YELLOW + rand() % 3;
					if (Money < 0)
					{
						scene = OVER;
						timer = 0;
						StopSoundMem(bgm);
						PlaySoundMem(jin, DX_PLAYTYPE_BACK);
					}
				}
			}
			else
			{
				computerY[i]--;
				if (computerY[i] < -100)
				{
					computerY[i] = HEIGHT + 100;
				}
			}
			drawCar(computerX[i],computerY[i],computerType[i]);
		}
		// ポイントアイテム
		if (scene == PLAY)
		{
			// 2秒に1回出現
			if (timer % 120 == 0 && pointFlag == 0)
			{
				pointX = rand() % 180 + 270;
				pointY = -40;
				pointFlag = 1;
			}

			if (pointFlag)
			{
				pointY += 6;

				int pointW, pointH;
				GetGraphSize(imgPoint, &pointW, &pointH);

				DrawExtendGraph(pointX - pointW / 35,pointY - pointH / 35,pointX + pointW / 35,pointY + pointH / 35,imgPoint,TRUE);
				const int PLAYER_W = playerW / 3;
				const int PLAYER_H = playerH / 3;
				if (abs(pointX - playerX) < PLAYER_W / 2 + 16 &&
					abs(pointY - playerY) < PLAYER_H / 2 + 16)
				{
					Money += 200;
					if (Money > MostMoney)
					{
						MostMoney = Money;
					}
					PlaySoundMem(sePoint, DX_PLAYTYPE_BACK);
					pointFlag = 0;
					pointY = -100;
				}

				if (pointY > HEIGHT)
				{
					pointFlag = 0;
				}
			}
		}
	   // 金インゴット
		if (scene == PLAY)
		{
			// 10秒に1回出現
			if (timer % 600 == 250 && goldFlag == 0)
			{
				goldX = rand() % 180 + 270;
				goldY = -40;
				goldFlag = 1;
			}

			if (goldFlag)
			{
				goldY += 6;
				int goldW, goldH;
				GetGraphSize(imgGold, &goldW, &goldH);
				DrawExtendGraph(goldX - goldW / 25,goldY - goldH / 25,goldX + goldW / 25,goldY + goldH / 25,imgGold,TRUE);
				const int PLAYER_W = playerW / 3;
				const int PLAYER_H = playerH / 3;
				if (abs(goldX - playerX) < PLAYER_W / 2 + 16 &&abs(goldY - playerY) < PLAYER_H / 2 + 16)
				{
					Money += 400;
					if (Money > MostMoney)
					{
						MostMoney = Money;
					}
					PlaySoundMem(sePoint, DX_PLAYTYPE_BACK);
					goldFlag = 0;
					goldY = -100;
				}
				if (goldY > HEIGHT)
				{
					goldFlag = 0;
				}
			}
		}
		// エメラルド
		if (scene == PLAY)
		{
			// 20秒に1回出現
			if (timer % 1200 == 220 && EmerarudoFlag == 0)
			{
				EmerarudoX = rand() % 180 + 270;
				EmerarudoY = -40;
				EmerarudoFlag = 1;
			}

			if (EmerarudoFlag)
			{
				EmerarudoY += 6;
				int EmerarudoW, EmerarudoH;
				GetGraphSize(imgEmerarudo,&EmerarudoW,&EmerarudoH);
				DrawExtendGraph(EmerarudoX - EmerarudoW / 38,EmerarudoY - EmerarudoH / 38,EmerarudoX + EmerarudoW / 38,EmerarudoY + EmerarudoH / 38,imgEmerarudo,TRUE);
				const int PLAYER_W = playerW / 3;
				const int PLAYER_H = playerH / 3;
				if (abs(EmerarudoX - playerX) < PLAYER_W / 2 + 16 &&abs(EmerarudoY - playerY) < PLAYER_H / 2 + 16)
				{
					Money += 800;
					if (Money > MostMoney)
					{
						MostMoney = Money;
					}
					PlaySoundMem(sePoint, DX_PLAYTYPE_BACK);
					EmerarudoFlag = 0;
					EmerarudoY = -100;
				}
				if (EmerarudoY > HEIGHT)
				{
					EmerarudoFlag = 0;
				}
			}
		}
		// アメジストアイテム
		if (scene == PLAY)
		{
			// 30秒に1回出現
			if (timer % 1800 == 500 && amejisutoFlag == 0)
			{
				amejisutoX = rand() % 180 + 270;
				amejisutoY = -40;
				amejisutoFlag = 1;
			}

			if (amejisutoFlag)
			{
				amejisutoY += 6;
				int amejisutoW, amejisutoH;
				GetGraphSize(imgAmejisuto,&amejisutoW,&amejisutoH);
				DrawExtendGraph(amejisutoX - amejisutoW / 20,amejisutoY - amejisutoH / 20,amejisutoX + amejisutoW / 20,amejisutoY + amejisutoH / 20,imgAmejisuto,TRUE);
				const int PLAYER_W = playerW / 3;
				const int PLAYER_H = playerH / 3;
				if (abs(amejisutoX - playerX) < PLAYER_W / 2 + 16 &&abs(amejisutoY - playerY) < PLAYER_H / 2 + 16)
				{
					Money += 1000;
					if (Money > MostMoney)
					{
						MostMoney = Money;
					}
					PlaySoundMem(sePoint, DX_PLAYTYPE_BACK);
					amejisutoFlag = 0;
					amejisutoY = -100;
				}
				if (amejisutoY > HEIGHT)
				{
					amejisutoFlag = 0;
				}
			}
		}
		// ダイヤアイテム
		if (scene == PLAY)
		{
			// 1分に1回出現
			if (timer % 3600 == 1000 && diamondFlag == 0)
			{
				diamondX = rand() % 180 + 270;
				diamondY = -40;
				diamondFlag = 1;
			}
			if (diamondFlag)
			{
				diamondY += 6;

				int diamondW, diamondH;
				GetGraphSize(imgDiamond,&diamondW,&diamondH);
				DrawExtendGraph(diamondX - diamondW / 15,diamondY - diamondH / 15,diamondX + diamondW / 15,diamondY + diamondH / 15,imgDiamond,TRUE);
				const int PLAYER_W = playerW / 3;
				const int PLAYER_H = playerH / 3;
				if (abs(diamondX - playerX) < PLAYER_W / 2 + 16 &&abs(diamondY - playerY) < PLAYER_H / 2 + 16)
				{
					Money += 5000;
					if (Money > MostMoney)
					{
						MostMoney = Money;
					}
					PlaySoundMem(sePoint, DX_PLAYTYPE_BACK);
					diamondFlag = 0;
					diamondY = -100;
				}
				if (diamondY > HEIGHT)
				{
					diamondFlag = 0;
				}
			}
		}
	  // Voidアイテム
		if (scene == PLAY)
		{
			// 1分30秒に1回出現
			if (timer % 5400 == 3550 && VoidFlag == 0)
			{
				VoidX = rand() % 180 + 270;
				VoidY = -40;
				VoidFlag = 1;
			}
			if (VoidFlag)
			{
				VoidY += 6;
				int voidW, voidH;
				GetGraphSize(imgVoid, &voidW, &voidH);
				DrawExtendGraph(VoidX - voidW / 15,VoidY - voidH / 15,VoidX + voidW / 15,VoidY + voidH / 15,imgVoid,TRUE);
				const int PLAYER_W = playerW / 3;
				const int PLAYER_H = playerH / 3;
				if (abs(VoidX - playerX) < PLAYER_W / 2 + 16 &&abs(VoidY - playerY) < PLAYER_H / 2 + 16)
				{
					Money += 10000;
					if (Money > MostMoney)
					{
						MostMoney = Money;
					}
					PlaySoundMem(sePoint, DX_PLAYTYPE_BACK);
					VoidFlag = 0;
					VoidY = -100;
				}
				if (VoidY > HEIGHT)
				{
					VoidFlag = 0;
				}
			}
		}
		timer++;
		// シーン処理
		switch (scene)
		{
		// タイトルシーン
		case TITLE:

			drawText(100,100,191970,"ハイスコアを更新しよう",0,50);
			drawText(170,160,0x1E90FF,"  Run",0,100);
			if (timer % 60 < 30)
			{
				drawText(180,400,0x00FF00,"Click to Start",0,40);
			}

			if (GetMouseInput() & MOUSE_INPUT_LEFT)
			{
				Money = 0;
				playerX = WIDTH / 2;
				playerY = HEIGHT / 2;
				// アイテム初期化
				pointFlag = 0;
				pointY = -100;

				goldFlag = 0;
				goldY = -100;

				EmerarudoFlag = 0;
				EmerarudoY = -100;

				amejisutoFlag = 0;
				amejisutoY = -100;

				diamondFlag = 0;
				diamondY = -100;

				VoidFlag = 0;
				VoidY = -100;
				// 敵車初期化
				for (int i = 0; i < COM_MAX; i++)
				{
					computerX[i] = rand() % 180 + 270;
					computerY[i] = -(rand() % 800);
					computerType[i] = YELLOW + rand() % 3;
				}
				timer = 0;
				scene = PLAY;
				PlaySoundMem(bgm,DX_PLAYTYPE_LOOP);
			}

			break;
	   // プレイシーン
		case PLAY:
			break;
			// ゲームオーバー
		case OVER:
			drawText(150,220,0xFF0000,"GAME OVER",0,80);
			if (timer > 180)
			{
				scene = TITLE;
				timer = 0;
			}
			break;
		}
		// スコア表示
		drawText(10,10,0x7CFC00,"Money %d",Money,30);
		drawText(WIDTH - 250,10,0x32CD32,"Most Money %d",MostMoney,30);
	 // 画面更新
		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1)
		{
			break;
		}
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}
	// 終了処理
	StopSoundMem(bgm);
	DxLib_End();
	return 0;
}