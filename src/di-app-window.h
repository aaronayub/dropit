// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** App window object: The window and main user interface for the program */

#pragma once

#include <gtk/gtk.h>

#define DI_APP_WINDOW_TYPE (di_app_window_get_type ())
G_DECLARE_FINAL_TYPE (DiAppWindow, di_app_window, DI, APP_WINDOW, GtkApplicationWindow)

DiAppWindow *di_app_window_new (GtkApplication *app);

/** Set up the window by loading all valid files */
void di_app_window_open (DiAppWindow *win, gboolean autoclose, GFile **files, int n_files);

/** Exit the application if a drag event ends within the application window. */
void drag_end_cb (GtkDragSource *self, GdkDrag *drag, gboolean delete_data, gpointer user_data);
