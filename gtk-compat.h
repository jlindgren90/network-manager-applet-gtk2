#ifndef GTK_COMPAT_H
#define GTK_COMPAT_H

#define GtkGrid GtkTable
#define GTK_GRID GTK_TABLE
#define gtk_grid_set_row_spacing gtk_table_set_row_spacings
#define gtk_grid_set_column_spacing gtk_table_set_col_spacings

static inline GtkWidget *
gtk_box_new (GtkOrientation orientation,
             gint spacing)
{
    if (orientation == GTK_ORIENTATION_HORIZONTAL)
        return gtk_hbox_new (FALSE, spacing);
    else
        return gtk_vbox_new (FALSE, spacing);
}

static inline GtkWidget *
gtk_separator_new (GtkOrientation orientation)
{
    if (orientation == GTK_ORIENTATION_HORIZONTAL)
        return gtk_hseparator_new ();
    else
        return gtk_vseparator_new ();
}

static inline GtkWidget *
gtk_grid_new (void)
{
    return gtk_table_new (1, 1, FALSE);
}

static inline void
gtk_grid_attach (GtkGrid *grid,
                 GtkWidget *child,
                 gint left,
                 gint top,
                 gint width,
                 gint height)
{
    gtk_table_attach (grid, child, left, left + width, top, top + height,
                      GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
}

static inline guint
gtk_builder_add_from_resource (GtkBuilder   *builder,
                               const gchar  *resource_path,
                               GError      **error)
{
  GBytes *data;
  guint ret;

  data = g_resources_lookup_data (resource_path, 0, error);
  if (data == NULL)
    return 0;

  ret = gtk_builder_add_from_string (builder,
                                     g_bytes_get_data (data, NULL),
                                     g_bytes_get_size (data),
                                     error);

  g_bytes_unref (data);

  return ret;
}

static inline guint
gtk_builder_add_objects_from_resource (GtkBuilder   *builder,
                                       const gchar  *resource_path,
                                       gchar       **object_ids,
                                       GError      **error)
{
  GBytes *data;
  guint ret;

  data = g_resources_lookup_data (resource_path, 0, error);
  if (data == NULL)
    return 0;

  ret = gtk_builder_add_objects_from_string (builder,
                                             g_bytes_get_data (data, NULL),
                                             g_bytes_get_size (data),
                                             object_ids,
                                             error);

  g_bytes_unref (data);

  return ret;
}

#endif
