//Compilación: "gcc deco.c tiempo.c -o deco"

#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

struct nodo {
	long valor;
	int ite;
	struct nodo *izq;
	struct nodo *der;
};
typedef struct nodo *ap_nodo;

ap_nodo raiz = NULL;
int aux_arr = 0;

ap_nodo crearNodo(long x, int it) {

	ap_nodo nuevo;
	nuevo = malloc(sizeof(struct nodo));
	nuevo->valor = x;
	nuevo->ite = it;
	nuevo->izq = NULL;
	nuevo->der = NULL;
	return nuevo;

}

int validarArreglo(ap_nodo arrN[]) {
	int banderita = -1;
	for (int i = 0; i < 256 && banderita == -1; ++i) {
		if (arrN[i]->valor != 0) {
			banderita = i;
		}
	}
	for (int j = 0; j < 256 - banderita; j++) {
		arrN[j] = arrN[banderita + j];
	}
	arrN = realloc(arrN, sizeof(ap_nodo) * (256 - banderita));

	return 256 - banderita;
}

void generarArbol(ap_nodo arrN[], int tam) {
	if (tam >= 2) {

		ap_nodo nuevo = malloc(sizeof(struct nodo));
		nuevo->ite = -2;
		nuevo->valor = (arrN[0]->valor) + (arrN[1]->valor);
		nuevo->izq = arrN[0];
		nuevo->der = arrN[1];
		//Metodo de reordenar el arreglo
		if (tam == 2) {
			arrN[0] = nuevo;
		} else {
			for (int a = 0; a < tam - 2; a++) {
				arrN[a] = arrN[a + 2];
			}
			arrN[tam - 1] = arrN[tam - 2];
			*arrN = realloc(*arrN, sizeof(ap_nodo) * (tam - 1));
			int i;
			long val = nuevo->valor;
			for (i = 0; i < tam - 2; i++) {
				if (arrN[i]->valor > val)
					break;
			}

			for (int k = tam - 1; k > i; k--) {
				arrN[k] = arrN[k - 1];
			}
			arrN[i] = nuevo;
			generarArbol(arrN, tam - 1);
		}
	}

}

void merge(int iArr[], long arr[], int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	long L[n1], R[n2];
	int iL[n1], iR[n2];

	for (i = 0; i < n1; i++) {
		L[i] = arr[l + i];
		iL[i] = iArr[l + i];
	}
	for (j = 0; j < n2; j++) {
		R[j] = arr[m + 1 + j];
		iR[j] = iArr[m + 1 + j];

	}

	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			iArr[k] = iL[i];
			i++;
		} else {
			arr[k] = R[j];
			iArr[k] = iR[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		arr[k] = L[i];
		iArr[k] = iL[i];
		i++;
		k++;
	}

	while (j < n2) {
		arr[k] = R[j];
		iArr[k] = iR[j];
		j++;
		k++;
	}
}


void mergeSort(int iArr[], long arr[], int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;

		mergeSort(iArr, arr, l, m);
		mergeSort(iArr, arr, m + 1, r);

		merge(iArr, arr, l, m, r);
	}
}

int obtBit(char wbyte, int pos) {
	int val = 0;
	char cero = (char)(1 >> 1);
	char mascara = (char)(1 << (7-pos));
	//printf("%c",mascara);
	if((char)(wbyte & mascara) == cero) {
		//printf("0 ");
		val = 0;
	}else
		if((char)(wbyte & mascara) == mascara) {
			//printf("1 ");
			val = 1;
		}
	
	return val;
}


int obtChar(ap_nodo raiz, int sec[]) {
	if(raiz -> izq == NULL && raiz -> der == NULL) {
		return raiz -> ite;
	}
	if(sec[aux_arr] == 1) {
		aux_arr++;
		return obtChar(raiz->der, sec);
	}
	if(sec[aux_arr] == 0) {
		aux_arr++;
		return obtChar(raiz->izq, sec);
	}

}

int main() {

	//double utime0, stime0, wtime0,utime1, stime1, wtime1;
	long sim, i ;
	char extension[5];
	long frec[256];
	int itFrec[256];
	char *textDeco;
	long tbits = 0;

	for (int a = 0; a < 256; a++) {
			frec[a] = 0;
			itFrec[a] = a;
		}

	//obtener datos

	FILE *infile = fopen("codificacion.mugi", "rb");
	if (!infile) {
		printf("\nError opening file\n");
		return -1;
	}
	
	FILE *doc
	doc = fopen("frecuencias.txt", "r");
	if(!doc) {
		printf("No pude abrir el archivo .txt\n");
		return -1;
	}
	fscanf(doc, "(%ld, %ld, %[^)])\n", &sim, &tbits, extension);
	
	for (int a = 0; a < 256; ++a) {
		fscanf(infile, "%ld,", &frec[a]);
	}
	
	textDeco = malloc(sim + 1);
	for (int a = 0; a < sim; ++a) {
		fscanf(infile, "%c", &textDeco[a]);
	}
	
	uswtime(&utime0, &stime0, &wtime0);
	fclose(infile);
	fclose(doc);
	//Reconstruccion del arbol de frecuencias:
	mergeSort(itFrec, frec, 0, 255);

	ap_nodo *arrNodos = calloc(256, sizeof(ap_nodo));

	for (int a = 0; a < 256; a++) {
		arrNodos[a] = crearNodo(frec[a], itFrec[a]);
	}
	int val_tam = validarArreglo(arrNodos);
	generarArbol(arrNodos, val_tam);
	raiz = arrNodos[0];

	int pos, aux;
	int *sec = malloc(sizeof(int) * tbits);
	for(int a = 0; a < tbits; a++) {
		pos = a/8;
		aux = a%8;
		sec[a] = obtBit(textDeco[pos], aux);
	}

	
	int *wchar = malloc(sizeof(char));
	long cont = 0;

	while(aux_arr < tbits) {
		//printf("%d ",aux_arr);
		wchar[cont] = obtChar(raiz, sec);
		//printf("%d, ", wchar[cont]);
		cont++;
		wchar = (int*)realloc(wchar,(cont+1)*sizeof(int)); 
	}
	
	uswtime(&utime1, &stime1, &wtime1);

	int tam_ext = sizeof(extension);
	int j = 0;
	char *nombre = malloc(5+tam_ext);
	char auxNombre[5] = "copia";
	for(int a = 0; a < 5; a++) {
		nombre[a] = auxNombre[a];
	}	
	for (int a = 5; a < 5+tam_ext; ++a)
	{
		nombre[a] = extension[j];
		j++;
	}
	
	infile = fopen(nombre, "wb");
	if (!infile){
		 printf("No he podido crear el archivo");
		 return -1;
	 }

	for(i = 0; i<cont; i++){
		fprintf(infile, "%c", wchar[i]);
	}
	
	fclose(infile);
	// los datos ya estan
	
	free(wchar);
	free(arrNodos);
	free(sec);
	free(textDeco);
	free(nombre);
	printf("Tamaño del archivo decodificado(en bytes): %ld\n", cont);
	printf("Nombre del archivo decodificado: %s\n", nombre);
	printf("Tiempo de ejecucion (Tiempo total)  %.10e s\n",  wtime1 - wtime0);
	
	return 0;
}

