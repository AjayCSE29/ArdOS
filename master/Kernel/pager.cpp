#include "pager.h"

#include "memory.h"
#include "swap.h"

static uint8_t nextVictim = 0;

uint8_t pagerChooseVictim()
{
    uint8_t victim = nextVictim;

    nextVictim++;

    if(nextVictim >= NUM_FRAMES)
        nextVictim = 0;

    return victim;
}

void handlePageFault(uint8_t requestedPage)
{
    pageFaults++;

    uint8_t victimFrame = pagerChooseVictim();

    uint8_t victimPage = frameTable[victimFrame].page;

    if(pageTable[victimPage].dirty)
    {
        swapOut(victimPage,
                victimFrame);

        pageTable[victimPage].dirty = false;
    }
    else
    {
        pageTable[victimPage].present = false;
    }

    swapIn(requestedPage,
           victimFrame);

    frameTable[victimFrame].page = requestedPage;

    Serial.println();
Serial.println(F("=== PAGE FAULT ==="));

Serial.print(F("Requested Page : "));
Serial.println(requestedPage);

Serial.print(F("Victim Frame   : "));
Serial.println(victimFrame);

Serial.print(F("Victim Page    : "));
Serial.println(victimPage);
}
