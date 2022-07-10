#ifndef C_PROJECT_UNBOUNDED_INT_H
#define C_PROJECT_UNBOUNDED_INT_H
#include <stdlib.h>

typedef struct Number {
    struct Number *mNext;
    char mVal; //Pour stocker un chiffre ('0', '1', '2', ..., '9')
    struct Number *mLast;
} Number;


typedef struct {
    char mSign; //soit + ou -
    size_t mLength; //Taille de la chaine
    Number *mFirst; //chiffre le plus élévé
    Number *mLast; //Chiffre des unités
} UnboundedInt;

#define UNBOUNDED_INT_ERROR ((UnboundedInt) {.mLength = 0, .mLast = NULL, .mFirst = NULL, .mSign = '*'})

/**
 * Prototypes de fonctions (headers)
 */

UnboundedInt unboundedInt_free(UnboundedInt u);

/**
 * Convertis un string en type UnboundedInt
 * @param e : le string à convertir
 * @return UnboundedInt la paramètre comme nouvelle structure
 */
UnboundedInt unboundedInt_newString(char *e);

/**
 * Convertit un long en type UnboundedInt
 * @param i variable à convertir
 * @return UnboundedInt : paramètre comme nouvelle struct
 */
UnboundedInt unboundedInt_newll(long long i);

/**
 * Convertis un type UnboundedInt en string
 * @param i struct UnboundedInt
 * @return char* le paramètre sous en format string
 */
char *unboundedInt_toString(UnboundedInt ui);

/**
 * Vérifie si la struct a est plus grande que celle de b (ou à l'inverse)
 * On va supposer que les struct sont pas vides (UNBOUNDED_INT_ERROR) et qu'elles sont précises
 * @param a struct UnboundedInt
 * @param b struct UnboundedInt
 * @return int : −1 si a < b; 0 si a == b et 1 sinon
 */
int unboundedInt_cmpUnboundedInt(UnboundedInt a, UnboundedInt b);

/**
 * Vérifie si la struct a est plus grande que long b (ou à l'inverse)
 * @param a struct UnboundedInt
 * @param b long
 * @return int : −1 si a < b; 0 si a == b et 1 sinon
 */
int unboundedInt_cmpll(UnboundedInt a, long long b);

/**
 * Fais la somme de deux struct UnboundedInt
 * @param a struct UnboundedInt
 * @param b struct UnboundedInt
 * @return la somme sous forme de struct UnboundedInt
 */
UnboundedInt unboundedInt_add(UnboundedInt a, UnboundedInt b);

/**
 * Fais la difference des deux struct struct UnboundedInt
 * @param a struct UnboundedInt
 * @param b struct UnboundedInt
 * @return la différence sous forme de struct UnboundedInt
 */
UnboundedInt unboundedInt_subtract(UnboundedInt a, UnboundedInt b);

/**
 * Fait la multiplication de deux structs UnboundedInt
 * @param a struct UnboundedInt
 * @param b struct UnboundedInt
 * @return le produit sous forme de struct UnboundedInt
 */
UnboundedInt unboundedInt_multiply(UnboundedInt a, UnboundedInt b);

/**
 * fais la division de deux structures unbounded-int
 * @param a struct UnboundedInt
 * @param b struct UnboundedInt
 * @return le quotient entier de la division
 */
UnboundedInt unboundedInt_divide(UnboundedInt a, UnboundedInt b);

UnboundedInt unboundedInt_pow(UnboundedInt x, UnboundedInt n);

UnboundedInt unboundedInt_abs(UnboundedInt x);

UnboundedInt unboundedInt_fact(UnboundedInt n);

int unboundedInt_isError(UnboundedInt i);

/**
 * Fonction auxiliaire pour afficher une structure UnboundedInt
 * @param ui structure UnboundedInt
 */
void unboundedInt_print(UnboundedInt ui);

#endif // !C_PROJECT_UNBOUNDED_INT_H
