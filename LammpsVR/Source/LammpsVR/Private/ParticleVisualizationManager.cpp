// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsVR.h"
#include "ParticleVisualizationManager.h"


// Sets default values
AParticleVisualizationManager::AParticleVisualizationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

