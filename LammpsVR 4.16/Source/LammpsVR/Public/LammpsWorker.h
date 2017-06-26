// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LammpsTypes.h"
#include "Worker.h"
/**
 * Worker class that runs a LAMMPS iteration on a separate thread. Ensure that the user (LammpsController) can continue 
 * to function, instead of wait for a large LAMMPS computation to finish. 
 */
class LAMMPSVR_API LammpsWorker : public Worker
{
public:
	LammpsWorker(void* lammps_, _LammpsCommand commandFunction_, std::mutex* lock_);
	LammpsWorker();
	~LammpsWorker();

	void SetLammpsInstance(void* lammps_, _LammpsCommand commandFunction_);

	/* Runnable Interface (all must be defined, but only run is used) */
	virtual uint32 Run();
	virtual void DeployWorker();

	virtual void SetCurrentLammpsCommand();
	virtual void RunCurrentLammpsCommand();

protected:
	/* Lammps Related */
	void* m_lammps;
	_LammpsCommand m_lammpsCommand;
	char m_currentLammpsCommand[256];

	/* Thread Tracking */
	int64 m_threadID;
};
