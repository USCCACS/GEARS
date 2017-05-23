// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsVR.h"
#include "LammpsController.h"


// Sets default values
ALammpsController::ALammpsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_lammps = nullptr;
	m_dllHandle = nullptr;

	m_lammpsOpenNoMPI = nullptr;
	m_lammpsCommand = nullptr;
	m_lammpsExtractGlobal = nullptr;
	m_lammpsExtractAtom = nullptr;
	m_lammpsClose = nullptr;

	m_lammpsWorker = nullptr;
	m_pvm = nullptr;

	m_paused = false;
}

// Called when the game starts or when spawned
void ALammpsController::BeginPlay()
{
	Super::BeginPlay();
	bool success = ImportLammps(FString("LammpsDll"), FString("lammpslib.dll"));

	if (success) {
		m_lammpsWorker = new LammpsWorker(m_lammps, m_lammpsCommand, &m_lammpsLock);
		m_pvm = new AParticleVisualizationManager();
	}

	// This is temporary. Remove once we let LammpsRunner handle generation of new atoms 
	//this->SpawnNewAtom(FVector::ZeroVector, FColor::Yellow, -150.0f, 300.0f);
	//this->SpawnNewAtom(FVector::ZeroVector, FColor::Red, -100.0f, 200.0f);
	//m_atoms[0]->SetRadius(10.0f);

	// Initialize the number of atoms by type 
	// Note that lammps types start from 1, but my types are indexed from 0
	//int atomCount = *((int*)(*m_lammpsExtractGlobal)(m_lammps, "natoms"));
	//int* types = (int*)(*m_lammpsExtractAtom)(m_lammps, "type");
	//for (int i = 0; i < atomCount; ++i) {
		//m_atoms[types[i] - 1]->AddInstance(FVector::ZeroVector);
	//}
}

// Called when the game endss or when destroyed
void ALammpsController::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	delete m_lammpsWorker;
	delete m_pvm;
}

// Called every frame
void ALammpsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/**************************************************************************************************
 ******** MAGICS Written Member Functions *********************************************************
 **************************************************************************************************/

#pragma region LAMMPS_IMPORT
bool 
ALammpsController::ImportLammps(FString folder_, FString filename_) {
	FString filePath = *FPaths::GameDir() + folder_ + "/" + filename_;
	if (FPaths::FileExists(filePath)) {
		m_dllHandle = FPlatformProcess::GetDllHandle(*filePath);
		if (m_dllHandle == NULL) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Dll failed to import.");
			return false;
		}

		if (ImportDllFunction<_LammpsOpenNoMPI>("lammps_open_no_mpi", m_lammpsOpenNoMPI)
			&& ImportDllFunction<_LammpsCommand>("lammps_command", m_lammpsCommand)
			&& ImportDllFunction<_LammpsExtract>("lammps_extract_global", m_lammpsExtractGlobal)
			&& ImportDllFunction<_LammpsExtract>("lammps_extract_atom", m_lammpsExtractAtom)
			&& ImportDllFunction<_LammpsClose>("lammps_close", m_lammpsClose))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Dll and functions successfully imported.");
			return true;
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Lammps Dll File Doesn't Exist");
	}

	return false;
}

void
ALammpsController::RunLammpsScript(FString scriptName_) {
	// Open/Startup Lammps
	(*m_lammpsOpenNoMPI)(0, nullptr, &m_lammps);

	// Parse Initial Script
	TArray<FString> lines;
	FString script = ReadLammpsScript(scriptName_);

	script.ParseIntoArrayLines(lines);

	// Execute Script line by line
	for (int i = 0; i < lines.Num(); ++i) {
		(*m_lammpsCommand)(m_lammps, TCHAR_TO_ANSI(*lines[i]));
	}
}

FString 
ALammpsController::ReadLammpsScript(FString scriptName_) {
	FString gameDir = FPaths::GameDir();
	_wchdir(*gameDir);

	FString scriptPath = FPaths::GameDir() + "/" + scriptName_;
	FString script = "";

	FFileHelper::LoadFileToString(script, *scriptPath);
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::White, *script);

	return script;
}
#pragma endregion LAMMPS_IMPORT

