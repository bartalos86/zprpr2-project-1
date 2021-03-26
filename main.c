#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_STRING_SIZE 50

void v(FILE **tovar, FILE **dodavatelia)
{
    //File TOVAR
    *tovar = fopen("tovar.txt", "r+");

    if (*tovar == NULL)
    {
        printf("Subor sa nepodarilo otvorit\n");
        return;
    }

    char nazovT[DEFAULT_STRING_SIZE+1];
    int pcT, idT;
    double cenaT, hmotT;

    while (fscanf(*tovar, "%[^\n]\n%d\n%lf\n%lf\n%d\n", nazovT, &pcT, &cenaT, &hmotT, &idT) > 0)
    {
        printf("Nazov tovaru: %s\nPocet kusov na sklade: %d\nCena: %g\nHmotnost: %g\nID dodavatela: %d\n\n",
               nazovT, pcT, cenaT, hmotT, idT);
    }
    rewind(*tovar);

    printf("----------------------------------------------------------------------------------------\n----------------------------------------------------------------------------------------\n\n");

    //File DODAVATELIA
    *dodavatelia = fopen("dodavatelia.txt", "r");

    if (*dodavatelia == NULL)
    {
        printf("Neotvoreny subor");
        return;
    }
    int idD;
    char menoD[DEFAULT_STRING_SIZE+1];
    char adresaD[DEFAULT_STRING_SIZE+1];

    while (fscanf(*dodavatelia, "%d\n%[^\n]\n%[^\n]\n", &idD, menoD, adresaD) > 0)
    {
        printf("ID dodavatela: %d\nMeno dodavatela: %s\nAdresa dodavatela: %s\n\n",
               idD, menoD, adresaD);
    }
    rewind(*dodavatelia);
}

void o(FILE *tovar, char **nazovTDB, double *cenaDB, int *idDDB, int size)
{
    int dodavatel;
    int idT;
    double cenaT, najhr = 0;
    char nazovT[DEFAULT_STRING_SIZE + 1];
    char najhrNazov[DEFAULT_STRING_SIZE + 1];
    scanf("%d", &dodavatel);
    int found = 0;

    //If the arrays arent created yet
    if (size == 0)
    {
        if (tovar == NULL)
        {
            printf("Subor nepodarilo otvorit\n");
            return;
        }

        while (fscanf(tovar, "%[^\n]\n%*[^\n]\n%lf\n%*[^\n]\n%d\n", nazovT, &cenaT, &idT) > 0)
        {
            if (idT == dodavatel)
            {

                if (najhr < cenaT)
                {
                    najhr = cenaT;
                    strcpy(najhrNazov, nazovT);
                }

                found = 1;
            }
        }
    }
    else
    {

        //If the arrays are allready created
        for (int i = 0; i < size; i++)
        {
            if (dodavatel == idDDB[i])
            {

                if (najhr < cenaDB[i])
                {
                    najhr = cenaDB[i];
                    strcpy(najhrNazov, nazovTDB[i]);
                }

                found = 1;
            }
        }
    }

    rewind(tovar);

    if (found)
        printf("Najdrahsi tovar je: %s\n", najhrNazov);
}

void n(FILE *tovar, FILE *dodavatelia, int *prevSize,
       char ***nazovTDB, int **pcDB, double **cenaDB, double **hmotnostDB,
       char ***menoDDB, char ***adresaDDB, int **idDDB)
{

    if (tovar == NULL || dodavatelia == NULL)
    {
        printf("Subor nepodarilo otvorit\n");
        return;
    }

    //Memory free based if there has been allocated previously
    if (*prevSize > 0)
    {
        for (int i = 0; i < *prevSize; i++)
        {
            free((*nazovTDB)[i]);
            free((*menoDDB)[i]);
            free((*adresaDDB)[i]);
        }

        free(*nazovTDB);
        free(*menoDDB);
        free(*adresaDDB);

        free(*pcDB);
        free(*cenaDB);
        free(*hmotnostDB);
        free(*idDDB);
    }

    int tovarCounter = 0;
  
    char tempbuffer[DEFAULT_STRING_SIZE + 1];

    /*
    Count the number of products, we need to read atleast one information per product
    Probably fgets() wold do it too, but I think its a little bit more secure and easier with frpintf()
    */
    while (fscanf(tovar, "%[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n", tempbuffer) > 0)
    {
        tovarCounter++;
    }

    rewind(tovar);
    rewind(dodavatelia);

    const int TOVAR_SIZE = tovarCounter;

    //Memory allocation
    *nazovTDB = (char **)malloc(sizeof(char *) * TOVAR_SIZE);
    *menoDDB = (char **)malloc(sizeof(char *) * TOVAR_SIZE);
    *adresaDDB = (char **)malloc(sizeof(char *) * TOVAR_SIZE);

    for (int i = 0; i < TOVAR_SIZE; i++)
    {
        (*nazovTDB)[i] = (char *)malloc(sizeof(char) * (DEFAULT_STRING_SIZE + 1));
        (*menoDDB)[i] = (char *)malloc(sizeof(char) * (DEFAULT_STRING_SIZE + 1));
        (*adresaDDB)[i] = (char *)malloc(sizeof(char) * (DEFAULT_STRING_SIZE + 1));
    }

    *pcDB = malloc(sizeof(int) * TOVAR_SIZE);
    *cenaDB = malloc(sizeof(double) * TOVAR_SIZE);
    *hmotnostDB = malloc(sizeof(double) * TOVAR_SIZE);
    *idDDB = malloc(sizeof(int) * TOVAR_SIZE);

    //Loading from file
    
    char nazovT[DEFAULT_STRING_SIZE + 1];
    int pcT, idT;
    double cenaT, hmotT;
    int counterT = 0;

    while (fscanf(tovar, "%[^\n]\n%d\n%lf\n%lf\n%d\n", nazovT, &pcT, &cenaT, &hmotT, &idT) > 0)
    {

        strcpy((*nazovTDB)[counterT], nazovT);
        (*pcDB)[counterT] = pcT;
        (*cenaDB)[counterT] = cenaT;
        (*hmotnostDB)[counterT] = hmotT;
        (*idDDB)[counterT] = idT;

        counterT++;
    }
    rewind(tovar);

    int idD;
    char menoD[DEFAULT_STRING_SIZE + 1];
    char adresaD[DEFAULT_STRING_SIZE + 1];

    while (fscanf(dodavatelia, "%d\n%[^\n]\n%[^\n]\n", &idD, menoD, adresaD) > 0)
    {
        //Finding the items with the right id
        for (int i = 0; i < TOVAR_SIZE; i++)
        {
            if (idD == (*idDDB)[i])
            {
                strcpy((*menoDDB)[i], menoD);
                strcpy((*adresaDDB)[i], adresaD);
            }
        }
    }
    
    rewind(dodavatelia);
    *prevSize = TOVAR_SIZE;
}

void s(char **nazovTDB, int *pcDB, int *idDDB, int size)
{

    if (nazovTDB == NULL || pcDB == NULL || idDDB == NULL)
    {
        printf("Polia nie su vytvorene\n");
        return;
    }

    int dodav;
    scanf("%d", &dodav);

    for (int i = 0; i < size; i++)
    {
        if (dodav == idDDB[i])
        {
            printf("%s (%d na sklade)\n", nazovTDB[i], pcDB[i]);
        }
    }
}

void h(int *pcDB, int size)
{

    if (pcDB == NULL)
    {
        printf("Polia nie su vytvorene\n");
        return;
    }

    int histCount[10] = {0};
    int ranges[11] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    for (int i = 0; i < size; i++)
    {
        //Dynamic range check
        for (int j = 0; j < 10; j++)
        {
            if (pcDB[i] >= ranges[j] && pcDB[i] <= (ranges[j + 1] - 1))
            {
                histCount[j]++;
                break;
            }
        }
    }

    for (int i = 0; i < 10; i++)
    {
        printf("<%d,%d>: %d\n", ranges[i], ranges[i + 1] - 1, histCount[i]);
    }
}

void p(FILE *tovar, char **nazovTDB, int **pcDB, double *cenaDB,
       double *hmotnostDB, int size)
{
    char nazov[DEFAULT_STRING_SIZE + 1];
    int novHod;
    scanf("%s", nazov);
    scanf("%d", &novHod);

    if (nazovTDB == NULL || *pcDB == NULL)
    {
        printf("Polia nie su vytvorene\n");
        return;
    }

    if (tovar == NULL)
    {
        printf("Neotvoreny subor\n");
        return;
    }

    for (int i = 0; i < size; i++)
    {
        if (strcmp(nazovTDB[i], nazov) == 0)
        {
            (*pcDB)[i] = novHod;
            break;
        }
    }

    int *tempIDs = malloc(sizeof(int) * size);
    int count = 0, currID;

    while (fscanf(tovar, "%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%d\n", &currID) > 0)
    {
        tempIDs[count++] = currID;
    }

    rewind(tovar);
    for (int i = 0; i < size; i++)
    {
        if (i < size - 1)
            fprintf(tovar, "%s\n%d\n%g\n%g\n%d\n\n", nazovTDB[i], (*pcDB)[i], cenaDB[i], hmotnostDB[i], tempIDs[i]);
        else
            fprintf(tovar, "%s\n%d\n%g\n%g\n%d\n", nazovTDB[i], (*pcDB)[i], cenaDB[i], hmotnostDB[i], tempIDs[i]);
    }

    fflush(tovar);

    rewind(tovar);

    printf("Aktualny poctu kusov na sklade tovaru %s je: %d.\n", nazov, novHod);

    free(tempIDs);
}

void z(char **nazovTDB, double *hmotnostDB, int size)
{

    double intMin, intMax;
    scanf("%lf", &intMin);
    scanf("%lf", &intMax);

    if (nazovTDB == NULL || hmotnostDB == NULL)
    {
        printf("Polia nie su vytvorene\n");
        return;
    }

    if (intMin > intMax)
    {
        printf("Prve cislo musi byt mensie ako druhe cislo.\n");
        return;
    }

    double *orderedHmot = calloc(size, sizeof(double));
    double *orderedIndex = malloc(sizeof(double) * size);

    int counter = 0;

    //Interval check
    for (int i = 0; i < size; i++)
    {
        double currHmot = hmotnostDB[i];

        if (currHmot >= intMin && currHmot <= intMax)
        {
            orderedHmot[counter] = currHmot;
            orderedIndex[counter] = i;
            counter++;
        }
    }

    //Bubble sort only on an interval
    for (int i = 0; i < counter; i++)
    {
        for (int j = 0; j < counter - i - 1; j++)
        {
            if (orderedHmot[j] > orderedHmot[j + 1])
            {
                double temp = orderedHmot[j];
                int tempId = orderedIndex[j];
                orderedIndex[j] = orderedIndex[j + 1];
                orderedIndex[j + 1] = tempId;

                orderedHmot[j] = orderedHmot[j + 1];
                orderedHmot[j + 1] = temp;
            }
        }
    }

    //Printing to the console
    for (int i = 0; i < counter; i++)
    {
        int index = orderedIndex[i];
        printf("%s\n", nazovTDB[index]);
    }

    free(orderedHmot);
    free(orderedIndex);
}

int main()
{
    FILE *tovar = NULL;
    FILE *dodavatelia = NULL;

    char **nazovTDB = NULL;
    int *pcDB = NULL;
    double *cenaDB = NULL, *hmotnostDB = NULL;
    char **menoDDB = NULL, **adresaDDB = NULL;
    int *idDDB = NULL;

    int prevSize = 0;

    char input;
    int exit = 0;
    while (!exit)
    {
        scanf("%c", &input);
        fflush(stdin);
        switch (input)
        {
        case 'v':
            v(&tovar, &dodavatelia);
            break;
        case 'o':
            o(tovar, nazovTDB, cenaDB, idDDB, prevSize);
            break;
        case 'n':
            n(tovar, dodavatelia, &prevSize, &nazovTDB, &pcDB, &cenaDB, &hmotnostDB, &menoDDB, &adresaDDB, &idDDB);
            break;
        case 's':
            s(nazovTDB, pcDB, idDDB, prevSize);
            break;
        case 'h':
            h(pcDB, prevSize);
            break;
        case 'p':
            p(tovar, nazovTDB, &pcDB, cenaDB, hmotnostDB, prevSize);
            break;
        case 'z':
            z(nazovTDB, hmotnostDB, prevSize);
            break;
        case 'k':
            exit = 1;
            break;
        }
    }

    //Free based on the number of elements allocated
    if (prevSize > 0)
    {
        free(pcDB);
        free(cenaDB);
        free(hmotnostDB);

        for (int i = 0; i < prevSize; i++)
        {
            free(nazovTDB[i]);
            free(menoDDB[i]);
            free(adresaDDB[i]);
        }

        free(nazovTDB);
        free(menoDDB);
        free(adresaDDB);
        free(idDDB);
    }

    if (tovar != NULL)
        fclose(tovar);

    if (dodavatelia != NULL)
        fclose(dodavatelia);
}