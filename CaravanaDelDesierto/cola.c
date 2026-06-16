#include "cola.h"

void colaCrear(tCola *c)
{
	if (!c) {
		return;
	}

	c->pri = c->ult = NULL;
}

int colaEncolar(tCola *c, const void *data, unsigned n)
{
	tNodo *nue = (tNodo *)malloc(sizeof(tNodo));

	if (!nue) {
		return ERR; /* Error: sin memoria para el nodo */
	}

	nue->data = malloc(n);
	if (!nue->data) {
		free(nue);
		return ERR; /* Error: sin memoria para los datos */
	}

	memcpy(nue->data, data, n);
	nue->n = n;
	nue->sig = NULL;

	if (c->ult == NULL) {
		/* Cola vacía: el nuevo nodo es tanto primero como último */
		c->pri = nue;
		c->ult = nue;
	} else {
		/* Cola no vacía: conectar después del último nodo */
		c->ult->sig = nue;
		c->ult = nue;
	}

	return OK;
}

int colaDesencolar(tCola *c, void *buf, unsigned n)
{
	tNodo *borrar;

	if (!c || !c->pri || !c->ult || !buf) {
		return ERR;
	}

	borrar = c->pri;

	memcpy(buf, borrar->data, MIN(borrar->n, n));

	if (c->pri == c->ult) {
		/* Solo hay un elemento: eliminarlo y dejar cola vacía */
		c->pri = NULL;
		c->ult = NULL;
	} else {
		/* Hay múltiples elementos: mover el siguiente al frente */
		c->pri = c->pri->sig;
	}

	free(borrar->data);
	free(borrar);

	return OK;
}

int colaFrente(const tCola *c, void *buf, unsigned n)
{
	if (!c || !c->pri || !c->ult || !buf) {
		return ERR;
	}

	memcpy(buf, c->pri->data, MIN(c->pri->n, n));

	return OK;
}

int colaEstaVacia(const tCola *c)
{
	if (!c) {
		return TRUE;
	}

	return (c->pri == NULL) ? TRUE : FALSE;
}

void colaDestruir(tCola *c)
{
	tNodo *tmp;

	while (c->pri) {
		tmp = c->pri;
		c->pri = c->pri->sig;
		free(tmp->data);
		free(tmp);
	}

	c->pri = c->ult = NULL;
}
