namespace game_framework {

	class CGameMap
	{
	public:
		CGameMap();
		int CGameMap::GetComponent(int x, int y);
		bool CGameMap::MoveRight();
		bool CGameMap::MoveLeft();
		bool CGameMap::MoveUp();
		bool CGameMap::MoveDown();
		bool CGameMap::IsFinish();
		void CGameMap::SetLeader();
		void CGameMap::Initialize();
	protected:
		int map[5][7];
		const int x, y;
		const int mx, my;
		int leadersite[2] = { 0 };
	};
}