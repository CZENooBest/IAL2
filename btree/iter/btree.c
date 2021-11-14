/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
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
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree != NULL)
    {
        if (key > tree->key)
        {
            tree = tree->right;
        }
        else if (key < tree->key)
        {
            tree = tree->left;
        }
        else
        {
            *value = tree->value;
            return true;
        }

    }
    return false;
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *CURRENTnode = *tree;
    bst_node_t *PREVIOUSnode = NULL;

    while (CURRENTnode != NULL)
    {
        PREVIOUSnode = CURRENTnode;

        if (key < CURRENTnode->key)
        {
            CURRENTnode = CURRENTnode->left;
        }
        else
        {
            CURRENTnode = CURRENTnode->right;
        }
    }

    bst_node_t *NEWnode = (bst_node_t *) malloc(sizeof(struct bst_node));
    if(NEWnode == NULL)
    {
        return;
    }

    NEWnode->key = key;
    NEWnode->value = value;
    NEWnode->left = NULL;
    NEWnode->right = NULL;

    if (PREVIOUSnode == NULL)
    {
        *tree = NEWnode;
        return;
    }

    if(key < PREVIOUSnode->key)
    {
        PREVIOUSnode->left = NEWnode;
        return;
    }

    if (key > PREVIOUSnode->key)
    {
        PREVIOUSnode->right = NEWnode;
        return;
    }

    if (key == PREVIOUSnode->key)
    {
        PREVIOUSnode->value = NEWnode->value;
    }
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    char TMPkey;
    int TMPvalue;

    if (*tree == NULL || target == NULL)
    {
        return;	// Osetreni chyboveho stavu
    }
        // Dostani se k nejpravejsimu uzlu
    else if ((*tree)->right != NULL)
    {
        bst_replace_by_rightmost(target, &(*tree)->right);
    }
    else
    {	// Samotne mazani
        TMPkey = (*tree)->key;
        TMPvalue = (*tree)->value;

        bst_delete(RootPtr, TMPkey);

        target->key = TMPkey;
        target->value = TMPvalue;
    }
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
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t *TMPnode;

    if (*tree == NULL)
    {
        return;	// Uzel s klicem ve stromu neni, nedelam nic
    }
        // Pokud je klic mensi nez v aktualnim uzlu, volam znovu funcki s levym synem
    else if (key < (*tree)->Key)
    {
        bst_delete(&(*tree)->left, key);
    }
        // Pokud je klic vetsi nez v aktualnim uzlu, volam znovu funcki s pravym synem
    else if (key > (*tree)->key)
    {
        bst_delete(&(*tree)->right, key);
    }
    else
    {	// Nasel jsem uzel ke smazani
        // Uzel nema zadne syny, jednoduse jej uvolnim
        if ((*tree)->left == NULL && (*tree)->right == NULL)
        {
            free (*tree);
            *tree = NULL;
        }
            // Uzel ma jen leveho syna, uzel uvolnim a na jeho misto dam adresu syna
        else if ((*tree)->left != NULL && (*tree)->right == NULL)
        {
            TMPnode = (*tree)->left;
            free (*tree);
            *tree = TMPnode;
        }
            // Uzel ma jen praveho syna, uzel uvolnim a na jeho misto dam adresu syna
        else if ((*tree)->left == NULL && (*tree)->right != NULL)
        {
            TMPnode = (*tree)->right;
            free (*tree);
            *tree = TMPnode;
        }
            // Uzel ma oba syny, je to peklo
        else {
            // Jako uzel, ktery ma odstranovany nahradit se zasle pravy uzel leveho podstromu jen pokud existuje
            if ((*tree)->left->right != NULL)
            {
                bst_replace_by_rightmost(*tree, &(*tree)->left->right);
            }
                // Jinak se zasle koren leveho podstromu
            else
            {
                bst_replace_by_rightmost(*tree, &(*tree)->left);
            }
        }
    }
}
/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if(*tree == NULL)
    {
        return;
    }

    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bst_push(&stack, *tree);


    while (!stack_bst_empty(&stack))
    {
        bst_node_t *DELnode = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        if (DELnode->left != NULL)
        {
            stack_bst_push(&stack, DELnode->left);
        }
        if (DELnode->right != NULL)
        {
            stack_bst_push(&stack, DELnode->right);
        }

        free(DELnode);
    }

    bst_init(tree);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree)
    {
        stack_bst_push(to_visit, tree);
        bst_print_node(tree);
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);

    bst_leftmost_preorder(tree, &stack);

    while (!stack_bst_empty(&stack))
    {
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_leftmost_preorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree)
    {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);

    bst_leftmost_inorder(tree, &stack);
    while (!stack_bst_empty(&stack))
    {
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_print_node(tree);
        bst_leftmost_inorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
    while (tree)
    {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    stack_bst_t stackBst;
    stack_bst_init(&stackBst);
    bool TEMPbool;

    stack_bool_t stackBool;
    stack_bool_init(&stackBool);

    bst_leftmost_postorder(tree, &stackBst, &stackBool);
    while (!stack_bst_empty(&stackBst))
    {
        tree = stack_bst_top(&stackBst);
        stack_bst_pop(&stackBst);

        TEMPbool = stack_bool_top(&stackBool);
        stack_bool_pop(&stackBool);
        if (TEMPbool)
        {
            stack_bool_push(&stackBool, false);
            stack_bst_push(&stackBst, tree);

            bst_leftmost_postorder(tree->right, &stackBst, &stackBool);
        }
        else
        {
            bst_print_node(tree);
        }
    }
}
