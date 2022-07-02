#ifndef _SINARTISEIS__h
#define _SINARTISEIS__h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
///////////////////////////////////////  DHLWSEIS ///////////////////////////////////////
typedef struct rating_node {// DOMH DIPLA SYNDEDEMENH LISTA ME TA RATINGS IDS TIME DILWSEIS DEIKTES KOMVWN
	struct rating_node *prev;
	int item_id;
	int rating;
	int timer;
	struct rating_node *next;
} RATING_NODE;

typedef struct user {		// DOMH GIA TOYS USERS 
	char name[20];          // PINAKAS ONOMA USER
	RATING_NODE* ratings;   // DEIKTHS STON KOMVO LISTAS TWN RATINGS (DOMH MESA SE DOMH)
	int num_ratings;        // COUNTER ARITHMOS RATINGS GIA THN EISODO N 
} USER;

typedef struct item {		// DOMH GIA TA ITEMS
	char name[20];
} ITEM;

int* user_ids;				// PINAKAS EPISTROFHS - ANATHESHS UNIQUE IDS TWN USERS
int* item_ids;				// PINAKAS EPISTROFHS - ANATHESHS UNIQUE IDS TWN ITEMS
int num_buckets;			// SYNOLO BUCKETS
int num_users;				// SYNOLO USERS COUNTER
int num_items;				// SYNOLO ITEMS COUNTER

//DILWSH PINAKA KAI DEIKTH TOY SE TIMES PINAKA SYNDEDEMENHS LISTAS GIA THN INITIALIZE KLP
USER** user_hash_table;
ITEM** item_hash_table;
///////////////////////////////////////  DHLWSEIS ///////////////////////////////////////

int initialize(int B)
{
	num_buckets = B;
	//DESMEYOYME DYNAMIKA
	user_hash_table = (USER**)malloc(sizeof(USER*) * B);
	item_hash_table = (ITEM**)malloc(sizeof(ITEM*) * B);

	user_ids = (int*)malloc(sizeof(int) * B);
	item_ids = (int*)malloc(sizeof(int) * B);
	if(user_hash_table == NULL || item_hash_table == NULL || user_ids == NULL || item_ids == NULL)
	{
		return -1;
	}
	int i;
	// KANOYME TIS THESIS STON KOMVO NULL KAI GEMIZOYME TOYS PINAKES ME -1 GIATI TA IDS ARXIZOYN APO 0
	for(i = 0; i < B; i++)
	{
		user_hash_table[i] = NULL;
		item_hash_table[i] = NULL;
		user_ids[i] = -1;
		item_ids[i] = -1;
	}
	//COUNTERS
	num_users = 0;
	num_items = 0;
	
	return 1;
}

int hashfunction(char* str)
{
	int length = strlen(str); int sum = 0;
	int i;
	for (i=0; i<length; i++){sum+= str[i];}return (sum%num_buckets);
	///////////// BRHKA KAI AYTH THN METHODO POU LEEI OTI EINAI KALYTERH (C++) ////////////////
	// Source: https://stackoverflow.com/questions/7666509/hash-function-for-string
	// djb2 by Dan Bernstein
	//unsigned long hash = 5381;
    //int c;
    //while (c = *str++)
    //    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    //return (hash % num_buckets);
}

int return_id(char* str, char attribute)
{//////  EDW ANATHETEI KAI EPISTREFEI KAI TA UNIQUE IDS -----  METRAME KAI USERS, ITEMS PARALLHLA == MEGETHOS PINAKA  //////
	// DEXETAI TO HASHARISMENO STRING KAI TO BAZOYME STON ANALOGO PINAKA VASH ATTRIBUTE
	int index = hashfunction(str);
	//printf("%s %d\n", str, index);
	int id,i;
	//AN EINAI USER
	if(attribute == 'u')
	{	
		if(user_hash_table[index] == NULL)								// AN H TIMH STHN THESI INDEX TOY SYNDEDEMEDNOY PINAKA EINAI ADEIA
		{
			id = num_users;               								// DINOUME TO DIATHESIMO(APO COUNTER) GIA RETURN
			user_ids[num_users++] = index;								// KAI VAZOYME TO HASH STHN ANALOGH THESH TO ENA KATW APO TO ALLO ME THN SEIRA
		}																// GIA KATHE NEO USER PROSTHETOYME KAI 1 STON COUNTER KAI MIA THESI STON PINAKA
		else
		{
			for(i = 0; i < num_users; i++)								// ALLIWS PSAHNOYME SE THESEIS PINAKA TO IDIO HASH KAI ID = I
			{
				if(user_ids[i] == index)
				{
					id = i;
					break;
				}
			}
		}
	}
	//AN EINAI ITEMS ---- PAROMOIA DIADIKASIA ME PANW
	else
	{
		if(item_hash_table[index] == NULL)
		{
			id = num_items;
			item_ids[num_items++] = index;
		}
		else
		{
			for(i = 0; i < num_items; i++)
			{
				if(item_ids[i] == index)
				{
					id = i;
					break;
				}
			}
		}
	}
	return id;
}


int create_num_id(char* input_file, char* output_file, int N, int S, int* plithos_users, int* plithos_items)
{
	FILE *inptr = fopen(input_file, "r");								// ANOIGOUME INPUT GIA DIAVASMA
	FILE *outptr = fopen(output_file, "w");								// ANOIGOUME OUTPUT GIA GRAPSIMO
	//DILWSEIS
	char user_name[20], item_name[20];
	int rating, timer;
	int user_id, item_id;
	// ELEGXOS
	if(inptr == NULL || outptr == NULL)
	{
		printf("Failed To Open File!!!");
		return -1;
	}
	// DIAVAZOYME KATHE GRAMMI ANA 4 KAI PAIRNAME STIS METAVLITES
	while(fscanf(inptr, "%s %s %d %d\n", user_name, item_name, &rating, &timer) == 4)
	{
		user_id = return_id(user_name, 'u');							// PAIRNOUME KAI TO UNIQUE USER ID
		item_id = return_id(item_name, 'i');							// PAIRNOUME KAI TO UNIQUE ITEMS ID
		int user_slot = user_ids[user_id];								// PAIRNOUME THN THESI APO TON PINAKA TWN HASHED USER
		int item_slot = item_ids[item_id];								// PAIRNOUME THN THESI APO TON PINAKA TWN HASHED ITEM
		RATING_NODE* rnode = (RATING_NODE*)malloc(sizeof(RATING_NODE)); // DESMEYOUME KOMVO LISTAS RATING ME TA DATA POY SYNODEYOUN THN EGGRAFH
		rnode->item_id = item_id;
		rnode->rating = rating;											// ANTISTOIXOYME TA DATA
		rnode->timer = timer;
		rnode->next = NULL;
		rnode->prev = NULL;
		// KAI PROSARTOUME-ANTOISTOIXIZOYME STON PINAKA USER THN RATING SYNDEDEMENH LISTA - KOMVO STHN ANTOISTIXH THESI VASH HASH
		if(user_hash_table[user_slot] == NULL) 							// NEA KATAXWRHSH UNIQUE USER --> AN H THESH TOY PINAKA EINAI KENH
		{ 
			USER* user = (USER*)malloc(sizeof(USER));					// DESMEYSH ENOS NEOY KOMVOU USER
			user->ratings = rnode;										// PERNAME DATA DOMHS RATING STHN DOMH USER
			user->num_ratings = 1;										// KAI PAIRNEI O COUNTER TIMH
			strcpy(user->name, user_name);								// COPY TO ONOMA STON PINAKA TOY KOMVOY "char name[20]"
			user_hash_table[user_slot] = user;							// SYNDEOUME THN LISTA ME PINAKA
		}
		else
		{   															// GIA YPARXWN
			USER* user = user_hash_table[user_slot];					// SYNDESH ME TABLE USER
			(user->num_ratings)++;										// +1 COUNTER GIA TA RATING PARAMETROS N
			RATING_NODE* prev = NULL;									// KANOYME TON DEIKTH NULL
			RATING_NODE* curr = user->ratings;							// ANTISTOIXIZOYME CURRENT TEMP ME KEFALH RATING
			while(curr != NULL)											// GIA OSO O CURRENT DEN EINAI NULL ANAZHTHSH -- EDW GINETAI KAI H TAXINOMHSH VASH ITEM ID STHN DOMH RATING
			{															// MEXRI TO TELOS
				if(rnode->item_id < curr->item_id) break;				// AN TO ID EINAI MEGALYTERO PAME STON EPOMENO    | .....ALLIWS BREAK KAI PAME SE ELEGXOUS
				prev = curr; 											// SEIRIAKA O CURRENT DEIKTHS DEIXNEI PROHGOYMENOS|
				curr = curr->next; 										// KAI O CURRENT DEIXNEI STON EPOMENO			  |
			}
			if(prev == NULL)    										// ELEGXOUME AN O PROHGOUMENOS KOMVOS EINAI KENOS
			{															// STHN ARXH ? UPDATE TOYS DEIKTES
				rnode->next = user->ratings;							// BAZOUME TON DEIKTH NEXT TOY NEOY NA DEIXNEI STON EPOMENO TOY RATING
				user->ratings->prev = rnode;							// KAI KANOUME TON DEIKTH PROHGOUMENO TOY RATING NA DEIXNEI TON NEO
				user->ratings = rnode;									// KAI PAIRNAEI DATA
			}	
			else if(curr == NULL)										// ELEGXOUME AN AYTOS O KOMVOS EINAI NULL
			{															// STO TELOS ? UPDATE TOYS DEIKTES
				prev->next = rnode;										// O EPOMENOS TOY PROHGOUMENOU KOMVOU GINETAI NEOS 
				rnode->prev = prev;										// O PROHGOUMENOS GINETAI PROHGOUMENOS TOY NEOY KOMVOU
			}
			else														// DIAFORETIKA
			{															// KAPOY ENDIAMESA ? UPDATE TOYS DEIKTES
				prev->next = rnode;										// O EPOMENOS TOY PROHGOUMENOU KOMVOU GINETAI NEOS 
				rnode->prev = prev;										// O PROHGOUMENOS GINETAI PROHGOUMENOS TOY NEOY KOMVOU
				rnode->next = curr;										// O EPOMENOS TOY NEOY GINETAI CURRENT
				curr->prev = rnode;										// O PROHGOUMENOS TOY CURRENT GINETAI NEOS
			}
		}
		// ANATHETOUME TA ITEM STON HASH PINAKA
		//printf("%s %d\n", item_name, item_slot);
		if(item_hash_table[item_slot] == NULL)
		{
			// KAINOURGIO ITEM
			ITEM* item = (ITEM*)malloc(sizeof(ITEM));
			strcpy(item->name,item_name);
			item_hash_table[item_slot] = item;
		}

	}
	int id;
	// GRAFOUME TO OUTPUT FILE
	fprintf(outptr, "%d %d\n", num_users, num_items);					// STHN KORYFH VAZOYME TA PLYTHOI USER ITEM
	for(id = 0; id < num_users; id++)									// GIA KATHE USER VASH ID - PERNAME OLO TON PINAKA GIA KATHE ID
	{
		int ItemCount=0;
		int temp=-1;
		int max=0; int min=0; int check=0;
		int user_slot = user_ids[id];																
		USER* user = user_hash_table[user_slot];						
		if(user->num_ratings >= N)										// AN TA RATINGS EINAI KAT ELAXISTO 'N' AXIOLOGISEIS	
		{
			RATING_NODE* UniqueItems = user->ratings;					
			while(UniqueItems != NULL)
			{
				if(temp != UniqueItems->item_id){ItemCount++;}			// METRHMA ITEM UNIQUE IDS
				temp = UniqueItems->item_id;							
				check = UniqueItems->timer;								// PARAMETROS S MAX MIN
				if(check > max){max=check;}
				if(min==0){min = check;}
				else if(check < min){min=check;}
				UniqueItems = UniqueItems->next;
			}
			//printf("MAX : %d MIN : %d S: %d\n",max,min,max-min);
			if(N == 1 || max-min <= S)									// AN N = 1 'H DIAFORA TIMERS = S
			{
				fprintf(outptr, "%d %d\n", id, ItemCount /*,num_ratings*/);	// PRINT ID ITEMS (H TELEYTAIA PARAMETROS STO COMM. EINAI METRHTHS RATING)
				RATING_NODE* rating = user->ratings;
				while(rating != NULL)
				{															// PRINT LOIPES PLHROFORIES
					fprintf(outptr, "%d %d %d %d\n", id, rating->item_id, rating->rating, rating->timer);
					rating = rating->next;
				}
			}
		}		
	}


	*plithos_users = num_users;
	*plithos_items = num_items;

	// KLEISMO KAI APOTHYKEYSH
	fclose(inptr);
	fclose(outptr);

	return 1;
}

void destroy()
{
	int i;
	// FREE ITEM HASH PINAKA KAI DIAFRAFH NODES
	for(i = 0; i < num_items; i++)
	{
		if(item_hash_table[i] != NULL)
		{
			free(item_hash_table[i]);
		}
	}
	free(item_hash_table);

	// FREE USERS HASH PINAKA KAI DIAFRAFH NODES
	for(i = 0; i < num_users; i++)
	{
		if(user_hash_table[i] != NULL)
		{
			RATING_NODE* curr = user_hash_table[i]->ratings;
			while(curr->next != NULL)
			{
				RATING_NODE* delete_node = curr;
				curr = curr->next;
				free(delete_node);
			}
			free(user_hash_table[i]);
		}
	}
	free(user_hash_table);
}
#endif
