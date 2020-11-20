// Fill out your copyright notice in the Description page of Project Settings.

#include "Particle.h"
// #include "LammpsVR.h"
#include "LammpsTypes.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

#define WIDTH "Width"
#define HEIGHT "Height"
#define COLOR "Color"

const FColor AParticle::m_defaultColor = FColor::Magenta;
const float AParticle::m_defaultRadius = ParticleConst::HydrogenRadius;	// from LammpsTypes.h
const float AParticle::m_defaultWidth = 50.0f;
const float AParticle::m_defaultHeight = 100.0f;

// Sets default values
AParticle::AParticle()
{
	ConstructorHelper();
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
void
AParticle::ConstructorHelper() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetCanBeDamaged(false);

	m_Mesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMesh"));
	m_Mesh->SetMobility(EComponentMobility::Stationary);
	m_Mesh->SetGenerateOverlapEvents(false);
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

int32
AParticle::GetInstanceCount() {
	return m_Mesh->GetNumRenderInstances();		// This may yield unreliable values if called in the middle of draw calls or instance adding/setting
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
	m_radius = radius_;
	m_color = color_;
	UpdateMeshMaterial();
}

void 
AParticle::SetRadius(float newRadius_) {
	SetColorAndRadius(m_color, newRadius_);
}

void 
AParticle::SetColor(FColor newColor_) {
	SetColorAndRadius(newColor_, m_radius);
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

/* Sets the scale of the billboard representing this atom. Scaling is standardized such that at
 *	Scale = 1.0f
 *	Radius = 0.25f
 *	Width = 20.0f
 *	Height = 40.0f
 * where 0.25 A is the empirical radius of hydrogen
 */
void
AParticle::SetMeshScale(float scale_) {
	m_scale = scale_;
	UpdateMeshMaterial();
}

/* For some reason, I got errors when trying to update the color, width, and height separately. 
 * So to fix it, I put any material change in this one function. Any time a user wants to change
 * the size or color of the atom, whether one or both, this function will get called.
 */
void
AParticle::UpdateMeshMaterial() {
	UMaterialInstanceDynamic* dynamicMat = GetMaterial();
	dynamicMat->SetVectorParameterValue(FName(COLOR), m_color);
	dynamicMat->SetScalarParameterValue(FName(WIDTH), -1*WidthFromRadius(m_radius));
	dynamicMat->SetScalarParameterValue(FName(HEIGHT), HeightFromRadius(m_radius));
	SetMaterial(dynamicMat);
}

/* Helper Functions for radius scaling */
float
AParticle::WidthFromRadius(float radius_) {
	return radius_ * (m_defaultWidth / m_defaultRadius) * m_scale;
}

float
AParticle::HeightFromRadius(float radius_) {
	return (2 * WidthFromRadius(radius_));
}
#pragma endregion SCALE_COLOR

