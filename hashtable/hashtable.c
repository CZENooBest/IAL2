/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
    int result = 1;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
        result += key[i];
    }
    return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    int i;
    for (i = 0; i < HT_SIZE; i++) //všechny ukazetele na hodnotu NULL
    {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    ht_item_t *hash_item = (*table)[get_hash(key)]; //položka na indexu key

    for (; hash_item; hash_item = hash_item->next) //hledání klíče
    {
        if(strcmp(key, hash_item->key) == 0)   //porovnání klíče
        {
            return hash_item;  //vrácení ukazatele na prvek
        }
    }
    return NULL;    //nenalezen
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    ht_item_t *hash_item = ht_search(table, key);    //hledání prvku s klíčem key

    if (hash_item)   //pokud už takový je aktualizuje se datová část
    {
        hash_item->value = value;
        return;
    }

    ht_item_t *NEWhash_item = (ht_item_t *) malloc(sizeof (ht_item_t));  //nová položka s klíčem key
    if(NEWhash_item == NULL)
    {
        return; //malloc neuspěl
    }
    NEWhash_item->key = key;
    NEWhash_item->value = value;
    NEWhash_item->next = NULL;
    int hashtag = get_hash(key);    //získání hashe klíče key

    if((hash_item = (*table)[hashtag]))  //pokud existují synonyma, tak je připojíme za nový prvek
    {
        NEWhash_item->next = hash_item;
    }

    (*table)[hashtag] = NEWhash_item;    //uložení nového prvku
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    ht_item_t *hash_item;

    if (table == NULL || key == NULL)
    {
        return NULL;  // Osetreni chyb
    }

    if ((hash_item = ht_search(table, key)) != NULL)
    {
        return &hash_item->value;
    }
    else
    {
        return NULL;
    }
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    int hashtag = get_hash(key);    //získání hashe klíče
    ht_item_t *hash_item = (*table)[hashtag];    //prvek na daném indexu
    ht_item_t *PREVhash_item = NULL;
    ht_item_t *NEXThash_item = NULL;

    for(; hash_item; PREVhash_item = hash_item, hash_item = hash_item->next) //hledá se klíč v synonymech daného klíče
    {
        NEXThash_item = hash_item->next;  //uložení ukazatele na následující prvek

        if(strcmp(key, hash_item->key) == 0) //porovnání
        {
            free(hash_item); //uvolnění

            if(!PREVhash_item)   //byl na začátku seznamu
            {
                (*table)[hashtag] = NEXThash_item;
                return;
            }

            PREVhash_item->next = NEXThash_item; //byl uprostřed seznamu
            return;
        }
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    ht_item_t *hash_item;
    ht_item_t *DELETEhash_item;

    int i;
    for (i = 0; i < HT_SIZE; i++)   //všechny indexy
    {
        hash_item = (*table)[i];
        while (hash_item)    //všechny prvky
        {
            DELETEhash_item = hash_item;
            hash_item = hash_item->next;
            free(DELETEhash_item);   //uvolnění
        }
        (*table)[i] = NULL; //nastavení na daném indexu na NULL
    }
}
