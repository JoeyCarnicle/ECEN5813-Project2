/*========================================================================
** Circular buffer
** ECEN5813
*========================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "ring.h"

ring_t *init( int length )
{
    ring_t *ring = malloc(sizeof(char)*length);
    
    ring->Buffer = malloc(sizeof(ring_t));
    ring->Length = length;
    ring->Ini = 0;
    ring->Outi = 0;
    
    return (ring);
}

int insert( ring_t *ring, char data )
{
    int iniPlus = ring->Ini + 1;
    int ret = -1;
    
    if (iniPlus >= ring->Length ) iniPlus = 0;
    if (iniPlus != ring->Outi)
    {
        ring->Buffer[ring->Ini] = data;
        ring->Ini = iniPlus;
        ret = 0;
        //printf("\nIni = %d, data = %d", ring->Ini, ring->Buffer[ring->Ini - 1]);
    }
    else 
    {
        //printf("\nBuffer is full");
    }
    
    return ret;
}

int r_remove( ring_t *ring, char *data )
{
    int ret = 0;
    
    if(ring == NULL) 
    {
        ret = -1;
        //printf("No buffer initialized");
    }
    else
    {
        *data = ring->Buffer[ring->Outi];
        ring->Outi = (ring->Outi+1) % ring->Length;
        //printf("\ndata removed = %d", *data);
    }
    
    return ret;
}

int entries( ring_t *ring )
{
    int number = 0;
    
    if (ring->Ini >= ring->Outi)
    {
        number = ring->Ini - ring->Outi;
        //printf("\nNumber of chars in buffer = %d", number);
        return number;
    }
    else
    {
        number = (ring->Ini + ring->Length) - ring->Outi;
        //printf("\nNumber of chars in buffer = %d", number);
        return number;
    }
}
