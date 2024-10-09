#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRANDEZZASTRINGA 800
#define MAXLIBRIPERCATEGORIA 15
#define MAXLUNGHEZZARIGACSV 2000
#define MAXCATEGORIE 10

int QuantiLibri = 0;
int QuanteCategorie = 0;

typedef struct Libro
{
    char titolo[GRANDEZZASTRINGA];
    char autore[GRANDEZZASTRINGA];
    int annoPubblicazione;
    float prezzo;
} Libro;

typedef struct CategoriaLibri
{
    char nomeCategoria[GRANDEZZASTRINGA];
    Libro libriCategoria[MAXLIBRIPERCATEGORIA];
    int quantiLibriPerCategoria;
} CategoriaLibri;

CategoriaLibri Libreria[MAXCATEGORIE];

void StampaCategoria(CategoriaLibri catalogo)
{
    printf("\nNome categoria:  %s\n", catalogo.nomeCategoria);
    for (int i = 0; i < catalogo.quantiLibriPerCategoria; i++)
    {
        printf("\nTitolo: %s, Autore: %s, Anno di Pubblicazione: %d, Prezzo: %.2f",
               catalogo.libriCategoria[i].titolo,
               catalogo.libriCategoria[i].autore,
               catalogo.libriCategoria[i].annoPubblicazione,
               catalogo.libriCategoria[i].prezzo);
    }
}

void CercaLibroPerTitolo(CategoriaLibri listaLibriCompleta[], char titoloLibro[])
{
    int trovato = 0; // variabile per controllare se il libro è stato trovato
    for (int i = 0; i < QuanteCategorie; i++)
    {
        for (int k = 0; k < listaLibriCompleta[i].quantiLibriPerCategoria; k++)
        {
            if (strcmp(titoloLibro, listaLibriCompleta[i].libriCategoria[k].titolo) == 0) //se trova il libro
            {
                printf("Libro trovato:\n");
                printf("Titolo: %s, Autore: %s, Anno di pubblicazione: %d, Prezzo: %.2f, Categoria: %s\n",
                       listaLibriCompleta[i].libriCategoria[k].titolo,
                       listaLibriCompleta[i].libriCategoria[k].autore,
                       listaLibriCompleta[i].libriCategoria[k].annoPubblicazione,
                       listaLibriCompleta[i].libriCategoria[k].prezzo,
                       listaLibriCompleta[i].nomeCategoria);
                trovato = 1;
            }
        }
    }
    if (!trovato)
    {
        printf("Nessun libro trovato con il titolo: %s\n", titoloLibro);
    }
}

int menu() //ritorna la scelta
{
    printf("\n[1] Visualizza elenco libri per categoria");
    printf("\n[2] Cerca Libro");
    printf("\n[3] Aggiungi libro");
    printf("\n[4] Elimina libro");
    printf("\n[0] Chiudi programma");
    printf("\ninserisci l'opzione scelta: ");

    int opzioneScelta;
    scanf("%d", &opzioneScelta);
    getchar(); 
    return opzioneScelta;
}

void AggiungiLibro(Libro libroDaAggiungere, char *nomeCategoria, FILE *fileScrittura)
{

    // Scrivi il libro nel file CSV
    fprintf(fileScrittura, "%s,%s,%d,%.2f,%s\n",
            libroDaAggiungere.titolo,
            libroDaAggiungere.autore,
            libroDaAggiungere.annoPubblicazione,
            libroDaAggiungere.prezzo,
            nomeCategoria);
}

void InizializzaStructConDatiCSV(FILE *LetturaCSV) //popola le struct con i dati del CSV
{
    for (int i = 0; i < MAXCATEGORIE; i++)
    {
        Libreria[i].quantiLibriPerCategoria = 0;
    }

    Libro temp;
    char bufferStringa[MAXLUNGHEZZARIGACSV];
    char bufferNomeCategoria[GRANDEZZASTRINGA];

    while (fgets(bufferStringa, MAXLUNGHEZZARIGACSV, LetturaCSV))
    {
        char *campoCSV = strtok(bufferStringa, ","); //separo e leggo i campi del csv
        if (campoCSV != NULL)
            strcpy(temp.titolo, campoCSV);

        campoCSV = strtok(NULL, ",");
        if (campoCSV != NULL)
            strcpy(temp.autore, campoCSV);

        campoCSV = strtok(NULL, ",");
        if (campoCSV != NULL)
            temp.annoPubblicazione = atoi(campoCSV);

        campoCSV = strtok(NULL, ",");
        if (campoCSV != NULL)
            temp.prezzo = atof(campoCSV);

        campoCSV = strtok(NULL, ",");
        if (campoCSV != NULL)
            strcpy(bufferNomeCategoria, campoCSV);

        QuantiLibri++;
        int nuovaCategoria = 1;

    
        for (int i = 0; i < QuanteCategorie; i++) //cerca se esiste già la categoria, altrimenti la aggiunge
        {
            if (strcmp(bufferNomeCategoria, Libreria[i].nomeCategoria) == 0)
            {
                Libreria[i].libriCategoria[Libreria[i].quantiLibriPerCategoria] = temp;
                Libreria[i].quantiLibriPerCategoria++;
                nuovaCategoria = 0;
                break; 
            }
        }

        
        if (nuovaCategoria == 1 && QuanteCategorie < MAXCATEGORIE)
        {
            strcpy(Libreria[QuanteCategorie].nomeCategoria, bufferNomeCategoria);
            Libreria[QuanteCategorie].libriCategoria[0] = temp;
            Libreria[QuanteCategorie].quantiLibriPerCategoria = 1;
            QuanteCategorie++;
        }
    }
}

void EliminaLibro(char *titoloLibro)
{

    Libro libriRimanenti[MAXLIBRIPERCATEGORIA * MAXCATEGORIE];
    int temp = 0;

  
    for (int i = 0; i < QuanteCategorie; i++)
    {
        for (int k = 0; k < Libreria[i].quantiLibriPerCategoria; k++)
        {
            if (strcmp(Libreria[i].libriCategoria[k].titolo, titoloLibro) != 0) // controllo tutto l'array e cerco il libro da eliminare
            {
                libriRimanenti[temp++] = Libreria[i].libriCategoria[k]; // se non devo eliminarlo lo inserisco nell'array libri rimanenti
            }
        }
    }

    FILE *tmp = fopen("Libri.csv", "w");
    fprintf(tmp, " "); // cancello il contenuto del file
    fclose(tmp);

    FILE *fileAppend = fopen("Libri.csv", "a");

    for (int i = 0; i < QuantiLibri-1; i++)
    {
        AggiungiLibro(libriRimanenti[i], Libreria[i / MAXLIBRIPERCATEGORIA].nomeCategoria, fileAppend);
    }
    fclose(fileAppend);
    printf("Libro '%s' eliminato con successo!\n", titoloLibro);
}

int main()
{
    FILE *LetturaCSV = fopen("Libri.csv", "r");
    if (LetturaCSV == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        return 1;
    }

    InizializzaStructConDatiCSV(LetturaCSV);
    fclose(LetturaCSV);

    int scelta;
    do
    {
        scelta = menu();
        switch (scelta)
        {
        case 1://stampa categorie con libri
        {

            for (int i = 0; i < QuanteCategorie; i++)
            {
                StampaCategoria(Libreria[i]);
                printf("\n=============================\n");
            }
        }
        break;
        case 2: //ricerca libro
        {
            char titoloLibro[GRANDEZZASTRINGA];
            printf("\nInserisci il titolo del libro da cercare: ");
            scanf("%s", titoloLibro);

            CercaLibroPerTitolo(Libreria, titoloLibro);
        }
        break;
        case 3: //aggiungi libro
        {
            FILE *ScritturaCSV = fopen("Libri.csv", "a");
            if (ScritturaCSV == NULL)
            {
                printf("Errore nell'apertura del file.\n");
                return 1;
            }

            Libro nuovoLibro;
            char nomeCategoria[GRANDEZZASTRINGA];

            printf("\nInserisci il titolo del libro: ");
            scanf("%[^\n]", nuovoLibro.titolo); // input stringa con spazi
            getchar();

            printf("Inserisci l'autore del libro: ");
            scanf("%[^\n]", nuovoLibro.autore);
            getchar();

            printf("Inserisci l'anno di pubblicazione: ");
            scanf("%d", &nuovoLibro.annoPubblicazione);

            printf("Inserisci il prezzo: ");
            scanf("%f", &nuovoLibro.prezzo);
            getchar();

            printf("Inserisci il nome della categoria: ");
            scanf("%s", nomeCategoria);

            AggiungiLibro(nuovoLibro, nomeCategoria, ScritturaCSV);

            fclose(ScritturaCSV);
            printf("Libro aggiunto con successo!\n");

            FILE *LetturaCSV = fopen("Libri.csv", "r");
            if (LetturaCSV == NULL)
            {
                printf("Errore nell'apertura del file.\n");
                return 1;
            }

            InizializzaStructConDatiCSV(LetturaCSV); //aggiorno i dati nel programma con quelli contenuti nel CSV
            fclose(LetturaCSV);
        }
        break;
        case 4: //elimina libro
        {

            char titoloLibro[GRANDEZZASTRINGA];
            printf("\nInserisci il titolo del libro da eliminare: ");
            scanf("%[^\n]", titoloLibro); 

            EliminaLibro(titoloLibro);

            FILE *LetturaCSV = fopen("Libri.csv", "r");
            if (LetturaCSV == NULL)
            {
                printf("Errore nell'apertura del file.\n");
                return 1;
            }

            InizializzaStructConDatiCSV(LetturaCSV);  //aggiorno i dati nel programma con quelli contenuti nel CSV
            fclose(LetturaCSV);
        }
        break;
        }
    } while (scelta != 0);

    return 0;
}
