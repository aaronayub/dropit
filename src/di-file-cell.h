// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** File cell object: Represents one file, displaying information such as its
 * thumbnail or name. */

#pragma once

#include <gtk/gtk.h>
#include "di-app-window.h"

#define DI_FILE_CELL_TYPE (di_file_cell_get_type ())
G_DECLARE_FINAL_TYPE (DiFileCell, di_file_cell, DI, FILE_CELL, GtkBox)

DiFileCell *di_file_cell_new (void);

/** Set up the file cell by loading information about the file it represents. */
void di_file_cell_load (DiFileCell *cell, GFile *file, DiAppWindow *win);
