#pragma once
void InitMemory();
int AllocMemory(int Size);
void FreeMemory(int MemoryHandle);
int WriteMemory(int MemoryHandle, int Offset, int Size, char* Data);
int ReadMemory(int MemoryHandle, int Offset, int Size, char* Data);
void DumpMemory(void);
void print();