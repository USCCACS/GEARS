// Fill out your copyright notice in the Description page of Project Settings.

#include "XYZWorker.h"
// #include "LammpsVR.h"

XYZWorker::XYZWorker()
{
}

XYZWorker::~XYZWorker()
{
}

void 
XYZWorker::DeployWorker() {
	//TODO
}

uint32
XYZWorker::Run() {
	
	LockThread();

	// TODO: Parse XYZ Frame/Files here
	
	UnlockThread();

	SignalCompletion();
	return 1;
}

TMap<int32, TArray<FVector> >* 
XYZWorker::RequestPositions() {
	// TODO
	return nullptr;
}