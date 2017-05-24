#pragma once

typedef void(*_LammpsOpenNoMPI)(int, char**, void**);
typedef char*(*_LammpsCommand)(void*, char*);
typedef void*(*_LammpsExtract)(void*, char*);
typedef void(*_LammpsClose)(void*);

namespace ParticleConst {
	const static FColor HydrogenColor = FColor::FromHex(TEXT("#FFDDDD"));
	const static float HydrogenRadius = 0.25f;		// Empirical Radius of Hydrogen in Angstroms
}