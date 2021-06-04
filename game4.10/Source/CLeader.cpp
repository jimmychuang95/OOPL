#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CLeader.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CLeader: Leader class
	/////////////////////////////////////////////////////////////////////////////

	CLeader::CLeader()
	{
		Initialize(1);
	}

	int CLeader::GetX1()
	{
		return x;
	}

	int CLeader::GetY1()
	{
		return y;
	}

	int CLeader::GetX2()
	{
		return x + animation_up.Width();
	}

	int CLeader::GetY2()
	{
		return y + animation_up.Height();
	}

	bool CLeader::GetStatus()
	{
		return isStanding;
	}

	void CLeader::Initialize(int level)
	{
		if (level == 1) {
			x = 113;
			y = 318;
		}
		else if (level == 2) {
			x = 149;
			y = 243;
		}
		else if (level == 3) {
			x = 74;
			y = 354;
		}
		else if (level == 4) {
			x = 74;
			y = 241;
		}
		else if (level == 5) {
			x = 74;
			y = 241;
		}
		else if (level == 6) {
			x = 113;
			y = 205;
		}
		else if (level == 7) {
			x = 224;
			y = 318;
		}
		else if (level == 8) {
			x = 262;
			y = 244;
		}
		else if (level == 9) {
			x = 75;
			y = 244;
		}
		else if (level == 10) {
			x = 188;
			y = 317;
		}
		else if (level == 11) {
			x = 111;
			y = 204;
		}
		else if (level == 12) {
			x = 75;
			y = 205;
		}
		else if (level == 13) {
			x = 75;
			y = 355;
		}
		else if (level == 14) {
			x = 224;
			y = 243;
		}
		else if (level == 15) {
			x = 36;
			y = 243;
		}
		else if (level == 16) {
			x = 224;
			y = 355;
		}
		else if (level == 17) {
			x = 224;
			y = 356;
		}
		else if (level == 18) {
			x = 150;
			y = 318;
		}
		else if (level == 19) {
			x = 75;
			y = 206;
		}
		else if (level == 20) {
			x = 187;
			y = 281;
		}
		else if (level == 21) {
			x = 187;
			y = 280;
		}
		else if (level == 22) {
			x = 75;
			y = 281;
		}
		else if (level == 23) {
			x = 75;
			y = 281;
		}
		else if (level == 24) {
			x = 75;
			y = 394;
		}
		else if (level == 25) {
			x = 224;
			y = 281;
		}
		else if (level == 26) {
			x = 187;
			y = 319;
		}
		else if (level == 27) {
			x = 225;
			y = 356;
		}
		else if (level == 28) {
			x = 112;
			y = 281;
		}
		else if (level == 29) {
			x = 75;
			y = 244;
		}
	


		
		current = 4;
		isStanding = true;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	}

	void CLeader::LoadBitmap()
	{	
		animation_up.LoadBitmap(LEADER_UP, RGB(255, 255, 255));
		animation_down.LoadBitmap(LEADER_DOWN, RGB(255, 255, 255));
		animation_left.LoadBitmap(LEADER_LEFT, RGB(255, 255, 255));
		animation_right.LoadBitmap(LEADER_RIGHT, RGB(255, 255, 255));
	}

	void CLeader::OnMove()
	{
		const int STEP_SIZE = 2;
		//animation_up.OnMove();
		if (isMovingLeft)
			x -= STEP_SIZE;
		if (isMovingRight)
			x += STEP_SIZE;
		if (isMovingUp)
			y -= STEP_SIZE;
		if (isMovingDown)
			y += STEP_SIZE;
	}

	void CLeader::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CLeader::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CLeader::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CLeader::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CLeader::SetStanding(bool flag) {
		isStanding = flag;
	}

	void CLeader::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CLeader::OnShow()
	{	
		if (isMovingLeft) {
			animation_left.SetTopLeft(x, y);
			animation_left.ShowBitmap();
			current = 1;
		}
		else if (isMovingRight) {
			animation_right.SetTopLeft(x, y);
			animation_right.ShowBitmap();
			current = 2;
		}
		else if (isMovingUp) {
			animation_up.SetTopLeft(x, y);
			animation_up.ShowBitmap();
			current = 3;
		}
		else if (isMovingDown) {
			animation_down.SetTopLeft(x, y);
			animation_down.ShowBitmap();
			current = 4;
		}
		if (isStanding) {
			if (current == 1) {
				animation_left.SetTopLeft(x, y);
				animation_left.ShowBitmap();
			}
			if (current == 2) {
				animation_right.SetTopLeft(x, y);
				animation_right.ShowBitmap();
			}
			if (current == 3) {
				animation_up.SetTopLeft(x, y);
				animation_up.ShowBitmap();
			}
			if (current == 4) {
				animation_down.SetTopLeft(x, y);
				animation_down.ShowBitmap();
			}
		}
	}

}