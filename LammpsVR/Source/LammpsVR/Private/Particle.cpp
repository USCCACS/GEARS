// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsVR.h"
#include "Particle.h"


// Sets default values
AParticle::AParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParticle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

