#include <iostream>
#include "memory.h"

int main()
{
    InitMemory();

    AllocMemory(1010);
    char a[1005];
    for (int i = 0; i < 1005; i++) {
        a[i] = 7;
    }
    WriteMemory(1, 1, 100, a);
    int block = AllocMemory(7500);
    //char m[7010];
    //for (int i = 0; i < 7010; i++) {
    //    m[i] = 4;
    //}

    
    WriteMemory(block, 0, 1005, a);
    int h = AllocMemory(500);
//    WriteMemory(, 0, 3, b);
    DumpMemory();
    if (h != 0) {
        char b[3] = { 1,2,3 };
        WriteMemory(h, 0, 3, b);
    }

    DumpMemory();
}