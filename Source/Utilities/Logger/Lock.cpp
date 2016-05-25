//
// Lock.h
//		Author: Joel Cioffi
//		Authored: 1/18/2012
//		Purpose: This class is a mutual exclusion lock. If a lock object
//				 is locked, it cannot be acquired by any other thread.
#include "Lock.h"

 //namespace Cioffi
// {
	Lock::Lock()
	{
		InitializeCriticalSection(&_lockobj);
		_locked = false;
	}

	Lock::~Lock()
	{ DeleteCriticalSection(&_lockobj); }

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will acquire the lock on the calling
	//			thread. If the lock is locked by the calling thread,
	//			nothing happens. If the lock is locked by another
	//			thread, the calling thread will block until it is
	//			able to acquire the lock.
	//
	//	NOTE:	THIS FUNCTION BLOCKS
	void Lock::Acquire()
	{
		EnterCriticalSection(&_lockobj);
		_locked = true;
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will release a lock that is locked by
	//			a thread, allowing other threads to lock it. If Release 
	//			is called more times than Acquire, the CRITICAL_SECTION
	//			will be marked as garbage, please avoid this.
	void Lock::Release()
	{
		LeaveCriticalSection(&_lockobj);
		_locked = false;
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will attempt to acquire a lock, without
	//			blocking if the lock is already acquired. This function
	//			returns immediately.
	//
	// Returns: True if the lock was successfully acquired by calling thread
	//			False if the lock is already locked by another thread
	bool Lock::TryAcquire()
	{
		// stupid microsoft BOOL type
		bool gotten = TryEnterCriticalSection(&_lockobj)?true:false;

		if (gotten)
			_locked = true;
		return gotten;
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will return the current state of the lock.
	//			There is no guarantee that this value will not change if
	//			called twice in a row. If you want to check the state before
	//			calling Acquire, use TryAcquire.
	//
	// Returns: True if the lock is acquired by a thread
	//			False if the lock is open to be acquired
	bool Lock::Locked() const
	{ return _locked; }
//}