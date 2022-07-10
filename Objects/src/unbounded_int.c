
#include "unbounded_int.h"

#include <stdlib.h>
#include <stdio.h>

#include "lib.h"
#include <string.h>
#include <ctype.h>

#include "exec_error.h"
#define NUM (48)

static Number *number_new(char c) {
	if (!isdigit(c)) {
		return NULL;
	}
	Number *n = malloc(sizeof(Number));
	if (n == NULL) {
		perror_src("");
		return NULL;
	}
	n->mNext = NULL;
	n->mLast = NULL;
	n->mVal = c;
	return n;
}

static UnboundedInt addFirst(UnboundedInt u, char c) {
	Number *new = number_new(c);
	if (new == NULL) {
		perror_src("Memory insufficient");
		return u;
	}

	if (u.mLength == 0) {
		u.mFirst = new;
		u.mLast = new;
		u.mLength++;
		return u;
	}

	new->mNext = u.mFirst;
	u.mFirst->mLast = new;
	u.mFirst = new;
	u.mLength++;
	return u;

}

static UnboundedInt addLast(UnboundedInt u, char c) {
	Number *new = number_new(c);
	if (new == NULL) {
		perror_src("Memory insufficient");
		return u;
	}

	if (u.mLength == 0) {
		u.mFirst = new;
		u.mLast = new;
		u.mLength++;
		return u;
	}

	new->mLast = u.mLast;
	u.mLast->mNext = new;
	u.mLast = new;
	u.mLength++;
	return u;

}

UnboundedInt UnboundedInt_free(UnboundedInt u) {
	Number *c = u.mFirst;
	if (c == NULL) return UNBOUNDED_INT_ERROR;
	while (c->mNext != NULL) {
		Number *n = c->mNext;
		free(c);
		c = n;
	}
	free(c);
	return UNBOUNDED_INT_ERROR;
}

static char *buildString(UnboundedInt ui, char *new, size_t length) {
	Number *current = ui.mFirst;
	for (size_t i = 0; i < length; i++) {
		new[i] = current->mVal;
	}
	new[length] = '\0';
	return new;
}

static UnboundedInt unboundedInt_cpy(UnboundedInt u) {
	UnboundedInt cpy = UNBOUNDED_INT_ERROR;
	cpy.mSign = u.mSign;
	Number *current = u.mFirst;
	while (current != NULL) {
		cpy = addLast(cpy, current->mVal);
		current = current->mNext;
	}
	return cpy;
}

static UnboundedInt removeFirst(UnboundedInt ui) {
	if (ui.mLength <= 1) return UNBOUNDED_INT_ERROR;
	Number *tmp;
	tmp = ui.mFirst;
	ui.mFirst = ui.mFirst->mNext;
	ui.mFirst->mLast = NULL;
	free(tmp);
	ui.mLength--;
	return ui;
}

/**
 * Clean up an UnboundedInt by removing the unnecessary zero before the first non-zero numbe
 * @param ui the UnboundedInt that will be cleaned 
 * @return return ui with the modification
 */

static UnboundedInt cleanUnbounded_int(UnboundedInt ui) {

	if (ui.mLength <= 1) return ui;
	if (ui.mFirst->mVal != '0') return ui;
	int index = 0;
	Number *tmp;// = ui->mFirst;
	for (tmp = ui.mFirst; tmp != NULL; tmp = tmp->mNext) {
		if (tmp->mVal != '0') {
			break;
		}
		index += 1;
	}
	if (tmp == NULL) {
		for (int i = 0; i < ui.mLength - 1; i++) {
			ui = removeFirst(ui);
		}
		return ui;
	}

	for (int i = 0; i < index; i++) {
		ui = removeFirst(ui);
	}

	return ui;

}

UnboundedInt UnboundedInt_newString(char *e) {
	if (isAStringNum(e) == 0) return UNBOUNDED_INT_ERROR;
	const char *str = cleanNumber(e);
	UnboundedInt res = UNBOUNDED_INT_ERROR;
	int i;
	res.mLength = 0;
	if (*str == '-' || *str == '+') {
		if (*str == '-') {
			res.mSign = '-';
		}
		else {
			res.mSign = '+';
		}
		i = 1;
	}
	else {
		res.mSign = '+';
		i = 0;
	}
	for (int j = i; j < strlen(str); j++) {
		res = addLast(res, *(str + j));
	}
	return res;
}

UnboundedInt UnboundedInt_newll(long long i) {
	UnboundedInt new = UNBOUNDED_INT_ERROR;
	int base = 10;
	new.mSign = (i >= 0) ? '+' : '-';
	long long n = llabs(i);
	while (n > 0) {
		int d = (int) (n % base);
		new = addFirst(new, (char) (NUM + d));
		n /= base;
	}
	return new;
}

char *UnboundedInt_toString(UnboundedInt ui) {
	if (ui.mLength == 0) {
		char *new = malloc(sizeof(char));
		if (new == NULL) {
			perror_src("");
			return NULL;
		}
		new[0] = '\0';
		return new;
	}
	size_t length = ui.mLength;
	char *new;
	if (ui.mSign == '-') {
		length++;
		new = malloc((length + 1) * sizeof(char));
		if (new == NULL) {
			perror_src("");
			return NULL;
		}
		new[0] = '-';
		buildString(ui, new + 1, length - 1);
		return new;

	}
	new = malloc((length + 1) * sizeof(char));
	if (new == NULL) {
		perror_src("");
		return NULL;
	}
	return buildString(ui, new, length);
}

int UnboundedInt_cmpUnboundedInt(UnboundedInt a, UnboundedInt b) {
	if (
		a.mSign == '+' && b.mSign == '-' || (a.mSign == b.mSign && ((a.mSign == '+' && a.mLength > b.mLength)
			|| (a.mSign == '-' && a.mLength < b.mLength)))
		)
	{
		return 1;
	}
	if (
		a.mSign == '-' && b.mSign == '+' || (a.mSign == b.mSign && ((a.mSign == '+' && a.mLength < b.mLength)
			|| (a.mSign == '-' && a.mLength > b.mLength)))
		)
	{
		return -1;
	}

	Number *ac = a.mFirst;
	Number *bc = b.mFirst;
	while (ac != NULL) {
		if (ac->mVal < bc->mVal) {
			return -1;
		}
		if (bc->mVal < ac->mVal) {
			return 1;
		}
		ac = ac->mNext;
		bc = bc->mNext;
	}
	return 0;
}

int UnboundedInt_cmpll(UnboundedInt a, long long b) {
	UnboundedInt tmp = UnboundedInt_newll(b);
	int const bool = UnboundedInt_cmpUnboundedInt(a, tmp);
	UnboundedInt_free(tmp);
	return bool;
}

UnboundedInt UnboundedInt_add(UnboundedInt a, UnboundedInt b) {
	int base = 10;
	if (a.mLength == 0) {
		return unboundedInt_cpy(b);
	}
	if (b.mLength == 0) {
		return unboundedInt_cpy(a);
	}
	if (a.mSign == '+' && b.mSign == '-') {
		UnboundedInt b2 = { .mSign = '+', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast };
		return UnboundedInt_subtract(a, b2);
	}
	if (a.mSign == '-' && b.mSign == '+') {
		UnboundedInt a2 = { .mSign = '+', .mLength = a.mLength, .mFirst = a.mFirst, .mLast = a.mLast };
		return UnboundedInt_subtract(b, a2);
	}
	UnboundedInt result = UNBOUNDED_INT_ERROR;
	result.mSign = a.mSign;
	int r = 0;
	Number *ac = a.mLast;
	Number *bc = b.mLast;
	while (ac != NULL && bc != NULL) {
		int res = r + (ac->mVal - NUM) + (bc->mVal - NUM);
		if (res >= base) {
			r = res / base;
		} else {
			r = 0;
		}
		char c = (char) NUM + (res % base);
		result = addFirst(result, c);
		ac = ac->mLast;
		bc = bc->mLast;
	}
	while (ac != NULL) {
		int res = r + ac->mVal - NUM;
		if (res >= base) {
			r = res / base;
		} else {
			r = 0;
		}
		char c = (char) (NUM + (res % base));
		result = addFirst(result, c);
		ac = ac->mLast;
	}
	while (bc != NULL) {
		int res = r + bc->mVal - NUM;
		if (res >= base) {
			r = res / base;
		} else {
			r = 0;
		}
		char c = (char) (NUM + (res % base));
		result = addFirst(result, c);
		bc = bc->mLast;
	}
	if (r != 0) {
		result = addFirst(result, NUM + r);
	}
	return result;
}

UnboundedInt UnboundedInt_subtract(UnboundedInt a, UnboundedInt b) {
	int base = 10;
	UnboundedInt result = UNBOUNDED_INT_ERROR;
	result.mSign = '+';
	if (a.mLength == 0) {
		result = unboundedInt_cpy(b);
		result.mSign = (b.mSign == '+') ? '-' : '+';
		return result;
	}
	if (b.mLength == 0) {
		return unboundedInt_cpy(a);
	}

	if (a.mSign == '-' && b.mSign == '+') {
		UnboundedInt b2 = { .mSign = '-', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast };
		return UnboundedInt_add(a, b2);
	}

	if (a.mSign == '+' && b.mSign == '-') {
		UnboundedInt b2 = { .mSign = '+', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast };
		return UnboundedInt_add(a, b2);
	}

	if (a.mSign == '-' && b.mSign == '-') {
		UnboundedInt tmp = (UnboundedInt){ .mSign = '+', .mLength = a.mLength, .mFirst = a.mFirst, .mLast = a.mLast };
		a = (UnboundedInt){.mSign = '+', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast};
		b = (UnboundedInt){ .mSign = '+', .mLength = tmp.mLength, .mFirst = tmp.mFirst, .mLast = tmp.mLast };
	}

	if (UnboundedInt_cmpUnboundedInt(a,b) < 0) {
		result.mSign = '-';
		UnboundedInt tmp = (UnboundedInt){ .mSign = '+', .mLength = a.mLength, .mFirst = a.mFirst, .mLast = a.mLast };
		a = (UnboundedInt){ .mSign = '+', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast };
		b = (UnboundedInt){ .mSign = '+', .mLength = tmp.mLength, .mFirst = tmp.mFirst, .mLast = tmp.mLast };
	}
	int r = 0;
	Number *ac = a.mLast;
	Number *bc = b.mLast;
	while (ac != NULL && bc != NULL) {
		int res = (ac->mVal - NUM) - (bc->mVal - NUM) - r;
		if (res < 0) {
			r = 1;
			res += 10;
		} else {
			r = 0;
		}
		char c = (char) NUM + (res % base);
		result = addFirst(result, c);
		ac = ac->mLast;
		bc = bc->mLast;
	}
	while (ac != NULL) {
		int res = ac->mVal - NUM - r;
		if (res < 0) {
			r = 1;
			res += 10;
		} else {
			r = 0;
		}
		char c = (char) (NUM + (res % base));
		result = addFirst(result, c);
		ac = ac->mLast;
	}
	while (bc != NULL) {
		int res = r + bc->mVal - NUM;
		if (res >= base) {
			r = res / base;
		} else {
			r = 0;
		}
		char c = (char) (NUM + (res % base));
		result = addFirst(result, c);
		bc = bc->mLast;
	}
	return cleanUnbounded_int(result);
}

UnboundedInt UnboundedInt_multiply(UnboundedInt a, UnboundedInt b) {
	return UNBOUNDED_INT_ERROR;
}

UnboundedInt UnboundedInt_divide(UnboundedInt a, UnboundedInt b) {
	return UNBOUNDED_INT_ERROR;
}

UnboundedInt UnboundedInt_pow(UnboundedInt x, UnboundedInt n) {
	//UnboundedInt result = UnboundedInt_newll(1);
	//UnboundedInt decr = UnboundedInt_newll(1);
	//UnboundedInt mod = UnboundedInt_newll(2);
	//if (UnboundedInt_cmpll(n, 0) == 0) {
	//	return result;
	//}
	/*while (UnboundedInt_cmpll(n, 0) == 1) {
		//UnboundedInt modulo = fun(n,mod);
		if (UnboundedInt_cmpll(MODULO(n,mod),1) == 0) { //modulo
			result = UnboundedInt_multiply(result,x);
			n = UnboundedInt_subtract(n,decr);
		}
		x = UnboundedInt_multiply(x,x);
		n = UnboundedInt_multiply(n,mod);
	}*/
	return UNBOUNDED_INT_ERROR;
}

UnboundedInt UnboundedInt_abs(UnboundedInt x) {
	UnboundedInt result = unboundedInt_cpy(x);
	result.mSign = '+';
	return result;
}

UnboundedInt UnboundedInt_fact(UnboundedInt n) {
	UnboundedInt decr = UnboundedInt_newll(1);
	UnboundedInt result = UnboundedInt_newll(1);
	while (UnboundedInt_cmpll(n, 0) > 0) {
		UnboundedInt tmp = UnboundedInt_multiply(result, n);
		UnboundedInt_free(result);
		result = tmp;
		tmp = UnboundedInt_subtract(n, decr);
		UnboundedInt_free(n); //access to desallocated memory
		n = tmp;
	}
	UnboundedInt_free(decr);
	return result;
}

int UnboundedInt_isError(UnboundedInt i) {
	return i.mSign == UNBOUNDED_INT_ERROR.mSign;
}

void UnboundedInt_print(UnboundedInt ui) {
	if (UnboundedInt_isError(ui)) {
		printf("The unbounded int is empty!\n");
		return;
	}
	if (ui.mSign == '-') {
		printf("-");
	}
	Number *p = ui.mFirst;
	for (int i = 0; i < ui.mLength; i++) {
		printf("%c", p->mVal);
		p = p->mNext;
	}
	printf("\n");
}