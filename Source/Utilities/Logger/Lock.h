//
// Lock.h
//		Author: Joel Cioffi
//		Authored: 1/18/2012
//		Purpose: This class is a mutual exclusion lock. If a lock object
//				 is locked, it cannot be acquired by any other thread.
#ifndef JCLOCK_H_
#define JCLOCK_H_

#include "../../PS/config.h"

// namespace Cioffi
// {
	class Lock
	{
	private:
		volatile bool _locked;
		CRITICAL_SECTION _lockobj;
	public:
		// Constructor and Destructor
		Lock();
		virtual ~Lock();

		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will acquire the lock on the calling
		//			thread. If the lock is locked by the calling thread,
		//			nothing happens. If the lock is locked by another
		//			thread, the calling thread will block until it is
		//			able to acquire the lock.
		//
		//	NOTE:	THIS FUNCTION BLOCKS
		void Acquire();
		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will release a lock that is locked by
		//			a thread, allowing other threads to lock it. If Release 
		//			is called more times than Acquire, the CRITICAL_SECTION
		//			will be marked as garbage, please avoid this.
		void Release();
		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will attempt to acquire a lock, without
		//			blocking if the lock is already acquired. This function
		//			returns immediately.
		//
		// Returns: True if the lock was successfully acquired by calling thread
		//			False if the lock is already locked by another thread
		bool TryAcquire();

		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will return the current state of the lock.
		//			There is no guarantee that this value will not change if
		//			called twice in a row. If you want to check the state before
		//			calling Acquire, use TryAcquire.
		//
		// Returns: True if the lock is acquired by a thread
		//			False if the lock is open to be acquired
		bool Locked() const;
	};
//};

#endif