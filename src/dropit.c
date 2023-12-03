// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>

#include "di-app-window.h"

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
	DiAppWindow *win;
	GtkCssProvider *provider;
	GdkDisplay *display;

	/** Set up styles */
	display = gdk_display_get_default ();
	provider = gtk_css_provider_new ();
	gtk_css_provider_load_from_resource(provider, "/com/github/aaronayub/dropit/style.css");
	gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider), 0);

	win = di_app_window_new (GTK_APPLICATION (app));
	di_app_window_open (win, files, n_files);
	gtk_window_present (GTK_WINDOW (win));
}

int main(int argc, char *argv[]) {
	GtkApplication *app;
	int status;

	app = gtk_application_new ("com.github.aaronayub.dropit", G_APPLICATION_HANDLES_OPEN);

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
