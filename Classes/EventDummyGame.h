#ifndef __EVENT_DUMMYGAME_H__
#define __EVENT_DUMMYGAME_H__

class Character;

class EventDummyGame :
	public cocos2d::EventCustom
{
public:
	enum DGEType
	{
		DGE_NONE = 0,
		DGE_ATTACK_SINGLE,
		DGE_ATTACK_ALL,
		DGE_ATTACK_SPECIAL,
		DGE_DISABLE_SKILL,
		DGE_DIZZY_SKILL,
		DGE_MAX
	};

	struct DGEInfo
	{
		DGEType type;
		Character* sender;
		Character* receiver;
	};

	EventDummyGame( const DGEInfo& info );
	~EventDummyGame(void);

	DGEType type(void) { return m_info.type; }
	Character* sender(void) { return m_info.sender; }
	Character* receiver(void) { return m_info.receiver; }

	const DGEInfo& getEventInfo(void)
	{
		return m_info; 
	}

	void setEventInfo( const DGEInfo& info )
	{
		m_info = info;
		setUserData( &m_info );
	}

private:
	DGEInfo m_info;
};

#endif //__EVENT_DUMMYGAME_H__

