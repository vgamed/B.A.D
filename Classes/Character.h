#ifndef __GAME_CHARACTER_H__
#define __GAME_CHARACTER_H__

enum CombatType
{
	COMBAT_NONE = 0,
	COMBAT_MELEE = 1,
	COMBAT_RANGE_NEAR = 2,
	COMBAT_RANGE_FAR = 3,
	COMBAT_MAX
};

struct CharInfo
{
	long id;
	std::string name;
	std::string desc; // brief description
	CombatType type;
	unsigned int level;
	unsigned long exp; //experience
	unsigned short star;
	unsigned short color;
};

struct CharState
{
	float health;
	float maxHealth;
	float healthRecover; // health recover speed
	float energy;
	float maxEnergy;
	float energyRecover; // energy recover speed
	float phyAttack; // physical attack power
	float mgcAttack; // magical attack power
	float phyResist; // phyical resistance
	float mgcResist; // magical resistance
	float attackInterval;
	float attackDistance;
	float moveSpeed;
	cocos2d::Vec2 front;
	cocos2d::Vec2 facingTo;
};

template<class T> class StateMachine;

class Character : public cocos2d::Ref
{
public:
	static const cocos2d::Size PHYSICS_BOX_SIZE;
	static const float SCALE_X;
	static const float SCALE_Y;

	Character(void);
	virtual ~Character(void);

	// interfaces
	virtual void init( const CharInfo& info, const CharState& basicState,
		cocostudio::Armature* arm = nullptr,
		cocos2d::PhysicsBody* body = nullptr );

	virtual void deploy( cocos2d::Layer* layer, const cocos2d::Vec2& pos ) = 0;
	virtual void update( float dt ) = 0;
	virtual	void onCustomEvent( cocos2d::EventCustom* event ) = 0;
	virtual void onAnimationEvent( cocostudio::Armature* arm, 
		cocostudio::MovementEventType type, const std::string& movementID ) = 0;

	// getters and setters
	const CharInfo& getCharInfo(void) { return m_info; }
	void setCharInfo( const CharInfo& info ) { m_info = info; }

	const CharState& getBasicCharState(void) { return m_basicState; }
	void setBasicCharState( const CharState& state )
	{
		m_basicState = state; 
		calcActualState();
	}

	const CharState& getActualCharState(void) { return m_actualState; }
	void setActualCharState( const CharState& state ) { m_actualState = state; }

	Character* getTarget(void) { return m_pTarget; }
	void setTarget( Character * target ) { m_pTarget = target; }

	cocostudio::Armature* getArmature(void) { return m_pArmature; }
	void setArmature( cocostudio::Armature* arm, cocos2d::PhysicsBody* body = nullptr ) 
	{
		if( m_pArmature )
			m_pArmature->release(); 

		m_pArmature = arm;

		if( m_pArmature )
		{
			m_pArmature->retain(); 
			if( body )
				m_pArmature->setPhysicsBody( body );
		}
	}

	StateMachine<Character> * getStateMachine(void) { return m_pStateMachine; }
	
	// utilities
	bool isDead() { return (m_actualState.health <= 0); }
	void reset( void );

	virtual cocos2d::Vec2 calcMoveTo( void );

protected:
	CharInfo m_info;
	CharState m_basicState;
	CharState m_actualState;

	cocostudio::Armature* m_pArmature;
	cocos2d::PhysicsBody* m_phyBody;

	Character * m_pTarget;
	StateMachine<Character>* m_pStateMachine;
	std::vector<State<Character>*> m_vecStates;

private:
	virtual void calcActualState(void) = 0;
	virtual void doDamage( float phy_atk, float mgc_atk ) = 0;
};

#endif //GAME_CHARACTER_H__

