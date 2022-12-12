//Compilación: "gcc codi.c tiempo.c -o codi"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tiempo.h"

#define rot 12

struct nodo {
	long valor;
	int ite;
	struct nodo *izq;
	struct nodo *der;
};
typedef struct nodo *ap_nodo;

ap_nodo raiz = NULL;
int enteritos[rot];
int auxEnteritos = -1, mos = 0;

ap_nodo crearNodo(long x, int it) {

	ap_nodo nuevo;
	nuevo = malloc(sizeof(struct nodo));
	nuevo->valor = x;
	nuevo->ite = it;
	nuevo->izq = NULL;
	nuevo->der = NULL;
	return nuevo;

}

unsigned long binarioToDecimal(char *binario, int length) {

	long a;
	unsigned long decimal = 0;
	unsigned long peso = 1;
	binario += length - 1;
	peso = 1;

	for (a = 0; a < length; ++a, --binario) {
		if (*binario == '1') {
			decimal += peso;
		}
		peso *= 2;
	}
	return decimal;
}

void binarioToText(char *informacionBin, long informacionLength, long sim,
		char *text) {

	long a;
	for (a = 0; a < informacionLength; a += 8, informacionBin += 8) {

		char *byte = informacionBin;
		//byte[8] = '\0';
		*text++ = binarioToDecimal(byte, 8);
	}
}

// funcion para conocer el recorrido del arbol, le pasas la raiz, el iterador para conocer
void recorrido(ap_nodo p, int target, int aux) {

	if (mos == 0)
		if (p != NULL) {
			if (p->ite == target) {
				// cuando el numero de bits pase de 11 imprimir aux ayuda con el num de enteritos
				if (aux > rot)
					printf("\nayuda con el tam de enteritos %d\n", aux);
				mos = 1;
				return;
			}

			if (p->izq != NULL && p->der != NULL) {

				auxEnteritos++;
				enteritos[auxEnteritos] = 0;
				recorrido(p->izq, target, ++aux);
				if (mos == 1)
					return;

				enteritos[auxEnteritos] = -1;
				auxEnteritos--;

				auxEnteritos++;
				enteritos[auxEnteritos] = 1;
				recorrido(p->der, target, aux++);
				if (mos == 1)
					return;
				enteritos[auxEnteritos] = -1;
				auxEnteritos--;

			}

		}
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

int main() {
	double utime0, stime0, wtime0,utime1, stime1, wtime1;
	long i;
	char cadenita[20];
	char cadenota[20];
	char caracter = 46;
	int ext = 0;

	char *extension = malloc(sizeof(char) * 1);
	printf("\nEscribe el nombre del archivo: ");
	scanf("%s", cadenita);

	for (int m = 0; m < 20; m++) {
		cadenota[m] = cadenita[m];
	}

	FILE *doc = fopen(cadenita, "rb");
	if (!doc) {
		printf("No he podido abrir el archivo");
		return 0;
	}

	uswtime(&utime0, &stime0, &wtime0);

	int extt = sizeof(cadenota) / sizeof(cadenota[0]);
	for (int y = 0; y < extt; y++) {
		if (cadenota[y] == caracter) {
			ext = y;
		}
	}

	extension = realloc(extension, sizeof(char) * (extt - ext));
	int a = 0;
	for (int j = ext; j < extt; j++) {
		extension[a] = cadenota[j];
		a++;
	}
	printf("Extension es: %s\n", extension);

	//final
	fseek(doc, 0, SEEK_END);
	//cuenta los bytes
	long tam = ftell(doc);
	// vuelve al principio
	rewind(doc);

	//El archivo tiene "tam" bytes

	// guardamos memoria para el arreglo de valores hexadecimales
	int *wchar = malloc(sizeof(*wchar) * (tam + 1));

	// creamos la tabla de frecuencias
	long frec[256];
	int itFrec[256];
	long frecCopy[256];

	for (int k = 0; k < 256; k++) {
		itFrec[k] = k;

		frec[k] = 0;
		frecCopy[k] = 0;
	}

	//guardamos cada byte que aparezca
	for (i = 0; i < tam; i++) {
		wchar[i] = fgetc(doc);
		// guardamos la frecuencia con la que aparece cada byte
		frec[wchar[i]]++;
		frecCopy[wchar[i]]++;
	}

	// el archivo ya esta guardado en wchar
	//printf("El archivo tiene %ld bytes\n", tam);
	// cerramos el aechivo

	fclose(doc);

	// creamos el nuevo nombre del archivo
	
	itFrec[0] = 0;
	itFrec[1] = 1;

	// ordenar por frecuencias:
	mergeSort(itFrec, frec, 0, 256 - 1);

	// Generacion del arreglo de tamano 256 que tiene guardado apuntadores de nodos ordenados ascendentemente por freccuencia

	ap_nodo *arrNodos = calloc(256, sizeof(ap_nodo));

	for (a = 0; a < 256; a++) {
		arrNodos[a] = crearNodo(frec[a], itFrec[a]);
	}
	// val_tam tiene el numero de nodos que realmente  existen
	int val_tam = validarArreglo(arrNodos);

	// al llamar la funcion generarArbol ordena de acuerdo a hoffman el arbol y nos devuelve en arrNodos en 0 la raiz del arbol
	generarArbol(arrNodos, val_tam);
	raiz = arrNodos[0];

	//crear un arreglo de 2 dimensiones de 256 columnas por 11 filas
	// la primera esta numerada de del 0 al 255
	// la columna del 1 al 7  va a tener el recorrido que tiene que hacer el arbol para llegar al iterador
	int fila = rot + 1, colu = 256;
	int rutas[fila][colu];

	for (int b = 0; b < colu; b++) {
		for (a = 0; a < fila; a++) {
			rutas[a][b] = -1;
		}
	}

	int buscado;
	for (int b = 0; b < 256; b++) {
		for (a = 0; a < rot; a++) {
			enteritos[a] = -1;
		}
		auxEnteritos = -1;
		mos = 0;
		buscado = b;
		recorrido(raiz, buscado, 0);

		for (a = 0; a < rot; a++) {
			rutas[a + 1][b] = enteritos[a];
		}
	}
	for (a = 0; a < 256; a++) {
		rutas[0][a] = a;
	}

	// las rutas ya estan en el arreglo bidimensional

	long tbits = 0;
	char *cadbits;
	cadbits = calloc(tam * rot, sizeof(char));

	for (i = 0; i < tam; i++) {
		for (a = 1; a < rot; a++) {
			if (rutas[a][wchar[i]] != -1) {
				if (rutas[a][wchar[i]] == 1) {
					cadbits[tbits] = 49;

				} else if (rutas[a][wchar[i]] == 0) {
					cadbits[tbits] = 48;
				}

				tbits++;

			}
		}
	}

	cadbits = (char*) realloc(cadbits, (tbits) * sizeof(char));

	printf("\n");

	char *textDeco;
	long informacionLength, sim;
	informacionLength = tbits;
	sim = informacionLength / 8 + 1;
	printf("tbits: %ld, sim: %ld\n", tbits, sim);
	textDeco = malloc(sim + 1);
	binarioToText(cadbits, informacionLength, sim, textDeco);

	uswtime(&utime1, &stime1, &wtime1);
	
	printf("\n"); 

	FILE *save_struct = fopen("codificacion.mugi", "wb");
	if (!save_struct) {
		printf("No he podido abrir el .mugi");
		return -1;
	}

	for (i = 0; i < sim; ++i) {
		fprintf(save_struct, "%c", textDeco[i]);
	}

	fclose(save_struct);

	save_struct = fopen("frecuencias.txt", "w");
	if(!save_struct) {
		printf("No he podido crear el archivo .txt\n");
		return -1;
	}

	fprintf(save_struct, "(%ld, %ld, %s)\n", sim, tbits, extension);
	for (a = 0; a < 256; ++a) {
		fprintf(save_struct, "%ld,", frecCopy[a]);
	}
	fclose(save_struct);

	free(textDeco);
	free(extension);
	free(arrNodos);
	free(wchar);
	free(cadbits);

	printf("Nombre del archivo original: %s\n", cadenita);
	printf("Tamaño del archivo original(en bytes): %ld\n", tam);
	printf("Nombre del archivo comprimido: codificacion.mugi\n");
	printf("Tamaño del archivo comprimido(en bytes): %ld\n", tbits/8);
	printf("Porcentaje de compresion: %ld\n", (tbits*100)/(8*tam));
	printf("Tiempo de ejecucion (Tiempo total)  %.10e s\n",  wtime1 - wtime0);

	return 0;
}

