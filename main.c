#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_STRING_SIZE 50

void v(FILE **tovar, FILE **dodavatelia)
{
    *tovar = fopen("tovar.txt", "r+");

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

    printf("----------------------------------------------------------------------------------------\n----------------------------------------------------------------------------------------\n\n");

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

void n(FILE *tovar, FILE *dodavatelia, int *prevSize,
      char ***nazovTDB, int **pcDB, double **cenaDB, double **hmotnostDB,
      char ***menoDDB, char ***adresaDDB)
{

    if (tovar == NULL || dodavatelia == NULL)
        return;

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

    char buffer[DEFAULT_STRING_SIZE + 1];
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

    const int TOVAR_SIZE = tovarCounter;
    //const int DODAV_SIZE = dodavatelCounter;

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
                //break;
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
}

void s(char **nazovTDB, char **menoDDB, int *pcDB, int size)
{

    if (nazovTDB == NULL || pcDB == NULL)
    {
        printf("Polia nie su vytvorene\n");
        return;
    }

    int dodav;
    scanf("%d", &dodav);
    char *menoDodavatela = menoDDB[dodav - 1];

    for (int i = 0; i < size; i++)
    {
        if (strcmp(menoDodavatela, menoDDB[i]) == 0)
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

    //Bubble sort with item count
    for (int i = 0; i < counter; i++)
    {
        for (int j = 0; j < counter - i - 1; j++)
        {
            //if(orderedHmot[j] != 0 && orderedHmot[j + 1] == 0)
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
            o(tovar);
            break;
        case 'n':
            n(tovar, dodavatelia, &prevSize, &nazovTDB, &pcDB, &cenaDB, &hmotnostDB, &menoDDB, &adresaDDB);
            break;
        case 's':
            s(nazovTDB, menoDDB, pcDB, prevSize);
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
    if(pcDB != NULL){
        free(pcDB);
        free(cenaDB);
        free(hmotnostDB);

        for (int i = 0; i < prevSize; i++){
            free(nazovTDB[i]);
            free(menoDDB[i]);
            free(adresaDDB[i]);
        }

        free(nazovTDB);
        free(menoDDB);
        free(adresaDDB);
    }

    fclose(dodavatelia);
    fclose(tovar);
}