#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../headers/unbounded_int.h"

#define NUM (48)
#define UNBOUNDED_INT_ERROR ((unbounded_int) {.len = 0, .dernier = NULL, .premier = NULL, .signe = '*'})


/**
 * Convertis un string en type unbounded_int
 * @param e : le string à convertir
 * @return unbounded_int la paramètre comme nouvelle structure
 */
unbounded_int string2unbounded_int(const char *e) {
    if (isAStringNum(e) == 0) return UNBOUNDED_INT_ERROR;
    char *str = cleanNumber(e);
    unbounded_int *res = malloc (sizeof(unbounded_int));
    if (res == NULL) return UNBOUNDED_INT_ERROR;
    int i;
    res->len = 0;
    if (*(str) == '-' || *str == '+') {
        if (*(str) == '-') res->signe = '-';    
        else res->signe = '+';
        i = 1;
    } else {
        res->signe = '+';
        i = 0;
    }
    for (int j = i; j < strlen(str); j++) {
        res = pushFront(res,*(str+j));
    }
    return *res;
}


/**
 * Convertis un long en type unbounded_int
 * @param i variable à convertir
 * @return unbounded_int : paramètre comme nouvelle struct
 */
unbounded_int ll2unbounded_int (long long i) {
    char buffer[100];
    return string2unbounded_int(lltoa(i,buffer,10));
}


/**
 * Convertis un type unbounded_int en string
 * @param i struct unbounded_int
 * @return char* le paramètre sous en format string
 */
char * unbounded_int2string (unbounded_int ui) {
    if (isUnboundedIntEmpty(ui)) return NULL;
    char *e = malloc(sizeof(char)*(ui.len+1) + 1);
    if (e == NULL) return NULL;
    int i;
    if (ui.signe == '-') {
        e[0] = '-';
        i = 1;
    } else i = 0;
    chiffre *tmp = ui.premier;
    for (int j = 0; j < ui.len; j++) {
        *(e+i+j) = tmp->c;
        tmp = tmp->suivant;
    }
    //free(tmp);
    return cleanNumber(e);
}


/**
 * Vérifie si la struct a est plus grande que celle de b (ou à l'inverse) 
 * On va supposer que les struct sont pas vides (UNBOUNDED_INT_ERROR) et qu'elles sont précises
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return int : −1 si a < b; 0 si a == b et 1 sinon  
 */
int unbounded_int_cmp_unbounded_int (unbounded_int a, unbounded_int b) {
    if ((a.premier->c == '0' && a.len == 1) && (b.premier->c == '0' && b.len == 1)) return 0;
    if (a.signe == b.signe) {
        if (a.len == b.len) {
            chiffre *tmp1 = a.premier;
            chiffre *tmp2 = b.premier; 
            for (int i = 0; i < a.len; i++) {
                if (tmp1->c > tmp2->c) return 1;
                if (tmp1->c < tmp2->c) return -1;
                tmp1 = tmp1->suivant;
                tmp2 = tmp2->suivant;
            }
            //free(tmp1);
            //free(tmp2);
            return 0;
        } else {
            if (a.signe == '-') return (a.len > b.len) ? -1 : 1;
            return (a.len > b.len) ? 1 : -1;
        }
    } 
    return (a.signe == '-') ? -1 : 1;
}


/**
 * Vérifie si la struct a est plus grande que long b (ou à l'inverse)
 * @param a struct unbounded_int
 * @param b long 
 * @return int : −1 si a < b; 0 si a == b et 1 sinon  
 */
int unbounded_int_cmp_ll (unbounded_int a, long long b) {
    return unbounded_int_cmp_unbounded_int(a,ll2unbounded_int(b));
}


/**
 * Fais la somme de deux struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la somme sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_somme (unbounded_int a, unbounded_int b) {
    if (isUnboundedIntEmpty(a) || isUnboundedIntEmpty(b)) return UNBOUNDED_INT_ERROR;
    if (isUnboundedZero(&a) || isUnboundedZero(&b)) {
        if (isUnboundedZero(&a) && isUnboundedZero(&b)) return a;
        else if (isUnboundedZero(&a)) return b;
        else return a;
    }
    if (a.signe == b.signe) {
        return sumPositifUnbounded(&a, &b);
    }
    return sumNegatifUnbounded(&a, &b);
}


/**
 * Fais la différence des deux struct struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la différence sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_difference (unbounded_int a, unbounded_int b) {
    if (isUnboundedIntEmpty(a) || isUnboundedIntEmpty(b)) return UNBOUNDED_INT_ERROR;
    if (isUnboundedZero(&a) || isUnboundedZero(&b)) {
        if (isUnboundedZero(&a) && isUnboundedZero(&b)) return a;
        else if (isUnboundedZero(&a)) {
            if (b.signe == '-') b.signe = '+';
            else b.signe = '-';
            return b;
        }    
        else return a;
    }
    if ((a.signe == '+' && b.signe == '-') || (a.signe == '-' && b.signe == '+')) return sumPositifUnbounded(&a, &b);
    return sumNegatifUnbounded(&a, &b);
}


/**
 * Fais la multiplication de deux structs unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return le produit sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_produit (unbounded_int a, unbounded_int b) {
    if (isUnboundedIntEmpty(a) || isUnboundedIntEmpty(b)) return UNBOUNDED_INT_ERROR;
    unbounded_int *res = malloc (sizeof (unbounded_int));
    if (isUnboundedZero(&a) || isUnboundedZero(&b)) {
        chiffre *c = initChiffre('0');
        res->len = 1;
        res->premier = c;
        res->dernier = c;
        res->signe = '+';
        return *res;
    }

    if (isUnboundedUnity(&a) || isUnboundedUnity(&b)) {
        if (isUnboundedUnity(&a) && isUnboundedUnity(&b)) return a;
        else if (isUnboundedUnity(&a)) return b;
        else return a;
    }

    chiffre *tmp = a.dernier;

    res = multiplicationPourUnChiffre(tmp->c, &b);
    for (int i = 1; i < a.len; i++) {
        tmp = tmp->precedent;
        unbounded_int *tmp_p = multiplicationPourUnChiffre(tmp->c, &b);

        for (int j = 0; j < i; j++) {
            tmp_p = pushFront(tmp_p, '0');
        }

        *res = sumPositifUnbounded(res, tmp_p);
    }
    res->signe = (a.signe == b.signe) ? '+' : '-';
    return *res;
}


/**
 * fais la division de deux structures unbounded-int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return le quotient entier de la division
 */
unbounded_int unbounded_int_division(unbounded_int a, unbounded_int b) {
    return UNBOUNDED_INT_ERROR;
}



/**
 * fonction main (tests)
 */
int main (int argc, char *argv[]) {
    
    printf("=AUXILIAR FUNCTIONS=\n");
    //isNum() test
    printf("==ISNUM() TEST==\n");
    char a = '2';
    char b = 'm';

    printf("%c => is a a number ? %s\n",a, isNum(a) ? "True" : "False");
    printf("%c => is a a number ? %s\n",b, isNum(b) ? "True" : "False");

    printf("===========\n");
    printf("==ISASTRINGNUM() TEST==\n");
    //isAStringNum() test
    char *st_n = malloc(sizeof(char)*6+1);
    char *st_p = malloc(sizeof(char)*5+1);
    char *stn_err = malloc(sizeof(char)*6+1);
    char *stp_err = malloc(sizeof(char)*5+1);
    char *st_zero = malloc(sizeof(char)*1+1);
    char *st_empty = malloc(sizeof(char)*1+1);
    st_n = strcpy(st_n,"-45612");
    st_p = strcpy(st_p,"31245");
    stn_err = strcpy(stn_err,"-548ff");
    stp_err = strcpy(stp_err,"542df");
    st_zero = strcpy(st_zero, "0");
    st_empty = strcpy(st_empty,"+");

    printf("%s => is a a number ? %s\n",st_n, isAStringNum(st_n) ? "True" : "False");
    printf("%s => is a a number ? %s\n",st_p, isAStringNum(st_p) ? "True" : "False");
    printf("%s => is a a number ? %s\n",stn_err, isAStringNum(stn_err) ? "True" : "False");
    printf("%s => is a a number ? %s\n",stp_err, isAStringNum(stp_err) ? "True" : "False");
    printf("%s => is a a number ? %s\n",st_zero, isAStringNum(st_zero) ? "True" : "False");
    printf("%s => is a a number ? %s\n",st_empty, isAStringNum(st_empty) ? "True" : "False");


    printf("===========\n");

    //lltoa() test
    printf("==LLTOA() TEST==\n");
    char *buffer [100];
    long long i = 45526229262;
    long long j = -893636383;
    printf("format long long: %lld => format string: %s\n",i ,lltoa(i,buffer,10));
    printf("format long long: %lld => format string: %s\n",j ,lltoa(j,buffer,10));


    //Puissance test
    printf("==PUISSANCE() TEST==\n");
    int base = 10;
    printf("%d puissance 5 = %lld \n",base, puissance(base, 5));

    printf("===========\n");
    printf("=MAIN FUNCTIONS=\n");
    printf("===========\n");

    //STRING2UNBOUNDED_INT () TEST
    printf("==STRING2UNBOUNDED_INT() TEST==\n");
    unbounded_int ui1 = string2unbounded_int(st_n);
    unbounded_int ui2 = string2unbounded_int(st_p);

    print_unbounded_int(ui1);
    print_unbounded_int(ui2);

    printf("===========\n");

    //LL2UNBOUNDED_INT() TEST
    printf("==LL2UNBOUNDED_INT() TEST==\n");
    unbounded_int ui3 = ll2unbounded_int(i);
    unbounded_int ui4 = ll2unbounded_int(j);

    print_unbounded_int(ui3);
    print_unbounded_int(ui4);

    printf("===========\n");

    //UNBOUNDED_INT2STRING() TEST
    printf("==UNBOUNDED_INT2STRING() TEST==\n");
    printf("%s\n",unbounded_int2string(ui1));
    printf("%s\n",unbounded_int2string(ui2));
    printf("%s\n",unbounded_int2string(ui3));
    printf("%s\n",unbounded_int2string(ui4));
    
    printf("===========\n");
    
    //UNBOUNDED_INT_SOMME() TEST
    printf("==UNBOUNDED_INT_SOMME() TEST==\n");
    
    unbounded_int ui5 = string2unbounded_int("645248");
    unbounded_int ui6 = string2unbounded_int("543124");
    unbounded_int ui7 = string2unbounded_int("-46872");
    unbounded_int ui8 = string2unbounded_int("-13246");
    unbounded_int ui9 = string2unbounded_int("5724");
    unbounded_int ui10 = string2unbounded_int("-6578");
    unbounded_int zero = string2unbounded_int("0");

    unbounded_int res1 = unbounded_int_somme(ui5, ui6);
    unbounded_int res2 = unbounded_int_somme(ui5, zero);
    unbounded_int res3 = unbounded_int_somme(ui7, ui8);
    unbounded_int res4 = unbounded_int_somme(ui7, zero);
    unbounded_int res5 = unbounded_int_somme(ui9, ui5);
    unbounded_int res6 = unbounded_int_somme(ui7, ui10);
    unbounded_int res6_5 = unbounded_int_somme(ui9, ui10);
    
    
    printf("%s + %s = %s\n",unbounded_int2string(ui5), unbounded_int2string(ui6), unbounded_int2string(res1));
    printf("%s + %s = %s\n",unbounded_int2string(ui5), unbounded_int2string(zero), unbounded_int2string(res2));
    printf("%s + (%s) = %s\n",unbounded_int2string(ui7), unbounded_int2string(ui8), unbounded_int2string(res3));
    printf("%s + %s = %s\n",unbounded_int2string(ui7), unbounded_int2string(zero), unbounded_int2string(res4));
    printf("%s + %s = %s\n",unbounded_int2string(ui9), unbounded_int2string(ui5), unbounded_int2string(res5));
    printf("%s + (%s) = %s\n",unbounded_int2string(ui7), unbounded_int2string(ui10), unbounded_int2string(res6));
    printf("%s + (%s) = %s\n",unbounded_int2string(ui9), unbounded_int2string(ui10), unbounded_int2string(res6_5));
    
    printf("===========\n");


    //UNBOUNDED_INT_SOMME() TEST
    printf("==UNBOUNDED_INT_DIFFERENCE() TEST==\n");
    
    unbounded_int res7 = unbounded_int_difference(ui7, ui5); // -a - (+b)
    unbounded_int res8 = unbounded_int_difference(ui5, ui8); // +a - (-b)
    unbounded_int res9 = unbounded_int_difference(zero, ui10);
    unbounded_int res10 = unbounded_int_difference(zero, ui9);
    unbounded_int res11 = unbounded_int_difference(ui10, ui10);
    unbounded_int res12 = unbounded_int_difference(ui5, ui6);
    unbounded_int res13 = unbounded_int_difference(ui7, ui8);


    printf("%s - %s = %s\n",unbounded_int2string(ui7), unbounded_int2string(ui5), unbounded_int2string(res7));
    printf("%s - (%s) = %s\n",unbounded_int2string(ui5), unbounded_int2string(ui8), unbounded_int2string(res8));
    printf("%s - (%s) = %s\n",unbounded_int2string(zero), unbounded_int2string(ui10), unbounded_int2string(res9));
    printf("%s - %s = %s\n",unbounded_int2string(zero), unbounded_int2string(ui9), unbounded_int2string(res10));
    printf("%s - (%s) = %s\n",unbounded_int2string(ui10), unbounded_int2string(ui10), unbounded_int2string(res11));
    printf("%s - %s = %s\n",unbounded_int2string(ui5), unbounded_int2string(ui6), unbounded_int2string(res12));
    printf("%s - (%s) = %s\n",unbounded_int2string(ui7), unbounded_int2string(ui8), unbounded_int2string(res13));
    

    
    printf("===========\n");

    //UNBOUNDED_INT_PRODUIT() TEST
    printf("==UNBOUNDED_INT_PRODUIT() TEST==\n");

    unbounded_int res14 = unbounded_int_produit(zero, ui5);
    unbounded_int res14_5 = unbounded_int_produit(ui5, zero);
    unbounded_int res15 = unbounded_int_produit(ui9, ui9);
    unbounded_int res16 = unbounded_int_produit(ui8, ui8);
    unbounded_int res17 = unbounded_int_produit(ui9, ui8);
    
    printf("%s * (%s) = %s\n",unbounded_int2string(zero), unbounded_int2string(ui5), unbounded_int2string(res14));
    printf("%s * (%s) = %s\n",unbounded_int2string(ui5), unbounded_int2string(zero), unbounded_int2string(res14_5));
    printf("%s * (%s) = %s\n",unbounded_int2string(ui9), unbounded_int2string(ui9), unbounded_int2string(res15));
    printf("%s * (%s) = %s\n",unbounded_int2string(ui8), unbounded_int2string(ui8), unbounded_int2string(res16));
    printf("%s * (%s) = %s\n",unbounded_int2string(ui9), unbounded_int2string(ui8), unbounded_int2string(res17));

    

    printf("===========\n");

    //UNBOUNDED_INT_DIVISION() TEST
    printf("==UNBOUNDED_INT_DIVISION() TEST==\n");

    
    return 0;
}



/**
 * vérifie si le code ASCII du char est un nombre (entre 48 et 57)
 * @param c char à tester
 * @return 1 si c est un nombre, 0 sinon
 */
static int isNum(char c) {
    return (int) c >= NUM && (int) c <= NUM + 9;
}


/**
 * Vérifie que le string est bien composé de nombres
 * @param c String à tester
 * @return 1 = true, et 0 sinon
 */
static int isAStringNum (const char *c) {
    size_t len = strlen(c);
    if (len == 0) return 0;
    int i;
    if (*c == '-' || *c == '+') {
        if (len == 1) return 0;
        i = 1;
    } else {
        i = 0;
    }
    for (size_t j = i; j < strlen(c); j++) {
        if (!isNum(*(c+j))) {
            return 0;
        }
    }
    return 1;
}


/**
 * Verifie si la struct est une UNBOUNDED_INT_ERROR
 * @param une struct unbounded_int
 * @return 0 si c'est le cas, et 1 sinon
 */
static int isUnboundedIntEmpty (unbounded_int ui) {
    return (ui.signe == '*' || ui.len == 0 || ui.premier == NULL || ui.dernier == NULL);
}


/**
 * Vérifie que la structure est bien un 0, c'est à dire, que len = 1, premier = dernier = (chiffre) {.suivant = null, .c = '0' , .precedant = null}
 * @param ui pointeur vers la structure 
 * @return True si la strcutre est un 0 false sinon
 */
static int isUnboundedZero (unbounded_int *ui) {
    return ((ui->signe == '+' || ui->signe == '-') && ui->len == 1 && ui->premier == ui->dernier && ui->premier->c == '0');
}


/**
 * Vérifie que la structure est bien un 1 (l'unité), c'est à dire, que len = 1, premier = dernier = (chiffre) {.suivant = null, .c = '1' , .precedant = null}
 * @param ui pointeur vers la structure 
 * @return True si la strcutre est un 1 (unité) false sinon
 */
static int isUnboundedUnity (unbounded_int *ui) {
    return (ui->len == 1 && ui->premier == ui->dernier && ui->premier->c == '1' && (ui->signe == '+' || ui->signe == '-'));
}


/**
 * Fonction qui permet de "nettoyer" un nombre
 * @param str le nombre passé en paramètre sous forme de string
 * @return le nombre sans 0 ou d'autres characters de plus 
 */
static char * cleanNumber (char *str) {
    size_t len = strlen(str);
    int i;
    if (*str == '-' || *str == '+') {
        if (len == 2) {
            return (*(str+1) == '0') ? "0" : str;
        }
        i = 1;
    } else {
        if (len == 1) return str;
        i = 0;
    }
    int index = i;
    for (size_t j = i; j < len; j++) {
        if (*(str+j) != '0') {
            break;
        }
        index++;
    }
    if (index == i) return str;
    if (index == len) return "0";
    int newLen = len - index + 1;
    char *newStr = malloc (sizeof(char)*newLen + 1);
    if (i == 1) newStr[0] = str[0];
    for (size_t j = index; j < len; j++) {
        newStr[i] = str[j];
        i++;
    }
    return newStr;
}


/**
 * Fonction qui permet de "nettoyer" une structure unbounded_int
 * @param ui struct à néttoyer
 * @return un pointeur vers la structure ou une nouvelle
 */
 
static unbounded_int * cleanUnbounded_int (unbounded_int ui) {
    /*
    if (ui->len <= 1) return ui;
    int index = 0;
    chiffre *tmp;// = ui->premier;
    for (tmp = ui->premier; tmp != NULL; tmp = tmp->suivant) {
        if (tmp->c != '0') {
            break;
        }
        index++;
    }
    if (tmp == NULL) {
        for (int i = 0; i < ui->len - 1; i++) {
            ui = deleteFirstElem (ui);
        }
        return ui;
    }

    for (int i = 0; i < index; i++) {

    }


    


    
    int len = ui.len;
    if (len == 1) {
        return ui;
    }
    int index = 0;
    chiffre *tmp = ui.premier;
    for (int j = index; j < len; j++) {
        if (tmp->c != '0') {
            break;
        }
        tmp = tmp->suivant;
        index++
    }
    if (index == 0) return *ui;
    unbounded_int newUi= malloc(sizeof(unbounded_int));
    if (index == len && tmp == NULL) {
        newUi.len = 1;
        chiffre ch = {
            .precedent = NULL,
            .c = '0',
            .suivant = NULL 
        };
        newUi.premier = ch;
        newUi.dernier = ch;
        return newUi;
    }
    newUi.len = len-index;
    
    newUi.premier = ch;
    newUi.dernier = ch;
    return newUi;
    return newUi;
    
    */
    return NULL;//UNBOUNDED_INT_ERROR;
}


/**
 * Fonction qui permet de transformer un long long en String
 * @param value long long à transformer
 * @param buffer allocation du string
 * @param base base de conversion
 * @return char* 
 */
static char * lltoa (long long value, char *buffer, int base) {
     if (base < 2 || base > 32) {
        return buffer;
    }
    long long n = abs(value);
    int i = 0;
    while (n) {
        int r = n % base;
        if (r >= 10) buffer[i++] = 65 + (r - 10);
        else buffer[i++] = 48 + r;
        n = n / base;
    }
    if (i == 0) buffer[i++] = '0';
    if (value < 0 && base == 10) buffer[i++] = '-';
    buffer[i] = '\0'; // null terminate string
    return reverse(buffer, 0, i - 1);
}


/**
 * Fonction qui permet de transformer un long long en String
 * @param value long long à transformer
 * @param buffer allocation du string
 * @param base base de conversion
 * @return char* 
 */
static char * itoa (int value, char *buffer, int base) {
     if (base < 2 || base > 32) {
        return buffer;
    }
    int n = abs(value);
    int i = 0;
    while (n) {
        int r = n % base;
        if (r >= 10) buffer[i++] = 65 + (r - 10);
        else buffer[i++] = 48 + r;
        n = n / base;
    }
    if (i == 0) buffer[i++] = '0';
    if (value < 0 && base == 10) buffer[i++] = '-';
    buffer[i] = '\0'; // null terminate string
    return reverse(buffer, 0, i - 1);
}


/**
 * Echange la valeur de deux pointeurs
 * @param x pointeur 1
 * @param y pointeur 2
 */
static void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}


/**
 * 
 * @param buffer allocationo du string
 * @param i 
 * @param j 
 * @return char* 
 */
static char* reverse(char *buffer, int i, int j) {
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
    return buffer;
}


/**
 * Initialise une struct chiffre à partir d'un char
 * @param c valeur de l'argument c de la struct chiffre
 * @return une nouvelle instance de chiffre
 */
static chiffre * initChiffre (const char c) {
    if (isNum(c) == 0) return NULL;
    chiffre *ch = malloc(sizeof (chiffre));
    if (ch == NULL) {
        perror("Pas de mémoire pour allouer la struct chiffre\n");
        return NULL;
    }
    ch->suivant = NULL;
    ch->precedent = NULL;
    ch->c = c;
    return ch;
}


/**
 * Ajoute d'une nouvelle struct Chiffre à la fin de la liste
 * @param ui struct source
 * @param c valeur de la nouvelle struct chiffre ajouté
 */
static unbounded_int * pushFront (unbounded_int *ui, const char c) {
    chiffre *ch = initChiffre(c);
    ch->precedent = ui->dernier;
    ch->suivant = NULL;
    if (ui->dernier != NULL) {
        ui->dernier->suivant = ch;
    } else if (ui->dernier == NULL && ui->premier == NULL) {
        ui->premier = ch;
    }
    ui->dernier = ch;
    ui->len++;
    return ui;
}


/**
 * Ajoute d'une nouvelle struct Chiffre au début de la liste
 * @param ui struct source
 * @param c valeur de la nouvelle struct chiffre ajouté
 */
static unbounded_int * pushBack (unbounded_int *ui, const char c) {
    chiffre *ch = initChiffre(c);
    ch->precedent = NULL;
    ch->suivant = ui->premier;
    if (ui->premier != NULL) {
        ui->premier->precedent = ch;
    } else if (ui->dernier == NULL && ui->premier == NULL) {
       ui->dernier = ch;
    }
    ui->premier = ch;
    ui->len++;
    return ui;
}


/**
 * Supprime le premier élément
 * @param la structure à modifier
 */
static unbounded_int * deleteFirstElem (unbounded_int *ui) {
    if (ui->len <= 1) return NULL;
    chiffre *tmp;
    tmp = ui->premier;
    ui->premier = ui->premier->suivant;
    ui->premier->precedent = NULL;
    //free(tmp->c);
    free(tmp);
    ui->len--;
    return ui;
}


/**
 * Delete last elem
 *
 */
static unbounded_int * deleteLastElem (unbounded_int *ui) {
    if (ui->len <= 1) return NULL;
    chiffre *tmp;
    tmp = ui->dernier;
    ui->dernier = ui->dernier->precedent;
    ui->dernier->suivant = NULL;
    //free(tmp->c);
    free(tmp);
    ui->len--;
    return ui;
}


/**
 * Fonction auxiliaire pour afficher une structure unbounded_int
 * @param ui structure unbounded_int
 */
static void print_unbounded_int (unbounded_int ui) {
    unbounded_int tmp = UNBOUNDED_INT_ERROR;
    if (ui.signe == tmp.signe && ui.len == tmp.len && ui.premier == tmp.premier && ui.dernier == tmp.dernier) {
        printf("L'unbounded_int est vide\n");
        return;
    }
    if (ui.signe == '-') {
        printf("-");
    }
    chiffre *p = ui.premier;
    for (int i = 0; i < ui.len; i++) {
        if (p == ui.dernier) {
            printf("%c\n", p->c);
            return;
        }
        printf("%c", p->c);
        p = p->suivant;
    }
}


/**
 *
 *
 *
 */
static unbounded_int sumPositifUnbounded (unbounded_int *a, unbounded_int *b) {
    
    unbounded_int *res = malloc(sizeof(unbounded_int));
    res->signe = a->signe;
    res->len = 0;
    int ret = 0;
    chiffre *tmp_a = a->dernier;
    chiffre *tmp_b = b->dernier;
    int max_len = (a->len < b->len) ? b->len : a->len;
    for (int i = 0; i < max_len; i++) {

        char a;
        if(tmp_a == NULL) a = '0';
        else a = tmp_a->c;
        char b;
        if(tmp_b == NULL) b = '0';
        else b = tmp_b->c;
        
        int c = (a - '0') + (b - '0') + ret;

        char unité = (char)((c%10) + '0');
        ret = c/10;
        res = pushBack(res,unité);
        
        if(tmp_a != NULL) {
            tmp_a = tmp_a->precedent;    
        } 
        
        if(tmp_b != NULL) {
            tmp_b = tmp_b->precedent;    
        } 

    }

    if (ret >= 1) {
        res = pushBack(res, (char)ret+'0');
    }
    return *res;
    
}


/**
 *
 */
static unbounded_int sumNegatifUnbounded(unbounded_int *a, unbounded_int *b) {
    int plusgrand = plusGrandAbs(*a, *b);
    unbounded_int *res = malloc(sizeof(unbounded_int));
    if (plusgrand == 0) { 
        res = pushBack(res, '0');
        
        return *res;
    }
    res->len = 0;
    int max_len = (a->len < b->len) ? b->len : a->len;
    chiffre *tmp_a;
    chiffre *tmp_b;
    if (plusgrand == -1) {

        res->signe = b->signe;
        tmp_a = b->dernier;
        tmp_b = a->dernier;
    } else {
        res->signe = a->signe;
        tmp_a = a->dernier;
        tmp_b = b->dernier;
    }
    int ret = 0;
    for (int i = 0; i < max_len; i++) {
        char a;
        if(tmp_a == NULL) a = '0';
        else a = tmp_a->c;
        char b;
        if(tmp_b == NULL) b = '0';
        else b = tmp_b->c;
        
        int c = (a - '0') - (b - '0') - ret;
        if (c < 0) {
            c += 10;
            ret = 1;
        } else ret = 0;   
        char unité = (char)((c%10) + '0');
        res = pushBack(res,unité);
        
        if(tmp_a != NULL) {
            tmp_a = tmp_a->precedent;    
        } 
        
        if(tmp_b != NULL) {
            tmp_b = tmp_b->precedent;    
        } 

    } 

    return *res;
}


/**
 * 
 * @return -1 si |a| < |b|, 0 si |a| = |b| et sinon 1 si |a| > |b| 
 */
static int plusGrandAbs (unbounded_int a, unbounded_int b) {
    a.signe = '+';
    b.signe = '+';
    return unbounded_int_cmp_unbounded_int(a, b);
}


/**
 *
 *
 */
static unbounded_int * multiplicationPourUnChiffre (char c, unbounded_int *ui) {
    unbounded_int *p = malloc (sizeof(unbounded_int));
    int a = c - '0';
    if (a == 1) return ui;
    int ret = 0;
    chiffre *tmp = ui->dernier;
    for (int i = 0; i < ui->len; i++) {
        int tmp_c = tmp->c -'0';
        int mul = (a * tmp_c) + ret;
        ret = mul/10;
        char num = (char)((mul%10) + '0');
        p = pushBack(p, num);

        tmp = tmp->precedent;
    }
    if (ret >= 1) {
        p = pushBack(p,(char)(ret+'0'));
    }
    return p;
}


/**
 *
 *
 *
 */
static long long puissance (int base, int puissance) {
    int res = 1;
    int puiss = puissance/2;
    for (int i = 0; i < puiss; i++) {
        res *= base;
    }
    res *= res;
    if (puissance%2 == 0) {
        return res; 
    }
    return res*base;
}
