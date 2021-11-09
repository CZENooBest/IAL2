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
    ht_item_t *item = (*table)[get_hash(key)]; //položka na indexu key

    for (; item; item = item->next) //hledání klíče
    {
          if(strcmp(key, item->key) == 0)   //porovnání klíče
        {
              return item;  //vrácení ukazatele na prvek
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
    ht_item_t *item = ht_search(table, key);    //hledání prvku s klíčem key

    if (item)   //pokud už takový je aktualizuje se datová část
    {
        item->value = value;
        return;
    }

    ht_item_t *NEWitem = (ht_item_t *) malloc(sizeof (ht_item_t));  //nová položka s klíčem key
    if(NEWitem == NULL)
    {
        return; //malloc neuspěl
    }
    NEWitem->key = key;
    NEWitem->value = value;
    NEWitem->next = NULL;
    int hashtag = get_hash(key);    //získání hashe klíče key

    if((item = (*table)[hashtag]))  //pokud existují synonyma, tak je připojíme za nový prvek
    {
        NEWitem->next = item;
    }

    (*table)[hashtag] = NEWitem;    //uložení nového prvku
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
    ht_item_t *item = ht_search(table, key);    //hledání prvku s klíčem key
    return item ? &(item->value) : NULL;    //pokud existuje, vrátí se data, pokud ne, tak NULL
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
    ht_item_t *item = (*table)[hashtag];    //prvek na daném indexu
    ht_item_t *PREVitem = NULL;
    ht_item_t *NEXTitem = NULL;

    for(; item; PREVitem = item, item = item->next) //hledá se klíč v synonymech daného klíče
    {
        NEXTitem = item->next;  //uložení ukazatele na následující prvek

        if(strcmp(key, item->key) == 0) //porovnání
        {
            free(item); //uvolnění

            if(!PREVitem)   //byl na začátku seznamu
            {
                (*table)[hashtag] = NEXTitem;
                return;
            }

            PREVitem->next = NEXTitem; //byl uprostřed seznamu
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
    ht_item_t *item;
    ht_item_t *DELETEitem;

    int i;
    for (i = 0; i < HT_SIZE; i++)   //všechny indexy
    {
        item = (*table)[i];
        while (item)    //všechny prvky
        {
            DELETEitem = item;
            item = item->next;
            free(DELETEitem);   //uvolnění
        }
        (*table)[i] = NULL; //nastavení na daném indexu na NULL
    }
}
