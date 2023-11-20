// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>

#include "di-app-window.h"

struct _DiAppWindow {
	GtkApplicationWindow parent;

	GtkWidget *box;
};

G_DEFINE_TYPE (DiAppWindow, di_app_window, GTK_TYPE_APPLICATION_WINDOW)

static void di_app_window_init (DiAppWindow *win) {
	gtk_widget_init_template (GTK_WIDGET (win));
}

static void di_app_window_class_init (DiAppWindowClass *class) {
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);
	gtk_widget_class_set_template_from_resource (widget_class,
			"/com/github/AaronAyub/dropit/window.ui");
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, box);
}

DiAppWindow *di_app_window_new (GtkApplication *app) {
	return g_object_new (DI_APP_WINDOW_TYPE, "application", app, NULL);
}

void di_app_window_open (DiAppWindow *win, GFile **files, int n_files) {
	int validFiles = 0; // Number of files which exist

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
			gtk_box_append (GTK_BOX (win->box), label);

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
	gtk_box_prepend (GTK_BOX (win->box), allFilesLabel);

	// Initialize a drag source from the label containing the allFiles string.
	dsource = gtk_drag_source_new ();
	GdkFileList* fileList = gdk_file_list_new_from_array (files, n_files);
	contentProvider = gdk_content_provider_new_typed (GDK_TYPE_FILE_LIST, fileList);

	gtk_drag_source_set_content (dsource, contentProvider);
	g_object_unref (contentProvider);
	gtk_widget_add_controller (GTK_WIDGET (allFilesLabel), GTK_EVENT_CONTROLLER (dsource));
}
