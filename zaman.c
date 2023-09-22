#include "philosophers.h"






int main()
{
	for(int i = 0; i <= 10; i++)
		printf("%d\n", rand() % 10);
}
















/* void freele(char *domat)
{
	char *patat = "patates";
	domat = patat;
}

int main()
{
	char *domat = malloc(10);
	freele(domat);
	printf("%shr\n", domat);
	//free(domat);
	return 0;
}

 */










/* typedef struct domat
{
	int shr;
	struct domat *next;
}domat;


typedef struct point_domat
{
	int shr;
	domat *next;
}point_domat;

void new_add(domat *patat)
{
	domat *tmp = malloc(sizeof(domat));
	patat->next = tmp;
}

void dongu(domat *patat)
{
	int i = 0;
	while (i++ < 10)
	{
		patat->shr = i;
		new_add(patat);
		patat = patat->next;
	}
}

void yeni_yap(domat **head)
{
	domat *yeni = malloc(sizeof(domat));
	*head = yeni;
}

int main()
{
	domat *patat = malloc(sizeof(domat));
	domat *head = patat;

	dongu(patat);

	printf("%d\n%d\n%d\n", head->shr, head->next->shr, head->next->next->shr);
	
	// yeni_yap(&head);
	// printf("%d\n%d\n%d\n", head->shr, head->next->shr, head->next->next->shr);
	
	
	return 0;
}


 */
/* int main()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	size_t usec = time.tv_sec * 1000 + (time.tv_usec / 1000);
	size_t msec = time.tv_sec * 1000000 + (time.tv_usec);
	size_t fark = msec / 1000;
	printf("%ld\n%ld\n%ld\n%d\n", usec, msec, fark, (int)time.tv_usec);
	return 0;
}
 */