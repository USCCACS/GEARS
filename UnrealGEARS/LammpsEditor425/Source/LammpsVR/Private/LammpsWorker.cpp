// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsWorker.h"
// #include "LammpsVR.h"

//std::mutex LammpsWorker::m_internalLock;

#pragma region init
LammpsWorker::LammpsWorker(void* lammps_, _LammpsCommand commandFunction_, std::mutex* lock_)
{
	SetLammpsInstance(lammps_, commandFunction_);
	SetSignalLock(lock_);
	m_thread = nullptr;
	m_threadID = 0;
}

LammpsWorker::LammpsWorker() {
	SetLammpsInstance(nullptr, nullptr);
	m_thread = nullptr;
	m_threadID = 0;
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
	SetCurrentLammpsCommand();
}

void
LammpsWorker::SetCurrentLammpsCommand() {
	strcpy(m_currentLammpsCommand, "run 1 pre no post no");
}

void
LammpsWorker::RunCurrentLammpsCommand() {
	(*m_lammpsCommand)(m_lammps, m_currentLammpsCommand);
}
#pragma endregion init

#pragma region deployment
void
LammpsWorker::DeployWorker() {
	delete m_thread;
	if (m_lammps) {
		FString threadName = "LammpsWorker" + m_threadID;
		++m_threadID;
		m_thread = FRunnableThread::Create(this, *threadName, 0, TPri_Highest);
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
		RunCurrentLammpsCommand();
		UnlockThread();

		SignalCompletion();
		return 1;
	}
	else {
		return -1;
	}
}
#pragma endregion deployment

