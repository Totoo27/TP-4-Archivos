#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const char *contador = "contador.txt";

typedef struct {
	int DNI;
	char nombre[20];
	char apellido[20];
} Personas;

void ingresoDatos(Personas personas, int *contador);
void busquedaDatos(Personas personas, int cantCuentas);
void intercambioPosicion(Personas *bajada, Personas *subida);
int cargar_contador(const char *nombreArchivo);
int guardar_contador(const char *nombreArchivo, int contador);

int main() {

	// Variables

	int opcion;
	int cantCuentas = cargar_contador(contador);
	bool terminado = false;

	// Struct
	Personas personas;

	// Proceso

	printf("SISTEMA DE ARCHIVOS\n");

	do {
		printf("\nMENU:\n\n1. Ingresar Datos\n2. Busqueda\n3. Salir.\nOpcion: ");
		scanf("%d", &opcion);

		switch(opcion) {
		case 1:
			ingresoDatos(personas, &cantCuentas);
			break;

		case 2:
			if(cantCuentas > 0) {
				busquedaDatos(personas, cantCuentas);
			} else {
				printf("\nTodavia no hay personas ingresadas.\n");
			}

			break;

		case 3:
			terminado = true;
			printf("Sistema cerrado.");
			break;

		default:
			printf("Esa no es una opcion valida.");
		}

	} while(terminado == false);

	return 0;
}

void ingresoDatos(Personas personas, int *contador) {

	int DNI;
	bool repetir = true;

	do {
		printf("Escriba el DNI del registro %d: ", (*contador) + 1);
		scanf("%d", &DNI);

		FILE *file = fopen("personas.txt", "rb");

		if(file != NULL) {
			bool encontrado = false;
			while (fread(&personas, sizeof(personas), 1, file) == 1) {
				if(DNI == personas.DNI) {
					printf("El ingresado DNI ya existe!\n");
					encontrado = true;
					break;
				}
			}
			fclose(file);

			if (!encontrado) {
				repetir = false;
			}
		} else {
			// Si no existe el archivo, asumimos que no hay repeticiones
			repetir = false;
		}

	} while(repetir == true);

	printf("Escriba el nombre de la persona: ");
	scanf("%s", personas.nombre);

	printf("Escriba el apellido de la persona: ");
	scanf("%s", personas.apellido);

	personas.DNI = DNI;

	FILE *file = fopen("personas.txt","ab");
	if(file != NULL) {
		fwrite(&personas, sizeof(personas), 1, file);
	} else {
		printf("ERROR: No se pudo cargar el archivo\n");
	}

	fclose(file);

	(*contador)++;
	guardar_contador("contador.txt", *contador);
}

void busquedaDatos(Personas personas, int cantCuentas) {

	int opcion;
	int DNI;
	bool terminado = false;
	FILE *file;

	int subopcion;
	bool subterminado = false;
	Personas orden_personas[cantCuentas];
	Personas auxiliar;

	do {
		printf("\nBUSQUEDA:\n\n");
		printf("1. Mostrar todos\n2. Buscar por DNI\n3. Buscar por nombre\n4. Atras\nOpcion: ");
		scanf("%d",&opcion);

		switch(opcion) {

		case 1:
			
			int cant = 0;

			file = fopen("personas.txt", "rb");
			while(fread(&personas, sizeof(personas), 1, file) == 1) {
				orden_personas[cant].DNI = personas.DNI;
				strcpy(orden_personas[cant].nombre, personas.nombre);
				strcpy(orden_personas[cant].apellido, personas.apellido);
				cant++;
			}
			fclose(file);

			do {

				printf("\nORDEN:\n\n");
				printf("1. Ordenar por DNI mayor a menor\n2. Ordenar por Apellido y Nombre A-Z\n3. Atras\nOpcion: ");
				scanf("%d", &subopcion);

				switch(subopcion) {

				case 1:

					for(int j = 0; j<cantCuentas - 1; j++){

						for(int i = 0; i<cantCuentas - 1 - j; i++){

							if(orden_personas[i].DNI < orden_personas[i+1].DNI){

								intercambioPosicion(&orden_personas[i], &orden_personas[i+1]);

							}

						}

					}


					printf("\nDNI\tNOMBRE\tAPELLIDO\n");
					for(int i = 0; i<cantCuentas; i++){
						printf("%d\t%s\t%s\n", orden_personas[i].DNI, orden_personas[i].nombre, orden_personas[i].apellido);
					}

					subterminado = true;
					break;

				case 2:

					for(int j = 0; j<cant - 1; j++){

						for(int i = 0; i<cant - 1 - j; i++){

							if(strcmp(orden_personas[i].apellido, orden_personas[i+1].apellido) > 0){

								intercambioPosicion(&orden_personas[i], &orden_personas[i+1]);

							} else if(strcmp(orden_personas[i].apellido, orden_personas[i+1].apellido) == 0){

								if(strcmp(orden_personas[i].nombre, orden_personas[i+1].nombre) > 0){

									intercambioPosicion(&orden_personas[i], &orden_personas[i+1]);

								}

							}

						}

					}

					printf("\nDNI\tNOMBRE\tAPELLIDO\n");
					for(int i = 0; i<cant; i++){
						printf("%d\t%s\t%s\n", orden_personas[i].DNI, orden_personas[i].nombre, orden_personas[i].apellido);
					}

					subterminado = true;
					break;

				case 3:
					subterminado = true;
					break;

				default:
					printf("Esa no es una opcion valida.");
				}

			} while(subterminado == false);



			terminado = true;
			break;

		case 2:
			

			printf("Escriba el DNI a buscar: ");
			scanf("%d", &DNI);

			file = fopen("personas.txt", "rb");

			bool encontrado = false;
			while (fread(&personas, sizeof(personas), 1, file) == 1) {
				if(DNI == personas.DNI) {

					printf("\nDNI\tNombre\tApellido\n");

					printf("%d\t", personas.DNI);
					printf("%s\t", personas.nombre);
					printf("%s\n", personas.apellido);

					encontrado = true;
					break;
				}
			}
			fclose(file);

			if (!encontrado) {
				printf("\nEse DNI no esta ingresado al sistema.\n");
			}

			terminado = true;
			break;

		case 3:
			char nombreBusqueda[20];
			char apellidoBusqueda[20];

			printf("Escriba el nombre a buscar: ");
			scanf("%s", nombreBusqueda);

			printf("Escriba el apellido a buscar: ");
			scanf("%s", apellidoBusqueda);

			file = fopen("personas.txt", "rb");
			encontrado = false;

			if (file != NULL) {
				printf("\nDNI\tNombre\tApellido\n");
				while (fread(&personas, sizeof(personas), 1, file) == 1) {
					if (strcmp(personas.nombre, nombreBusqueda) == 0 && strcmp(personas.apellido, apellidoBusqueda) == 0) {
						
						printf("%d\t%s\t%s\n", personas.DNI, personas.nombre, personas.apellido);
						encontrado = true;
					}
				}
				fclose(file);
			}

			if (!encontrado) {
				printf("\nNo se encontro a ninguna persona con ese nombre y apellido.\n");
			}

			terminado = true;
			break;


		case 4:
			terminado = true;
			break;

		default:
			printf("Esa no es una opcion valida.");
		}
	} while(terminado == false);


}

int cargar_contador(const char *nombreArchivo) {
	FILE *file = fopen(nombreArchivo, "r");
	int contador = 0;
	if (file != NULL) {
		fscanf(file, "%d", &contador);
		fclose(file);
	} else {
		printf("No se pudo abrir el archivo %s. Se iniciar! el contador en 0.\n", nombreArchivo);
	}
	return contador;
}

int guardar_contador(const char *nombreArchivo, int contador) {
	FILE *file = fopen(nombreArchivo, "w");
	if(file!=NULL) {
		fprintf(file, "%d", contador);
		fclose(file);
	} else {
		printf("\nNo se encontro el archivo %s.\n", nombreArchivo);
	}
}

void intercambioPosicion(Personas *bajada, Personas *subida){

    Personas auxiliar;

    auxiliar.DNI = bajada->DNI;
    strcpy(auxiliar.nombre, bajada->nombre);
    strcpy(auxiliar.apellido, bajada->apellido);

    bajada->DNI = subida->DNI;
    strcpy(bajada->nombre, subida->nombre);
    strcpy(bajada->apellido, subida->apellido);

    subida->DNI = auxiliar.DNI;
    strcpy(subida->nombre, auxiliar.nombre);
    strcpy(subida->apellido, auxiliar.apellido);
}
