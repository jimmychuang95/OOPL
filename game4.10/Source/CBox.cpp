#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CBox.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CBox: Leader class
	/////////////////////////////////////////////////////////////////////////////

	CBox::CBox()
	{
		Initialize(1, 1);
	}

	int CBox::GetX1()
	{
		return x;
	}

	int CBox::GetY1()
	{
		return y;
	}

	int CBox::GetX2()
	{
		return x + bmp_box.Width();
	}

	int CBox::GetY2()
	{
		return y + bmp_box.Height();
	}

	bool CBox::GetStatus()
	{
		return isStanding;
	}

	void CBox::PushedRight(bool flag) {
		canPushRight = flag;
	}

	void CBox::PushedLeft(bool flag) {
		canPushLeft = flag;
	}

	void CBox::PushedUp(bool flag) {
		canPushUp = flag;
	}

	void CBox::PushedDown(bool flag) {
		canPushDown = flag;
	}

	bool CBox::CanPushRight() {
		return canPushRight;
	}

	bool CBox::CanPushLeft() {
		return canPushLeft;
	}

	bool CBox::CanPushDown() {
		return canPushDown;
	}

	bool CBox::CanPushUp() {
		return canPushUp;
	}

	void CBox::Initialize(int level, int num)
	{
		if (level == 1) {
			if (num == 1) {
				x = 150;
				y = 244;
			}
			else if (num > 1) {		//將多餘的箱子丟到螢幕外
				x = 5000;
				y = 5000;
			}
		}
		else if (level == 2) {
			if (num == 1) {
				x = 187;
				y = 317;
			}
			else if (num > 1) {		//將多餘的箱子丟到螢幕外
				x = 5000;
				y = 5000;
			}
		}
		else if (level == 3) {
			if (num == 1) {
				x = 186;
				y = 205;
			}
			else if (num == 2) {
				x = 149;
				y = 318;
			}
			else if (num == 3) {	//將多餘的箱子丟到螢幕外
				x = 5000;
				y = 5000;
			}
		}
		else if (level == 4) {
			if (num == 1) {
				x = 149;
				y = 318;
			}
			else if (num == 2) {
				x = 225;
				y = 318;
			}
			else if (num == 3) {
				x = 5000;
				y = 5000;
			}
		}
		else if (level == 5) {
			if (num == 1) {
				x = 188;
				y = 242;
			}
			else if (num == 2) {
				x = 188;
				y = 279;
			}
			else if (num == 3) {
				x = 5000;
				y = 5000;
			}
		}
		else if (level == 6) {
			if (num == 1) {
				x = 112;
				y = 280;
			}
			else if (num == 2) {
				x = 150;
				y = 280;
			}
			else if (num == 3) {
				x = 188;
				y = 280;
			}
		}
		else if (level == 7) {
			if (num == 1) {
				x = 112;
				y = 318;
			}
			else if (num == 2) {
				x = 150;
				y = 318;
			}
			else if (num == 3) {
				x = 188;
				y = 318;
			}
		}
		else if (level == 8) {
			if (num == 1) {
				x = 225;
				y = 243;
			}
			else if (num == 2) {
				x = 225;
				y = 317;
			}
			else if (num == 3) {
				x = 112;
				y = 318;
			}
		}
		else if (level == 9) {
			if (num == 1) {
				x = 112;
				y = 244;
			}
			else if (num == 2) {
				x = 112;
				y = 280;
			}
			else if (num == 3) {
				x = 112;
				y = 318;
			}
		}
		else if (level == 10) {
			if (num == 1) {
				x = 149;
				y = 279;
			}
			else if (num == 2) {
				x = 187;
				y = 279;
			}
			else if (num == 3) {
				x = 225;
				y = 279;
			}
		}
		else if (level == 11) {
			if (num == 1) {
				x = 149;
				y = 206;
			}
			else if (num == 2) {
				x = 75;
				y = 242;
			}
			else if (num == 3) {
				x = 187;
				y = 243;
			}
		}



		isStanding = true;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	}

	void CBox::LoadBitmap()
	{
		bmp_box.LoadBitmap(MY_BOX, RGB(255, 255, 255));
	}

	void CBox::OnMove()
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

	void CBox::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CBox::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CBox::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CBox::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CBox::SetStanding(bool flag) {
		isStanding = flag;
	}

	void CBox::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CBox::OnShow()
	{
		bmp_box.SetTopLeft(x, y);
		bmp_box.ShowBitmap();
	}


}