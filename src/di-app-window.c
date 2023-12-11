// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>

#include "di-app-window.h"
#include "di-file-cell.h"

struct _DiAppWindow {
	GtkApplicationWindow parent;

	GtkWidget *box;
	GtkWidget *mainContainer;
	GtkWidget *allFilesContainer;
	GtkWidget *allFilesLabel;
};

/* If allowQuit and autoclose are true, drag/drop events outside the application
 * will exit the application. */
extern gboolean autoclose;
gboolean allowQuit = false;

G_DEFINE_TYPE (DiAppWindow, di_app_window, GTK_TYPE_APPLICATION_WINDOW)


static void motion_enter (void) {
	allowQuit = false;
}
static void motion_leave (void) {
	allowQuit = true;
}

/** Exit the application if a drag event ends within the application window. */
void drag_end_cb (void) {
	if (autoclose && allowQuit) {
		exit (0);
	}
}

static void di_app_window_init (DiAppWindow *win) {
	gtk_widget_init_template (GTK_WIDGET (win));

	gtk_widget_add_css_class (GTK_WIDGET (win->allFilesContainer), "drag");
}

static void di_app_window_class_init (DiAppWindowClass *class) {
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);
	gtk_widget_class_set_template_from_resource (widget_class,
			"/com/github/aaronayub/dropit/window.ui");
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, box);
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, mainContainer);
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, allFilesContainer);
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, allFilesLabel);
}

DiAppWindow *di_app_window_new (GtkApplication *app) {
	return g_object_new (DI_APP_WINDOW_TYPE, "application", app, NULL);
}

void di_app_window_open (DiAppWindow *win, GFile **files, int n_files) {
	int validFiles = 0; // Number of files which exist

	for (int i = 0; i < n_files; ++i) {
		if (g_file_query_exists (files[i], NULL)) {
			DiFileCell *fileCell;

			validFiles++;
			fileCell = di_file_cell_new ();
			di_file_cell_load (fileCell, files[i]);
			gtk_flow_box_append (GTK_FLOW_BOX (win->box), GTK_WIDGET (fileCell));
		}
	}

	// Set up drag source for the all files label.
	GtkDragSource *dsource;
	GdkContentProvider *contentProvider;

	char *allFilesString;
	asprintf (&allFilesString, "Drag all files\nTotal files: %d", validFiles);
	gtk_label_set_text (GTK_LABEL (win->allFilesLabel), allFilesString);
	free (allFilesString);

	// Initialize a drag source from the label containing the allFiles string.
	dsource = gtk_drag_source_new ();
	GdkFileList* fileList = gdk_file_list_new_from_array (files, n_files);
	contentProvider = gdk_content_provider_new_typed (GDK_TYPE_FILE_LIST, fileList);

	gtk_drag_source_set_content (dsource, contentProvider);
	g_object_unref (contentProvider);
	g_signal_connect (dsource, "drag-end", G_CALLBACK (drag_end_cb), NULL);
	gtk_widget_add_controller (GTK_WIDGET (win->allFilesContainer), GTK_EVENT_CONTROLLER (dsource));

	// Set up event controller for the window
	GtkEventController *controller = gtk_drop_controller_motion_new ();
	g_signal_connect (controller, "enter", G_CALLBACK (motion_enter), NULL);
	g_signal_connect (controller, "leave", G_CALLBACK (motion_leave), NULL);
	gtk_widget_add_controller (GTK_WIDGET (win->mainContainer), GTK_EVENT_CONTROLLER (controller));
}

