// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Particle.h"
#include "LammpsTypes.h"

#include "GameFramework/Actor.h"
#include "ParticleVisualizationManager.generated.h"

UCLASS()
class LAMMPSVR_API AParticleVisualizationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleVisualizationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


/**************************************************************************************************
 ******** MAGICS Written Member Functions *********************************************************
 **************************************************************************************************/
public:
	/* Lammps Communication/Synchronization */
	void InitWithLammps(void* lammps_,
		_LammpsExtract lammpsExtractGlobal_,
		_LammpsExtract lammpsExtractAtom_);
	void GetCurrentLammpsPositions(_PositionMapTArray &positions_);

	/* XYZ Communication/Synchronization */
	void InitWithXYZ(_PositionMapPtr &positions_, TSharedPtr<uint32> types_);

	/* Core Update Function. Responsible for changing the rendered positions */
	virtual void Update();

	/* Particle Creation */
	void SetSpawnReference(TSubclassOf<AParticle> spawnReference_);

	void ManageNewParticleType(int32 type_, FString typeName_);
	void ManageNewParticleType(int32 type_, FColor color_, float radius_);
	void ManageNewParticleType(int32 type_, AParticle* particle_);

	AParticle* SpawnNewParticleType(FName typeName_);
	AParticle* SpawnNewParticleType(FColor color_, float radius_);
	AParticle* SpawnNewParticleType(FColor color_, float radius_, FVector position_);
	AParticle* SpawnNewParticleType(FName typeName_, FVector position_);

	void SetTypeColor(int32 type_, FColor color_);
	void SetTypeRadius(int32 type_, float radius_);

	/* Particle Position Manipulation */
	void SetParticleInstancePosition(int32 type_, int32 index_, FVector newPosition_);
	void SetParticleInstancePositions(int32 type_, TArray<FVector> positions_);
	void SetSystemScale(float scale_);

	/* Data Access */
	AParticle* GetParticle(int32 type_) { return *(m_particles.Find(type_));  }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Particle Management")
		TSubclassOf<AParticle> m_spawnReference;

	/* Helper functions for updating particle instance positions */
	void SpawnNewParticle(int32 type_, FVector position_);
	bool RequestLammpsPositionData(int& natom_, double** &pos_, int* &type_);

private:
	/* Particle Tracking/Management */
	TMap<int32, AParticle*> m_particles;		// Key = type, Value = instantiated particle
	_PositionMapTArray m_positions;

	/* Lammps Dll Related */
	void* m_lammps;
	_LammpsExtract m_lammpsExtractGlobal;
	_LammpsExtract m_lammpsExtractAtom;
};
