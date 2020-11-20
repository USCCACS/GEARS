// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LammpsTypes.h"
#include "ParticleVisualizationManager.h"
#include "LammpsRerunWorker.h"
#include <mutex>

#include "GameFramework/Actor.h"
#include "LammpsController.generated.h"

UCLASS()
class LAMMPSVR_API ALammpsController : public AActor
{
/****************************************************************
 * Unreal-Generated Member Functions 
 ****************************************************************/
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALammpsController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game endss or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


/****************************************************************
 * MAGICS Written Member Functions
 ****************************************************************/
public:
	UFUNCTION(BlueprintCallable, Category = "Lammps")
		bool ImportLammps(FString folder_, FString filename_);
	UFUNCTION(BlueprintCallable, Category = "Lammps")
		void RunLammpsScript(FString scriptName_);
	UFUNCTION(BlueprintCallable, Category = "Lammps")
		FString ReadLammpsScript(FString scriptName_);
	UFUNCTION(BlueprintCallable, Category = "Lammps")
		void InitializeWorkerAndParticleVisualizationManager();
	UFUNCTION(BlueprintCallable, Category = "Lammps")
		void SetupAnimationState(FString dumpfilePrefix_, int32 firstTimeStep_, int32 lastTimeStep_, int32 stepSize_);

	UFUNCTION(BlueprintCallable, Category = "Lammps")
		void ManageParticle(int32 type_, float radius_, FColor color_);
	UFUNCTION(BlueprintCallable, Category = "Lammps")
		void SetSystemScale(float scale_);

	template<typename T>
	bool ImportDllFunction(FString functionName_, T& functionPtr_);

protected:
	/* My homeboys */
	LammpsWorker* m_lammpsWorker = nullptr;
	AParticleVisualizationManager* m_ParticleVisualizationManager = nullptr;
	std::mutex m_lammpsLock;

	/* Dll Variables */
	void* m_lammps;
	void* m_dllHandle;

	/* Lammps Functions */
	_LammpsOpenNoMPI m_lammpsOpenNoMPI;
	_LammpsCommand m_lammpsCommand;
	_LammpsExtract m_lammpsExtractGlobal;
	_LammpsExtract m_lammpsExtractAtom;
	_LammpsClose m_lammpsClose;

	/* Files/Input Scripts */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lammps", Displayname = "Dll Name")
		FString m_dllName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lammps", Displayname = "Input Script")
		FString m_scriptName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lammps", Displayname = "Lammps Files Root Directory")
		FString m_ResourceDir = FPaths::ProjectContentDir() + FString("LammpsResource/");

	/* Simulation Control Variables */
	UPROPERTY(BlueprintReadWrite)
		bool m_paused = false;

	/* Animation Control Variable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lammps")
		bool m_animationMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Displayname = "Dump File Prefix")
		FString m_dumpFilePrefix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Displayname = "First TimeStep")
		int32 m_firstTimeStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Displayname = "Final TimeStep")
		int32 m_lastTimeStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Displayname = "TimeStep Size")
		int32 m_stepSize;

	/* Spawning/Rendering Variables */
	FActorSpawnParameters m_spawnParams;
	UPROPERTY(EditDefaultsOnly, Category = "Particle Management")
		TSubclassOf<AParticleVisualizationManager> m_managerReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Management", Displayname = "System Scale")
		float m_systemScale;
	UPROPERTY(EditAnywhere, Category = "Particle Management", Displayname = "Particles")
		TMap<int32, FParticleInstanceData> m_ManagedParticles;

	/* For error checking before you do something really bad */
	UFUNCTION(BlueprintCallable, Category = "Lammps")
		bool LammpsIsActive();
  UFUNCTION(BlueprintCallable, Category = "Control")
    void Pause();
};

template<typename T>
bool ALammpsController::ImportDllFunction(FString functionName_, T& functionPtr_) {
	functionPtr_ = NULL;
	if (m_dllHandle != NULL) {
		functionPtr_ = (T)FPlatformProcess::GetDllExport(m_dllHandle, *functionName_);
		if (functionPtr_ != NULL)
			return true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Failed to import: " + functionName_);
	return false;
}
