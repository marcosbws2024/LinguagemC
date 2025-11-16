#include <gtk/gtk.h>
//pacman -S mingw-w64-x86_64-gtk3
// Função chamada quando o botão é clicado
static void print_hello (GtkWidget *widget, gpointer data) {
    g_print ("Olá, Mundo Gráfico!\n");
}

// Função chamada quando a janela principal é fechada
static void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;

    // 1. Cria a nova janela de nível superior
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Tela de Cadastro Gráfica");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 150);

    // 2. Cria um botão
    button = gtk_button_new_with_label ("Clique Aqui");
    
    // Conecta o sinal "clicado" do botão à função 'print_hello'
    g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

    // Adiciona o botão à janela
    gtk_window_set_child (GTK_WINDOW (window), button);

    // Exibe todos os widgets
    gtk_widget_show (window);
}

int main (int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Inicializa a aplicação GTK
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    
    // Conecta a função 'activate' (onde a janela é criada) ao sinal "activate"
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    
    // Executa a aplicação
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}