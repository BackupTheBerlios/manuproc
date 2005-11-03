// Code to work aroung gtk bug 141834

extern "C" {
gulong
gtk_expander_hook_size_to_parent(GtkExpander *expander);
}
