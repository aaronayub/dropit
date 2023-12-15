// Copyright (c) 2023 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>

#include "di-file-cell.h"
#include "di-app-window.h"

static char *getReadableSize (long sizeLong); 

struct _DiFileCell {
	GtkBox parent;

	GtkWidget *name;
	GtkWidget *size;
	GtkImage *image;
};

G_DEFINE_TYPE (DiFileCell, di_file_cell, GTK_TYPE_BOX)

static void di_file_cell_init (DiFileCell *cell) {
	gtk_widget_init_template (GTK_WIDGET (cell));
}

static void di_file_cell_class_init (DiFileCellClass *class) {
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);
	gtk_widget_class_set_template_from_resource (widget_class,
			"/com/github/aaronayub/dropit/file-cell.ui");
	gtk_widget_class_bind_template_child (widget_class, DiFileCell, name);
	gtk_widget_class_bind_template_child (widget_class, DiFileCell, size);
	gtk_widget_class_bind_template_child (widget_class, DiFileCell, image);
}

DiFileCell *di_file_cell_new (void) {
	return g_object_new (DI_FILE_CELL_TYPE, NULL);
}

void di_file_cell_load (DiFileCell *cell, GFile *file) {
	GFileInfo *fileInfo;
	GdkTexture *texture;
	GError *err = NULL;
	GtkIconTheme *iconTheme;
	GtkIconPaintable *iconPaintable;

	iconTheme = gtk_icon_theme_get_for_display (
		gtk_widget_get_display (GTK_WIDGET (cell))
	);

	// Set up image and labels
	fileInfo = g_file_query_info (file, "standard::name,standard::size,standard::icon", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, NULL);
	const char *name = g_file_info_get_name (fileInfo);
	goffset size = g_file_info_get_size (fileInfo);

	/** If the file is an image, display a thumbnail of the image in the file cell
	 * and drag-icon, otherwise use a GIcon from the filetype */
	texture = gdk_texture_new_from_file (file, &err);
	if (!err) {
		gtk_image_set_from_paintable (cell->image, GDK_PAINTABLE (texture));
		iconPaintable = gtk_icon_paintable_new_for_file (file, 64, 1);
	} else {
		GIcon *icon = g_file_info_get_icon (fileInfo);
		gtk_image_set_from_gicon (cell->image, icon);
		iconPaintable = gtk_icon_theme_lookup_by_gicon (iconTheme, icon,
			64, 1, GTK_TEXT_DIR_NONE, 0
		);
	}

	char *sizeText = getReadableSize(size);

	gtk_label_set_label (GTK_LABEL (cell->name), name);
	gtk_label_set_label (GTK_LABEL (cell->size), sizeText);

	free (sizeText);

	// Set the drag source
	GtkDragSource *dsource;
	GdkContentProvider *contentProvider;

	dsource = gtk_drag_source_new ();
	GdkFileList* fileList = gdk_file_list_new_from_array (&file, 1);
	contentProvider = gdk_content_provider_new_typed (GDK_TYPE_FILE_LIST, fileList);

	gtk_drag_source_set_content (dsource, contentProvider);
	gtk_drag_source_set_icon (dsource, GDK_PAINTABLE (iconPaintable), 0, 0);
	g_object_unref (contentProvider);
	g_signal_connect (dsource, "drag-end", G_CALLBACK (drag_end_cb), NULL);
	gtk_widget_add_controller (GTK_WIDGET (cell), GTK_EVENT_CONTROLLER (dsource));
}

/** Create a human-readable string from a provided long representing
 * the size of a file.*/
static char *getReadableSize (long sizeLong) {
	char *suffixes[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};
	int suffixIndex = 0; // Indicates which suffixes a file should be represented by
	char *output;
	double size = (double) sizeLong;

	// Move to a larger suffix if one is available.
	while (size > 1024) {
		size/=1024;
		suffixIndex++;
	}

	/* Represent the output string as a decimal if the file is measured
	 * in KiB or greater. */
	if (suffixIndex == 0) {
		asprintf (&output, "%ld B", sizeLong);
	} else {
		asprintf (&output, "%.3lf %s", size, suffixes[suffixIndex]);
	}

	return output;
}
