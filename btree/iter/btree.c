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
    bst_node_t *after = NULL;
    bst_node_t *iterator = *tree;

    while (iterator)
    {
        after = iterator;

        if (key < iterator->key)
        {
            iterator = iterator->left;
            continue;
        }
        else if (key > iterator->key)
        {
            iterator = iterator->right;
            continue;
        }
        else
        {
            return;
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

    if (!after)
    {
        *tree = NEWnode;
        return;
    }
    if(key < after->key)
    {
        after->left = NEWnode;
        return;
    }
    after->right = NEWnode;
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
    bst_node_t *right_node;
    bst_node_t *iterator = *tree;

    while (iterator)
    {
        if((iterator)->right == NULL)
        {
            right_node = iterator;

            target->key = right_node->key;
            target->value = right_node->value;

            target->left = NULL;
            if (right_node->left)
            {
                target->left = right_node->left;
            }

            free(right_node);
        }

        if (!(iterator)->right->right)
        {
            right_node = (iterator)->right;
            target->key = right_node->key;
            target->value = right_node->value;
            (iterator)->right = NULL;

            if(right_node->left)
            {
                (iterator)->right = right_node->left;
            }

            free(right_node);
            return;
        }

        iterator = (iterator)->right;
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
    bst_node_t *CURRENTnode = *tree;
    bst_node_t *PARENTnode = NULL;

    while (CURRENTnode != NULL)
    {
        if(CURRENTnode->key > key)
        {
            PARENTnode = CURRENTnode;
            CURRENTnode = CURRENTnode->left;
        }
        else if (CURRENTnode->key < key)
        {
            PARENTnode = CURRENTnode;
            CURRENTnode = CURRENTnode->right;
        }
        else
        {
            if(CURRENTnode->left == NULL && CURRENTnode->right == NULL)
            {
                if(PARENTnode->left == CURRENTnode)
                {
                    PARENTnode->left = NULL;
                }
                else
                {
                    PARENTnode->right = NULL;
                }

                free(CURRENTnode);
                return;
            }
            else if ((CURRENTnode->left == NULL && CURRENTnode->right != NULL) || (CURRENTnode->left != NULL && CURRENTnode->right == NULL))
            {
                if (CURRENTnode->right != NULL && CURRENTnode->left == NULL)    //left != NULL
                {
                    if(PARENTnode->right == CURRENTnode)
                    {
                        PARENTnode->right = CURRENTnode->right;
                    }
                    else if (PARENTnode->left == CURRENTnode)
                    {
                        PARENTnode->left = CURRENTnode->right;
                    }
                }
                else if (CURRENTnode->left != NULL && CURRENTnode->right == NULL)
                {
                    if(PARENTnode->right == CURRENTnode)
                    {
                        PARENTnode->right = CURRENTnode->left;
                    }
                    else if (PARENTnode->left == CURRENTnode)
                    {
                        PARENTnode->left = CURRENTnode->left;
                    }
                }

                free(CURRENTnode);
                return;
            }
            else
            {
                bst_node_t *SWAPnode = CURRENTnode->right;
                bst_node_t *SWAP_PREVnode = CURRENTnode;

                while (SWAPnode->left)
                {
                    SWAP_PREVnode = SWAPnode;
                    SWAPnode = SWAPnode->left;
                }
                CURRENTnode->key = SWAPnode->key;
                CURRENTnode->value = SWAPnode->value;

                if(SWAP_PREVnode->left == SWAPnode)
                {
                    SWAP_PREVnode->left = SWAPnode->right;
                }
                else if (SWAP_PREVnode->right == SWAPnode)
                {
                    SWAP_PREVnode->right = SWAPnode->right;
                }

                free(SWAPnode);
                return;
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
