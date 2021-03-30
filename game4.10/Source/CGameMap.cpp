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
		int map_init[5][7] = {  {1,1,1,1,1,1,1},
								{1,0,0,2,0,3,1},
								{1,0,0,1,1,1,1},
								{1,0,4,1,1,1,1},
								{1,1,1,1,1,1,1} }; //0=floor,1=wall,2=box,3=finish,4=leader
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 7; j++)
			{
				map[i][j] = map_init[i][j];
			}
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 7; j++)
			{
				if (map[i][j] == 4) {
					leadersite[0] = i;
					leadersite[1] = j;
				}
			}
	}

	int CGameMap::GetComponent(int x, int y)
	{
		return map[x][y];
	}


	bool CGameMap::MoveRight()
	{
		if (map[leadersite[0]][leadersite[1] + 1] != 1) {
			if (map[leadersite[0]][leadersite[1] + 1] == 0) {//leader floor
				map[leadersite[0]][leadersite[1]] = 0;
				map[leadersite[0]][leadersite[1] + 1] = 4;
				this->SetLeader();
				TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
				return true;
			}

			if (map[leadersite[0]][leadersite[1] + 1] == 3) {//leader finish
				map[leadersite[0]][leadersite[1]] = 0;
				map[leadersite[0]][leadersite[1] + 1] = 5;	  //5 = leader in finish
				leadersite[1] += 1;                           //設leadersite在finish上
				TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
				return true;
			}

			if (map[leadersite[0]][leadersite[1] + 1] == 2) {
				if (map[leadersite[0]][leadersite[1] + 2] == 0) {// leader box floor
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0]][leadersite[1] + 1] = 4;
					map[leadersite[0]][leadersite[1] + 2] = 2;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}
				if (map[leadersite[0]][leadersite[1] + 2] == 3) {//leader box finish
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0]][leadersite[1] + 1] = 4;
					map[leadersite[0]][leadersite[1] + 2] = 6; // 6 = finished
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}
				if (map[leadersite[0]][leadersite[1] + 1] == 6) {
					if (map[leadersite[0]][leadersite[1] + 2] == 0) {// leader finished floor
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] + 1] = 5;
						map[leadersite[0]][leadersite[1] + 2] = 2;
						leadersite[1] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] + 2] == 3) {// leader finished floor
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] + 1] = 5;
						map[leadersite[0]][leadersite[1] + 2] = 6;
						leadersite[1] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
				if (map[leadersite[0]][leadersite[1]] == 5) {
					if (map[leadersite[0]][leadersite[1] + 1] == 0) {//leader in finished flloor
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] + 1] = 4;
						leadersite[1] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] + 1] == 6) {
						if (map[leadersite[0]][leadersite[1] + 1] == 0) {//leader in finished finished floor
							map[leadersite[0]][leadersite[1]] = 3;
							map[leadersite[0]][leadersite[1] + 1] = 5;
							map[leadersite[0]][leadersite[1] + 2] = 0;
							leadersite[1] += 1;
							TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
							return true;
						}
						if (map[leadersite[0]][leadersite[1] + 1] == 3) {//leader in finished finished finish
							map[leadersite[0]][leadersite[1]] = 3;
							map[leadersite[0]][leadersite[1] + 1] = 5;
							map[leadersite[0]][leadersite[1] + 2] = 6;
							leadersite[1] += 1;
							TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
							return true;
						}
					}
				}

			}
		}
		return false;
	}

	bool CGameMap::MoveLeft()
	{
		if (map[leadersite[0]][leadersite[1] - 1] != 1) {
			if (map[leadersite[0]][leadersite[1] - 1] == 0) {
				map[leadersite[0]][leadersite[1]] = 0;
				map[leadersite[0]][leadersite[1] - 1] = 4;
				this->SetLeader();
				TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
				return true;
			}

			if (map[leadersite[0]][leadersite[1] - 1] == 3) {
				map[leadersite[0]][leadersite[1]] = 0;
				map[leadersite[0]][leadersite[1] - 1] = 5;
				leadersite[1] -= 1;
				TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
				return true;
			}

			if (map[leadersite[0]][leadersite[1] - 1] == 2) {
				if (map[leadersite[0]][leadersite[1] - 2] == 0) {
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0]][leadersite[1] - 1] = 4;
					map[leadersite[0]][leadersite[1] - 2] = 2;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}
				if (map[leadersite[0]][leadersite[1] - 2] == 3) {
					map[leadersite[0]][leadersite[1]] = 0;
					map[leadersite[0]][leadersite[1] - 1] = 4;
					map[leadersite[0]][leadersite[1] - 2] = 6;
					this->SetLeader();
					TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
					return true;
				}
				if (map[leadersite[0]][leadersite[1] - 1] == 6) {
					if (map[leadersite[0]][leadersite[1] - 2] == 0) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] - 1] = 5;
						map[leadersite[0]][leadersite[1] - 2] = 2;
						leadersite[1] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] - 2] == 3) {
						map[leadersite[0]][leadersite[1]] = 0;
						map[leadersite[0]][leadersite[1] - 1] = 5;
						map[leadersite[0]][leadersite[1] - 2] = 6;
						leadersite[1] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
				}
				if (map[leadersite[0]][leadersite[1]] == 5) {
					if (map[leadersite[0]][leadersite[1] - 1] == 0) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0]][leadersite[1] - 1] = 4;
						leadersite[1] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0]][leadersite[1] - 1] == 6) {
						if (map[leadersite[0]][leadersite[1] - 1] == 0) {
							map[leadersite[0]][leadersite[1]] = 3;
							map[leadersite[0]][leadersite[1] - 1] = 5;
							map[leadersite[0]][leadersite[1] - 2] = 0;
							leadersite[1] -= 1;
							TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
							return true;
						}
						if (map[leadersite[0]][leadersite[1] - 1] == 3) {
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
		}
		return false;
	}

	bool CGameMap::MoveUp()
	{
		if (map[leadersite[0] - 1][leadersite[1]] != 1) {
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
				if (map[leadersite[0]][leadersite[1]] == 5) {
					if (map[leadersite[0] - 1][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] - 1][leadersite[1]] = 4;
						leadersite[0] -= 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] - 1][leadersite[1]] == 6) {
						if (map[leadersite[0] - 1][leadersite[1]] == 0) {
							map[leadersite[0]][leadersite[1]] = 3;
							map[leadersite[0] - 1][leadersite[1]] = 5;
							map[leadersite[0] - 2][leadersite[1]] = 0;
							leadersite[0] -= 1;
							TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
							return true;
						}
						if (map[leadersite[0] - 1][leadersite[1]] == 3) {
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
		}
		return false;
	}

	bool CGameMap::MoveDown()
	{
		if (map[leadersite[0] + 1][leadersite[1]] != 1) {
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
				if (map[leadersite[0]][leadersite[1]] == 5) {
					if (map[leadersite[0] + 1][leadersite[1]] == 0) {
						map[leadersite[0]][leadersite[1]] = 3;
						map[leadersite[0] + 1][leadersite[1]] = 4;
						leadersite[0] += 1;
						TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
						return true;
					}
					if (map[leadersite[0] + 1][leadersite[1]] == 6) {
						if (map[leadersite[0] + 1][leadersite[1]] == 0) {
							map[leadersite[0]][leadersite[1]] = 3;
							map[leadersite[0] + 1][leadersite[1]] = 5;
							map[leadersite[0] + 2][leadersite[1]] = 0;
							leadersite[0] += 1;
							TRACE("my leader site = (%d,%d\n)", leadersite[0], leadersite[1]);
							return true;
						}
						if (map[leadersite[0] + 1][leadersite[1]] == 3) {
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
		}
		return false;
	}

	bool CGameMap::IsFinish() {
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 7; j++)
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
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 7; j++)
			{
				if (map[i][j] == 4) {
					leadersite[0] = i;
					leadersite[1] = j;
				}
			}
	}
}