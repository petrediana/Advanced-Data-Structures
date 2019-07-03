#include "pch.h"
#include <stdio.h>
#include <malloc.h>

struct nod_arbore_AVL {
	int inf;
	nod_arbore_AVL *stanga;
	nod_arbore_AVL *dreapta;	
	int balance_f;
};

nod_arbore_AVL *creare_nod(int inf, nod_arbore_AVL *stanga, nod_arbore_AVL *dreapta) {
	nod_arbore_AVL *nou = (nod_arbore_AVL*)malloc(sizeof(nod_arbore_AVL));

	nou->inf = inf;
	nou->stanga = stanga;
	nou->dreapta = dreapta;
	nou->balance_f = 0;

	return nou;
}

nod_arbore_AVL *inserare_nod(nod_arbore_AVL *radacina, int inf) {
	nod_arbore_AVL *aux = radacina;

	if (radacina == NULL) {
		return creare_nod(inf, NULL, NULL);
	}
	else {
		while (true) {
			if (aux->inf > inf) {
				//sunt pe ramura stanga
				if (aux->stanga == NULL) {
					aux->stanga = creare_nod(inf, NULL, NULL);
					return radacina;
				}
				else {
					aux = aux->stanga;
				}
			}
			else {
				if (aux->inf < inf) {
					//sunt pe ramura dreapta
					if (aux->dreapta == NULL) {
						aux->dreapta = creare_nod(inf, NULL, NULL);
						return radacina;
					}
					else {
						aux = aux->dreapta;
					}
				}
				else {
					//cheie duplicat..
					return radacina;
				}
			}
		}
	}
}

//radacina, stanga, dreapta
void traversare_PREORDINE(nod_arbore_AVL *radacina) {
	if (radacina != NULL) {
		printf("\nInformatia: %d + BF: %d", radacina->inf, radacina->balance_f);
		traversare_PREORDINE(radacina->stanga);
		traversare_PREORDINE(radacina->dreapta);
	}
}

//stanga, radacina, dreapta
void traversare_INORDINE(nod_arbore_AVL *radacina) {
	if (radacina != NULL) {
		traversare_INORDINE(radacina->stanga);
		printf("\nInformatia: %d + BF : %d", radacina->inf, radacina->balance_f);
		traversare_INORDINE(radacina->dreapta);
	}
}

int get_maxim(int a, int b) {
	return a > b ? a : b;
}

int calculeaza_nr_nivel(nod_arbore_AVL *radacina) {
	if (radacina != NULL) {
		return 1 + get_maxim(calculeaza_nr_nivel(radacina->stanga), calculeaza_nr_nivel(radacina->dreapta));
	}
	else {
		return 0;
	}
}

void calcul_BF_toate_nodurile(nod_arbore_AVL *radacina) {
	if (radacina != NULL) {
		radacina->balance_f = calculeaza_nr_nivel(radacina->dreapta) - calculeaza_nr_nivel(radacina->stanga);

		calcul_BF_toate_nodurile(radacina->stanga);
		calcul_BF_toate_nodurile(radacina->dreapta);
	}
}

//am dezechilibru pe partea stanga!!
nod_arbore_AVL *rotire_dreapta(nod_arbore_AVL *radacina_ini) {
	printf("\nRotire dreapta");
	if (radacina_ini != NULL) {

		nod_arbore_AVL *radacina_noua = radacina_ini->stanga;
		radacina_ini->stanga = radacina_noua->dreapta;
		radacina_noua->dreapta = radacina_ini;

		radacina_ini = radacina_noua;
		return radacina_ini;
	}
}

//am dezechilibru pe partea dreapta!!
nod_arbore_AVL *rotire_stanga(nod_arbore_AVL *radacina_ini) {
	printf("\nRotire stanga");
	if (radacina_ini != NULL) {

		nod_arbore_AVL *radacina_noua = radacina_ini->dreapta;
		radacina_ini->dreapta = radacina_noua->stanga;
		radacina_noua->stanga = radacina_ini;

		radacina_ini = radacina_noua;
		return radacina_ini;
	}
}

//am dezechilibru pe partea dreapta si apoi pe stanga!! deci prima data rotesc dreapta apoi stanga
nod_arbore_AVL *rotire_dreapta_stanga(nod_arbore_AVL *radacina_ini) {
	printf("\nRotire dreapta");
	nod_arbore_AVL *nod1 = radacina_ini->dreapta;
	nod_arbore_AVL *radacina_noua = nod1->stanga;

	nod1->stanga = radacina_noua->dreapta;
	radacina_noua->dreapta = nod1;

	radacina_ini->dreapta = radacina_noua->stanga;
	radacina_noua->stanga = radacina_ini;
	radacina_ini = radacina_noua;

	return radacina_ini;
}

nod_arbore_AVL *rotire_stanga_dreapta(nod_arbore_AVL *rad) {
	printf("\nRotatie stanga-dreapta\n");
	nod_arbore_AVL* nod1 = rad->stanga;
	nod_arbore_AVL* nod2 = nod1->dreapta;


	nod1->dreapta = nod2->stanga;
	nod2->stanga = nod1;
	rad->stanga = nod2->dreapta;
	nod2->dreapta = rad;
	rad = nod2;
	return rad;
}

nod_arbore_AVL *re_echilibru(nod_arbore_AVL *radacina) {
	if (radacina != NULL) {
		calcul_BF_toate_nodurile(radacina);

		//1.rotesc la dreapta
		if (radacina->balance_f <= -2 && radacina->stanga->balance_f <= -1) {
			radacina = rotire_dreapta(radacina);
			calcul_BF_toate_nodurile(radacina);
		}
		else {
			//2.rotesc la stanga
			if (radacina->balance_f >= 2 && radacina->dreapta->balance_f >= 1) {
				radacina = rotire_stanga(radacina);
				calcul_BF_toate_nodurile(radacina);
			}
			else {
				//3.rotesc dreapta-stanga
				if (radacina->balance_f >= 2 && radacina->dreapta->balance_f <= -1) {
					radacina = rotire_dreapta_stanga(radacina);
					calcul_BF_toate_nodurile(radacina);
				}
				else {
					//4.rotesc stanga_dreapta
					if (radacina->balance_f <= -2 && radacina->stanga->balance_f >= 1) {
						radacina = rotire_stanga_dreapta(radacina);
						calcul_BF_toate_nodurile(radacina);
					}
				}
			}
		}
	}
	return radacina;
}

nod_arbore_AVL *get_minim_arbore(nod_arbore_AVL *radacina) {
	nod_arbore_AVL *aux = radacina;
	
	if (radacina != NULL) {		

		while (aux->stanga)
			aux = aux->stanga;
	}

	return aux;
}

nod_arbore_AVL *sterge_nod_arbore(nod_arbore_AVL *radacina, int element_de_sters) {
	//spionez radacina sa vad daca gasesc elementul pe ea.. daca nu il caut

	if (radacina->inf == element_de_sters) {
		//ok am gasit elementul acum verific sa il sterg

		if (radacina->stanga == NULL && radacina->dreapta == NULL) {
			//cazul cel mai fericit
			nod_arbore_AVL *sterge = radacina;
			radacina = NULL;

			free(sterge);
		}
		else {
			if (radacina->stanga == NULL && radacina->dreapta != NULL) {
				//am un nod in dreapta radacinii
				nod_arbore_AVL *sterge = radacina;
				radacina = radacina->dreapta;

				free(sterge);
			}
			else {
				if (radacina->stanga != NULL && radacina->dreapta == NULL) {
					//am un nod in stanga radacinii
					nod_arbore_AVL *sterge = radacina;
					radacina = radacina->stanga;

					free(sterge);
				}
				else {
					//exista nod la stanga si exista nod la dreapta!! -> caz mai naspa
					//caut minimul pe partea dreapta, sa imi puna radacina cea mai apropiata valoare de radacina veche
					//deci mut radacina initiala pe pozitia care avea valoarea cea mai apropiata de ea
					//incerc sa o aduc la un caz mai simplu

					nod_arbore_AVL *minim_partea_dreapta = get_minim_arbore(radacina->dreapta);
					radacina->inf = minim_partea_dreapta->inf;
					radacina->dreapta = sterge_nod_arbore(radacina->dreapta, radacina->inf);
					radacina = re_echilibru(radacina);
				}
			}
		}
	}
	else {
		if (radacina->inf > element_de_sters) {
			radacina ->stanga= sterge_nod_arbore(radacina->stanga, element_de_sters);
		}
		else {
			if (radacina->inf < element_de_sters)
				radacina ->dreapta= sterge_nod_arbore(radacina->dreapta, element_de_sters);
		}
	}

	//traversare_INORDINE(radacina);
	return radacina;
}


void main() {
	///*int v[] = { 40, 35, 70, 30, 37, 65 };
	//nod_arbore_AVL *radacina = NULL;

	//for (int i = 0; i < 6; i++) {
	//	radacina = inserare_nod(radacina, v[i]);
	//	calcul_BF_toate_nodurile(radacina);
	//}

	//printf("\n\nTraversare PREORDINE");
	//traversare_PREORDINE(radacina);

	//printf("\n\nTraversare INORDINE");
	//traversare_INORDINE(radacina);*/

	//printf("\n\n---TEST ROTIRE DREAPTA");
	//int v1[] = { 40, 35, 30, 25 };
	//nod_arbore_AVL *rad1 = NULL;

	//for (int i = 0; i < 4; i++) {
	//	rad1 = inserare_nod(rad1, v1[i]);
	//	calcul_BF_toate_nodurile(rad1);

	//	if (rad1->balance_f <= -2 && rad1->stanga->balance_f <= -1) {
	//		rad1 = rotire_dreapta(rad1);
	//		calcul_BF_toate_nodurile(rad1);
	//	}
	//}	

	//traversare_INORDINE(rad1);


	//printf("\n\n---TEST ROTIRE STANGA");
	//int v2[] = { 40, 50, 60, 70 };
	//nod_arbore_AVL *rad2 = NULL;


	//for (int i = 0; i < 4; i++) {
	//	rad2 = inserare_nod(rad2, v2[i]);
	//	calcul_BF_toate_nodurile(rad2);

	//	if (rad2->balance_f >= 2 && rad2->dreapta->balance_f >= 1) {
	//		rad2 = rotire_stanga(rad2);
	//		calcul_BF_toate_nodurile(rad2);
	//	}
	//}	

	//traversare_INORDINE(rad2);

	//printf("\n\n---REST ROTIRE DREAPTA STANGA");
	//int v3[] = { 40, 35, 45, 42, 50, 41 };
	//nod_arbore_AVL *rad3 = NULL;

	//for (int i = 0; i < 6; i++) {
	//	rad3 = inserare_nod(rad3, v3[i]);
	//	calcul_BF_toate_nodurile(rad3);

	//	if (rad3->balance_f >= 2 && rad3->dreapta->balance_f <= -1) {
	//		rad3 = rotire_dreapta_stanga(rad3);
	//		calcul_BF_toate_nodurile(rad3);
	//	}
	//}
	//traversare_INORDINE(rad3);

	/*int v[] = { 50, 40, 55, 45, 42, 46 };
	nod_arbore_AVL *radacina = NULL;

	for (int i = 0; i < 6; i++) {
		radacina = inserare_nod(radacina, v[i]);
		radacina = re_echilibru(radacina);
	}

	traversare_INORDINE(radacina);*/

	int v[] = { 40, 35, 45 };
	nod_arbore_AVL *radacina = NULL;

	for (int i = 0; i < 3; i++) {
		radacina = inserare_nod(radacina, v[i]);
		radacina = re_echilibru(radacina);
	}

	traversare_INORDINE(radacina); 


	nod_arbore_AVL *aux = radacina;
	printf("\nSterg un element => %d", 35);
	aux = sterge_nod_arbore(radacina, 40);

	re_echilibru(radacina);

	traversare_INORDINE(radacina);
}

