#include "DxLib.h"

int WinAPI(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
		return -1;

	int x = 320;
	int y = 400;
	int vy = 0;

	const int moveSpeed = 5;
	const int jumpPower = -15;
	const int gravity = 1;

	while (ProcessMessage() == 0)
	{
		// 左右移動
		if (CheckHitKey(KEY_INPUT_A))
			x -= moveSpeed;

		if (CheckHitKey(KEY_INPUT_D))
			x += moveSpeed;

		// ジャンプ
		if (CheckHitKey(KEY_INPUT_SPACE) && y == 400)
			vy = jumpPower;

		// 重力
		vy += gravity;
		y += vy;

		// 地面
		if (y > 400)
		{
			y = 400;
			vy = 0;
		}

		ClearDrawScreen();

		// プレイヤーを描画
		DrawCircle(x, y, 20, GetColor(255, 0, 0), TRUE);

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}