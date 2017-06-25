// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Particle.generated.h"

USTRUCT()
struct FParticleInstanceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Displayname = "Radius")
		float Radius;
	UPROPERTY(EditAnywhere, Displayname = "Color")
		FColor Color;

	FParticleInstanceData()
		: Radius(1.0f)
		, Color(FColor::Red)
	{
	}

	FParticleInstanceData(float radius_, FColor color_)
		: Radius(radius_)
		, Color(color_)
	{
	}

	friend FArchive& operator<<(FArchive& Ar, FParticleInstanceData& InstanceData)
	{
		// @warning BulkSerialize: FParticleInstanceData is serialized as memory dump
		// See TArray::BulkSerialize for detailed description of implied limitations.
		Ar << InstanceData.Radius << InstanceData.Color;
		return Ar;
	}
};

UCLASS()
class LAMMPSVR_API AParticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

/****************************************************************
* MAGICS Written Member Functions
****************************************************************/
public:
	AParticle(FColor color_, float radius);
	AParticle(FColor color_);
	AParticle(float radius_);

	virtual bool SetPosition(int32 index, FVector newPosition, bool dirty);
	virtual void AddInstance(FVector position);
	virtual int32 GetInstanceCount();
	virtual void ClearInstances();

	virtual void SetSystemScale(float scale_);
	virtual void SetTotalScale(float scale_);

	void SetRadius(float newRadius_);
	void SetColor(FColor newColor_);
	void SetColorAndRadius(FColor color_, float radius_);
	float GetRadius() { return m_radius; }
	FColor GetColor() { return m_color; }

protected:
	UPROPERTY(Category = "ParticleMeshComponents", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UHierarchicalInstancedStaticMeshComponent* m_Mesh;
	UPROPERTY(Category = "ParticleMeshComponents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_radius = 25.0f;
	UPROPERTY(Category = "ParticleMeshComponents", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float m_scale = 1.0f;
	UPROPERTY(Category = "ParticleMeshComponents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FColor m_color = FColor::Magenta;

	virtual void ConstructorHelper();

	virtual UMaterialInstanceDynamic* GetMaterial();
	virtual void SetMaterial(UMaterialInterface* material_);
	virtual void SetMeshScale(float scale_);
	virtual void UpdateMeshMaterial();

	float WidthFromRadius(float radius_);
	float HeightFromRadius(float radius_);

public:
	/* Default/Standardized Sizes and Colors */
	const static FColor m_defaultColor;
	const static float m_defaultRadius;	
	const static float m_defaultWidth;
	const static float m_defaultHeight;
};
