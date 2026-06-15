#include "listaCircularDoble.h"

void listaCircularDobleCrear(tListaCircularDoble *l)
{
	if (!l) {
		return;
	}

	*l = NULL;
}

int listaCircularDobleEmpujar(tListaCircularDoble *l, const void *data,
			      unsigned n)
{
	tNodo2 *nuevo;

	if (!l || !data) {
		return ERR;
	}

	nuevo = malloc(sizeof(tNodo2));

	if (!nuevo) {
		return ERR;
	}

	nuevo->data = malloc(n);

	if (!nuevo->data) {
		free(nuevo);
		return ERR;
	}

	memcpy(nuevo->data, data, n);
	nuevo->n = n;

	if (!*l) {
		(*l)->sig = nuevo;
		(*l)->ant = nuevo;
		(*l) = nuevo;
	} else {
		nuevo->ant = (*l)->ant;
		nuevo->sig = (*l);
		nuevo->ant->sig = nuevo;
		(*l)->sig = nuevo;
	}

	return OK;
}

int listaCircularDobleBuscar(tListaCircularDoble *l, const void *clave,
			     void *buf, unsigned n, fnCmp cmp)
{
	int compar;
	tNodo2 *comienzo = *l;

	if (!l || !clave || !buf || !cmp) {
		return ERR;
	}

	while (*l != comienzo) {
		compar = cmp((*l)->data, clave);

		if (compar == 0) {
			memcpy(buf, (*l)->data, MIN((*l)->n, n));
			return OK;
		}

		l = &(*l)->sig;
	}

	return ERR;
}

int listaCircularDobleActualizarEnPos(tListaCircularDoble *l, const void *data,
				      int pos, fnAccion accion)
{
	tNodo2 *pri, *act;
	int i = 0;

	if (!l || !*l || !data || !accion) {
		return ERR;
	}

	pri = *l;
	act = pri;

	do {
		if (i == pos) {
			break;
		}

		act = act->sig;
	} while (act != pri);

	if (i != pos) {
		return ERR;
	}

	accion(act->data, (void *)data);

	return OK;
}

void listaCircularDobleDestruir(tListaCircularDoble *l)
{
	tNodo2 *tmp;

	while (*l) {
		tmp = *l;
		l = &(*l)->sig;

		tmp->ant->sig = tmp->sig;
		tmp->sig->ant = tmp->ant;

		free(tmp->data);
		free(tmp);
		tmp = NULL;
	}
}

int listaCircularDobleBuscarPos(tListaCircularDoble *l, const void *d,
				fnCmp cmp)
{
	if (!l || !*l || !cmp)
		return ERR;

	tNodo2 *head = *l;
	tNodo2 *act = head;
	int pos = 0;
	do {
		if (cmp(d, act->data) == 0)
			return pos;
		pos++;
		act = act->sig;
	} while (act != head);
	return ERR;
}
int listaCircularDobleMostrarLR(const tListaCircularDoble *l,
				const fnAccion print)
{
	if (!l || !*l || !print)
		return 0;

	tNodo2 *start = (*l)->ant;
	tNodo2 *act = start;
	int cant = 0;
	do {
		print(act->data, NULL);
		act = act->ant;
		cant++;
	} while (act != start);
	return cant;
}
int listaCirclarDobleMirarEnPos(tListaCircularDoble *l, void *buff,
				const unsigned tam, int pos)
{
	if (!l || !*l || pos < 0)
		return ERR;

	tNodo2 *head = *l;
	tNodo2 *act = head;
	int i = 0;
	do {
		if (i == pos)
			break;
		act = act->sig;
		i++;
	} while (act != head);

	if (i != pos)
		return ERR;

	if (buff)
		memcpy(buff, act->data, MIN(tam, act->n));
	return OK;
}
int listaCircularDobleMirarEnPos(tListaCircularDoble *l, void *buff,
				 const unsigned tam, int pos)
{
	if (!l || !*l || pos < 0)
		return ERR;

	tNodo2 *head = *l;
	tNodo2 *act = head;
	int i = 0;
	do {
		if (i == pos)
			break;
		act = act->sig;
		i++;
	} while (act != head);

	if (i != pos)
		return ERR;

	if (buff)
		memcpy(buff, act->data, MIN(tam, act->n));
	return OK;
}
