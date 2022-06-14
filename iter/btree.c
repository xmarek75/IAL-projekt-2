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
if(tree != NULL) // pokud binarni strom existuje
  {
    while((tree->right != NULL) || (tree->left != NULL))
    {
      if(tree->key == key)
      {
        *value = tree->value;
        return true;
      }
      else
      {
          if(key > tree->key) //kdyz je hledany klic vetsi tak pujdeme do praveho podstromu
          {
            tree = tree->right;
          }
          else  //hledany klic je mensi nez ten s kterym ho porovnavame tak pujdeme do leveho podstromu
          {
            tree = tree->left;
          }
      }
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
while(true)
{
  if((*tree) == NULL)//kdyz je to list
    {
      *tree = malloc(sizeof(bst_node_t));
      (*tree)->key = key;
      (*tree)->value = value;
      (*tree)->right = NULL;
      (*tree)->left = NULL;
      break;
    }
    else
    {
      if((*tree)->key == key)
      {
        (*tree)->value = value;
        break;
      }
      else
      {
        if(key > (*tree)->key)
        {
          tree = &((*tree)->right);
        }
        else
        {
          tree = &((*tree)->left);
        }
      }
    }
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
  bst_node_t *tmp;
  while((*tree)->right != NULL) // jestli ma podstrom tree nejaky pravy podstrom
  {
    tree = &((*tree)->right);
  }
    target->key = (*tree)->key;
    target->value = (*tree)->value; //na miste target je ted key a value nejpravejsiho uzlu
    tmp = (*tree)->left; //musime zalohovat jeho levy podstrom v pripade ze ho ma
    free(*tree); // uvolnime uzel
    *tree = tmp; //nahradi zmazany uzel jeho levym podstromem, v pripade ze neexistoval ho nahradime NULL


  
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

  if(*tree == NULL)
  {
    return;
  }
  while(key != (*tree)->key && (((*tree)->right != NULL) && ((*tree)->left != NULL))) // tento uzel nechceme smazat, takze se posuneme do praveho nebo leveho podstromu podle key hodnoty
  {
    if(key > (*tree)->key) //hledany key je vetsi nez nalezeny takze pujdeme doprava
    {  
      tree = &((*tree)->right); // rekurzivni volani funkce, posuneme se do praveho podstromu
    }
    else //hledany key je mensi nez nalezeny, posuneme se do leveho podstromu
    {
      tree = &((*tree)->left);
    } 
  }  
    if( ( (*tree)->left != NULL ) && ( (*tree)->right != NULL ) ) // pokud ma uzel jak pravej tak i levej podstrom
    {
      bst_replace_by_rightmost(*tree, &((*tree)->left)); // vyuzijeme funkci bst_replace_by_rightmost
    }
    else if( ( (*tree)->left == NULL ) && ( (*tree)->right == NULL ) )//uzel je zaroven listem
    {
      free(*tree);//uvolnime uzel
      *tree = NULL; // zmenime ukazatel otcovskeho uzlu

    }
    
    else if( ( (*tree)->left == NULL ) && ( (*tree)->right != NULL ) ) // uzel ma pouze pravy podstrom
    { 
      bst_node_t *tmp; // pomocny uzel 
      tmp = (*tree)->right; //ulozime si ukazatel na jeho pravy podstrom
      free(*tree);
      *tree = tmp; //otcovsky uzel smazaneho uzlu ted ukazuje na pravy podstrom smazaneho uzlu

    }
    else if( ( (*tree)->left != NULL ) && ( (*tree)->right == NULL ) ) // uzel ma pouze levy podstrom
    { 
      bst_node_t *tmp; // pomocny uzel 
      tmp = (*tree)->left; //ulozime si ukazatel na jeho pravy podstrom
      free(*tree);
      *tree = tmp; //otcovsky uzel smazaneho uzlu ted ukazuje na pravy podstrom smazaneho uzlu

    }
    else return;

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
  stack_bst_t stack;
  stack_bst_init(&stack);
  while( ( (*tree) != NULL )||(!stack_bst_empty(&stack)) )
  {
    if((*tree) == NULL)
    {
      if(!stack_bst_empty(&stack))
      {
        *tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
      }
    }
      else
      {
        if((*tree)->right != NULL)
        {
          stack_bst_push(&stack, (*tree)->right);
        }
        bst_node_t *tmp;
        tmp = (*tree);
        (*tree) = (*tree)->left;
        free(tmp);
      }
    }
  
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

  while(tree != NULL)
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
  bst_leftmost_preorder(tree,&stack);

  while(!stack_bst_empty(&stack))
  {
    bst_node_t *tmp = stack_bst_pop(&stack);
    if(tmp->right != NULL)
    {
      bst_leftmost_preorder(tmp->right, &stack);
    }
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
  while(tree != NULL)
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
  while(!stack_bst_empty(&stack))
  {
    bst_node_t *tmp = stack_bst_pop(&stack);
    bst_print_node(tmp);
    if(tmp->right != NULL)
    {
      bst_leftmost_inorder(tmp->right, &stack);
    }
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
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  while(tree != NULL)
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

  bool tmp_bool;
  stack_bst_t stack;
  stack_bool_t stack_bool;

  stack_bst_init(&stack);
  stack_bool_init(&stack_bool);
  bst_leftmost_postorder(tree, &stack, &stack_bool);

  while(!stack_bst_empty(&stack))
  {
    tree = stack_bst_pop(&stack);
    tmp_bool = stack_bool_pop(&stack_bool);
    if(tmp_bool == true)
    {
      stack_bool_push(&stack_bool, false);
      stack_bst_push(&stack, tree);
      bst_leftmost_postorder(tree->right, &stack, &stack_bool);
    }
    else
      bst_print_node(tree);
  }
  

}
