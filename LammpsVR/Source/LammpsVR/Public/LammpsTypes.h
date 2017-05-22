#pragma once

typedef void(*_LammpsOpenNoMPI)(int, char**, void**);
typedef char*(*_LammpsCommand)(void*, char*);
typedef void*(*_LammpsExtract)(void*, char*);
typedef void(*_LammpsClose)(void*);