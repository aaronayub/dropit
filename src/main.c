// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>
#include "di-app.h"

int main (int argc, char *argv[]) {
	return g_application_run (G_APPLICATION (di_app_new ()), argc, argv);
}
