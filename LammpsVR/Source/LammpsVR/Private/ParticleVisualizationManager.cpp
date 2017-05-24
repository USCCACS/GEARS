// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsVR.h"
#include "ParticleVisualizationManager.h"


// Sets default values
AParticleVisualizationManager::AParticleVisualizationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bCanBeDamaged = false;

	// Initialize lammps variables
	m_lammps = nullptr;
	m_lammpsExtractGlobal = nullptr;
	m_lammpsExtractAtom = nullptr;

}

// Called when the game starts or when spawned
void AParticleVisualizationManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParticleVisualizationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**************************************************************************************************
******** MAGICS Written Member Functions *********************************************************
**************************************************************************************************/

#pragma region LAMMPS
void 
AParticleVisualizationManager::InitWithLammps(void* lammps_,
	_LammpsExtract lammpsExtractGlobal_,
	_LammpsExtract lammpsExtractAtom_) {
	m_lammps = lammps_;
	m_lammpsExtractGlobal = lammpsExtractGlobal_;
	m_lammpsExtractAtom = lammpsExtractAtom_;

	//std::unordered_map<int, int> 
}

void 
AParticleVisualizationManager::GetCurrentLammpsPositions(TMap<int32, 
										TArray<FVector> > &positions_) {

}

void 
AParticleVisualizationManager::UpdateWithLammps() {

}

void 
AParticleVisualizationManager::SetupParticleTypesFromLammps() {

}
#pragma endregion LAMMPS

#pragma region PARTICLE_CREATION
void 
AParticleVisualizationManager::SetSpawnReference(TSubclassOf<AParticle> spawnReference_) {
	m_spawnReference = spawnReference_;
}

void 
AParticleVisualizationManager::ManageNewParticleType(int32 type_, FString typeName_) {
	// TODO Spawn and manage new particle type based off type name from a database
}

void
AParticleVisualizationManager::ManageNewParticleType(int32 type_, FColor color_, float radius_) {
	if (!m_particles.Contains(type_))
	{
		AParticle* newParticle = SpawnNewParticleType(color_, radius_);
		ManageNewParticleType(type_, newParticle);
	}
}

void
AParticleVisualizationManager::ManageNewParticleType(int32 type_, AParticle* particle_) {
	if(!m_particles.Contains(type_)) {
		m_particles.Add(type_, particle_);
	}
}

void 
AParticleVisualizationManager::SetTypeColor(int32 type_, FColor color_) {
	m_particles[type_]->SetColor(color_);
}

void 
AParticleVisualizationManager::SetTypeRadius(int32 type_, float radius_) {
	m_particles[type_]->SetRadius(radius_);
}

#pragma region NEW_TYPE_SPAWNING
AParticle*
AParticleVisualizationManager::SpawnNewParticleType(FName typeName_) {
	return SpawnNewParticleType(typeName_, FVector::ZeroVector);
}

AParticle*
AParticleVisualizationManager::SpawnNewParticleType(FName typeName_, FVector position_) {
	// TODO look up name in database, then creat particle
	return nullptr;
}

AParticle*
AParticleVisualizationManager::SpawnNewParticleType(FColor color_, float radius_) {
	return SpawnNewParticleType(color_, radius_, FVector::ZeroVector);
}

AParticle*
AParticleVisualizationManager::SpawnNewParticleType(FColor color_, float radius_, FVector position_) {
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;					// Who does the spawning
	spawnParams.Instigator = Instigator;		// 

	FTransform transform = GetActorTransform();
	transform.SetTranslation(position_);

	AParticle* newParticle = GetWorld()->SpawnActor<AParticle>(m_spawnReference,
		transform,
		spawnParams);

	newParticle->SetColorAndRadius(color_, radius_);

	return newParticle;
}
#pragma endregion NEW_TYPE_SPAWNING

#pragma endregion PARTICLE_CREATION

#pragma region PARTICLE_POSITION_UPDATING
void 
AParticleVisualizationManager::SetParticleInstancePosition(int32 type_, int32 index_, FVector newPosition_) {

}

void 
AParticleVisualizationManager::SetParticleInstancePositions(int32 type_, TArray<FVector> positions_) {

}

void 
AParticleVisualizationManager::SpawnNewParticle(int32 type_, FVector position_) {

}
#pragma endregion PARTICLE_POSITION_UPDATING