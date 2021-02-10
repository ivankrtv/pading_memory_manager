#include <iostream>
#include <fstream>
#include "memory.h"

#define PAGESIZE 1000
#define PAGENUM 30
#define MEMSIZE 10000

using namespace std;

struct pages {
	int num;
	int handle;
	int use;
	int serialnum;
};

int h = 0;
pages page[PAGENUM];
char mainmem[MEMSIZE];

void InitMemory() {
	ofstream f("virtual.txt");
	f.trunc;
	f.close();
	for (int i = 0; i < PAGENUM; i++) {
		page[i].num = i + 1;
	}
	page[PAGENUM - 1].num = 0;
}

int AllocMemory(int Size) {	//проверить
	if (Size == 0) return 0;
	int k = Size / PAGESIZE;
	if (Size % PAGESIZE != 0) k++;
	int count = 0;
	for (int i = 0; i < PAGENUM; i++) {
		if (page[i].handle == 0 && page[i].num != 0) count++;
		if (count == k) break;
	}
	if (count == k) {
		h++; int sernum = 0;
		for (int i = 0; i < PAGENUM; i++) {
			if (k == 0) break;
			if (page[i].handle == 0) {
				page[i].handle = h;
				page[i].serialnum = ++sernum;
				k--;
			}
		}
	}
	return h;
}

void FreeMemory(int MemoryHandle) {
	for (int i = 0; i < PAGENUM; i++) {
		if (page[i].handle == MemoryHandle) page[i].handle = 0;
	}
}


int swipe(int i, int handle) {
	int min = 0;
	for (int j = 1; j < 9; j++) {
		if (page[min].use > page[j].use) {
			min = j;
			if (min == 0) break;
		}
	}
	int start = min * PAGESIZE;
	fstream f;
	f.open("virtual.txt", ios::out | ios::in);
	if (f.is_open()) {
		int startfile = 0;
		for (int t = 10; t < PAGENUM; t++) {
			if (page[t].num == 0) {
				startfile = (t - 10) * PAGESIZE;
				page[t].num = t + 1;
				page[t].handle = page[min].handle;
				page[t].use = page[min].use;
				page[t].serialnum = page[min].serialnum;
				break;
			}
		}
		for (int t = 0; t < PAGESIZE; t++) {
			f.seekp(startfile + t, ios::beg);
			f.put(mainmem[start + t]);
		}
		startfile = (i - 10) * PAGESIZE;
		for (int t = 0; t < PAGESIZE; t++) {
			f.seekg(startfile + t, ios::beg);
			f.get(mainmem[start + t]);
		}
	}
	f.close();

	page[i].num = 0;
	page[min].handle = page[i].handle;
	page[min].use = page[i].use;
	page[min].serialnum = page[i].serialnum;
	page[i].handle = 0;
	
	return min;
}


int WriteMemory(int MemoryHandle, int Offset, int Size, char* Data){
	int kol = 0;
	int sernum = 1;
	for (int i = 0; i < PAGENUM; i++) {
		if (page[i].handle == MemoryHandle) kol++;
	}
	kol *= PAGESIZE;
	int sz = Size;
	if (Offset + Size > kol) return 1;
	for (int i = 0; i < PAGENUM; i++) {
		if (page[i].handle == MemoryHandle && page[i].serialnum == sernum) {
			if (sz == 0) break;
			if (i > 9) {
				int num = swipe(i, MemoryHandle);
				int start = num * PAGESIZE;
				for (int j = 0; j < Size; j++) {
					if (Offset + j > PAGESIZE) break;
					if (sz == 0) break;
					mainmem[start + Offset + j] = *Data;
					Data++;
					sz--;
				}
				//page[num].use++;
			}
			else {
				int start = i * PAGESIZE;
				for (int j = 0; j < Size; j++) {
					if (Offset + j > PAGESIZE) break;
					if (sz == 0) break;
					mainmem[start + Offset + j] = *Data;
					Data++;
					sz--;
				}
				page[i].use++;
			}
			sernum++;
		}
	}
	return 0;
}

int ReadMemory(int MemoryHandle, int Offset, int Size, char* Data) {
	int kol = 0;
	for (int i = 0; i < PAGENUM; i++) {
		if (page[i].handle == MemoryHandle) kol++;
	}
	if (Offset + Size > kol * PAGESIZE) return 1;
	int sernum = 1;
	int sz = Size;
	for (int i = 0; i < PAGENUM; i++) {
		if (kol == 0) break;
		if (sz == 0) break;
		if (page[i].handle == MemoryHandle && page[i].serialnum == sernum) {
			if (i > 9) {
				int num = swipe(i, MemoryHandle);
				int start = num * PAGESIZE;
				for (int j = 0; j < Size; j++) {
					if (Offset + j > PAGESIZE) break;
					if (sz == 0) break;
					*Data = mainmem[start + Offset + j];
					Data++;
					sz--;
				}
				//page[num].use++;
			}
			else {
				int start = i * PAGESIZE;
				for (int j = 0; j < Size; j++) {
					if (Offset + j > PAGESIZE) break;
					if (sz == 0) break;
					*Data = mainmem[start + Offset + j];
					Data++;
					sz--;
				}
				page[i].use++;
			}
			sernum++;
			kol--;
			i = 0;
		}
	}
	return 0;
}


void DumpMemory(void) {
	
	int flag = 0;
	for (int i = 1; i < h+1; i++) {
		for (int j = 0; j < PAGENUM; j++) {
			if (page[j].handle == i) {
				printf("H: %d", page[j].handle);
				flag = 1;
				break;
			}
		}
		if (flag == 1) {
			printf(" P: ");
			int kol = 0;
			for (int j = 0; j < PAGENUM; j++) {
				if (page[j].handle == i) kol++;
			}
			int sernum = 1;
			for (int j = 0; j < PAGENUM; j++) {
				if (page[j].handle == i && page[j].serialnum == sernum) {
					printf("%d", page[j].num);
					if (j > 9) printf("* ");
					else printf(" ");
					sernum++;
					j = 0;
				}
			}
			printf("S: ");
			sernum = 1;
			for (int j = 0; j < 9; j++) {
				if (page[j].handle == i && page[j].serialnum == sernum) {
					int start = j * PAGESIZE;
					for (int t = 0; t < 10; t++) {
						printf("%d", mainmem[start + t]);
					}
				}
			}
		}
		printf("\n");
	}
}