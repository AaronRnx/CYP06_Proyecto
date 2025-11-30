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
	// Definimos el alfabeto exacto solicitado (32 caracteres)
	const char ALFABETO[] = "abcdefghijklmnñopqrstuvwxyzáéíóú";

	// Calculamos la longitud automaticamente
	int lenAlfabeto = (int)strlen(ALFABETO);

	int totalSugeridas = 0;
	int longitud = (int)strlen(szPalabraLeida);
	char buffer[TAMTOKEN];

	if (longitud > 1) {

		// 1. Eliminación de un caracter
		for (int i = 0; i < longitud; i++) {
			strcpy_s(buffer, TAMTOKEN, szPalabraLeida);
			// Desplazar a la izquierda para borrar
			for (int j = i; j < longitud; j++) {
				buffer[j] = buffer[j + 1];
			}
			strcpy_s(szPalabrasSugeridas[totalSugeridas++], TAMTOKEN, buffer);
		}

		// 2. Transposición (intercambio) de caracteres adyacentes
		for (int i = 0; i < longitud - 1; i++) {
			strcpy_s(buffer, TAMTOKEN, szPalabraLeida);
			char temp = buffer[i];
			buffer[i] = buffer[i + 1];
			buffer[i + 1] = temp;
			strcpy_s(szPalabrasSugeridas[totalSugeridas++], TAMTOKEN, buffer);
		}
	}

	// 3. Sustitución (reemplazar cada letra por una del alfabeto)
	for (int i = 0; i < longitud; i++) {
		strcpy_s(buffer, TAMTOKEN, szPalabraLeida);
		for (int j = 0; j < lenAlfabeto; j++) {
			buffer[i] = ALFABETO[j];
			strcpy_s(szPalabrasSugeridas[totalSugeridas++], TAMTOKEN, buffer);
		}
	}

	// 4. Inserción (insertar cada letra del alfabeto en cada posicion)
	for (int i = 0; i <= longitud; i++) {
		for (int j = 0; j < lenAlfabeto; j++) {
			strcpy_s(buffer, TAMTOKEN, szPalabraLeida);
			int lenActual = (int)strlen(buffer);
			// Abrir espacio desplazando a la derecha
			for (int k = lenActual; k >= i; k--) {
				buffer[k + 1] = buffer[k];
			}
			buffer[i] = ALFABETO[j];
			strcpy_s(szPalabrasSugeridas[totalSugeridas++], TAMTOKEN, buffer);
		}
	}

	// 5. Incluir la palabra original
	strcpy_s(szPalabrasSugeridas[totalSugeridas++], TAMTOKEN, szPalabraLeida);

	// 6. Ordenar alfabéticamente todas las sugerencias
	iNumSugeridas = totalSugeridas;

	for (int i = 0; i < iNumSugeridas - 1; i++) {
		for (int j = i + 1; j < iNumSugeridas; j++) {
			if (strcmp(szPalabrasSugeridas[i], szPalabrasSugeridas[j]) > 0) {
				strcpy_s(buffer, TAMTOKEN, szPalabrasSugeridas[i]);
				strcpy_s(szPalabrasSugeridas[i], TAMTOKEN, szPalabrasSugeridas[j]);
				strcpy_s(szPalabrasSugeridas[j], TAMTOKEN, buffer);
			}
		}
	}
}
