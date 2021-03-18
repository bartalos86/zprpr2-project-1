#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_STRING_SIZE 50

void v(FILE **tovar, FILE **dodavatelia)
{
    *tovar = fopen("tovar.txt", "r");

    if (*tovar == NULL)
    {
        printf("Subor sa nepodarilo otvorit\n");
        return;
    }

    char nazovT[DEFAULT_STRING_SIZE];
    int pcT, idT;
    double cenaT, hmotT;

    while (fscanf(*tovar, "%[^\n]\n%d\n%lf\n%lf\n%d\n", nazovT, &pcT, &cenaT, &hmotT, &idT) > 0)
    {
        printf("Nazov tovaru: %s\nPocet kusov na sklade: %d\nCena: %.2lf\nHmotnost: %.4lf\nID dodavatela: %d\n\n",
               nazovT, pcT, cenaT, hmotT, idT);
    }
    rewind(*tovar);

    printf("----------------------------------------------------------------------------------------\n----------------------------------------------------------------------------------------\n");

    *dodavatelia = fopen("dodavatelia.txt", "r");

    if (*dodavatelia == NULL)
    {
        printf("Neotvoreny subor");
        return;
    }
    int idD;
    char menoD[DEFAULT_STRING_SIZE];
    char adresaD[DEFAULT_STRING_SIZE];

    while (fscanf(*dodavatelia, "%d\n%[^\n]\n%[^\n]\n", &idD, menoD, adresaD) > 0)
    {
        printf("ID dodavatela: %d\nMeno dodavatela: %s\nAdresa dodavatela: %s\n\n",
               idD, menoD, adresaD);
    }
    rewind(*dodavatelia);
}

void o(FILE *tovar)
{
    int dodavatel;
    int idT;
    double cenaT, najhr = 0;
    char nazovT[50];
    char najhrNazov[50];
    scanf("%d", &dodavatel);
    int found = 0;
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

    rewind(tovar);

    if (found)
        printf("Najdrahší tovar je: %s\n", najhrNazov);
    else
        printf("Tovar s dodavatelom neexistuje\n");
}

int n(FILE *tovar, FILE *dodavatelia, int *prevSize,
      char ***nazovTDB, int **pcDB, double **cenaDB, double **hmotnostDB,
      char ***menoDDB, char ***adresaDDB)
{

    if (tovar == NULL || dodavatelia == NULL)
        return 0;

    //Memory free based on previous size
    //TODO: control free mechanism
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
    }

    int tovarCounter = 0;
    int dodavatelCounter = 0;

    char buffer[50];
    while (fscanf(tovar, "%[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n%*[^\n]\n", buffer) > 0)
    {
        tovarCounter++;
    }

    while (fscanf(dodavatelia, "%[^\n]\n%*[^\n]\n%*[^\n]\n", buffer) > 0)
    {
        dodavatelCounter++;
    }

    rewind(tovar);
    rewind(dodavatelia);

    //TODO: Theese maybe should be the same
    const int TOVAR_SIZE = tovarCounter;
    const int DODAV_SIZE = dodavatelCounter;

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

    int *temp_dodavID = malloc(sizeof(int) * TOVAR_SIZE);

    //File stuff
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

        temp_dodavID[counterT] = idT;
        counterT++;
    }
    rewind(tovar);

    int idD;
    char menoD[DEFAULT_STRING_SIZE + 1];
    char adresaD[DEFAULT_STRING_SIZE + 1];

    while (fscanf(dodavatelia, "%d\n%[^\n]\n%[^\n]\n", &idD, menoD, adresaD) > 0)
    {
        //Find the item with the right id
        for (int i = 0; i < TOVAR_SIZE; i++)
        {

            if (idD == temp_dodavID[i])
            {
                strcpy((*menoDDB)[i], menoD);
                strcpy((*adresaDDB)[i], adresaD);
                break;
            }
        }
    }
    rewind(dodavatelia);

    //Control
    /* for (int i = 0; i < TOVAR_SIZE; i++)
    {
        printf("%s\n", (*nazovTDB)[i]);
        printf("%d\n", (*pcDB)[i]);
        printf("%lf\n", (*cenaDB)[i]);
        printf("%lf\n", (*hmotnostDB)[i]);
        printf("%s\n", (*menoDDB)[i]);
        printf("%s\n\n", (*adresaDDB)[i]);
        
        //printf("Nazov tovaru: %s  Dodavatel: %s Adresa: %s\n", **(*nazovTDB+i), *menoDDB[i], *adresaDDB[i]);
    }*/

    *prevSize = TOVAR_SIZE;
    // printf("tovar: %d %d\n", tovarCounter, dodavatelCounter);

    free(temp_dodavID);

    return TOVAR_SIZE;
}

void s(char **nazovTDB, int *pcDB)
{

    if (nazovTDB == NULL || pcDB == NULL)
    {
        printf("Polia nie su vytvorene\n");
        return;
    }

    int dodav;
    scanf("%d", &dodav);

    printf("%s (%d na sklade)\n", nazovTDB[dodav - 1], pcDB[dodav - 1]);
}

void h(int *pcDB, int size)
{

    if (pcDB == NULL)
    {
        printf("Polia nie su vytvorene\n");
        return;
    }

    int histCount[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int ranges[11] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90,100};

    for (int i = 0; i < size; i++)
    {
        //Shorter, but less optimised and more versitile
        for (int j = 0; j < 10; j++)
        {
            if (pcDB[i] >= ranges[j] && pcDB[i] <= (ranges[j + 1] - 1))
            {
                histCount[j]++;
                break;
            }
        }

        //Pritive but probably the more effective method
        /*if (pcDB[i] >= 0 && pcDB[i] <= 9)
            histCount[0]++;
        else if (pcDB[i] >= 10 && pcDB[i] <= 19)
            histCount[1]++;
        else if (pcDB[i] >= 20 && pcDB[i] <= 29)
            histCount[2]++;
        else if (pcDB[i] >= 30 && pcDB[i] <= 39)
            histCount[3]++;
        else if (pcDB[i] >= 40 && pcDB[i] <= 49)
            histCount[4]++;
        else if (pcDB[i] >= 50 && pcDB[i] <= 59)
            histCount[5]++;
        else if (pcDB[i] >= 60 && pcDB[i] <= 69)
            histCount[6]++;
        else if (pcDB[i] >= 70 && pcDB[i] <= 79)
            histCount[7]++;
        else if (pcDB[i] >= 80 && pcDB[i] <= 89)
            histCount[8]++;
        else if (pcDB[i] >= 90 && pcDB[i] <= 99)
            histCount[9]++;*/
    }

    for(int i = 0; i < 10; i++)
    {
        printf("<%d,%d>: %d\n", ranges[i], ranges[i + 1] - 1, histCount[i]);
    }
}

void p(){
    
}

int main()
{
    FILE *tovar = NULL;
    FILE *dodavatelia = NULL;

    char **nazovTDB = NULL;
    int *pcDB = NULL;
    double *cenaDB = NULL, *hmotnostDB = NULL;
    char **menoDDB = NULL, **adresaDDB = NULL;

    int prevSize = 0;

    v(&tovar, &dodavatelia);

    //o(tovar);
    n(tovar, dodavatelia, &prevSize, &nazovTDB, &pcDB, &cenaDB, &hmotnostDB, &menoDDB, &adresaDDB);

    //Control
    /*  for (int i = 0; i < prevSize; i++)
    {
        printf("%s\n", (nazovTDB)[i]);
        printf("%d\n", (pcDB)[i]);
        printf("%lf\n", (cenaDB)[i]);
        printf("%lf\n", (hmotnostDB)[i]);
        printf("%s\n", (menoDDB)[i]);
        printf("%s\n\n", (adresaDDB)[i]);
        
        //printf("Nazov tovaru: %s  Dodavatel: %s Adresa: %s\n", **(*nazovTDB+i), *menoDDB[i], *adresaDDB[i]);
    }*/
    //s(nazovTDB, pcDB);
    h(pcDB, prevSize);
}