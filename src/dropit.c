// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>

static void action_quit (GSimpleAction *action, GVariant *parameter, gpointer app) {
	g_application_quit (G_APPLICATION (app));
}

static GActionEntry action_entries[] = {
	{"quit", action_quit, NULL, NULL, NULL, NULL}
};

static void activate_cb (GApplication *app) {
	g_printerr ("Please provide one or more files to use dropit.\n");
}

static void open_cb (GApplication *app, GFile ** files, int n_files, char *hint) {
	int validFiles = 0; // Number of files which exist
	GtkBuilder *builder;
	GtkWidget *win;
	GtkWidget *box;

	// Read GTKWidgets from GtkBuilder ui file
	builder = gtk_builder_new_from_resource ("/com/github/AaronAyub/dropit/app.ui");
	win = GTK_WIDGET (gtk_builder_get_object (builder, "win"));
	gtk_window_set_application (GTK_WINDOW (win), GTK_APPLICATION (app));
	box = GTK_WIDGET (gtk_builder_get_object (builder, "box"));
	g_object_unref(builder);

	gtk_window_present (GTK_WINDOW (win));

	for (int i = 0; i < n_files; ++i) {
		if (g_file_query_exists (files[i], NULL)) {
			validFiles++;
			
			// Set up drag source for individual files.
			GtkWidget *label;
			GtkDragSource *dsource;
			GdkContentProvider *contentProvider;

			char * path = g_file_get_path (files[i]);
			label = gtk_label_new (path);
			free (path);
			gtk_box_append (GTK_BOX (box), label);

			dsource = gtk_drag_source_new ();
			GdkFileList* fileList = gdk_file_list_new_from_array (&files[i], 1);
			contentProvider = gdk_content_provider_new_typed (GDK_TYPE_FILE_LIST, fileList);

			gtk_drag_source_set_content (dsource, contentProvider);
			g_object_unref (contentProvider);
			gtk_widget_add_controller (GTK_WIDGET (label), GTK_EVENT_CONTROLLER (dsource));
		}
	}

	// Set up drag source for the all files label.
	GtkWidget *allFilesLabel;
	GtkDragSource *dsource;
	GdkContentProvider *contentProvider;

	char *allFilesString;
	asprintf (&allFilesString, "All files (%d)", validFiles);
	allFilesLabel = gtk_label_new (allFilesString);
	free (allFilesString);
	gtk_box_prepend (GTK_BOX (box), allFilesLabel);

	// Initialize a drag source from the label containing the allFiles string.
	dsource = gtk_drag_source_new ();
	GdkFileList* fileList = gdk_file_list_new_from_array (files, n_files);
	contentProvider = gdk_content_provider_new_typed (GDK_TYPE_FILE_LIST, fileList);

	gtk_drag_source_set_content (dsource, contentProvider);
	g_object_unref (contentProvider);
	gtk_widget_add_controller (GTK_WIDGET (allFilesLabel), GTK_EVENT_CONTROLLER (dsource));
}

int main(int argc, char *argv[]) {
	GtkApplication *app;
	int status;

	app = gtk_application_new ("com.github.AaronAyub.dropit", G_APPLICATION_HANDLES_OPEN);
	g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);
	g_signal_connect (app, "open", G_CALLBACK (open_cb), NULL);

	// Set up accelerators to quit the application
	g_action_map_add_action_entries (G_ACTION_MAP (app), action_entries, G_N_ELEMENTS (action_entries), app);
	const char *accels[3] = {"q", "<Control>q", NULL};
	gtk_application_set_accels_for_action (app, "app.quit", accels);

	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
