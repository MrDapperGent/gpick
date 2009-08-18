/*
 * Copyright (c) 2009, Albertas Vyšniauskas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of the software author nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "uiListPalette.h"
#include "uiUtilities.h"
#include "ColorNames.h"
#include "gtk/uiSwatch.h"

#include <sstream>
#include <iomanip>

using namespace std;

static void palette_list_cell_edited(GtkCellRendererText *cell, gchar *path, gchar *new_text, gpointer user_data) {
	GtkTreeIter iter;
	GtkTreeModel *model=GTK_TREE_MODEL(user_data);

	gtk_tree_model_get_iter_from_string(model, &iter, path );

	gtk_list_store_set(GTK_LIST_STORE(model), &iter,
		2, new_text,
	-1);
	
	struct ColorObject *color_object;
	gtk_tree_model_get(model, &iter, 0, &color_object, -1);
	dynv_system_set(color_object->params, "string", "name", (void*)new_text);
}

static void palette_list_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data) {
	GtkTreeModel* model;
	GtkTreeIter iter;

	model=gtk_tree_view_get_model(tree_view);
	gtk_tree_model_get_iter(model, &iter, path);

	struct ColorObject *color_object;
	gtk_tree_model_get(model, &iter, 0, &color_object, -1);
	Color c;
	color_object_get_color(color_object, &c);
	gtk_swatch_set_active_color(GTK_SWATCH(user_data), &c);


}

GtkWidget* palette_list_new(GtkWidget* swatch) {

	GtkListStore  		*store;
	GtkCellRenderer     *renderer;
	GtkTreeViewColumn   *col;
	GtkWidget           *view;

	view = gtk_tree_view_new ();



	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view),1);

	store = gtk_list_store_new (3, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING);

	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_sizing(col,GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_column_set_resizable(col,1);
	gtk_tree_view_column_set_title(col, "Color");
	renderer = custom_cell_renderer_color_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "color", 0);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_sizing(col,GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_column_set_resizable(col,1);
	gtk_tree_view_column_set_title(col, "Color");
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", 1);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);


	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_sizing(col,GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_column_set_resizable(col,1);
	gtk_tree_view_column_set_title(col, "Name");
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", 2);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect(renderer, "edited", (GCallback) palette_list_cell_edited, store);

	gtk_tree_view_set_model (GTK_TREE_VIEW (view), GTK_TREE_MODEL(store));
	g_object_unref (GTK_TREE_MODEL(store));

	GtkTreeSelection *selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW(view) );

	gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);

	g_signal_connect (G_OBJECT (view), "row-activated", G_CALLBACK(palette_list_row_activated) , swatch);
	
	gtk_tree_view_set_reorderable(GTK_TREE_VIEW (view), TRUE);

	return view;
}

void palette_list_remove_all_entries(GtkWidget* widget) {
	GtkTreeIter iter;
	GtkListStore *store;
	gboolean valid;

	store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));
	valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);

	while (valid){
		struct ColorObject* c;
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &c, -1);
		color_object_release(c);
		valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
	}

	gtk_list_store_clear(GTK_LIST_STORE(store));
}

gint32 palette_list_get_selected_count(GtkWidget* widget) {
	return gtk_tree_selection_count_selected_rows(gtk_tree_view_get_selection(GTK_TREE_VIEW(widget)));
}

gint32 palette_list_get_count(GtkWidget* widget){
	GtkTreeModel *store;
	store=gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
	return gtk_tree_model_iter_n_children(store, NULL);
}

gint32 palette_list_get_selected_color(GtkWidget* widget, Color* color) {
	GtkTreeSelection *selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW(widget) );
	GtkListStore *store;
	GtkTreeIter iter;

	if (gtk_tree_selection_count_selected_rows(selection)!=1){
		return -1;
	}

	store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));

	GList *list = gtk_tree_selection_get_selected_rows ( selection, 0 );
	GList *i=list;

	struct ColorObject* c;
	while (i) {
		gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, (GtkTreePath*)i->data);

		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &c, -1);
		color_object_get_color(c, color);
		break;

		i = g_list_next(i);
	}

	g_list_foreach (list, (GFunc)gtk_tree_path_free, NULL);
	g_list_free (list);

	return 0;
}

void palette_list_remove_selected_entries(GtkWidget* widget) {
	
	GtkTreeIter iter;
	GtkListStore *store;
	gboolean valid;

	store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);

	struct ColorObject* color_object;

    while (valid){
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &color_object, -1);
		if (color_object->selected){
			valid = gtk_list_store_remove(GTK_LIST_STORE(store), &iter);
			color_object_release(color_object);
		}else{
			valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
		}
    }
	
/*
	GtkTreeSelection *selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW(widget) );
	GtkListStore *store;
	GtkTreeIter iter;

	if (gtk_tree_selection_count_selected_rows(selection) == 0){
		return;
	}

	store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));

	GList *list = gtk_tree_selection_get_selected_rows ( selection, 0 );
	GList *ref_list = NULL;

	GList *i = list;
	while (i) {
		ref_list = g_list_prepend(ref_list, gtk_tree_row_reference_new(GTK_TREE_MODEL(store), (GtkTreePath*) (i->data)));
		i = g_list_next(i);
	}

	i = ref_list;
	GtkTreePath *path;
	while (i) {
		path = gtk_tree_row_reference_get_path((GtkTreeRowReference*) i->data);
		if (path) {
			gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);
			gtk_tree_path_free(path);

			struct ColorObject* c;
			gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &c, -1);
			color_object_release(c);

			gtk_list_store_remove(GTK_LIST_STORE(store), &iter);
		}
		i = g_list_next(i);
	}
	g_list_foreach (ref_list, (GFunc)gtk_tree_row_reference_free, NULL);
	g_list_free (ref_list);

	g_list_foreach (list, (GFunc)gtk_tree_path_free, NULL);
	g_list_free (list);
*/
}


void palette_list_add_entry(GtkWidget* widget, struct ColorObject* color_object){
	GtkTreeIter iter1;
	GtkListStore *store;

	store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));

	//GdkPixbuf* pixbuf = palette_list_pixbuf_from_color(color, 24,16);

	Color color;
	color_object_get_color(color_object, &color);

	stringstream s;
	s<<int(color.rgb.red * 255)<<", "<<int(color.rgb.green * 255)<<", "<<int(color.rgb.blue * 255);

	gchar* hex_text = g_strdup(s.str().c_str());//g_strdup_printf("%d %d %d", int(color->rgb.red*255), int(color->rgb.green*255), int(color->rgb.blue*255));

	//Color* c=color_new();
	//color_copy(color, c );
	
	const char* name=(const char*)dynv_system_get(color_object->params, "string", "name");

	gtk_list_store_append(store, &iter1);
	gtk_list_store_set(store, &iter1,
		0, color_object_ref(color_object),
		1, hex_text,
		2, name,
	-1);

	g_free(hex_text);
	//g_object_unref (pixbuf);
}


gint32 palette_list_foreach(GtkWidget* widget, gint32 (*callback)(struct ColorObject* color_object, void *userdata), void *userdata){
	GtkTreeIter iter;
	GtkListStore *store;
	gboolean valid;

	store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);

	struct ColorObject* color_object;
	//gchar* color_name;

    while (valid){
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &color_object, -1);
		callback(color_object, userdata);
		valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
	return 0;
}

gint32 palette_list_foreach_selected(GtkWidget* widget, gint32 (*callback)(struct ColorObject* color_object, void *userdata), void *userdata){
	GtkTreeSelection *selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW(widget) );
	GtkListStore *store;
	GtkTreeIter iter;

	store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));

	GList *list = gtk_tree_selection_get_selected_rows ( selection, 0 );
	GList *i = list;

	struct ColorObject* color_object;
	//gchar* color_name;

	while (i) {
		gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, (GtkTreePath*) (i->data));
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &color_object, -1);
		callback(color_object, userdata);
		i = g_list_next(i);
	}

	g_list_foreach (list, (GFunc)gtk_tree_path_free, NULL);
	g_list_free (list);
	return 0;
}

gint32 palette_list_forfirst_selected(GtkWidget* widget, gint32 (*callback)(struct ColorObject* color_object, void *userdata), void *userdata){
	GtkTreeSelection *selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW(widget) );
	GtkListStore *store;
	GtkTreeIter iter;

	store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));

	GList *list = gtk_tree_selection_get_selected_rows ( selection, 0 );
	GList *i = list;

	struct ColorObject* color_object;
	//gchar* color_name;

	if (i) {
		gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, (GtkTreePath*) (i->data));
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &color_object, -1);
		callback(color_object, userdata);
	}

	g_list_foreach (list, (GFunc)gtk_tree_path_free, NULL);
	g_list_free (list);
	return 0;
}

