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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

  if(tree != NULL) // pokud binarni strom existuje
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
        return bst_search(tree->right, key, value); //rekurzivni volani funkce
      }
      else  //hledany klic je mensi nez ten s kterym ho porovnavame tak pujdeme do leveho podstromu
      {
        return bst_search(tree->left, key, value); //
      }
    }
  }
  else // binarni strom neexistuje
  {
    return false;
  }
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

  if((*tree) == NULL)//kdyz je to list
  {
    *tree = malloc(sizeof(bst_node_t));
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->right = NULL;
    (*tree)->left = NULL;
  }
  else
  {
    if((*tree)->key == key)
    {
      (*tree)->value = value;
    }
    else
    {
      if(key > (*tree)->key)
      {
        bst_insert(&((*tree)->right), key, value);
      }
      else
      {
        bst_insert(&((*tree)->left), key, value);  
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  bst_node_t *tmp;
  if((*tree)->right != NULL) // jestli ma podstrom tree nejaky pravy podstrom
  {
    bst_replace_by_rightmost(target, &((*tree)->right)); // rekurzivni volani postupne jdeme az do nejvice praveho uzlu
  }
  else // uz jsme se dostali na nejpravejsi uzel
  {
    target->key = (*tree)->key;
    target->value = (*tree)->value; //na miste target je ted key a value nejpravejsiho uzlu
    tmp = (*tree)->left; //musime zalohovat jeho levy podstrom v pripade ze ho ma
    free(*tree); // uvolnime uzel
    *tree = tmp; //nahradi zmazany uzel jeho levym podstromem, v pripade ze neexistoval ho nahradime NULL


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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {

  if(*tree == NULL)
  {
    return;
  }
  if(key != (*tree)->key) // tento uzel nechceme smazat, takze se posuneme do praveho nebo leveho podstromu podle key hodnoty
  {
    if(key > (*tree)->key) //hledany key je vetsi nez nalezeny takze pujdeme doprava
    {  
      bst_delete(&((*tree)->right), key); // rekurzivni volani funkce, posuneme se do praveho podstromu
    }
    else //hledany key je mensi nez nalezeny, posuneme se do leveho podstromu
    {
      bst_delete(&((*tree)->left), key);
    } 
  }
  else // nasli jsme uzel ktery chceme smazat
  { 
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
  }
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
  if((*tree) != NULL)
  {
    bst_dispose( &((*tree)->left) );
    bst_dispose( &((*tree)->right) );
    free(*tree);
  }
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
  if(tree != NULL)
  {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if(tree != NULL)
  {
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if(tree != NULL)
  {
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
