#pragma once

/* LAMMPS Library Function Pointers */
typedef void(*_LammpsOpenNoMPI)(int, char**, void**);
typedef char*(*_LammpsCommand)(void*, char*);
typedef void*(*_LammpsExtract)(void*, char*);
typedef void(*_LammpsClose)(void*);

/* Position Tracking Variable Types */
typedef TMap<uint32, TArray<FVector> > _PositionMapTArray;
typedef TMap<uint32, TSharedPtr<TSharedPtr<float> > > _PositionMapPtr;

namespace ParticleConst {
	const static FColor HydrogenColor = FColor::FromHex(TEXT("#FFDDDD"));
	const static float HydrogenRadius = 0.25f;		// Empirical Radius of Hydrogen in Angstroms
}