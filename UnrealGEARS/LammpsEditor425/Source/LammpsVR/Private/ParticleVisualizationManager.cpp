// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleVisualizationManager.h"
// #include "LammpsVR.h"

#include <unordered_map>

#define POS_REQUEST "x"
#define TYPE_REQUEST "type"
#define NATOM_REQUEST "natoms"

// Sets default values
AParticleVisualizationManager::AParticleVisualizationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetCanBeDamaged(false);

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

	double** pos = nullptr;
	int* type = nullptr;
	int natoms = -1;
	if (RequestLammpsPositionData(natoms, pos, type)) {
		// Go through each atom and add instances to our map M_PARTICLES for the corresponding type. 
		// Note that this does not support systems with variable number of atoms (e.g. Grand Canonical).
		for (int i = 0; i < natoms; ++i) {
			int32 utype = static_cast<int32>(type[i]);	// this may not be necessary, but just in case I'll keep it
			if (!m_particles.Contains(utype)) {
				ManageNewParticleType(utype, FColor::MakeRandomColor(), ParticleConst::HydrogenRadius);		// This won't add the new type if it already exists
			}
			m_particles[utype]->AddInstance(FVector::ZeroVector);
		}
	}

}

void 
AParticleVisualizationManager::GetCurrentLammpsPositions(_PositionMapTArray &positions_) {

}


void 
AParticleVisualizationManager::InitWithXYZ(_PositionMapPtr &positions_, TSharedPtr<uint32> types_) {

}

void 
AParticleVisualizationManager::Update() {
	double** pos = nullptr; int* type = nullptr; int natoms = -1;
	_PositionMapTArray positionsByType;

	if (RequestLammpsPositionData(natoms, pos, type)) {
		// Preprocessing: Initialize the TArrays of position vectors
		for (auto& particleType : m_particles) {
			int32 typeInt = particleType.Key;
			positionsByType.Add(typeInt);
			positionsByType[typeInt].Reserve(particleType.Value->GetInstanceCount());		// AParticle::GetInstanceCount() may not be totally reliable/thread safe.
		}

		// Fill in the array. Anders recommended the Reserve() step earlier to speed this part up. 
		// No proper benchmarking/measurements have been made to confirm though
		for (int i = 0; i < natoms; ++i) {
			int32 unrealType = static_cast<int32>(type[i]);	// this may not be necessary, but just in case I'll keep it
			FVector unrealPosition(static_cast<float>(pos[i][0]),
				static_cast<float>(pos[i][1]),
				static_cast<float>(pos[i][2]));
			positionsByType[unrealType].Add(unrealPosition);
		}

		// Update positions of the mesh instances based on the map POSITIONSBYTYPE
		for (auto& positionData : positionsByType) {
			int32 typeInt = positionData.Key;
			TArray<FVector> positions = positionData.Value;
			bool dirty = false;

			for (int i = 0; i < positions.Num(); ++i) {
				dirty = (i + 1 == positions.Num()) ? true : false;			// on the last instance, mark the render state as dirty
				m_particles[typeInt]->SetPosition(i, positions[i], dirty);
			}
		}
	}
}

/* HELPER FUNCTIONS */
bool 
AParticleVisualizationManager::RequestLammpsPositionData(int& natoms_, double** &pos_, int* &type_) {
	if (m_lammps && m_lammpsExtractGlobal && m_lammpsExtractAtom) {
		pos_ = (double**)(*m_lammpsExtractAtom)(m_lammps, POS_REQUEST);
		type_ = (int*)(*m_lammpsExtractAtom)(m_lammps, TYPE_REQUEST);
		natoms_ = *(int*)(*m_lammpsExtractGlobal)(m_lammps, NATOM_REQUEST);

		if (pos_ && type_)
			return true;
	}

	return false;
	
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
		AParticle* newParticle = SpawnNewParticleType(color_, radius_);
		ManageNewParticleType(type_, newParticle);
}

void
AParticleVisualizationManager::ManageNewParticleType(int32 type_, AParticle* particle_) {
	if (!m_particles.Contains(type_)) {
		m_particles.Add(type_, particle_);
	}
	else {
		// If the desired particle is already being managed, it will get overwritten
		int instanceCount = m_particles[type_]->GetInstanceCount();
		m_particles[type_]->Destroy();
		
		m_particles[type_] = particle_;
		for (int i = 0; i < instanceCount; ++i) {
			m_particles[type_]->AddInstance(FVector::ZeroVector);
		}
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
	spawnParams.Instigator = GetInstigator();		// 

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
	// TODO Call SetPosition() on mesh
}

void 
AParticleVisualizationManager::SetParticleInstancePositions(int32 type_, TArray<FVector> positions_) {
	// TODO Call SetPosition() on mesh
}

void 
AParticleVisualizationManager::SpawnNewParticle(int32 type_, FVector position_) {
	// TODO Call AddInstance() on mesh
}

void
AParticleVisualizationManager::SetSystemScale(float scale_) {
	for (auto& particle : m_particles) {
		particle.Value->SetSystemScale(scale_);
	}
}

#pragma endregion PARTICLE_POSITION_UPDATING