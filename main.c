/*
 * main.c
 *
 *  Created on: 17 déc. 2017
 *      Author: louis
 */

#include "function.h"


static char *fileName, *keyName;

static void handle_file(GtkFileChooser *widget) {
	GtkWidget *dialog;

    fileName = gtk_file_chooser_get_filename (widget);
    if(fileName == NULL) {
        exit(EXIT_FAILURE);
        return;
    }
}

static void handle_matrice(GtkFileChooser *widget){
	keyName = gtk_file_chooser_get_filename (widget);
	if(keyName == NULL){
		exit(EXIT_FAILURE);
	}
}

void launch_algo_cryptage(){
	FILE * openFile = NULL;
	FILE * file = NULL;
	int i;
	int size = 0;
	unsigned char buffer[100];
	char c1, c2;

	openFile = fopen(fileName, "r");
	if(openFile == NULL){
		printf("Erreur de l ouverture du fichier ! \n");
		exit(EXIT_FAILURE);
	}

	// Cryptage et envoie dans le fichier
	strcat(fileName, "c");
	file = createFile(fileName);

	// Get la key
	matrice(keyName);

	// Lecture du fichier par 100 caractères
	while((size = fread(buffer, 1, 100, openFile)) > 0){
		for(i = 0; i < size; i++){
			// Les 4 bits de poid fort
			c1 = chiffrer(buffer[i] >> 4);

			// Supression des bits de poid fort pour avoir bit de poid faible
			c2 = chiffrer(((char)(buffer[i] << 4)) >> 4);
			fputc(c1, file);
			fputc(c2, file);
		}
	}
	fclose(openFile);
	fclose(file);
	printf("Fin de cryptage !\n");
}

void launch_algo_decryptage(){
	FILE * openFile = NULL;
	FILE * file = NULL;
	unsigned char buffer[100];
	int size = 0;
	int identity[4] = {0};
	int trueCode;
	int i;
	char result = 0;

	openFile = fopen(fileName, "r");
	if(openFile == NULL){
		printf("Erreur de l ouverture du fichier ! \n");
		exit(EXIT_FAILURE);
	}

	// Cryptage et envoie dans le fichier
	strcat(fileName, "d");
	file = createFile(fileName);

	// Get la key
	matrice(keyName);

	// L'ordre de la matrice d'identité
	sortKey(identity);

	// Lecture du fichier par 100 caractères
	while((size = fread(buffer, 1, 100, openFile)) > 0){
		for(i = 0; i < size; i++){
			trueCode = getCodeDecryptage(buffer[i], identity);
			if(i % 2 == 0){
				// Position bit de point fort
				result = trueCode << 4;
			}else{
				// Position poid faible
				result += trueCode;
				fputc(result, file);
			}
		}
	}
	fclose(openFile);
	fclose(file);
	printf("Fin de decryptage ! \n");
}

static void activate (GtkApplication *app, gpointer user_data){
  GtkWidget *window;
  GtkWidget *button1, *button2, *button3, *button4;
  GtkWidget *grid;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Logiciel de cryptographie");
  gtk_container_set_border_width (GTK_CONTAINER (window), 60);

  grid = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (window), grid);

  button1 = gtk_file_chooser_button_new ("Sélectionner un fichier", GTK_FILE_CHOOSER_ACTION_OPEN);
  g_signal_connect (button1, "file-set", G_CALLBACK (handle_file), NULL);
  gtk_grid_attach (GTK_GRID (grid), button1, 0, 0, 2, 2);


  button2 = gtk_file_chooser_button_new ("Sélectionner une matrice", GTK_FILE_CHOOSER_ACTION_OPEN);
  g_signal_connect (button2, "file-set", G_CALLBACK (handle_matrice), NULL);
  gtk_grid_attach (GTK_GRID (grid), button2, 2, 0, 2, 2);

  button3 = gtk_button_new_with_label ("Cryptage");
  g_signal_connect (button3, "clicked", G_CALLBACK (launch_algo_cryptage), NULL);
  gtk_grid_attach (GTK_GRID (grid), button3, 0, 2, 2, 2);

  button4 = gtk_button_new_with_label ("Decryptage");
  g_signal_connect (button4, "clicked", G_CALLBACK (launch_algo_decryptage), NULL);
  gtk_grid_attach (GTK_GRID (grid), button4, 2, 2,2, 2);

  gtk_widget_show_all (window);
}

int main (int argc, char **argv){
  GtkApplication *app;
  int status;

  app = gtk_application_new ("fr.gtk.TrySecurity", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
