// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsVR.h"
#include "LammpsWorker.h"

std::mutex LammpsWorker::m_internalLock;

#pragma region init
LammpsWorker::LammpsWorker(void* lammps_, _LammpsCommand commandFunction_, std::mutex* lock_)
{
	SetLammpsInstance(lammps_, commandFunction_);
	SetSignalLock(lock_);
	m_thread = nullptr;
}

LammpsWorker::~LammpsWorker()
{
	delete m_thread;
	m_thread = nullptr;
}

void
LammpsWorker::SetLammpsInstance(void* lammps_, _LammpsCommand commandFunction_) {
	m_lammps = lammps_;
	m_lammpsCommand = commandFunction_;
}

void
LammpsWorker::SetSignalLock(std::mutex* lock_) {
	m_signalLock = lock_;

}
#pragma endregion init

#pragma region deployment
void
LammpsWorker::DeployWorker() {
	delete m_thread;
	if (m_lammps) {
		m_thread = FRunnableThread::Create(this, TEXT("LammpsWorker"), 0, TPri_Highest);
	}
	else {
		m_thread = nullptr;
		SignalCompletion();
	}
}

uint32
LammpsWorker::Run() {
	if (m_lammps && m_lammpsCommand) {
		LockThread();
		(*m_lammpsCommand)(m_lammps, "run 1 pre no post no");
		UnlockThread();

		SignalCompletion();
		return 1;
	}
	else {
		return -1;
	}
}
#pragma endregion deployment

#pragma region synch
void 
LammpsWorker::LockThread() {
	m_internalLock.lock();
}

void 
LammpsWorker::UnlockThread() {
	m_internalLock.unlock();
}

void
LammpsWorker::EnsureCompletion() {
	if (m_thread) m_thread->WaitForCompletion();
}

void
LammpsWorker::SignalCompletion() {
	if (m_signalLock) m_signalLock->unlock();
}
#pragma endregion synch

#pragma region unused

bool
LammpsWorker::Init() {
	return true;
}

void
LammpsWorker::Stop() {
	return;
}

#pragma endregion unused
