#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#define BOLD "\x1b[1m"

#define MAX_ROOMS 50
#define MAX_GUESTS 100
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin123"

typedef struct
{
    char username[50];
    char password[50];
    char name[100];
    char phone[15];
    char email[100];
    int is_active;
} User;

typedef struct
{
    int room_number;
    char type[20];
    float price;
    int is_booked;
    int is_locked;
    int is_reserved;
    char guest_name[100];
} Room;

typedef struct
{
    char item_name[50];
    float price;
} MenuItem;

Room rooms[MAX_ROOMS];
User guests[MAX_GUESTS];
int guest_count = 0;
MenuItem menu[10] = {
    {"Coffee", 5.0}, {"Tea", 3.0}, {"Breakfast Buffet", 20.0}, {"Lunch Special", 25.0}, {"Dinner Set", 35.0}, {"Pasta", 18.0}, {"Burger", 15.0}, {"Salad", 12.0}, {"Juice", 6.0}, {"Dessert", 10.0}};

void display_header();
void get_current_time(char *buffer);
void init_rooms();
void main_menu();
void admin_login();
void guest_login();
void admin_dashboard();
void guest_dashboard(int guest_index);
void book_room(int guest_index);
void view_rooms();
void order_food(int guest_index);
void send_notifications(User u);
void clear_screen_alt();
void view_guest_records();
void save_data_txt();
void load_data_txt();
void add_history_permanent(const char *desc);
void view_history_permanent();
void camera_control();
void manage_rooms();
void process_payment(int guest_idx, float amount, const char *item);
void custom_sleep(int seconds);

void clear_screen_alt()
{
    for (int i = 0; i < 50; i++)
        printf("\n");
}

void custom_sleep(int seconds)
{
    time_t start = time(NULL);
    while (time(NULL) - start < seconds)
        ;
}

void get_current_time(char *buffer)
{
    time_t t;
    t = time(NULL);
    sprintf(buffer, "%s", ctime(&t));
}

void display_header()
{
    char time_str[30];
    get_current_time(time_str);
    printf(CYAN "====================================================\n" RESET);
    printf(BOLD BLUE "            LUXURY HOTEL MANAGEMENT SYSTEM          \n" RESET);
    printf(CYAN "====================================================\n" RESET);
    printf(YELLOW " Current Date/Time: %s\n" RESET, time_str);
    printf(CYAN "----------------------------------------------------\n" RESET);
}

void init_rooms()
{
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        rooms[i].room_number = 101 + i;
        rooms[i].is_booked = 0;
        rooms[i].is_locked = 0;
        rooms[i].is_reserved = 0;
        strcpy(rooms[i].guest_name, "None");
        if (i < 20)
        {
            strcpy(rooms[i].type, "Standard");
            rooms[i].price = 100.0;
        }
        else if (i < 40)
        {
            strcpy(rooms[i].type, "Deluxe");
            rooms[i].price = 200.0;
        }
        else
        {
            strcpy(rooms[i].type, "Suite");
            rooms[i].price = 500.0;
        }
    }
}

int main()
{
    srand(time(NULL));
    init_rooms();
    load_data_txt();
    while (1)
    {
        main_menu();
        save_data_txt();
    }
    return 0;
}

void main_menu()
{
    int choice;
    clear_screen_alt();
    display_header();
    printf(GREEN "1. Admin Login\n" RESET);
    printf(GREEN "2. Guest Login\n" RESET);
    printf(GREEN "3. Exit\n" RESET);
    printf("\nEnter choice: ");
    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n')
            ;
        return;
    }

    switch (choice)
    {
    case 1:
        admin_login();
        break;
    case 2:
        guest_login();
        break;
    case 3:
        exit(0);
    default:
        printf(RED "Invalid choice!\n" RESET);
        custom_sleep(1);
    }
}

void admin_login()
{
    char user[50], pass[50];
    clear_screen_alt();
    display_header();
    printf(BOLD "--- ADMIN LOGIN ---\n" RESET);
    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, ADMIN_USER) == 0 && strcmp(pass, ADMIN_PASS) == 0)
    {
        admin_dashboard();
    }
    else
    {
        printf(RED "Login Failed!\n" RESET);
        custom_sleep(1);
    }
}

void guest_login()
{
    char user[50], pass[50];
    clear_screen_alt();
    display_header();
    printf(BOLD "--- GUEST LOGIN ---\n" RESET);
    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    for (int i = 0; i < guest_count; i++)
    {
        if (strcmp(guests[i].username, user) == 0 && strcmp(guests[i].password, pass) == 0)
        {
            guest_dashboard(i);
            return;
        }
    }
    printf(RED "Invalid Credentials!\n" RESET);
    custom_sleep(1);
}

void admin_dashboard()
{
    int choice;
    while (1)
    {
        clear_screen_alt();
        display_header();
        printf(MAGENTA "--- ADMIN PANEL ---\n" RESET);
        printf("1. Add New Guest       2. View All Rooms      3. View Guest Records\n");
        printf("4. Manage Rooms        5. Camera Control      6. View History Logs\n");
        printf("7. Logout\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 7)
            break;

        switch (choice)
        {
        case 1:
        {
            User new_guest;
            printf("Enter Name: ");
            scanf(" %[^\n]s", new_guest.name);
            printf("Enter Phone: ");
            scanf("%s", new_guest.phone);
            printf("Enter Email: ");
            scanf("%s", new_guest.email);
            sprintf(new_guest.username, "guest%d", guest_count + 1);
            sprintf(new_guest.password, "pass%d", rand() % 9000 + 1000);
            new_guest.is_active = 1;
            guests[guest_count++] = new_guest;
            char log[250];
            sprintf(log, "Admin added new guest: %s", new_guest.name);
            add_history_permanent(log);
            send_notifications(new_guest);
            printf(GREEN "\nGuest added successfully!\n" RESET);
            printf("Press enter to continue...");
            getchar();
            getchar();
            break;
        }
        case 2:
            view_rooms();
            printf("\nPress enter...");
            getchar();
            getchar();
            break;
        case 3:
            view_guest_records();
            printf("\nPress enter...");
            getchar();
            getchar();
            break;
        case 4:
            manage_rooms();
            break;
        case 5:
            camera_control();
            break;
        case 6:
            view_history_permanent();
            break;
        default:
            printf(RED "Invalid choice!\n" RESET);
            custom_sleep(1);
        }
    }
}

void manage_rooms() {
    int r_num, action;
    printf("Enter Room Number: "); scanf("%d", &r_num);
    int idx = -1;
    for(int i=0; i<MAX_ROOMS; i++) if(rooms[i].room_number == r_num) idx = i;
    if(idx == -1) { printf(RED "Invalid Room!\n" RESET); custom_sleep(1); return; }

    printf("1. Lock Room  2. Unlock Room  3. Reserve Room  4. Cancel Reservation\nChoice: ");
    scanf("%d", &action);
    char log[250];
    if(action == 1) { rooms[idx].is_locked = 1; sprintf(log, "Room %d LOCKED by Admin", r_num); }
    else if(action == 2) { rooms[idx].is_locked = 0; sprintf(log, "Room %d UNLOCKED by Admin", r_num); }
    else if(action == 3) { rooms[idx].is_reserved = 1; sprintf(log, "Room %d RESERVED by Admin", r_num); }
    else if(action == 4) { rooms[idx].is_reserved = 0; sprintf(log, "Room %d RESERVATION CANCELLED", r_num); }
    add_history_permanent(log);
    printf(GREEN "Action completed!\n" RESET); custom_sleep(1);
}

void camera_control()
{
    int r_num;
    printf("Enter Room Number to view camera: ");
    scanf("%d", &r_num);
    clear_screen_alt();
    display_header();
    printf(RED "[LIVE CAMERA FEED - ROOM %d]\n" RESET, r_num);
    printf("Status: ACTIVE | Recording: YES | Cloud Sync: ENABLED\n");
    printf("Movement detected: NO\n");
    printf("\n[CAMERA HISTORY]\n");
    printf("10:00 AM - No movement\n11:30 AM - Cleaning staff entered\n12:00 PM - Guest entered\n");
    char log[250];
    sprintf(log, "Admin accessed camera feed for Room %d", r_num);
    add_history_permanent(log);
    printf("\nPress enter to exit feed...");
    getchar();
    getchar();
}

void view_guest_records()
{
    clear_screen_alt();
    display_header();
    printf(MAGENTA "--- GUEST RECORDS ---\n" RESET);
    printf("%-5s %-20s %-15s %-25s %-15s\n", "ID", "Name", "Phone", "Email", "Username");
    for (int i = 0; i < guest_count; i++)
    {
        if (guests[i].is_active)
        {
            printf("%-5d %-20s %-15s %-25s %-15s\n",
                   i + 1, guests[i].name, guests[i].phone, guests[i].email, guests[i].username);
        }
    }
}

void guest_dashboard(int idx)
{
    int choice;
    while (1)
    {
        clear_screen_alt();
        display_header();
        printf(MAGENTA "--- WELCOME %s ---\n" RESET, guests[idx].name);
        printf(CYAN "1. View Available Rooms    2. Book a Room           3. Order Food\n" RESET);
        printf(CYAN "4. Room Service            5. Laundry Service       6. Gym Access\n" RESET);
        printf(CYAN "7. Spa Booking             8. Swimming Pool Access  9. Wake-up Call\n" RESET);
        printf(CYAN "10. Request Cleaning       11. View Bill            12. Wi-Fi Password\n" RESET);
        printf(CYAN "13. Newspaper Request      14. Taxi Booking         15. Luggage Help\n" RESET);
        printf(CYAN "16. Doctor on Call         17. Mini Bar Access      18. Movie Rental\n" RESET);
        printf(CYAN "19. Local Tour Info        20. Feedback             21. Logout\n" RESET);
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 21)
            break;

        switch (choice)
        {
        case 1:
            view_rooms();
            break;
        case 2:
            book_room(idx);
            break;
        case 3:
            order_food(idx);
            break;
        case 4:
            printf(CYAN "Room service requested!\n" RESET);
            break;
        case 5:
            printf(CYAN "Laundry service requested!\n" RESET);
            break;
        case 6:
            printf(CYAN "Gym access granted!\n" RESET);
            break;
        case 7:
            printf(CYAN "Spa booking confirmed!\n" RESET);
            break;
        case 8:
            printf(CYAN "Pool access granted!\n" RESET);
            break;
        case 9:
            printf(CYAN "Wake-up call scheduled!\n" RESET);
            break;
        case 10:
            printf(CYAN "Cleaning requested!\n" RESET);
            break;
        case 11:
            printf(YELLOW "Total Bill: $150.00 (Demo)\n" RESET);
            break;
        case 12:
            printf(GREEN "Wi-Fi Password: LUXURY_GUEST_2026\n" RESET);
            break;
        default:
            printf(RED "Feature activated!\n" RESET);
        }
        printf("\nPress enter to continue...");
        getchar();
        getchar();
    }
}

void process_payment(int idx, float amount, const char *item)
{
    int pay_choice;
    printf(YELLOW "\nTotal for %s: $%.2f\n" RESET, item, amount);
    printf("1. Pay Now\n2. Pay Later\nChoice: ");
    scanf("%d", &pay_choice);

    char log[250];
    if (pay_choice == 1)
    {
        printf(GREEN "Payment Successful! SMS sent to %s\n" RESET, guests[idx].phone);
        sprintf(log, "Guest %s PAID $%.2f for %s", guests[idx].name, amount, item);
    }
    else
    {
        printf(YELLOW "Added to bill. SMS sent to %s\n" RESET, guests[idx].phone);
        sprintf(log, "Guest %s DEFERRED $%.2f for %s", guests[idx].name, amount, item);
    }
    add_history_permanent(log);
}

void book_room(int idx)
{
    int room_num;
    printf("Enter Room Number: ");
    scanf("%d", &room_num);
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        if (rooms[i].room_number == room_num)
        {
            if (rooms[i].is_booked || rooms[i].is_locked || rooms[i].is_reserved)
            {
                printf(RED "Room unavailable!\n" RESET);
            }
            else
            {
                rooms[i].is_booked = 1;
                strcpy(rooms[i].guest_name, guests[idx].name);
                process_payment(idx, rooms[i].price, "Room Booking");
            }
            return;
        }
    }
    printf(RED "Room not found!\n" RESET);
}

void order_food(int idx)
{
    int choice;
    clear_screen_alt();
    display_header();
    printf(MAGENTA "--- RESTAURANT MENU ---\n" RESET);
    for (int i = 0; i < 10; i++)
        printf("%d. %-20s $%.2f\n", i + 1, menu[i].item_name, menu[i].price);
    printf("\nEnter item: ");
    scanf("%d", &choice);
    if (choice >= 1 && choice <= 10)
        process_payment(idx, menu[choice - 1].price, menu[choice - 1].item_name);
    else
        printf(RED "Invalid item!\n" RESET);
}

void view_rooms()
{
    printf("\n%-10s %-15s %-10s %-15s\n", "Room #", "Type", "Price", "Status");
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        printf("%-10d %-15s $%-9.2f %-15s\n",
               rooms[i].room_number, rooms[i].type, rooms[i].price,
               rooms[i].is_locked ? RED "LOCKED" RESET : (rooms[i].is_reserved ? YELLOW "RESERVED" RESET : (rooms[i].is_booked ? RED "BOOKED" RESET : GREEN "AVAILABLE" RESET)));
    }
}

void save_data_txt()
{
    FILE *fg = fopen("guests.txt", "w");
    if (fg)
    {
        fprintf(fg, "%d\n", guest_count);
        for (int i = 0; i < guest_count; i++)
        {
            fprintf(fg, "%s|%s|%s|%s|%s|%d\n", guests[i].username, guests[i].password, guests[i].name, guests[i].phone, guests[i].email, guests[i].is_active);
        }
        fclose(fg);
    }
    FILE *fr = fopen("rooms.txt", "w");
    if (fr)
    {
        for (int i = 0; i < MAX_ROOMS; i++)
        {
            fprintf(fr, "%d|%s|%f|%d|%d|%d|%s\n", rooms[i].room_number, rooms[i].type, rooms[i].price, rooms[i].is_booked, rooms[i].is_locked, rooms[i].is_reserved, rooms[i].guest_name);
        }
        fclose(fr);
    }
}

void load_data_txt()
{
    FILE *fg = fopen("guests.txt", "r");
    if (fg)
    {
        if (fscanf(fg, "%d\n", &guest_count) == 1)
        {
            for (int i = 0; i < guest_count; i++)
            {
                fscanf(fg, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d\n", guests[i].username, guests[i].password, guests[i].name, guests[i].phone, guests[i].email, &guests[i].is_active);
            }
        }
        fclose(fg);
    }
    FILE *fr = fopen("rooms.txt", "r");
    if (fr)
    {
        for (int i = 0; i < MAX_ROOMS; i++)
        {
            fscanf(fr, "%d|%[^|]|%f|%d|%d|%d|%[^\n]\n", &rooms[i].room_number, rooms[i].type, &rooms[i].price, &rooms[i].is_booked, &rooms[i].is_locked, &rooms[i].is_reserved, rooms[i].guest_name);
        }
        fclose(fr);
    }
}

void add_history_permanent(const char *desc)
{
    FILE *fh = fopen("history.txt", "a");
    if (fh)
    {
        char time_str[30];
        get_current_time(time_str);
        fprintf(fh, "[%s] %s\n", time_str, desc);
        fclose(fh);
    }
}

void view_history_permanent()
{
    char line[300];
    FILE *fh = fopen("history.txt", "r");
    clear_screen_alt();
    display_header();
    printf(MAGENTA "--- PERMANENT HISTORY LOGS ---\n" RESET);
    if (!fh)
    {
        printf("No history found.\n");
    }
    else
    {
        while (fgets(line, sizeof(line), fh))
            printf("%s", line);
        fclose(fh);
    }
    printf("\nPress enter to continue...");
    getchar();
    getchar();
}

void send_notifications(User u)
{
    printf(CYAN "\n[NOTIFICATION SYSTEM]\n" RESET);
    printf("Credentials sent to %s and %s\n", u.phone, u.email);
    printf("User: %s | Pass: %s\n", u.username, u.password);
}
