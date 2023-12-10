// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <gtk/gtk.h>

#define DI_APP_TYPE (di_app_get_type ())
G_DECLARE_FINAL_TYPE (DiApp, di_app, DI, APP, GtkApplication)

DiApp *di_app_new (void);
