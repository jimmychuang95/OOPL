﻿#include "stdafx.h"
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
		Initialize(1);
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

	void CBox::Initialize(int level)
	{
		if (level == 1) {
			x = 150;
			y = 244;
		}
		else if (level == 2) {
			x = 187;
			y = 317;
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