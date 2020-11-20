// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsRerunWorker.h"
// #include "LammpsVR.h"

LammpsRerunWorker::LammpsRerunWorker()
{
}

LammpsRerunWorker::~LammpsRerunWorker()
{
}

#pragma region animation
void
LammpsRerunWorker::SetupAnimationState(FString dumpFilePrefix_, uint32 firstTimeStep_, uint32 lastTimeStep_, uint32 stepSize_) {
	m_dumpFilePrefix = dumpFilePrefix_;
	m_firstTimeStep = firstTimeStep_;
	m_lastTimeStep = lastTimeStep_;
	m_stepSize = stepSize_;

	m_currentTimeStep = m_firstTimeStep;
}

void
LammpsRerunWorker::IncrementTimeStep() {
	m_currentTimeStep += m_stepSize;
	if (m_currentTimeStep > m_lastTimeStep) m_currentTimeStep = m_firstTimeStep;
}
#pragma endregion animation

#pragma region dumpfile
void
LammpsRerunWorker::SetCurrentLammpsCommand() {
	sprintf(m_currentLammpsCommand, "rerun %s first %u last %u dump x y z",
		TCHAR_TO_ANSI(*GetCurrentDumpFilename()), m_currentTimeStep, m_currentTimeStep + 10);
}

FString
LammpsRerunWorker::GetCurrentDumpFilename() {
	return FString("LammpsDump/") + m_dumpFilePrefix + "." + FString::FromInt(m_currentTimeStep) + FString(".dump");
}

bool
LammpsRerunWorker::CurrentDumpFileExists() {
	FString filePath = m_rootDir + FString("/") + GetCurrentDumpFilename();
	return FPaths::FileExists(filePath);
}
#pragma endregion dumpfile

#pragma region deployment
uint32
LammpsRerunWorker::Run() {
	LockThread();
	{
		if ((m_lammps != nullptr)
			&& (m_lammpsCommand != nullptr)
			&& (CurrentDumpFileExists()))
		{
			//UE_LOG(LogTemp, Log, TEXT("%s"), *FString(m_currentLammpsCommand));
			SetCurrentLammpsCommand();
			RunCurrentLammpsCommand();
		}

		IncrementTimeStep();
		SignalCompletion();
	}
	UnlockThread();

	return 1;
}
#pragma endregion deployment
