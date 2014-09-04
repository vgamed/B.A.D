#ifndef __GAME_TEAM_H__
#define __GAME_TEAM_H__

#define NUM_TEAM_MEMBER 9

class Character;

struct Member
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

union TeamMember
{
	struct Member m;
	Character* ma[NUM_TEAM_MEMBER];
};

class Team
{
public:
	Team(void);
	~Team(void);

	void update( float dt );
	void deploy( cocos2d::Layer* layer );
	void clear(void);

	TeamMember tm;
};

#endif //__GAME_TEAM_H__

