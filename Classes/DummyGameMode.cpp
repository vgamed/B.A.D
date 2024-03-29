#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "AppDelegate.h"
#include "StateMachine.h"
#include "Character.h"
#include "Hero.h"
#include "Monster.h"
#include "GameMode.h"
#include "Team.h"
#include "DummyGameMode.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;

const Size DummyGameMode::SLOT_OFFSET = Size(100.0f, 5.0f);

const Vec2 DummyGameMode::BF_LT_BTM = Vec2(10.0f, AppDelegate::DESIGN_RESOLUTION.height/6);
const Vec2 DummyGameMode::BF_RT_TOP = Vec2(AppDelegate::DESIGN_RESOLUTION.width-10.0f, 
										   AppDelegate::DESIGN_RESOLUTION.height/3);

const Vec2 DummyGameMode::PLAYER_F_U = Vec2(AppDelegate::DESIGN_RESOLUTION.width/2-DummyGameMode::SLOT_OFFSET.width,
											DummyGameMode::BF_RT_TOP.y-DummyGameMode::SLOT_OFFSET.height);
const Vec2 DummyGameMode::PLAYER_F_M = Vec2(AppDelegate::DESIGN_RESOLUTION.width/2-DummyGameMode::SLOT_OFFSET.width, 
											(DummyGameMode::BF_RT_TOP.y+DummyGameMode::BF_LT_BTM.y)/2);
const Vec2 DummyGameMode::PLAYER_F_B = Vec2(AppDelegate::DESIGN_RESOLUTION.width/2-DummyGameMode::SLOT_OFFSET.width, 
											DummyGameMode::BF_LT_BTM.y+DummyGameMode::SLOT_OFFSET.height);

const Vec2 DummyGameMode::PLAYER_B_U = Vec2(DummyGameMode::SLOT_OFFSET.width, 
											DummyGameMode::BF_RT_TOP.y-DummyGameMode::SLOT_OFFSET.height);
const Vec2 DummyGameMode::PLAYER_B_M = Vec2(DummyGameMode::SLOT_OFFSET.width, 
											(DummyGameMode::BF_RT_TOP.y+DummyGameMode::BF_LT_BTM.y)/2);
const Vec2 DummyGameMode::PLAYER_B_B = Vec2(DummyGameMode::SLOT_OFFSET.width, 
											DummyGameMode::BF_LT_BTM.y+DummyGameMode::SLOT_OFFSET.height);

const Vec2 DummyGameMode::PLAYER_M_U = Vec2((DummyGameMode::PLAYER_F_U.x+DummyGameMode::PLAYER_B_U.x)/2, 
											DummyGameMode::BF_RT_TOP.y-DummyGameMode::SLOT_OFFSET.height);
const Vec2 DummyGameMode::PLAYER_M_M = Vec2((DummyGameMode::PLAYER_F_M.x+DummyGameMode::PLAYER_B_M.x)/2, 
											(DummyGameMode::BF_RT_TOP.y+DummyGameMode::BF_LT_BTM.y)/2);
const Vec2 DummyGameMode::PLAYER_M_B = Vec2((DummyGameMode::PLAYER_F_B.x+DummyGameMode::PLAYER_B_B.x)/2, 
											DummyGameMode::BF_LT_BTM.y+DummyGameMode::SLOT_OFFSET.height);

const Vec2 DummyGameMode::MONSTER_F_U = Vec2(AppDelegate::DESIGN_RESOLUTION.width/2+DummyGameMode::SLOT_OFFSET.width,
								   		DummyGameMode::BF_RT_TOP.y-DummyGameMode::SLOT_OFFSET.height);
const Vec2 DummyGameMode::MONSTER_F_M = Vec2(AppDelegate::DESIGN_RESOLUTION.width/2+DummyGameMode::SLOT_OFFSET.width,
								   		(DummyGameMode::BF_RT_TOP.y+DummyGameMode::BF_LT_BTM.y)/2);
const Vec2 DummyGameMode::MONSTER_F_B = Vec2(AppDelegate::DESIGN_RESOLUTION.width/2+DummyGameMode::SLOT_OFFSET.width,
								   		DummyGameMode::BF_LT_BTM.y+DummyGameMode::SLOT_OFFSET.height);

const Vec2 DummyGameMode::MONSTER_B_U = Vec2(AppDelegate::DESIGN_RESOLUTION.width-DummyGameMode::SLOT_OFFSET.width, 
								   		DummyGameMode::BF_RT_TOP.y-DummyGameMode::SLOT_OFFSET.height);
const Vec2 DummyGameMode::MONSTER_B_M = Vec2(AppDelegate::DESIGN_RESOLUTION.width-DummyGameMode::SLOT_OFFSET.width, 
								   		(DummyGameMode::BF_RT_TOP.y+DummyGameMode::BF_LT_BTM.y)/2);
const Vec2 DummyGameMode::MONSTER_B_B = Vec2(AppDelegate::DESIGN_RESOLUTION.width-DummyGameMode::SLOT_OFFSET.width, 
								   		DummyGameMode::BF_LT_BTM.y+DummyGameMode::SLOT_OFFSET.height);

const Vec2 DummyGameMode::MONSTER_M_U = Vec2((DummyGameMode::MONSTER_F_U.x+DummyGameMode::MONSTER_B_U.x)/2, 
								   		DummyGameMode::BF_RT_TOP.y-DummyGameMode::SLOT_OFFSET.height);
const Vec2 DummyGameMode::MONSTER_M_M = Vec2((DummyGameMode::MONSTER_F_M.x+DummyGameMode::MONSTER_B_M.x)/2, 
								   		(DummyGameMode::BF_RT_TOP.y+DummyGameMode::BF_LT_BTM.y)/2);
const Vec2 DummyGameMode::MONSTER_M_B = Vec2((DummyGameMode::MONSTER_F_B.x+DummyGameMode::MONSTER_B_B.x)/2, 
								   		DummyGameMode::BF_LT_BTM.y+DummyGameMode::SLOT_OFFSET.height);

DummyGameMode::DummyGameMode(void)
{
	resetGame();
}


DummyGameMode::~DummyGameMode(void)
{
	resetGame();
}

bool DummyGameMode::initGame( Layer * layer )
{
	m_layer = layer;

	// load fight scene data 
	Node* node = SceneReader::getInstance()->createNodeWithSceneFile( "publish/FightScene.json" );
	if( !node )
		return false;
	layer->addChild( node, 0, 101 ); //tag 101

	// commented because SceneReader has already loaded these datus
	// read animation data for hero and monster
	//ArmatureDataManager::getInstance()->addArmatureFileInfo( "ani/hero/girl0.png", 
	//	"ani/hero/girl0.plist", "ani/hero/girl.ExportJson" );
	//ArmatureDataManager::getInstance()->addArmatureFileInfo( "ani/enemy/monster0.png", 
	//	"ani/enemy/monster0.plist", "ani/enemy/monster.ExportJson" );

	// create teams
	CharInfo info;
	CharState state;
	Armature* arm;
	char str[12] = {'\0'};

	do{
		info.level = 1;
		info.type = COMBAT_MELEE;
		state.health = state.maxHealth = 1000.0f;
		state.energy = state.maxEnergy = 500.0f;
		state.phyAttack = 500.0f;
		state.mgcAttack = 0.0f;
		state.phyResist = 0.2f;
		state.mgcResist = 1.0f;
		state.energyRecover = 10.0f;
		state.healthRecover = 50.0f;
		state.attackInterval = 1.5f;
		state.attackDistance = 50.0f;
		state.moveSpeed = 300.0f;
		state.facingTo = Vec2( 1.0f, 0.0f );

		info.id = 102;
		sprintf( str, "%s%ld", "Hero", info.id );
		info.name = str;
		state.front = Vec2( -1.0f, 0.0f );
		arm = Armature::create("monster");
		CC_BREAK_IF( !arm );
		m_teamHero.retrieveTeamByPosition().bm = new Hero();
		m_teamHero.retrieveTeamByPosition().bm->init(info, state, arm);

		info.id = 103;
		sprintf( str, "%s%ld", "Hero", info.id );
		info.name = str;
		state.front = Vec2( 1.0f, 0.0f );
		arm = Armature::create("girl");
		CC_BREAK_IF( !arm );
		m_teamHero.retrieveTeamByPosition().fm = new Hero();
		m_teamHero.retrieveTeamByPosition().fm->init(info, state, arm);

		//info.id = 104;
		//sprintf( str, "%s%ld", "Hero", info.id );
		//info.name = str;
		//state.front = Vec2( 1.0f, 0.0f );
		//arm = Armature::create("girl");
		//CC_BREAK_IF( !arm );
		//m_teamHero.retrieveTeamByPosition().mu = new Hero();
		//m_teamHero.retrieveTeamByPosition().mu->init(info, state, arm);

		info.id = 105;
		sprintf( str, "%s%ld", "Hero", info.id );
		info.name = str;
		state.front = Vec2( 1.0f, 0.0f );
		arm = Armature::create("girl");
		CC_BREAK_IF( !arm );
		m_teamHero.retrieveTeamByPosition().mb = new Hero();
		m_teamHero.retrieveTeamByPosition().mb->init(info, state, arm);

		info.level = 1;
		info.type = COMBAT_MELEE;
		state.health = state.maxHealth = 2000.0f;
		state.energy = state.maxEnergy = 500.0f;
		state.phyAttack = 100.0f;
		state.mgcAttack = 0.0f;
		state.phyResist = 0.3f;
		state.mgcResist = 1.0f;
		state.energyRecover = 0.0f;
		state.healthRecover = 0.0f;
		state.attackInterval = 2.0f;
		state.attackDistance = 50.0f;
		state.moveSpeed = 125.0f;
		state.facingTo = Vec2( -1.0f, 0.0f );

		info.id = 202;
		sprintf( str, "%s%ld", "Monster", info.id );
		info.name = str;
		state.front = Vec2( 1.0f, 0.0f );
		arm = Armature::create("girl");
		CC_BREAK_IF( !arm );
		m_teamMonster.retrieveTeamByPosition().bm = new Monster();
		m_teamMonster.retrieveTeamByPosition().bm->init(info, state, arm);

		//info.id = 203;
		//sprintf( str, "%s%ld", "Monster", info.id );
		//info.name = str;
		//state.front = Vec2( -1.0f, 0.0f );
		//arm = Armature::create("monster");
		//CC_BREAK_IF( !arm );
		//m_teamMonster.retrieveTeamByPosition().fm = new Monster();
		//m_teamMonster.retrieveTeamByPosition().fm->init(info, state, arm);

		info.id = 204;
		sprintf( str, "%s%ld", "Monster", info.id );
		info.name = str;
		state.front = Vec2( -1.0f, 0.0f );
		arm = Armature::create("monster");
		CC_BREAK_IF( !arm );
		m_teamMonster.retrieveTeamByPosition().fu = new Monster();
		m_teamMonster.retrieveTeamByPosition().fu->init(info, state, arm);

		info.id = 205;
		sprintf( str, "%s%ld", "Monster", info.id );
		info.name = str;
		state.front = Vec2( -1.0f, 0.0f );
		arm = Armature::create("monster");
		CC_BREAK_IF( !arm );
		m_teamMonster.retrieveTeamByPosition().fb= new Monster();
		m_teamMonster.retrieveTeamByPosition().fb->init(info, state, arm);

		return true;
	}while(0);

	resetGame();

	return false;
}

bool DummyGameMode::startGame(void)
{
	if( !isGameStarted() )
	{
		// deploy teams into battle field
		m_teamHero.retrieveTeamByPosition().fm->deploy( m_layer, PLAYER_F_M );
		//m_teamHero.retrieveTeamByPosition().mu->deploy( m_layer, PLAYER_M_U );
		m_teamHero.retrieveTeamByPosition().mb->deploy( m_layer, PLAYER_M_B );
		m_teamHero.retrieveTeamByPosition().bm->deploy( m_layer, PLAYER_B_M );

		m_teamMonster.retrieveTeamByPosition().fu->deploy( m_layer, MONSTER_F_U );
		//m_teamMonster.retrieveTeamByPosition().fm->deploy( m_layer, MONSTER_F_M );
		m_teamMonster.retrieveTeamByPosition().fb->deploy( m_layer, MONSTER_F_B );
		m_teamMonster.retrieveTeamByPosition().bm->deploy( m_layer, MONSTER_B_M );

		setGameStarted( true );
	}
	return isGameStarted();
}

void DummyGameMode::updateGame(float dt)
{
	if( isGameStarted() )
	{
		m_teamHero.update( dt );
		m_teamMonster.update( dt );
	}
}

void DummyGameMode::endGame(void)
{
	setGameStarted(false);
}

void DummyGameMode::resetGame(void)
{
	setGameStarted(false);

	m_layer = nullptr;
	m_teamMonster.clear();
	m_teamHero.clear();
}

int DummyGameMode::calcZOrder( float y )
{
	float rate = (BF_RT_TOP.y - y) / (BF_RT_TOP.y - BF_LT_BTM.y);
	return (int)(rate*100);
}
