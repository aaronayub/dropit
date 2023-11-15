// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>

static void activate_cb (GApplication *app) {
	g_printerr ("Please provide one or more files to use gdrag.\n");
}

static void open_cb (GApplication *app, GFile ** files, int n_files, char *hint) {
	GString *allFiles = g_string_new (""); // String containing the paths of all existing files

	// Build the allFiles string
	for (int i = 0; i < n_files; ++i) {
		if (!g_file_query_exists (files[i], NULL)) {
			continue;
		}
		char * filePath = g_file_get_path (files[i]);
		g_string_append_printf (allFiles, "%s ", filePath);
		free(filePath);
	}

	g_print ("%s\n", allFiles->str);

	GtkWidget *win;
	GtkWidget *box;
	GtkWidget *lbl;

	win = gtk_application_window_new (GTK_APPLICATION (app));
	gtk_window_set_title (GTK_WINDOW (win), "gdrag");
	gtk_window_set_default_size (GTK_WINDOW (win), 500,300);

	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
	gtk_window_set_child (GTK_WINDOW (win), box);
	gtk_window_present (GTK_WINDOW (win));

	lbl = gtk_label_new (allFiles->str);
	g_free (allFiles);
	gtk_box_append (GTK_BOX (box), lbl);
}

int main(int argc, char *argv[]) {
	GtkApplication *app;
	int status;

	app = gtk_application_new ("com.github.AaronAyub.gdrag", G_APPLICATION_HANDLES_OPEN);
	g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);
	g_signal_connect (app, "open", G_CALLBACK (open_cb), NULL);

	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
