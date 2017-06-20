// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <mutex>
/**
 * 
 */
class LAMMPSVR_API Worker : public FRunnable
{
public:
	Worker();
	~Worker();

	virtual void DeployWorker() = 0;
	virtual void SetSignalLock(std::mutex* lock_);

	/* Runnable Interface (all must be defined, but only run is used) */
	virtual bool Init();
	virtual uint32 Run() = 0;
	virtual void Stop();

	/* Clean up Functions */
	void EnsureCompletion();
	void SignalCompletion();

	/* Internal Synchronization */
	static void LockThread();
	static void UnlockThread();

protected:
	/* Multi-Threading Related */
	FRunnableThread* m_thread;              // Instance of the thread that actually run separate from unreal
	std::mutex* m_signalLock;				// Unlocking this signals to the LammpsController that an iteration has finished
	static std::mutex m_internalLock;		// Just in case
};
