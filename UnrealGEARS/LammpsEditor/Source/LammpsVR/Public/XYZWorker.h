// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Worker.h"
/**
* Worker class that reads cooridnates and atom types from an XYZ file on a separate thread.
* When the worker is done, the LammpsController (or whoever deploys the worker) can
* then access and visualize the parsed coordinates 
*/
class LAMMPSVR_API XYZWorker : Worker
{
public:
	XYZWorker();
	~XYZWorker();

	virtual void DeployWorker();
	virtual uint32 Run();

	TMap<int32, TArray<FVector> >* RequestPositions();
};
