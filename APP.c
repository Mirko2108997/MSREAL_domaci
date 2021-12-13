#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ()
{
	FILE *fp;
	char *str;
	size_t num_of_bytes = 100;
	char string[100];
	int fja, skrati;
	
	while(1)
	{
	  printf("izaberi jednu od opcija\n");
	  printf("1) ispis stringa \n");
	  printf("2) upis stringa \n");
	  printf("3) brisanje stringa \n");
	  printf("4) brisanje razmaka \n");
	  printf("5) dodavanje stringa na postojeci \n");
	  printf("6) smanjivanje stringa \n");
	  printf("7) brisanje odabrane rijeci iz stringa \n");
	  printf("8) izlaz iz programa \n");
	  
	  scanf( "%d", &fja);
	  if (fja > 0 && fja < 9){
	    
	  switch (fja){
	  case 1:
		fp = fopen ("/dev/stred", "r");
		if(fp==NULL)
		{
			puts("Problem pri otvaranju /dev/stred");
			return -1;
		}


		str = (char *)malloc(num_of_bytes+1); 
		getline(&str, &num_of_bytes, fp); 
		printf("string je: %s \n\n", str);
		if(fclose(fp))
		  {
			puts("Problem pri zatvaranju /dev/stred");
			return -1;
		  }

		free(str);
		break;
	  case 2 :
	    fp = fopen ("/dev/stred", "w");
		if(fp==NULL)
		{
			puts("Problem pri otvaranju /dev/stred");
			return -1;
		}

		printf("\nunesite string \n");
		str = (char *)malloc(num_of_bytes+1);  
		fgets(str, 100, stdin);
		fprintf (fp,"string=%s\n", str);
		if(fclose(fp))
		  {
			puts("Problem pri zatvaranju /dev/stred");
			return -1;
		  }
		free(str);
	    break;

	  case 3 :
	    fp = fopen ("/dev/stred", "w");
		if(fp==NULL)
		{
			puts("Problem pri otvaranju /dev/stred");
			return -1;
		}

		
		str = (char *)malloc(num_of_bytes+1);
		fputs ("clear\n",fp);
		if(fclose(fp))
		  {
			puts("Problem pri zatvaranju /dev/stred");
			return -1;
		  }
		free(str);
	    break;
	  case 4 :
	    fp = fopen ("/dev/stred", "w");
		if(fp==NULL)
		{
			puts("Problem pri otvaranju /dev/stred");
			return -1;
		}

		
		str = (char *)malloc(num_of_bytes+1);
		fputs ("shrink\n",fp);
		if(fclose(fp))
		  {
			puts("Problem pri zatvaranju /dev/stred");
			return -1;
		  }
		free(str);
	    break;

	  case 5 :
	    fp = fopen ("/dev/stred", "w");
		if(fp==NULL)
		{
			puts("Problem pri otvaranju /dev/stred");
			return -1;
		}

		printf("unesite string \n");
		
		str = (char *)malloc(num_of_bytes+1);  
		scanf( "%100s", str);
		fprintf (fp,"append=%s", str);
		if(fclose(fp))
		  {
			puts("Problem pri zatvaranju /dev/stred");
			return -1;
		  }
		
		free(str);
	    break;

	  case 6 :
	    fp = fopen ("/dev/stred", "w");
		if(fp==NULL)
		{
			puts("Problem pri otvaranju /dev/stred");
			return -1;
		}

		printf("unesite broj za skracivanje \n");
		
		str = (char *)malloc(num_of_bytes+1);  
		scanf( "%d", &skrati);
		fprintf (fp,"truncate=%d", skrati);
		if(fclose(fp))
		  {
			puts("Problem pri zatvaranju /dev/stred");
			return -1;
		  }
		
		free(str);
	    break;

	  case 7 :
	    fp = fopen ("/dev/stred", "w");
		if(fp==NULL)
		{
			puts("Problem pri otvaranju /dev/stred");
			return -1;
		}

		printf("unesite rijec za brisanje\n");
		
		str = (char *)malloc(num_of_bytes+1);  
		scanf( "%s", str);
		fprintf (fp,"remove=%s\n", str);
		if(fclose(fp))
		  {
			puts("Problem pri zatvaranju /dev/stred");
			return -1;
		  }
		free(str);
		break;
	  case 8:
	    return 0;
	    break;
	  default:
	    break;
	  }
	  }
	}
}
