/*========================================================================
** Circular buffer
** ECEN5813
*========================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "ring.h"

extern ring_t *ring;
extern char data;

//ring_t *ring = NULL;
//char data = 4;

/*int main()
{
    int ret = 0;
    
    ret = r_remove(ring, &data);
    if(ret == -1) printf("\nremove failed");
    init(3);
    printf("\nAddress of ring = %x", ring);
    ret = insert(ring, data);
    if(ret == -1) printf("\ninsert failed");
    ret = r_remove(ring, &data);
    if(ret == -1) printf("\nremove failed");
    ret = insert(ring, data);
    if(ret == -1) printf("\ninsert failed");
    entries(ring);
    init(10);
    entries(ring);
    ret = r_remove(ring, &data);
    if(ret == -1) printf("\nremove failed");
    ret = insert(ring, data);
    if(ret == -1) printf("\ninsert failed");
    for (int i = 0; i < 9; i++)
    {
	insert(ring, data);
    }
    for (int i = 0; i < 10; i++)
    {
	r_remove(ring, &data);
    }
    
    
    return 0;
}*/

void *init( int length )
{
    ring = malloc(sizeof(char)*length);
    
    ring->Buffer = malloc(sizeof(ring_t));
    ring->Length = length;
    ring->Ini = 0;
    ring->Outi = 0;
    
    printf("\nIni = %d, Outi = %d, length = %d", ring->Ini, ring->Outi, ring->Length);
    
}

void resize( int length )
{

    if(ring != NULL){
	printf("\nprevious ring freed");
	ring = realloc(ring, sizeof(char)*length);
    }
    else{
	printf("\nNo ring initialized, new ring initialized");
	ring = malloc(sizeof(char)*length);
   }
   
   ring->Buffer = malloc(sizeof(ring_t));
   ring->Length = length;

   printf("\nIni = %d, Outi = %d, length = %d", ring->Ini, ring->Outi, ring->Length);

}

int insert( ring_t *ring, char data )
{
    int ret = 0;

    if(ring == NULL) 
    {
        ret = -1;
        printf("\nNo buffer initialized");
    }
    else{
	int iniPlus = ring->Ini + 1;
    	if (iniPlus >= ring->Length ) iniPlus = 0;
    	if (iniPlus != ring->Outi)
    	{
      	  ring->Buffer[ring->Ini] = data;
       	 ring->Ini = iniPlus;
        	printf("\nIni = %d, Outi = %d, data = %d", ring->Ini, ring->Outi, ring->Buffer[ring->Ini - 1]);
    	}
    	else 
    	{
		ret = -1;
        	printf("\nBuffer is full");
    	}
    }
    
    return ret;
}

int r_remove( ring_t *ring, char *data )
{
    int ret = 0;
    
    if(ring == NULL) 
    {
        ret = -1;
        printf("\nNo buffer initialized");
    }
    else if(ring->Outi == ring->Ini){
	ret = -1;
	printf("\nNo data stored in the buffer");
    }
    else
    {
        *data = ring->Buffer[ring->Outi];
        ring->Outi = (ring->Outi+1) % ring->Length;
        printf("\ndata removed = %d from %d ", ring->Buffer[ring->Outi - 1], ring->Outi);
    }
    
    return ret;
}

int entries( ring_t *ring )
{
    int number = 0;
    int ret = 0;
    
    if(ring == NULL) 
    {
        ret = -1;
        printf("\nNo buffer initialized");
    }

    else if (ring->Ini >= ring->Outi)
    {
        number = ring->Ini - ring->Outi;
        printf("\nNumber of chars in buffer = %d", number);
    }
    else
    {
        number = (ring->Ini + ring->Length) - ring->Outi;
        printf("\nNumber of chars in buffer = %d", number);
    }
    
    return ret;

}
