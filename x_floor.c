#include "lib.c"
#include <gtk/gtk.h>
GtkWidget *window;
GtkWidget *tableParent;
GtkWidget *button;
GtkWidget *buttonCall;
GtkWidget *hAlign;
GtkWidget *labelNumberFloor;
GtkWidget *hbox;
GtkWidget *arrow;
GtkStyleContext *context;
int floor;
char floorTitle[15];
int *pid_list;
int shmid;
void handle(int sigNo);
int main(int *argc, char* argv[])
{
    if(argc != 2) {
        printf("Loi tham so dau vao\n");
        exit(1);
    }

    if ((shmid = shmget(KEY_SHM_PID,SHM_PID_SIZE*sizeof(int),IPC_CREAT|0660)) == -1)
    {
        perror("shmget");
        exit(1);
    }
    pid_list = (int*)shmat(shmid,0,0);
    if ((pid_list == (int*)-1))
    {
        perror("shmat");
        exit(1);
    }
    
    floor = atoi(argv[1]);
    setFloorInfo();
    

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

     // <-------------  For add stylesheet.css
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

void updateFloorText(int numberFloor) {
    char numberFloorString[4];
    sprintf(numberFloorString, "%d", numberFloor);
    gtk_label_set_text(labelNumberFloor, numberFloorString);
    if(numberFloor == floor) {
        addArrival();
    } 
    gtk_widget_queue_draw(labelNumberFloor);
}


void setFloorInfo() {
    switch(floor) {
        case 2:
            strcpy(floorTitle, "Tang 2");
            pid_list[FLOOR_2ND] = getpid(); 
            break;
        case 3:
            strcpy(floorTitle, "Tang 3");
            pid_list[FLOOR_3ND] = getpid();
            break;
        case 4:
            strcpy(floorTitle, "Tang 4");
            pid_list[FLOOR_4ND] = getpid(); 
            break;
        case 5:
            strcpy(floorTitle, "Tang 5");
            pid_list[FLOOR_5ND] = getpid(); 
            break;
    }
}

void callRequest() {
    switch(floor) {
        case 2:
            kill(pid_list[LIFT_MNG], enSigNo(SIGNAL_ELEVATOR_CALL_221));
            break;
        case 3:
            kill(pid_list[LIFT_MNG], enSigNo(SIGNAL_ELEVATOR_CALL_321));
            break;
        case 4:
            kill(pid_list[LIFT_MNG], enSigNo(SIGNAL_ELEVATOR_CALL_421));
            break;
        case 5:
            kill(pid_list[LIFT_MNG], enSigNo(SIGNAL_ELEVATOR_CALL_521));
            break;
    }
}

void initGtk() {
    GtkCssProvider* Provider = gtk_css_provider_new();
    GdkDisplay* Display = gdk_display_get_default();
    GdkScreen* Screen = gdk_display_get_default_screen(Display);

    gtk_style_context_add_provider_for_screen(Screen, GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), "style.css", NULL);
    //
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    switch(floor) {
        case 2:
            gtk_window_move(GTK_WINDOW(window), 500, 435);
            break;
        case 3:
            gtk_window_move(GTK_WINDOW(window), 500, 295);
            break;
        case 4:
            gtk_window_move(GTK_WINDOW(window), 500, 160);
            break;
        case 5:
            gtk_window_move(GTK_WINDOW(window), 500, 0);
            break;
    }
    gtk_window_set_default_size(GTK_WINDOW(window), 100, 100);
    gtk_window_set_title(GTK_WINDOW(window), floorTitle);
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);

    tableParent = gtk_table_new(2,1,FALSE);

    gtk_table_set_row_spacings(GTK_TABLE(tableParent), 5);
    gtk_table_set_col_spacings(GTK_TABLE(tableParent), 5);

    labelNumberFloor = gtk_label_new("1");
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

    buttonCall = gtk_button_new_with_label("Call");
    gtk_widget_set_size_request(buttonCall, 50, 30);
    context = gtk_widget_get_style_context(buttonCall);
    gtk_style_context_add_class(context,"btn-call");

    gtk_table_attach(GTK_TABLE(tableParent), hbox, 0,1,0,1,GTK_FILL, GTK_FILL, 100, 5);
    gtk_table_attach(GTK_TABLE(tableParent), buttonCall, 0,1,1,2, GTK_FILL, GTK_FILL, 100, 5);


    gtk_container_add(GTK_CONTAINER(window), tableParent);
    g_signal_connect(G_OBJECT(window), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(buttonCall, "clicked", G_CALLBACK(callRequest), NULL);
    
}

