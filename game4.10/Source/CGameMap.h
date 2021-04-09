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
		void CGameMap::Initialize(int level);
		void CGameMap::PreviousStep(int dir, bool boxPush);
		void CGameMap::HackModeOn();
	protected:
		int map[9][9];
		const int x, y;
		const int mx, my;
		int leadersite[2] = { 0 };
	};
}