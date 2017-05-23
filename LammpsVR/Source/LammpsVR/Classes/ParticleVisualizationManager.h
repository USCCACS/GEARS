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
	AParticleVisualizationManager(TSubclassOf<AParticle> spawnReference_);
	AParticleVisualizationManager(TSubclassOf<AParticle> spawnReference_, float systemScale_);

	// Lammps Communication/Synchronization
	void InitWithLammps(void* lammps_,
		_LammpsExtract lammpsExtractGlobal_,
		_LammpsExtract lammpsExtractAtom_);
	void GetCurrentLammpsPositions(TMap<int32, TArray<FVector> > &positions_);
	void UpdateWithLammps();

	// Particle Creation and Manipulation 
	bool AddNewParticleType(int32 typeNum_, FString typeName_, FColor color_, float radius_);
	AParticle* SpawnNewParticle(FVector position_);

	void SetParticleInstancePosition(int32 type_, int32 index_, FVector newPosition_);
	void SetParticlePositions(int32 type_, TArray<Fvector> positions_);

	void SetTypeColor(int32 type_);
	void SetTypeRadius(int32 type_);

protected:
	TSubclassOf<AParticle> m_spawnReference;

	/* Functions */
	void AddNewParticle(int32 type_, FVector position_);

private:
	/* Particle Tracking/Management */
	TMap<int32, AParticle*> m_particles;		// Key = type, Value = instantiated particle

	/* Lammps Related */
	void* m_lammps;
	_LammpsExtract m_lammpsExtractGlobal;
	_LammpsExtract m_lammpsExtractAtom;
};
