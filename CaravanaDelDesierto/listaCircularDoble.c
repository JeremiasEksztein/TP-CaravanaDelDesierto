#include "listaCircularDoble.h"

void listaCircularDobleCrear(tListaCircularDoble *l)
{
	if (!l) {
		return;
	}

	*l = NULL;
}

int listaCircularDobleEmpujar(tListaCircularDoble *l, const void *d,
			      const unsigned tam)
{
	tNodo2 *nue = (tNodo2 *)malloc(sizeof(tNodo2));
	if (!nue)
		return ERR;
	nue->data = malloc(tam);
	if (!nue->data) {
		free(nue);
		return ERR;
	}
	memcpy(nue->data, d, tam);
	nue->n = tam;

	if (*l == NULL) {
		nue->sig = nue;
		nue->ant = nue;
		*l = nue;
	} else {
		tNodo2 *tail = (*l)->ant;
		nue->sig = *l;
		nue->ant = tail;
		tail->sig = nue;
		(*l)->ant = nue;
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
