/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES ARAGON

	Computadoras y programacion. 
	(c) Ponga su nombre y numero de cuenta aqui.
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include "corrector.h"
#include <ctype.h>
//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void	Diccionario			(char *szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int &iNumElementos)
{

	FILE* fp;
	char c;
	char tmp[TAMTOKEN];
	int idx = 0;
	int n = 0;

	int err;

	err = fopen_s(&fp, szNombre, "r");

	if (err == 0 && fp != NULL) {
		while (fscanf_s(fp, "%c", &c, 1) == 1) {

			int esSeparador = 0;

			// Verificamos manualmente los separadores
			if (c == ' ' || c == ',' || c == '\n' || c == '\t' ||
				c == '\r' || c == '.' || c == '(' || c == ')' || c == ';') {
				esSeparador = 1;
			}

			if (esSeparador == 0) {
				if (idx < TAMTOKEN - 1) {
					tmp[idx++] = tolower(c);
				}
			}

			// Si es separador o se llenó el buffer
			if ((esSeparador == 1 || idx >= TAMTOKEN - 1) && idx > 0) {
				tmp[idx] = '\0';

				int existe = -1;
				// Busqueda lineal
				for (int k = 0; k < n; k++) {
					if (strcmp(szPalabras[k], tmp) == 0) {
						existe = k;
						break;
					}
				}

				if (existe == -1) {
					if (n < NUMPALABRAS) {
						strcpy_s(szPalabras[n], TAMTOKEN, tmp);
						iEstadisticas[n] = 1;
						n++;
					}
				}
				else {
					iEstadisticas[existe]++;
				}
				idx = 0; // Reiniciar palabra
			}
		}
		fclose(fp);
	}

	iNumElementos = n;

	// Ordenar alfabeticamente (Burbuja simple)
	for (int i = 0; i < iNumElementos - 1; i++) {
		for (int j = i + 1; j < iNumElementos; j++) {
			if (strcmp(szPalabras[i], szPalabras[j]) > 0) {
				char buffer[TAMTOKEN];
				int aux;

				// Intercambiar Texto
				strcpy_s(buffer, TAMTOKEN, szPalabras[i]);
				strcpy_s(szPalabras[i], TAMTOKEN, szPalabras[j]);
				strcpy_s(szPalabras[j], TAMTOKEN, buffer);

				// Intercambiar Numero
				aux = iEstadisticas[i];
				iEstadisticas[i] = iEstadisticas[j];
				iEstadisticas[j] = aux;
			}
		}
	}
}

/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas		(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
{

	iNumLista = 0;
	int count = 0;

	// 1. Cruzar las listas (Filtro)
	for (int i = 0; i < iNumElementos; i++) {
		for (int j = 0; j < iNumSugeridas; j++) {
			if (strcmp(szPalabras[i], szPalabrasSugeridas[j]) == 0) {
				if (count < NUMPALABRAS) {
					iPeso[count] = iEstadisticas[i];
					strcpy_s(szListaFinal[count], TAMTOKEN, szPalabrasSugeridas[j]);
					count++;
				}
			}
		}
	}

	// 2. Eliminar repetidas
	int unicos = 0;
	for (int i = 0; i < count; i++) {
		int yaExiste = 0; 

		for (int j = 0; j < unicos; j++) {
			if (strcmp(szListaFinal[j], szListaFinal[i]) == 0) {
				yaExiste = 1; // Verdadero
				break;
			}
		}

		if (yaExiste == 0) {
			strcpy_s(szListaFinal[unicos], TAMTOKEN, szListaFinal[i]);
			iPeso[unicos] = iPeso[i];
			unicos++;
		}
	}

	iNumLista = unicos;

	// 3. Ordenar por peso (Mayor a Menor)
	for (int i = 0; i < iNumLista - 1; i++) {
		for (int j = 0; j < iNumLista - 1 - i; j++) {
			if (iPeso[j] < iPeso[j + 1]) {
				int auxPeso;
				char waux[TAMTOKEN];

				// Swap peso
				auxPeso = iPeso[j];
				iPeso[j] = iPeso[j + 1];
				iPeso[j + 1] = auxPeso;

				// Swap palabra
				strcpy_s(waux, TAMTOKEN, szListaFinal[j]);
				strcpy_s(szListaFinal[j], TAMTOKEN, szListaFinal[j + 1]);
				strcpy_s(szListaFinal[j + 1], TAMTOKEN, waux);
			}
		}
	}
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
{

}
