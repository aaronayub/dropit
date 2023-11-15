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
	g_free (allFiles);
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
