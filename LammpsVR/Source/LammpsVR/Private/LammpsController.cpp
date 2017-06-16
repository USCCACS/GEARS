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
	m_ParticleVisualizationManager = nullptr;

	m_spawnParams.Owner = this;
	m_spawnParams.Instigator = Instigator;

	m_paused = false;
}

// Called when the game starts or when spawned
void ALammpsController::BeginPlay()
{
	Super::BeginPlay();
//	bool success = ImportLammps(FString("LammpsDll"), m_dllName);
//
//	if (success) {
//		RunLammpsScript(m_scriptName);
//
//		m_lammpsWorker = new LammpsWorker(m_lammps, m_lammpsCommand, &m_lammpsLock);
//		m_ParticleVisualizationManager = GetWorld()->SpawnActor<AParticleVisualizationManager>(m_managerReference, GetTransform(), m_spawnParams);
//
///************************** FOR THE CACS CREW ***************************************************************/
//#pragma region ADJUST_PARTICLES_HERE
//		{	
//			/* Add New Particle Types ************************************************************************************/
//			/* 
//			 * To add a new type of particle to the system with a specific lammps type number, do it here. If you do not
//			 * add a new particle type yourself, my system will create one for you when it reads lammps, but it will assign
//			 * it a random color and a radius of 0.25f (Hydrogen).
//			 * To add a new type of particle, simply call the ManageNewParticleType() member function of the 
//			 * ParticleVisualizationManager. Right now, the only input paramters that will work are a color and radius of
//			 * variable type FColor and float (respectively). Here is an example:
//				* m_ParticleVisualizationManager->ManageNewParticleType(1, FColor::Yellow, 1.11f);
//				* m_ParticleVisualizationManager->ManageNewParticleType(2, FColor::Red, 0.6f);
//			 */
//
//			// Insert Code Here
//			m_ParticleVisualizationManager->ManageNewParticleType(1, FColor::Yellow, 1.11f);
//			m_ParticleVisualizationManager->ManageNewParticleType(2, FColor::Red, 0.6f);
//
//			/************************************************************************************************************/
//		}
//		{
//			/* Adjust System Scale* ************************************************************************************/
//			/*
//			* You can also adjust the scale of the spacing between atoms (or rather, the factor by which the position
//			* coordinates get scaled by when reading from lammps. You can adjust this value in the blueprint editor for 
//			* BP_Particle (i.e. the blueprint version of the AParticle class. However, doing it via code works just as well.
//			* Having too small a system scale may cause billboards to weirdly overlap with each other.
//			* Here is an example:
//			* m_ParticleVisualizationManager->SetSystemScale(100.0f);
//			*/
//
//			// Insert Code Here
//			m_ParticleVisualizationManager->SetSystemScale(200.0f);		// I found that 200.0f is a good scale for the porous
//																		// Vashishta potential. But change it if you feel it's
//																		// for the given lammps script
//
//			/************************************************************************************************************/
//		}
//			/*
//			 * REMEMBER to compile once you've made changes to the system. To do that, click the "Compile" button at the top
//			 * of the Unreal Editor window. You can also right-click "LammpsVR" in the SolutionExplorer (the right panel in 
//			 * visual studio) and select "Build" or "Rebuild". However, you may need to restart the Unreal Editor for these
//			 * changes to take effect.
//			 *
//			 * ALSO here's a neat debugging tool to check the frame rate of the game:
//			 * In the editor, type the '`' key (tick), then type "stat unit" then press ENTER. Do the same, but type in 
//			 * "stat FPS" instead. Those numbers will help lots. :)
//			 */
//#pragma endregion ADJUST_PARTICLES_HERE
///************************** FOR THE CACS CREW ***************************************************************/
//
//		m_ParticleVisualizationManager->InitWithLammps(m_lammps, m_lammpsExtractGlobal, m_lammpsExtractAtom);
//	}
}

// Called when the game endss or when destroyed
void ALammpsController::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	//(*m_lammpsClose)(m_lammps);
	m_lammps = nullptr;
	m_lammpsOpenNoMPI = nullptr;
	m_lammpsCommand = nullptr;
	m_lammpsExtractGlobal = nullptr;
	m_lammpsExtractAtom = nullptr;
	m_lammpsClose = nullptr;

	delete m_lammpsWorker;
	m_lammpsWorker = nullptr;

	m_ParticleVisualizationManager->Destroy();
}

// Called every frame
void ALammpsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LammpsIsActive()) {
		if (m_lammpsLock.try_lock()) {
			m_ParticleVisualizationManager->Update();
			m_lammpsWorker->DeployWorker();
		}
	}
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

	FString scriptPath = FPaths::GameDir() + "/Scripts/" + scriptName_;
	FString script = "";

	FFileHelper::LoadFileToString(script, *scriptPath);
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::White, *script);

	return script;
}

bool
ALammpsController::LammpsIsActive() {
	return m_lammps &&
		m_lammpsOpenNoMPI &&
		m_lammpsCommand &&
		m_lammpsExtractGlobal &&
		m_lammpsExtractAtom &&
		m_lammpsClose;
}

#pragma region BLUEPRINT
/* 
 * Starts up the Lammps script, and then instantiates the Lammps worker and the particle visualization manager. 
 * This is meant to be called from the Blueprint Editor before spawning the particles in the scene.
 */
void 
ALammpsController::InitializeWorkerAndParticleVisualizationManager() {
	RunLammpsScript(m_scriptName);

	m_lammpsWorker = new LammpsWorker(m_lammps, m_lammpsCommand, &m_lammpsLock);
	m_ParticleVisualizationManager = GetWorld()->SpawnActor<AParticleVisualizationManager>(m_managerReference, GetTransform(), m_spawnParams);
	m_ParticleVisualizationManager->SetActorRelativeLocation(FVector::ZeroVector);
}

/*
* Starts up the Lammps script, and then instantiates the Lammps worker and the particle visualization manager.
* This is meant to be called from the Blueprint Editor after spawning the particles in the scene.
*/
void 
ALammpsController::SynchronizeLammpsAndParticleVisualizationManager() {
	if (m_lammps) 
		m_ParticleVisualizationManager->InitWithLammps(m_lammps, m_lammpsExtractGlobal, m_lammpsExtractAtom);
}

void 
ALammpsController::ManageParticle(int32 type_, FColor color_, float radius_) {
	if (m_ParticleVisualizationManager)
		m_ParticleVisualizationManager->ManageNewParticleType(type_, color_, radius_);
}

void
ALammpsController::SetSystemScale(float scale_) {
	if (m_ParticleVisualizationManager)
		m_ParticleVisualizationManager->SetSystemScale(scale_);
}
#pragma endregion BLUEPRINT
#pragma endregion LAMMPS_IMPORT

