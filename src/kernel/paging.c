#include "paging.h"
#include "physalloc.h"

// Static page directory
// Other paging structures are dynamically allocated, but there will only ever be one PDE
uint32_t pageDirectory[1024] __attribute__((aligned(4096)));

// Initialize paging
void setupPaging() {

    // Clear paging directory, initialize with non-present user pages
    for(int  i = 0; i < 1024; i++) {
        pageDirectory[i] = PDE_NOT_PRESENT | PDE_USER | PDE_READ_WRITE;
    }

    // Identity page first 4M
    uint32_t *baseTable = allocPhysPage();
    for(int i = 0; i < 1024; i++) {
        baseTable[i] = (i << 12) | PTE_SUPERVISOR | PTE_READ_WRITE | PTE_PRESENT;
    }
    pageDirectory[0] = (uint32_t)baseTable | PDE_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;

    loadPageDirectory(pageDirectory);
    enablePaging();

}