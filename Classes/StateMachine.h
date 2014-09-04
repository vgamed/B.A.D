#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

template <class T>
class State
{
public:
	virtual ~State(void) {}
	virtual void enter( T* t ) = 0;
	virtual void exec( T* t, float dt ) = 0;
	virtual void exit( T* t ) = 0;
};

template <class T>
class StateMachine
{
public:
	StateMachine( T* owner )
		: m_pOwner(owner)
		, m_pPreState(nullptr)
		, m_pCurState(nullptr)
		, m_pGlobalState(nullptr){}

	/**
	 * @brief set the current state
	 * @param state pointer of the new state
	 * @return void
	 */
	void setCurrentState( State<T>* state ) 
	{
		m_pCurState = state; 
		if( m_pCurState )
			m_pCurState->enter( m_pOwner );
	}
	/**
	 * @brief	set the global state.
	 *			set to null to reset the whole state machine.
	 * @param state pointer of the new state
	 * @return void
	 */
	void setGlobalState( State<T>* state ) 
	{
		changeState( nullptr );
		setPreviousState( nullptr );

		m_pGlobalState = state; 
		if( m_pGlobalState )
			m_pGlobalState->enter( m_pOwner );
	}
	/**
	 * @brief set the previous state
	 * @param state pointer of the new state
	 * @return void
	 */
	void setPreviousState( State<T>* state ) { m_pPreState = state; }
	/**
	 * @brief tick the machine
	 * @param void
	 * @return void
	 */
	void update( float dt ) 
	{
		if( m_pGlobalState )
			m_pGlobalState->exec( m_pOwner, dt );

		if( m_pCurState )
			m_pCurState->exec( m_pOwner, dt );
	}
	/**
	 * @brief change the current state to a new state
	 * @param state pointer of the new state
	 * @return void
	 */
	void changeState( State<T>* state )
	{
		if( m_pCurState )
			m_pCurState->exit( m_pOwner );

		setPreviousState( m_pCurState );
		setCurrentState( state );
	}
	/**
	 * @brief	change the global state to a new state
	 *			and reset current state and previous state to null
	 * @param state pointer of the new state
	 * @return void
	 */
	void changeGlobalState( State<T>* state )
	{
		if( m_pGlobalState )
			m_pGlobalState->exit( m_pOwner );

		setGlobalState( state );
	}
	/**
	 * @brief revert to the previous state
	 * @param void
	 * @return void
	 */
	void revertToPreviousState( void )
	{
		changeState( m_pPreState );
	}

	/**
	 * @brief get the pointer of the current state
	 * @param void
	 * @return pointer of the current state
	 */
	State<T>* getCurrentState( void ) { return m_pCurState; }
	/**
	 * @brief get the pointer of the previous state
	 * @param void
	 * @return pointer of the previous state
	 */
	State<T>* getPreviousState( void ) { return m_pPreState; }
	/**
	 * @brief get the pointer of the global state
	 * @param void
	 * @return pointer of the global state
	 */
	State<T>* getGlobalState( void ) { return m_pGlobalState; }
	/**
	 * @brief tell if the owner is in the state indicated by the parameter
	 * @param state the state reference for comparison
	 * @return bool true or false
	 */
	bool isInState( State<T>* state ) 
	{
		return (m_pCurState==state); 
	}


private:
	T* m_pOwner; // owner of the state machine
	State<T>* m_pPreState; // pointer to the previous state
	State<T>* m_pCurState; // pointer to the current state
	State<T>* m_pGlobalState; // pointer to the global state
};

#endif //__STATE_MACHINE_H__

