// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** Dropit GtkApplication, managing the program's lifecycle */

#include <gtk/gtk.h>
#include "di-app.h"
#include "di-app-window.h"

/** Global Variables */
gboolean autoclose = FALSE; // If true, the application closes after drag-drop operations

struct _DiApp {
	GtkApplication parent;
};

G_DEFINE_TYPE (DiApp, di_app, GTK_TYPE_APPLICATION)

static void action_quit (GSimpleAction *action, GVariant *parameter, gpointer app) {
	g_application_quit (G_APPLICATION (app));
}

static GActionEntry action_entries[] = {
	{"quit", action_quit, NULL, NULL, NULL, NULL}
};

static gint di_app_handle_local_options (GApplication *app, GVariantDict *options) {
	if (g_variant_dict_contains (options, "autoclose")) {
		autoclose = TRUE;
	}

	// The return value of -1 indicates that the application keeps running
	return -1;
}

/** Provide users with an error message if the app is opened with no files */
static void di_app_activate (GApplication *app) {
	g_printerr ("Please provide one or more files to use dropit.\n");
}

/** Sets up the application interface with the provided files */
static void di_app_open (GApplication *app, GFile **files, int n_files, const char *hint) {
	DiAppWindow *win;
	GtkCssProvider *provider;
	GdkDisplay *display;
	
	// Set up accelerators to quit the application
	g_action_map_add_action_entries (G_ACTION_MAP (app), action_entries, G_N_ELEMENTS (action_entries), app);
	const char *accels[3] = {"q", "<Control>q", NULL};
	gtk_application_set_accels_for_action (app, "app.quit", accels);

	/** Set up styles */
	display = gdk_display_get_default ();
	provider = gtk_css_provider_new ();
	gtk_css_provider_load_from_resource(provider, "/com/github/aaronayub/dropit/style.css");
	gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider), 0);

	win = di_app_window_new (GTK_APPLICATION (app));
	di_app_window_open (win, files, n_files);
	gtk_window_present (GTK_WINDOW (win));
}

static void di_app_class_init (DiAppClass *class) {
	GApplicationClass *app_class = G_APPLICATION_CLASS (class);
	app_class->handle_local_options = di_app_handle_local_options;
	app_class->activate = di_app_activate;
	app_class->open = di_app_open;
}

static void di_app_init (DiApp *app) {
	// Set up command-line arguments
	const GOptionEntry options[] = {
		{"autoclose", 'a', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, NULL,
			"Automatically close the app after a drag-drop operation.", NULL},
		{NULL}
	};
	g_application_add_main_option_entries (G_APPLICATION (app), options);
}

DiApp *di_app_new (void) {
	return g_object_new (DI_APP_TYPE, "application-id", "com.github.aaronayub.dropit", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
