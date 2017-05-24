// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsVR.h"
#include "Particle.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

const FColor AParticle::m_defaultColor = FColor::Magenta;
const float AParticle::m_defaultRadius = 0.25f;				// Radius of Hydrogen in Angstroms
const float AParticle::m_defaultWidth = 50.0f;
const float AParticle::m_defaultHeight = 100.0f;

// Sets default values
AParticle::AParticle()
{
	ConstructorHelper();

	/*if (m_Mesh->GetMaterial(0)) {
		UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(m_Mesh->GetMaterial(0), this);
		dynamicMat->SetVectorParameterValue(FName("Color"), m_defaultColor);
		dynamicMat->SetScalarParameterValue(FName("Width"), m_defaultWidth);
		dynamicMat->SetScalarParameterValue(FName("Height"), m_defaultHeight);
		m_Mesh->SetMaterial(0, dynamicMat);
	}*/
}

// Called when the game starts or when spawned
void AParticle::BeginPlay()
{
	Super::BeginPlay();
}

/**************************************************************************************************
******** MAGICS Written Member Functions *********************************************************
**************************************************************************************************/
#pragma region CONSTRUCTOR
AParticle::AParticle(FColor color_, float radius_) {
	ConstructorHelper();
	m_color = color_;
	m_radius = radius_;

	//TODO: Modularize thi section 
	/*UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(m_Mesh->GetMaterial(0), this);
	dynamicMat->SetVectorParameterValue(FName("Color"), m_color);
	dynamicMat->SetScalarParameterValue(FName("Width"), RadiusToWidth(m_radius));
	dynamicMat->SetScalarParameterValue(FName("Height"), RadiusToHeight(m_radius));
	m_Mesh->SetMaterial(0, dynamicMat);*/
}

AParticle::AParticle(FColor color_) {
	ConstructorHelper();
	m_color = color_;

	/*UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(m_Mesh->GetMaterial(0), this);
	dynamicMat->SetVectorParameterValue(FName("Color"), m_color);
	dynamicMat->SetScalarParameterValue(FName("Width"), m_defaultWidth);
	dynamicMat->SetScalarParameterValue(FName("Height"), m_defaultHeight);
	m_Mesh->SetMaterial(0, dynamicMat);*/
}

AParticle::AParticle(float radius_) {
	ConstructorHelper();
	m_radius = radius_;

	/*UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(m_Mesh->GetMaterial(0), this);
	dynamicMat->SetVectorParameterValue(FName("Color"), m_defaultColor);
	dynamicMat->SetScalarParameterValue(FName("Width"), RadiusToWidth(m_radius));
	dynamicMat->SetScalarParameterValue(FName("Height"), RadiusToHeight(m_radius));
	m_Mesh->SetMaterial(0, dynamicMat);*/
}

void
AParticle::ConstructorHelper() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bCanBeDamaged = false;

	m_Mesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMesh"));
	m_Mesh->SetMobility(EComponentMobility::Stationary);
	m_Mesh->bGenerateOverlapEvents = false;
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	m_Mesh->CanCharacterStepUpOn = TEnumAsByte<ECanBeCharacterBase>(false);
	m_Mesh->SetCastShadow(false);
	m_Mesh->SetSimulatePhysics(false);
	m_Mesh->SetEnableGravity(false);
}
#pragma endregion CONSTRUCTOR

#pragma region INSTANCING
bool 
AParticle::SetPosition(int32 index_, FVector newPosition_, bool dirty_) {
	/*
	 *FTransform transform_;
	 *transform_.SetTranslation(newPosition_);
	 *transform_.SetRotation(FQuat(FRotator::ZeroRotator));
	 *transform_.SetScale3D(FVector(1.0f));
	 */
	return m_Mesh->UpdateInstanceTransform(index_, FTransform(newPosition_), false, dirty_, false);
}

void 
AParticle::AddInstance(FVector position_) {
	/*
	*FTransform transform_;
	*transform_.SetTranslation(position_);
	*transform_.SetRotation(FQuat(FRotator::ZeroRotator));
	*transform_.SetScale3D(FVector(1.0f));
	*/
	m_Mesh->AddInstance(FTransform(position_));
}

void 
AParticle::ClearInstances() {
	m_Mesh->ClearInstances();
}

#pragma endregion INSTANCING

#pragma region SCALE_COLOR
UMaterialInstanceDynamic* 
AParticle::GetMaterial() {
	return UMaterialInstanceDynamic::Create(m_Mesh->GetMaterial(0), this);
}

void 
AParticle::SetMaterial(UMaterialInterface* material_) {
	m_Mesh->SetMaterial(0, material_);
}

/* Available to Users */
void
AParticle::SetColorAndRadius(FColor color_, float radius_) {
	SetColor(color_);
	SetRadius(radius_);
}

void 
AParticle::SetRadius(float newRadius_) {
	m_radius = newRadius_;
	SetMeshScale(m_scale);
}

void 
AParticle::SetColor(FColor newColor_) {
	m_color = newColor_;
	UMaterialInstanceDynamic* dynamicMat = GetMaterial();
	dynamicMat->SetVectorParameterValue(FName("Color"), newColor_);
	SetMaterial(dynamicMat);
}

void
AParticle::SetSystemScale(float scale_) {
	SetActorScale3D(FVector(scale_));
}

void
AParticle::SetTotalScale(float scale_) {
	SetMeshScale(scale_);
	SetSystemScale(scale_);
}

/* Only Available to Manager */
void
AParticle::SetMeshScale(float scale_) {
	m_scale = scale_;
	SetMeshDimensions(RadiusToWidth(m_radius), RadiusToHeight(m_radius));
}

void 
AParticle::SetMeshDimensions(float width_, float height_) {
	UMaterialInstanceDynamic* dynamicMat = GetMaterial();
	dynamicMat->SetScalarParameterValue(FName("Width"), width_);
	dynamicMat->SetScalarParameterValue(FName("Heigth"), height_);
	SetMaterial(dynamicMat);
}

/* Helper Functions for radius scaling */
float
AParticle::RadiusToWidth(float radius_) {
	return radius_ * (m_defaultWidth / m_defaultRadius) * m_scale;
}

float
AParticle::RadiusToHeight(float radius_) {
	return RadiusToWidth(radius_) * 2;
}
#pragma endregion SCALE_COLOR

