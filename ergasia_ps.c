#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "sinartiseis.h"

#define infile "Magazine_Subscriptions.txt"
#define outfile	"out_Magazine.txt"


/* afairontas ta sxolia xtypaei kai ektypwnei xrono (diarkeia ektelesis) se perivallon windows */

int main()
{
	int buckets, error, N, S, plithos_u, plithos_i;
	char test_string[50];
	clock_t t1, t2;
	double ratio = 1./CLOCKS_PER_SEC;
	
	printf("\nGive B: ");	scanf("%d", &buckets);
	error = initialize(buckets);
	if(error == -1)	{	printf("\nMAIN: initialize error");		return 0;	}
	
	printf("\nGive N: ");	scanf("%d", &N);
	printf("\nGive S: ");	scanf("%d", &S);
	
	t1 = clock(); 
	error = create_num_id (infile, outfile, N, S, &plithos_u, &plithos_i);
	if(error == -1)	{	printf("\nMAIN: create_num_id error");		return 0;	}
	t2 = clock(); 
	printf("\nTime = %g seconds", ratio*(long)t2 - ratio*(long)t1 );
	
	destroy();

	return 0;
}
