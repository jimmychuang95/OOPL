#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"

namespace game_framework {


	CGameMap::CGameMap()
		:x(34), y(206), mx(36), my(37)
	{
		Initialize(1);
	}
	
	void CGameMap::Initialize(int level)
	{
		int stage_one[9][9] = { {1,1,1,1,1,1,1,1,1},
								{1,1,1,1,1,1,1,1,1},
								{1,1,0,0,2,0,3,1,1},
								{1,1,0,0,1,1,1,1,1},
								{1,1,0,4,1,1,1,1,1},
								{1,1,1,1,1,1,1,1,1},
								{1,1,1,1,1,1,1,1,1},
								{1,1,1,1,1,1,1,1,1},
								{1,1,1,1,1,1,1,1,1}, }; //0=floor,1=wall,2=box,3=finish,4=leader

		int stage_two[9][9] = { {1,1,1,1,1,1,1,1,1},
								{1,1,1,1,1,1,1,1,1},
								{1,1,0,0,4,0,0,1,1},
								{1,1,0,0,0,0,0,1,1},
								{1,1,0,0,0,2,3,1,1},
								{1,1,0,0,0,0,0,1,1},
								{1,1,1,1,1,1,1,1,1},
								{1,1,1,1,1,1,1,1,1},
								{1,1,1,1,1,1,1,1,1}, };

		int stage_three[9][9] = { {1,1,1,1,1,1,1,1,1},
								  {1,1,1,1,1,1,1,1,1},
								  {1,1,1,1,3,2,0,1,1},
								  {1,1,0,0,0,0,0,1,1},
								  {1,1,0,0,0,0,0,1,1},
								  {1,1,0,0,2,3,0,1,1},
								  {1,1,4,0,0,1,1,1,1},
								  {1,1,1,1,1,1,1,1,1},
								  {1,1,1,1,1,1,1,1,1}, };


		if (level == 1) {
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
				{
					map[i][j] = stage_one[i][j];
				}
		}
		else if (level == 2) {
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
				{
					map[i][j] = stage_two[i][j];
				}
		}
		else if (level == 3) {
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
				{
					map[i][j] = stage_three[i][j];
				}
		}

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (map[i][j] == 4) {
					leadersite[0] = i;
					leadersite[1] = j;
				}
			}
		}
	}

	int CGameMap::GetComponent(int x, int y)
	{
		return map[x][y];
	}


	bool CGameMap::MoveRight()
	{
		if (map[leadersite[0]][leadersite[1] + 1] != 1) {				//leader右邊不是牆壁
			if (map[leadersite[0]][leadersite[1]] != 5) {					//leader不在終點上!!!!
				if (map[leadersite[0]][leadersite[1] + 1] == 0) {				//leader 右邊是地板
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0]][leadersite[1] + 1] = 4;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0]][leadersite[1] + 1] == 3) {				//leader 右邊是終點
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0]][leadersite[1] + 1] = 5;					/*	5 = leader in finish	*/
					leadersite[1] += 1;											/*	設leadersite在finish上	*/
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0]][leadersite[1] + 1] == 2) {				//leader右邊是箱子
					if (map[leadersite[0]][leadersite[1] + 2] == 0) {				// leader - 箱子 - 地板
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] + 1] = 4;
						map[leadersite[0]][leadersite[1] + 2] = 2;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] + 2] == 3) {				//leader - 箱子 - 終點
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] + 1] = 4;
						map[leadersite[0]][leadersite[1] + 2] = 6;					/*	6 = finished	*/
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}

				if (map[leadersite[0]][leadersite[1] + 1] == 6) {				//leader右邊是完成的箱子
					if (map[leadersite[0]][leadersite[1] + 2] == 0) {				// leader - 完成的箱子 - 地板
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] + 1] = 5;
						map[leadersite[0]][leadersite[1] + 2] = 2;
						leadersite[1] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] + 2] == 3) {				// leader - 完成的箱子 - 終點
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] + 1] = 5;
						map[leadersite[0]][leadersite[1] + 2] = 6;
						leadersite[1] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
			}

			if (map[leadersite[0]][leadersite[1]] == 5) {					//leader在終點上
				if (map[leadersite[0]][leadersite[1] + 1] == 0) {				//leader在終點上 - 地板
					map[leadersite[0]][leadersite[1]] = 3;
					map[leadersite[0]][leadersite[1] + 1] = 4;
					leadersite[1] += 1;
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0]][leadersite[1] + 1] == 3) {				//leader在終點上 - 終點
					map[leadersite[0]][leadersite[1]] = 3;
					map[leadersite[0]][leadersite[1] + 1] = 5;					
					leadersite[1] += 1;											
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0]][leadersite[1] + 1] == 2) {				//leader在終點上 - 箱子
					if (map[leadersite[0]][leadersite[1] + 2] == 0) {				// leader在終點 - 箱子 - 地板
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] + 1] = 4;
						map[leadersite[0]][leadersite[1] + 2] = 2;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] + 2] == 3) {				//leader在終點 - 箱子 - 終點
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] + 1] = 4;
						map[leadersite[0]][leadersite[1] + 2] = 6;					
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}

				if (map[leadersite[0]][leadersite[1] + 1] == 6) {				//leader在終點上 - 完成的箱子
					if (map[leadersite[0]][leadersite[1] + 1] == 0) {				//leader在終點上 - 完成的箱子 - 地板
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] + 1] = 5;
						map[leadersite[0]][leadersite[1] + 2] = 0;
						leadersite[1] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] + 2] == 3) {				//leader在終點上 - 完成的箱子 - 終點 
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] + 1] = 5;
						map[leadersite[0]][leadersite[1] + 2] = 6;
						leadersite[1] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
			}
		}
		return false;
	}

	bool CGameMap::MoveLeft()
	{
		if (map[leadersite[0]][leadersite[1] - 1] != 1) {				//leader左邊不是牆壁
			if (map[leadersite[0]][leadersite[1]] != 5) {					//leader不在終點上!!!!
				if (map[leadersite[0]][leadersite[1] - 1] == 0) {				//leader 左邊是地板
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0]][leadersite[1] - 1] = 4;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0]][leadersite[1] - 1] == 3) {				//leader 左邊是終點
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0]][leadersite[1] - 1] = 5;					/*	5 = leader in finish	*/
					leadersite[1] -= 1;											/*	設leadersite在finish上	*/
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0]][leadersite[1] - 1] == 2) {				//leader左邊是箱子
					if (map[leadersite[0]][leadersite[1] - 2] == 0) {				//地板 - 箱子 - leader
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] - 1] = 4;
						map[leadersite[0]][leadersite[1] - 2] = 2;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] - 2] == 3) {				//終點 - 箱子 - leader
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] - 1] = 4;
						map[leadersite[0]][leadersite[1] - 2] = 6;					/*	6 = finished	*/
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}

				if (map[leadersite[0]][leadersite[1] - 1] == 6) {				//leader左邊是完成的箱子
					if (map[leadersite[0]][leadersite[1] - 2] == 0) {				//地板 - 完成的箱子 - leader
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] - 1] = 5;
						map[leadersite[0]][leadersite[1] - 2] = 2;
						leadersite[1] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] - 2] == 3) {				//終點 - 完成的箱子 - leader
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] - 1] = 5;
						map[leadersite[0]][leadersite[1] - 2] = 6;
						leadersite[1] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
			}

			if (map[leadersite[0]][leadersite[1]] == 5) {					//leader在終點上
				if (map[leadersite[0]][leadersite[1] - 1] == 0) {				//地板 - leader在終點上
					map[leadersite[0]][leadersite[1]] = 3;
					map[leadersite[0]][leadersite[1] - 1] = 4;
					leadersite[1] -= 1;
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0]][leadersite[1] - 1] == 3) {				//終點 - leader在終點上
					map[leadersite[0]][leadersite[1]] = 3;
					map[leadersite[0]][leadersite[1] - 1] = 5;
					leadersite[1] -= 1;
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0]][leadersite[1] - 1] == 2) {				//箱子 - leader在終點上
					if (map[leadersite[0]][leadersite[1] - 2] == 0) {				//地板 - 箱子 - leader在終點
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] - 1] = 4;
						map[leadersite[0]][leadersite[1] - 2] = 2;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] - 2] == 3) {				//終點 - 箱子 - leader在終點
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] - 1] = 4;
						map[leadersite[0]][leadersite[1] - 2] = 6;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}

				if (map[leadersite[0]][leadersite[1] - 1] == 6) {				//完成的箱子 - leader在終點上
					if (map[leadersite[0]][leadersite[1] - 1] == 0) {				//地板 - 完成的箱子 - leader在終點上
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] - 1] = 5;
						map[leadersite[0]][leadersite[1] - 2] = 0;
						leadersite[1] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] - 2] == 3) {				//終點 - 完成的箱子 - leader在終點上
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] - 1] = 5;
						map[leadersite[0]][leadersite[1] - 2] = 6;
						leadersite[1] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
			}
		}
		return false;
	}

	bool CGameMap::MoveUp()
	{
		if (map[leadersite[0] - 1][leadersite[1]] != 1) {
			if (map[leadersite[0]][leadersite[1]] != 5) {
				if (map[leadersite[0] - 1][leadersite[1]] == 0) {
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0] - 1][leadersite[1]] = 4;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0] - 1][leadersite[1]] == 3) {
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0] - 1][leadersite[1]] = 5;
					leadersite[0] -= 1;
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0] - 1][leadersite[1]] == 2) {
					if (map[leadersite[0] - 2][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0] - 1][leadersite[1]] = 4;
						map[leadersite[0] - 2][leadersite[1]] = 2;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] - 2][leadersite[1]] == 3) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0] - 1][leadersite[1]] = 4;
						map[leadersite[0] - 2][leadersite[1]] = 6;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
				if (map[leadersite[0] - 1][leadersite[1]] == 6) {
					if (map[leadersite[0] - 2][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0] - 1][leadersite[1]] = 5;
						map[leadersite[0] - 2][leadersite[1]] = 2;
						leadersite[0] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] - 2][leadersite[1]] == 3) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0] - 1][leadersite[1]] = 5;
						map[leadersite[0] - 2][leadersite[1]] = 6;
						leadersite[0] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
			}
			if (map[leadersite[0]][leadersite[1]] == 5) {
				if (map[leadersite[0] - 1][leadersite[1]] == 0) {
					map[leadersite[0]][leadersite[1]] = 3;
					map[leadersite[0] - 1][leadersite[1]] = 4;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0] - 1][leadersite[1]] == 3) {
					map[leadersite[0]][leadersite[1]] = 3;
					map[leadersite[0] - 1][leadersite[1]] = 5;
					leadersite[0] -= 1;
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0] - 1][leadersite[1]] == 2) {
					if (map[leadersite[0] - 2][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] - 1][leadersite[1]] = 4;
						map[leadersite[0] - 2][leadersite[1]] = 2;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] - 2][leadersite[1]] == 3) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] - 1][leadersite[1]] = 4;
						map[leadersite[0] - 2][leadersite[1]] = 6;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
				if (map[leadersite[0] - 1][leadersite[1]] == 6) {
					if (map[leadersite[0] - 2][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] - 1][leadersite[1]] = 5;
						map[leadersite[0] - 2][leadersite[1]] = 2;
						leadersite[0] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] - 2][leadersite[1]] == 3) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] - 1][leadersite[1]] = 5;
						map[leadersite[0] - 2][leadersite[1]] = 6;
						leadersite[0] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
			}
		}
		return false;
	}

	bool CGameMap::MoveDown()
	{
		if (map[leadersite[0] + 1][leadersite[1]] != 1) {
			if (map[leadersite[0]][leadersite[1]] != 5) {
				if (map[leadersite[0] + 1][leadersite[1]] == 0) {
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0] + 1][leadersite[1]] = 4;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0] + 1][leadersite[1]] == 3) {
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0] + 1][leadersite[1]] = 5;
					leadersite[0] += 1;
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0] + 1][leadersite[1]] == 2) {
					if (map[leadersite[0] + 2][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0] + 1][leadersite[1]] = 4;
						map[leadersite[0] + 2][leadersite[1]] = 2;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] + 2][leadersite[1]] == 3) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0] + 1][leadersite[1]] = 4;
						map[leadersite[0] + 2][leadersite[1]] = 6;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}

				if (map[leadersite[0] + 1][leadersite[1]] == 6) {
					if (map[leadersite[0] + 2][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0] + 1][leadersite[1]] = 5;
						map[leadersite[0] + 2][leadersite[1]] = 2;
						leadersite[0] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] + 2][leadersite[1]] == 3) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0] + 1][leadersite[1]] = 5;
						map[leadersite[0] + 2][leadersite[1]] = 6;
						leadersite[0] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
			}

			if (map[leadersite[0]][leadersite[1]] == 5) {
				if (map[leadersite[0] + 1][leadersite[1]] == 0) {
					map[leadersite[0]][leadersite[1]] = 3;
					map[leadersite[0] + 1][leadersite[1]] = 4;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0] + 1][leadersite[1]] == 3) {
					map[leadersite[0]][leadersite[1]] = 3;
					map[leadersite[0] + 1][leadersite[1]] = 5;
					leadersite[0] += 1;
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}

				if (map[leadersite[0] + 1][leadersite[1]] == 2) {
					if (map[leadersite[0] + 2][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] + 1][leadersite[1]] = 4;
						map[leadersite[0] + 2][leadersite[1]] = 2;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] + 2][leadersite[1]] == 3) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] + 1][leadersite[1]] = 4;
						map[leadersite[0] + 2][leadersite[1]] = 6;
						this->SetLeader();
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}

				if (map[leadersite[0] + 1][leadersite[1]] == 6) {
					if (map[leadersite[0] + 2][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] + 1][leadersite[1]] = 5;
						map[leadersite[0] + 2][leadersite[1]] = 2;
						leadersite[0] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] + 2][leadersite[1]] == 3) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] + 1][leadersite[1]] = 5;
						map[leadersite[0] + 2][leadersite[1]] = 6;
						leadersite[0] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
			}
		}
		return false;
	}

	bool CGameMap::IsFinish() {
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				if (map[i][j] == 3) {
					return false;
				}
				if (map[i][j] == 5) {
					return false;
				}
			}
		return true;
	}

	void CGameMap::SetLeader()
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				if (map[i][j] == 4) {
					leadersite[0] = i;
					leadersite[1] = j;
				}
			}
	}
}