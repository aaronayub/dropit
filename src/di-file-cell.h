// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <gtk/gtk.h>

#define DI_FILE_CELL_TYPE (di_file_cell_get_type ())
G_DECLARE_FINAL_TYPE (DiFileCell, di_file_cell, DI, FILE_CELL, GtkBox)

DiFileCell *di_file_cell_new (void);
void di_file_cell_load (DiFileCell *cell, GFile *file);
