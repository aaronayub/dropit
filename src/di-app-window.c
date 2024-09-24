// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** App window object: The window and main user interface for the program */

#include <gtk/gtk.h>

#include "di-app-window.h"
#include "di-file-cell.h"

struct _DiAppWindow {
	GtkApplicationWindow parent;

	GtkWidget *box;
	GtkWidget *mainContainer;
	GtkWidget *allFilesContainer;
	GtkWidget *allFilesLabel;
	GtkWidget *allFilesImage;
  gboolean allowQuit;
  gboolean autoclose;
};

G_DEFINE_TYPE (DiAppWindow, di_app_window, GTK_TYPE_APPLICATION_WINDOW)

static void motion_enter (GtkEventControllerMotion *self, gdouble x, gdouble y, gpointer user_data) {
  DiAppWindow *win = DI_APP_WINDOW (user_data);
	win->allowQuit = FALSE;
}
static void motion_leave (GtkEventControllerMotion *self, gdouble x, gdouble y, gpointer user_data) {
  DiAppWindow *win = DI_APP_WINDOW (user_data);
	win->allowQuit = TRUE;
}

/** Exit the application if a drag event ends within the application window
 * and the user has the autoclose option set. */
void drag_end_cb (GtkDragSource *self, GdkDrag *drag, gboolean delete_data, gpointer user_data) {
  DiAppWindow *win = DI_APP_WINDOW (user_data);
	if (win->autoclose && win->allowQuit) {
    gtk_window_close (GTK_WINDOW (win));
	}
}

static void di_app_window_init (DiAppWindow *win) {
	gtk_widget_init_template (GTK_WIDGET (win));
}

static void di_app_window_class_init (DiAppWindowClass *class) {
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);
	gtk_widget_class_set_template_from_resource (widget_class,
			"/com/github/aaronayub/dropit/window.ui");
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, box);
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, mainContainer);
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, allFilesContainer);
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, allFilesLabel);
	gtk_widget_class_bind_template_child (widget_class, DiAppWindow, allFilesImage);
}

DiAppWindow *di_app_window_new (GtkApplication *app) {
	return g_object_new (DI_APP_WINDOW_TYPE, "application", app, NULL);
}

/** Set up the window by loading all valid files */
void di_app_window_open (DiAppWindow *win, gboolean autoclose, GFile **files, int n_files) {
	GtkIconTheme *iconTheme;
	GtkIconPaintable *iconPaintable;
	const char *iconName;
  win->autoclose = autoclose;

	int validFiles = 0; // Number of files which exist

	for (int i = 0; i < n_files; ++i) {
		if (g_file_query_exists (files[i], NULL)) {
			DiFileCell *fileCell;

			validFiles++;
			fileCell = di_file_cell_new ();
			di_file_cell_load (fileCell, files[i], win);
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

	// Set up icon for the drag source
	iconTheme = gtk_icon_theme_get_for_display (
		gtk_widget_get_display (GTK_WIDGET (win))
	);
	iconName = gtk_image_get_icon_name (GTK_IMAGE (win->allFilesImage));
	iconPaintable = gtk_icon_theme_lookup_icon (iconTheme, iconName, NULL,
		64, 1, GTK_TEXT_DIR_NONE, 0
	);

	// Initialize a drag source from the label containing the allFiles string.
	dsource = gtk_drag_source_new ();
	GdkFileList* fileList = gdk_file_list_new_from_array (files, n_files);
	contentProvider = gdk_content_provider_new_typed (GDK_TYPE_FILE_LIST, fileList);

	gtk_drag_source_set_content (dsource, contentProvider);
	gtk_drag_source_set_icon (dsource, GDK_PAINTABLE (iconPaintable), 0, 0);
	g_object_unref (contentProvider);
	g_signal_connect (dsource, "drag-end", G_CALLBACK (drag_end_cb), win);
	gtk_widget_add_controller (GTK_WIDGET (win->allFilesContainer), GTK_EVENT_CONTROLLER (dsource));

	// Set up event controller for the window
	GtkEventController *controller = gtk_drop_controller_motion_new ();
	g_signal_connect (controller, "enter", G_CALLBACK (motion_enter), win);
	g_signal_connect (controller, "leave", G_CALLBACK (motion_leave), win);
	gtk_widget_add_controller (GTK_WIDGET (win->mainContainer), GTK_EVENT_CONTROLLER (controller));
}

