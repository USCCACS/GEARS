// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsController.h"
// #include "LammpsVR.h"

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
	m_spawnParams.Instigator = GetInstigator();

	m_paused = false;
}

// Called when the game starts or when spawned
void ALammpsController::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the game endss or when destroyed
void ALammpsController::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

  if (m_ParticleVisualizationManager != nullptr)
    m_ParticleVisualizationManager->Destroy();

  if (m_lammpsWorker != nullptr) 
    m_lammpsWorker->EnsureCompletion();
  delete m_lammpsWorker;
  m_lammpsWorker = nullptr;

  if (LammpsIsActive())
	(*m_lammpsClose)(m_lammps);

	m_lammps = nullptr;
	m_lammpsOpenNoMPI = nullptr;
	m_lammpsCommand = nullptr;
	m_lammpsExtractGlobal = nullptr;
	m_lammpsExtractAtom = nullptr;
	m_lammpsClose = nullptr;

}

// Called every frame
void ALammpsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LammpsIsActive() && !m_paused) {
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
	
	FString filePath = m_ResourceDir + folder_ + "/" + filename_;
	GEngine->AddOnScreenDebugMessage(11, 10, FColor::Cyan, filePath);

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
	_wchdir(*m_ResourceDir);

	FString scriptPath = m_ResourceDir + "/Scripts/" + scriptName_;
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

void
ALammpsController::Pause() {
  m_paused = !m_paused;
}

#pragma region BLUEPRINT
/* 
 * Starts up the Lammps script, and then instantiates the Lammps worker and the particle visualization manager. 
 * This is meant to be called from the Blueprint Editor before spawning the particles in the scene.
 */
void 
ALammpsController::InitializeWorkerAndParticleVisualizationManager() {
	// Runs lammps script, including opening lammps 
	RunLammpsScript(m_scriptName);

	if (LammpsIsActive()) {
		// Initialize the correct worker depending on the M_ANIMATIONMODE boolean
		m_lammpsWorker = m_animationMode ? new LammpsRerunWorker(m_lammps, m_lammpsCommand, &m_lammpsLock, m_ResourceDir) :
										   new LammpsWorker(m_lammps, m_lammpsCommand, &m_lammpsLock);

		// Initialize the particle visualization system
		m_ParticleVisualizationManager = GetWorld()->SpawnActor<AParticleVisualizationManager>(m_managerReference, GetTransform(), m_spawnParams);
		m_ParticleVisualizationManager->SetActorRelativeLocation(FVector::ZeroVector);

		// Add the custom chosen particle parameters. Otherwise, default values will be used
		for (auto Instance : m_ManagedParticles) {
			float radius = Instance.Value.Radius;
			FColor color = Instance.Value.Color;
			m_ParticleVisualizationManager->ManageNewParticleType(Instance.Key, color, radius);
		}

		// Synchronize the particle visualization manager with Lammps
		m_ParticleVisualizationManager->InitWithLammps(m_lammps, m_lammpsExtractGlobal, m_lammpsExtractAtom);
	}
	else {
		GEngine->AddOnScreenDebugMessage(666, 30.0f, FColor::Red, "Error: Can't initialize particle visualization, because lammps Dll failed",
			true, FVector2D(2.0f, 2.0f));
	}
	
}

void
ALammpsController::SetupAnimationState(FString dumpfilePrefix_, int32 firstTimeStep_, int32 lastTimeStep_, int32 stepSize_) {
	LammpsRerunWorker* rerunWorker = static_cast<LammpsRerunWorker*>(m_lammpsWorker);
	rerunWorker->SetupAnimationState(dumpfilePrefix_, firstTimeStep_, lastTimeStep_, stepSize_);
}

/*
* Starts up the Lammps script, and then instantiates the Lammps worker and the particle visualization manager.
* These are wrappers meant to be accessed from the Blueprint Editor
*/
void 
ALammpsController::ManageParticle(int32 type_, float radius_, FColor color_) {
	m_ManagedParticles.Add(type_, FParticleInstanceData(radius_, color_));
}

void
ALammpsController::SetSystemScale(float scale_) {
	if (m_ParticleVisualizationManager)
		m_ParticleVisualizationManager->SetSystemScale(scale_);
}
#pragma endregion BLUEPRINT
#pragma endregion LAMMPS_IMPORT

