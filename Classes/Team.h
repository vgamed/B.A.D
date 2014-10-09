#ifndef __GAME_TEAM_H__
#define __GAME_TEAM_H__

#define NUM_TEAM_MEMBER 9

class Character;

struct TeamByPosition
{
	Character* fu;
	Character* fm;
	Character* fb;

	Character* mu;
	Character* mm;
	Character* mb;

	Character* bu;
	Character* bm;
	Character* bb;
};

union TeamUnion
{
	TeamByPosition m;
	Character* ma[NUM_TEAM_MEMBER];
};

class Team
{
public:
	Team(void);
	~Team(void);

	void update( float dt );
	void clear(void);

	Character** retrieveTeamArray( void ) { return m_team.ma; }
	TeamByPosition& retrieveTeamByPosition( void ) { return m_team.m; }

private:
	TeamUnion m_team;
};

#endif //__GAME_TEAM_H__

