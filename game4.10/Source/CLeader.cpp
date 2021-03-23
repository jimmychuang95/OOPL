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
	const int MOVE_SIZE = 38;

	CLeader::CLeader()
	{
		Initialize();
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
		return x + animation.Width();
	}

	int CLeader::GetY2()
	{
		return y + animation.Height();
	}

	void CLeader::Initialize()
	{
		const int X_POS = 280;
		const int Y_POS = 400;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	}

	void CLeader::LoadBitmap()
	{	
		animation.AddBitmap(LEADER_DOWN, RGB(255, 255, 255));
		animation.AddBitmap(LEADER_UP, RGB(255, 255, 255));
		animation.AddBitmap(LEADER_LEFT, RGB(255, 255, 255));
		animation.AddBitmap(LEADER_RIGHT, RGB(255, 255, 255));
	}

	void CLeader::OnMove()
	{
		const int STEP_SIZE = 3;
		animation.OnMove();
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

	void CLeader::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CLeader::OnShow()
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}
}