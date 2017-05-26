#include "lib.c"
#include <gtk/gtk.h>
GtkWidget *window;
GtkWidget *tableParent;
GtkWidget *table;
GtkWidget *button;
GtkWidget *buttonCall;
GtkWidget *hAlign;
GtkWidget *labelNumberFloor;
GtkWidget *hbox;
GtkWidget *arrow;
GtkStyleContext *context;
int floor = 1;
int *pid_list;
int shmid;

void handle(int sigNo);

int main(int *argc, char* argv[])
{
    if ((shmid = shmget(KEY_SHM_PID,SHM_PID_SIZE*sizeof(int),IPC_CREAT|0660)) == -1)
    {
        perror("shmget error");
        exit(1);
    }
    pid_list = (int*)shmat(shmid,0,0);
    if ((pid_list == (int*)-1))
    {
        perror("shmat error");
        exit(1);
    }
    // write pid of 1nd floor to share memory
    pid_list[FLOOR_1ND] = getpid();
    

    signal(enSigNo(SIGNAL_ARRIVAL_1ND), handle);
    signal(enSigNo(SIGNAL_ARRIVAL_2ND), handle);
    signal(enSigNo(SIGNAL_ARRIVAL_3ND), handle);
    signal(enSigNo(SIGNAL_ARRIVAL_4ND), handle);
    signal(enSigNo(SIGNAL_ARRIVAL_5ND), handle);

    signal(enSigNo(SIGNAL_MOVEOUT), handle);

    // signal to update arrow
    signal(enSigNo(SIGNAL_START_MOVE_UP), handle);
    signal(enSigNo(SIGNAL_START_MOVE_DOWN), handle);
    signal(enSigNo(SIGNAL_STOP), handle);

    gtk_init(&argc, &argv);
    initGtk();
    gtk_widget_show_all(window);
    gtk_main();
    gtk_widget_queue_draw(window);
}

void handle(int sigNo)
{
    switch(deSigNo(sigNo)) {
        case SIGNAL_ARRIVAL_1ND:
            updateFloorText(1);
            break;
        case SIGNAL_ARRIVAL_2ND:
            updateFloorText(2);
            break;
        case SIGNAL_ARRIVAL_3ND:
            updateFloorText(3);
            break;
        case SIGNAL_ARRIVAL_4ND:
            updateFloorText(4);
            break;
        case SIGNAL_ARRIVAL_5ND:
            updateFloorText(5);
            break;
        case SIGNAL_MOVEOUT:
            removeArrival();
            break;

        case SIGNAL_START_MOVE_UP:
            updateStatusUpDownStop(1);
            break;
        case SIGNAL_STOP:
            updateStatusUpDownStop(0);
            break;
        case SIGNAL_START_MOVE_DOWN:
            updateStatusUpDownStop(2);
            break;
    }
}

void updateFloorText(int numberFloor) {
    char numberFloorString[4];
    sprintf(numberFloorString, "%d", numberFloor);
    gtk_label_set_text(labelNumberFloor, numberFloorString);
    if(numberFloor == floor) {
        addArrival();
    }
}


void updateStatusUpDownStop(int status) {
    // upDownStatus: 0 <-> stop, 1-<->up, 2 <-> down
    if(status == 0) {
        gtk_image_clear(arrow);
    } else
    if(status == 1) {
        gtk_image_set_from_file(arrow, "up1.gif");
    } else {
        gtk_image_set_from_file(arrow, "down1.gif");
    } 
    gtk_widget_queue_draw(arrow);
}


void addArrival() {
    context = gtk_widget_get_style_context(labelNumberFloor);
    gtk_style_context_add_class(context,"bg-red");
}

void removeArrival() {
    context = gtk_widget_get_style_context(labelNumberFloor);
    gtk_style_context_remove_class(context,"bg-red");
}

void callRequest(GtkWidget* button, gpointer* data) {
    int *numberFloor = data;
    int floorRequest = -1;
    if(numberFloor == 0) floorRequest = 2;
    if(numberFloor == 1) floorRequest = 3;
    if(numberFloor == 2) floorRequest = 4;
    if(numberFloor == 3) floorRequest = 5;
    switch(floorRequest)
    {
        case 2:
            kill(pid_list[LIFT_MNG], enSigNo(SIGNAL_ELEVATOR_CALL_122));
            printf("--> ADD REQUEST: 1 floor request to 2 floor\n");
            break;
        case 3:
            kill(pid_list[LIFT_MNG], enSigNo(SIGNAL_ELEVATOR_CALL_123));
            printf("--> ADD REQUEST: 1 floor request to 3 floor\n");  
            break;
        case 4:
            kill(pid_list[LIFT_MNG], enSigNo(SIGNAL_ELEVATOR_CALL_124));
            printf("--> ADD REQUEST: 1 floor request to 4 floor\n");
            break;
        case 5:
            kill(pid_list[LIFT_MNG], enSigNo(SIGNAL_ELEVATOR_CALL_125));
            printf("--> ADD REQUEST: 1 floor request to 5 floor\n"); 
            break;
    }
}

void initGtk() {

     // <-------------  For add stylesheet.css
    GtkCssProvider* Provider = gtk_css_provider_new();
    GdkDisplay* Display = gdk_display_get_default();
    GdkScreen* Screen = gdk_display_get_default_screen(Display);

    gtk_style_context_add_provider_for_screen(Screen, GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), "style.css", NULL);
    //
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_move(GTK_WINDOW(window), 500,650);
    gtk_window_set_default_size(GTK_WINDOW(window), 150, 150);
    gtk_window_set_title(GTK_WINDOW(window), "Tang 1");
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);

    gchar *values[4] = {"2", "3", "4","5"};

    tableParent = gtk_table_new(2,1,FALSE);

    gtk_table_set_row_spacings(GTK_TABLE(tableParent), 5);
    gtk_table_set_col_spacings(GTK_TABLE(tableParent), 5);


    table = gtk_table_new(2, 2, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 2);
    gtk_table_set_col_spacings(GTK_TABLE(table), 2);

    int i = 0;
    int j = 0;
    int pos = 0;

    for (i=0; i < 2; i++)
    {
        for (j=0; j < 2; j++)
        {
            button = gtk_button_new_with_label(values[pos]);
            // add event listen when click button floor
            g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(callRequest), i*2+j);
            context = gtk_widget_get_style_context(button);
            gtk_style_context_add_class(context,"btn-floor");
            gtk_table_attach_defaults(GTK_TABLE(table), button, j, j+1, i, i+1);
            pos++;
        }
    }
    labelNumberFloor = gtk_label_new("1");
    addArrival();
    gtk_widget_set_size_request(labelNumberFloor, 50, 30);
    context = gtk_widget_get_style_context(labelNumberFloor);
    gtk_style_context_add_class(context,"bg-black");
    gtk_style_context_add_class(context,"border-black-1");
    gtk_style_context_add_class(context,"text-white");

    arrow = gtk_image_new();
    gtk_widget_set_size_request(arrow, 30, 30);

     hbox = gtk_hbox_new(TRUE, 3);
     gtk_container_add(GTK_CONTAINER(hbox), arrow);
     gtk_container_add(GTK_CONTAINER(hbox), labelNumberFloor);

    gtk_table_attach(GTK_TABLE(tableParent), hbox, 0,1,0,1,GTK_FILL, GTK_FILL, 100, 5);
    gtk_table_attach_defaults(GTK_TABLE(tableParent), table, 0,1,1,2);

    gtk_container_add(GTK_CONTAINER(window), tableParent);
    g_signal_connect(G_OBJECT(window), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

}


