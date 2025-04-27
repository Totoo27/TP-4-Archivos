// Ignacio Diaz Diaz

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const char *contador = "contador.txt";

typedef struct{

	char nombre[20];
	int puntaje;

} participantes;

// Prototipados

void ingresoDatos(int *contador);
void mostrarTOP(int cantParticipantes);
int cargar_contador(const char *nombreArchivo);
int guardar_contador(const char *nombreArchivo, int contador);

int main(){

	// Variables

	int cantParticipantes = cargar_contador(contador);
	bool terminado = false;
	int opcion;

	// Proceso

	printf("ARCHIVOS PUNTAJES:\n");

	do{

		printf("\nMENU:\n1. Ingresar Participante\n2. Ver TOP 10\n3. Buscar por nombre\n4. Salir\nOpcion: ");
		scanf("%d",&opcion);

		switch(opcion){
		case 1:
			ingresoDatos(&cantParticipantes);
			break;

		case 2:
			if(cantParticipantes > 0){
				mostrarTOP(cantParticipantes);
			} else{
				printf("\nNo hay participantes ingresados.\n");
			}
			break;

		case 3:
			if(cantParticipantes > 0){
				busqueda();
			} else{
				printf("\nNo hay participantes ingresados.\n");
			}
			break;

		case 4:
			terminado = true;
			printf("\nSistema finalizado.");
			break;

		default:
			printf("\nEsa no es una opcion valida.\n");
		}

	}while(terminado == false);

	return 0;
}

// Funciones

void busqueda(){

	participantes participante;
	char busqueda[20];
	int cant = 0;

	printf("Escriba el nombre que quiere buscar: ");
	scanf("%s", busqueda);


	printf("\nNOMBRE\tPUNTAJE\n");
	FILE *file = fopen("participantes.txt", "rb");
	while(fread(&participante, sizeof(participante), 1, file) == 1) {
		
		if(strcmp(busqueda, participante.nombre) == 0){

			printf("%s\t%d\n",participante.nombre, participante.puntaje);
			cant++;

		}

	}
	fclose(file);

	if(cant == 0){
		printf("No se ha encontrado ese nombre en los participantes.\n");
	}

}

void ingresoDatos(int *contador) {

	participantes participante;

	printf("Escriba el nombre de la participante: ");
	scanf("%s", participante.nombre);

	printf("Escriba el puntaje del participante: ");
	scanf("%d", &participante.puntaje);

	FILE *file = fopen("participantes.txt","ab");
	if(file != NULL) {
		fwrite(&participante, sizeof(participante), 1, file);
	} else {
		printf("ERROR: No se pudo cargar el archivo\n");
	}

	fclose(file);

	(*contador)++;
	guardar_contador("contador.txt", *contador);
}

void mostrarTOP(int cantParticipantes){

	int cant = 0;

	participantes participante;
	participantes temp;
	participantes part_top[cantParticipantes];

	FILE *file = fopen("participantes.txt", "rb");

	while(fread(&participante, sizeof(participante), 1, file) == 1) {
		part_top[cant] = participante;
		cant++;
	}

	fclose(file);

	for(int j = 0; j < cant - 1; j++){

		for(int i = 0; i < cant - j - 1; i++){

			if(part_top[i].puntaje < part_top[i+1].puntaje){

				temp.puntaje = part_top[i].puntaje;
				strcpy(temp.nombre, part_top[i].nombre);

				part_top[i].puntaje = part_top[i+1].puntaje;
				strcpy(part_top[i].nombre, part_top[i+1].nombre);

				part_top[i+1].puntaje = temp.puntaje;
				strcpy(part_top[i+1].nombre, temp.nombre);


			}

		}

	}

	printf("\nPUESTO\tNOMBRE\tPUNTAJE\n");

	int top = cant;
	if (top > 10) {
   		top = 10;
	}

	for(int i = 0; i < top; i++){

		printf("%d -\t%s\t%d\n", i+1, part_top[i].nombre, part_top[i].puntaje);

	}

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