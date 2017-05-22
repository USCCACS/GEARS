// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsVR.h"
#include "LammpsController.h"


// Sets default values
ALammpsController::ALammpsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALammpsController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALammpsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

