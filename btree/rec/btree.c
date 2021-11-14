/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree){
    *tree = NULL;  //tree na hodnotu NULL
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if(tree == NULL)    //kontrola jestli máme strom
    {
        return false;
    }

    if (key < tree->key)    //pokud je hledaný klíč menší než klíč ve stromu
    {
        return bst_search(tree->left, key, value);  //posuneme se o hodnotu doleva
    }

    if (tree->key == key)   //pokud se hledaný klíč rovná klíči ve stromu
    {
        *value = tree->value;   //uložíme hodnotu
        return true;            //vracíme NULL
    }

    return bst_search(tree->right, key, value);     //nic z předešlého neplatí takže se posuneme doprava
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if(!*tree)  //kontrola jestli máme strom
    {
        *tree = (bst_node_t *) malloc(sizeof(struct bst_node));
        if (!(*tree))
        {
            return; //nezdařil se malloc
        }

        (*tree)->key = key;         //
        (*tree)->value = value;     //
        (*tree)->left = NULL;       //nový element
        (*tree)->right = NULL;      //
    }

    if ((*tree)->key > key)     //pokud je hledaný klíč menší než klíč
    {
        bst_insert(&((*tree)->left), key, value);   //posunu se doleva
        return;
    }

    if ((*tree)->key == key)    //pokud je hledaný klíč rovný klíči
    {
        (*tree)->value = value;     //přepíšu value
        return;
    }

    bst_insert(&((*tree)->right), key, value);  //nic z toho předtím takže se posunu doprava
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *RIGHTnode;

    if((*tree)->right == NULL)  //kontrola jestli můžu jít doprava
    {
        RIGHTnode = *tree;                  // nemůžu takže uložím tree na nejpravější
        target->key = RIGHTnode->key;       //
        target->value = RIGHTnode->value;   //

        target->left = NULL;
        if (RIGHTnode->left)
        {
            target->left = RIGHTnode->left;  //pokud má nejpravější levou větev
        }

        free(RIGHTnode);
    }

    if (!(*tree)->right->right)     //kontrola jestli pravý pravého není NULL
    {
        RIGHTnode = (*tree)->right;        //
        target->key = RIGHTnode->key;      // přepsání hodnoty
        target->value = RIGHTnode->value;  //
        (*tree)->right = NULL;

        if(RIGHTnode->left)
        {
            (*tree)->right = RIGHTnode->left;
        }

        free(RIGHTnode);
        return;
    }

    bst_replace_by_rightmost(target, &((*tree)->right));
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if(*tree == NULL)
    {
        return;
    }

    bst_node_t *DELnode;

    if (key < (*tree)->key)
    {
        if ((*tree)->left && (*tree)->left->key == key)
        {
            DELnode = (*tree)->left;

            if (DELnode->left && DELnode->right)
            {
                bst_replace_by_rightmost(DELnode, &(DELnode->left));
                return;
            }

            if (DELnode->left && !DELnode->right)
            {
                (*tree)->left = DELnode->left;
            }
            else if (!DELnode->left && DELnode->right)
            {
                (*tree)->left = DELnode->right;
            }
            else
            {
                (*tree)->left = NULL;
            }

            free(DELnode);
            return;
        }

        bst_delete(&((*tree)->left), key);
        return;
    }

    if (key > (*tree)->key)
    {
        if ((*tree)->right && (*tree)->right->key == key)
        {
            DELnode = (*tree)->right;

            if (DELnode->left && DELnode->right)
            {
                bst_replace_by_rightmost(DELnode, &(DELnode->left));
                return;
            }

            if (DELnode->left && !DELnode->right)
            {
                (*tree)->right = DELnode->left;
            }
            else if (!DELnode->left && DELnode->right)
            {
                (*tree)->right = DELnode->right;
            }
            else
            {
                (*tree)->right = NULL;
            }

            free(DELnode);
            return;
        }

        bst_delete(&((*tree)->right), key);
        return;
    }

    DELnode = *tree;

    if (DELnode->left && DELnode->right)
    {
        bst_replace_by_rightmost(DELnode, &(DELnode->left));
        return;
    }

    if(DELnode->left && !DELnode->right)
    {
        *tree = DELnode->left;
    }
    else if (!DELnode->left && DELnode->right)
    {
        *tree = DELnode->right;
    }
    else
    {
        *tree = NULL;
    }

    free(DELnode);
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if(!*tree)
    {
        return;
    }
    bst_dispose(&((*tree)->left));
    bst_dispose(&((*tree)->right));

    free(*tree);
    *tree = NULL;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    if(!tree)
    {
        return;
    }

    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    if (!tree)
    {
        return;
    }

    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    if (!tree)
    {
        return;
    }

    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
}
