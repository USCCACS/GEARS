// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LammpsTypes.h"
#include "ParticleVisualizationManager.h"
#include "LammpsWorker.h"
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

	template<typename T>
	bool ImportDllFunction(FString functionName_, T& functionPtr_);

protected:
	/* My homeboys */
	LammpsWorker* m_lammpsWorker;
	AParticleVisualizationManager* m_pvm;
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

	/* Spawning/Rendering Variables */
	FActorSpawnParameters m_spawnParams;
	UPROPERTY(EditDefaultsOnly, Category = "Particle Management")
		TSubclassOf<AParticleVisualizationManager> m_managerReference;

	/* Simulation Control Variables */
	UPROPERTY(BlueprintReadWrite)
		bool m_paused = false;
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
