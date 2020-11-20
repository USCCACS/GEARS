// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LammpsWorker.h"
/**
 * 
 */
class LAMMPSVR_API LammpsRerunWorker : public LammpsWorker
{
public:
	LammpsRerunWorker(void* lammps_, _LammpsCommand commandFunction_, std::mutex* lock_, FString rootDir_) : 
		LammpsWorker(lammps_, commandFunction_, lock_), m_rootDir(rootDir_) {}
	LammpsRerunWorker();
	~LammpsRerunWorker();

	virtual uint32 Run() override;
	virtual void SetupAnimationState(FString dumpfilePrefix_, uint32 firstTimeStep_, uint32 lastTimeStep_, uint32 stepSize_);

private:
	virtual void SetCurrentLammpsCommand() override;
	void IncrementTimeStep();

	FString GetCurrentDumpFilename();
	bool CurrentDumpFileExists();

protected:
	uint32 m_firstTimeStep = 0;
	uint32 m_lastTimeStep = 0;
	uint32 m_currentTimeStep = 0;
	uint32 m_stepSize = 0;

	FString m_rootDir;
	FString m_dumpFilePrefix = "";
};
