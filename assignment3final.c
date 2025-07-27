/***
 *      ______                       __             ______
 *     /      \                     |  \           /      \
 *    |  $$$$$$\ __    __   _______ | $$  ______  |  $$$$$$\ __    __  _______    _______
 *    | $$   \$$|  \  |  \ /       \| $$ /      \ | $$___\$$|  \  |  \|       \  /       \
 *    | $$      | $$  | $$|  $$$$$$$| $$|  $$$$$$\ \$$    \ | $$  | $$| $$$$$$$\|  $$$$$$$
 *    | $$   __ | $$  | $$| $$      | $$| $$    $$ _\$$$$$$\| $$  | $$| $$  | $$| $$
 *    | $$__/  \| $$__/ $$| $$_____ | $$| $$$$$$$$|  \__| $$| $$__/ $$| $$  | $$| $$_____
 *     \$$    $$ \$$    $$ \$$     \| $$ \$$     \ \$$    $$ \$$    $$| $$  | $$ \$$     \
 *      \$$$$$$  _\$$$$$$$  \$$$$$$$ \$$  \$$$$$$$  \$$$$$$  _\$$$$$$$ \$$   \$$  \$$$$$$$
 *              |  \__| $$                                  |  \__| $$
 *               \$$    $$                                   \$$    $$
 *                \$$$$$$                                     \$$$$$$
 */
//Project Title: CycleSync
//Team Name : C Queens
//Team members : Sakina Aliasgar Barodawala , Doreen Gyimah, Mahya Hajimohammadi, Valerie Rodriguez - Porras

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#define MAX_ENTRIES 100               // 100 Maximum period entries stored
#define FILENAME "period_data.txt"    // Main file to store all period data
#define MAX_NOTE_LENGTH 400    // Character maximum per diary entries
#define MONTHSOFYEAR 12        // Number of months in a year
#define MAX_LEN 100            // Maximum characters for password and username

void addCycle();      // Function to add a new period entry
void updateCycle();   // Function to update an existing period entry
void deleteCycle();   // Function to delete an existing period entry
void searchCycle();   // Search function to search for specific period entry
void predictCycle();  // Function to predict next period date
void addDiaryEntry(); // Function to add notes related to period cycle
void displayCalendar(struct Calendar calendar[], int currentYear);  // Function to display calendar
int daysInMonth(int month, int year);      // Function to calculate days in a month for displayed calendar
void saveToFile(PeriodEntry entries[], int count);  // Function to save period entries to .txt file
int loadFromFile(PeriodEntry entries[]);   // Function to load period entries from .txt file


// Function to check if the username exists in the USERINFO.txt file
int usernameExists(char* username) {
    FILE* userFile = fopen("USERINFO.txt", "r");  // Open the USERINFO.txt file for reading
    char storedUsername[MAX_LEN];

    if (userFile == NULL) {
        return 0;  // If file doesn't exist or can't be opened, return false
    }

    // Read through the file to find the matching username
    while (fscanf(userFile, "%s", storedUsername) != EOF) {  // read username to the end of the file
        if (strcmp(storedUsername, username) == 0) {
            fclose(userFile);
            return 1;  // Username found
        }
    }

    fclose(userFile);
    return 0;  // Username not found
}

// Function to check if password exists in the USERINFO.txt file
int verifyPassword(char* username, char* password) {
    FILE* file = fopen("USERINFO.txt", "r");  // Open the file for reading
    if (file == NULL) {
        printf("Error: Could not open USERINFO.txt\n");
        return 0;  // Return false if the file cannot be opened
    }

    char storedUsername[MAX_LEN], storedPassword[MAX_LEN];

    // Read username and password as pairs from file 
    while (fscanf(file, "%s %s", storedUsername, storedPassword) != EOF) { // read username and password to the end of the file
        if (strcmp(storedUsername, username) == 0) {
            fclose(file);  // Close the file before returning
            return strcmp(storedPassword, password) == 0;  // Return 1 if passwords match, otherwise 0
        }
    }

    fclose(file);
    return 0;  // Username not found or password incorrect
}


// Function to add a new user and password to the text files
void addNewUser(char* username, char* password) {
    FILE* userFile = fopen("USERINFO.txt", "a");  // Open USERINFO.txt for appending to store info
    FILE* passFile = fopen("USERINFO.txt", "a");  // Open USERINFO.txt for appending to store info

    if (userFile == NULL || passFile == NULL) {
        printf("Error opening file for writing.\n"); // if empty print error message
        return;
    }

    // Write the new username and password to the files
    fprintf(userFile, "%s\n", username);
    fprintf(passFile, "%s\n", password);

    fclose(userFile); //close file after username input is finished
    fclose(passFile); //close file after password input is finished
}

// Function to print the login menu and handle user input for authentication
void printLoginStart(struct Login* login) {
    printf("Are you already a member? (Y/N):  ");
    scanf(" %c", &login->mem);  // Ensure to consume any previous newline character

    while (!(login->mem == 'y') && !(login->mem == 'Y') && !(login->mem == 'n') && !(login->mem == 'N')) {
        printf("Invalid entry. Please try again!\n"); // if user doesn't input these characters throw error message
        getchar();  // clear input buffer
        printf("Are you already a member? (Y/N):  ");
        scanf(" %c", &login->mem);  // Consume any leftover newline character
    }

    if (login->mem == 'Y' || login->mem == 'y') {
        getchar();  // clear input buffer

        while (1) {  // Loop until correct credentials are entered
            printf("Enter Username or Email: ");
            scanf("%s", login->user);

            if (!usernameExists(login->user)) {   // if username doesn't exist print error message
                printf("Username not found. Please try again.\n");
                continue; // Restart the loop if username does not exist
            }

            printf("Enter Password: ");
            scanf("%s", login->pass);

            if (verifyPassword(login->user, login->pass)) {
                printf("Login successful!\n");
                break; // Exit loop on successful login
            }
            else {
                printf("Incorrect password. Please try again.\n");
            }
        }
    }
    else if (login->mem == 'N' || login->mem == 'n') {
        getchar();  // clear input buffer
        printf("Create a Username or Enter Your Email: "); // If user enters N, create new username
        scanf("%s", login->user);

        // Check if username already exists
        while (usernameExists(login->user)) {
            printf("This username already exists. Please choose a different username.\n");
            printf("Enter a new Username or Email: ");
            scanf("%s", login->user);
        }

        printf("Create a Password: ");
        scanf("%s", login->pass);

        // Check password confirmation until passwords match
        do {
            printf("Confirm Password: ");
            scanf("%s", login->confirmPass);

            if (strcmp(login->pass, login->confirmPass) != 0) { // compare pass and confirmPass strings if it doesn't match
                printf("Passwords do not match. Please try again.\n"); // print error message if it doesn't match
            }
        } while (strcmp(login->pass, login->confirmPass) != 0);  // compare pass and confirmPass strings if it doesn't match until passwords match

        // Add the new username and password to the text files
        addNewUser(login->user, login->pass);
        printf("Account created successfully!\n");
    }
}

// Function to save period entries to file in an array then count the entries in the array
static void saveToFile(PeriodEntry entries[], int count) {
    FILE* file = fopen(FILENAME, "w"); //open .txt file in write mode to save data
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %d %d %d\n", entries[i].day, entries[i].month, entries[i].year, entries[i].cycle_length);
    }
    fclose(file); // Close the file after writing
}

// Function to load period entries from .txt file into an array
int loadFromFile(PeriodEntry entries[]) {
    FILE* file = fopen(FILENAME, "r");  // open .txt in read mode
    if (!file) return 0; // if file cannot be opened return 0
    int count = 0;
    while (fscanf(file, "%d %d %d %d", &entries[count].day, &entries[count].month, &entries[count].year, &entries[count].cycle_length) != EOF) {
        count++; // increment count for each period entry read in .txt
    }
    fclose(file); // close file
    return count;
}

// function to add a new period entry and save it to .txt file
void addCycle() {
    PeriodEntry entries[MAX_ENTRIES];
    int count = loadFromFile(entries); //load pre-existing period entries

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Enter period date (DD MM YYYY): ");
    scanf("%d %d %d", &entries[count].day, &entries[count].month, &entries[count].year);
    printf("Enter cycle length (in days): ");
    scanf("%d", &entries[count].cycle_length);

    count++;
    saveToFile(entries, count);  // Save the updated data to the period file
    //saveCycleToFile(entries, count);  // Save cycles to a new file
    printf("Cycle added successfully!\n");
}

// function to update period length for pre-existing period entry
void updateCycle() {
    PeriodEntry entries[MAX_ENTRIES];
    int count = loadFromFile(entries);  //load pre-existing period entries
    int day, month, year, i;

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Enter the date of the cycle to update (DD MM YYYY): ");
    scanf("%d %d %d", &day, &month, &year);

    for (i = 0; i < count; i++) {
        if (entries[i].day == day && entries[i].month == month && entries[i].year == year) {
            printf("Enter new cycle length: ");
            scanf("%d", &entries[i].cycle_length);
            saveToFile(entries, count); // save updated data
            printf("Cycle updated successfully!\n");
            return;
        }
    }
    printf("Cycle not found!\n"); // if date of cycle entered isn't in file print error message
}

// function to delete period entry for pre-existing period entry
void deleteCycle() {
    PeriodEntry entries[MAX_ENTRIES];
    int count = loadFromFile(entries);  //load pre-existing period entries
    int day, month, year, i, j;

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Enter the date of the cycle to delete (DD MM YYYY): ");
    scanf("%d %d %d", &day, &month, &year);

    for (i = 0; i < count; i++) {
        if (entries[i].day == day && entries[i].month == month && entries[i].year == year) {
            for (j = i; j < count - 1; j++) {
                entries[j] = entries[j + 1];
            }
            count--;
            saveToFile(entries, count);   // save updated data
            printf("Cycle deleted successfully!\n");
            return;
        }
    }
    printf("Cycle not found!\n"); // if date of cycle entered isn't in file print error message
}

// function to search .txt file for pre-existing period entry
void searchCycle() {
    PeriodEntry entries[MAX_ENTRIES];
    int count = loadFromFile(entries);  //load pre-existing period entries
    int day, month, year, i;

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Enter date to search (DD MM YYYY): ");
    scanf("%d %d %d", &day, &month, &year);

    for (i = 0; i < count; i++) {
        if (entries[i].day == day && entries[i].month == month && entries[i].year == year) {
            printf("Cycle found: %02d/%02d/%d, Cycle Length: %d days\n",
                entries[i].day, entries[i].month, entries[i].year, entries[i].cycle_length);
            return;
        }
    }
    printf("Cycle not found!\n");  // if date of cycle entered isn't in file print error message
}

// function to predict next period from .txt file for latest period entry
void predictCycle() {
    PeriodEntry entries[MAX_ENTRIES];
    int count = loadFromFile(entries);  //load pre-existing period entries
    if (count == 0) { // if file has no entries print error message
        printf("No data available for prediction.\n");
        return;
    }

    PeriodEntry last = entries[count - 1]; // get last period entered

    int predicted_day = last.day + last.cycle_length;
    int predicted_month = last.month;
    int predicted_year = last.year;

    // Days in each month (non-leap year)
    int days_in_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Check for leap year (February has 29 days)
    if ((predicted_year % 4 == 0 && predicted_year % 100 != 0) || (predicted_year % 400 == 0)) {
        days_in_month[2] = 29;
    }

    // Adjust date
    while (predicted_day > days_in_month[predicted_month]) {
        predicted_day -= days_in_month[predicted_month];
        predicted_month++;
        if (predicted_month > 12) {
            predicted_month = 1;
            predicted_year++;
        }
    }

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Predicted next period date: %02d/%02d/%d\n", predicted_day, predicted_month, predicted_year);
}



void addDiaryEntry() {
    FILE* file = fopen("period_diary.txt", "a");  // Open file in append mode
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    char note[MAX_NOTE_LENGTH];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Enter your diary entry: ");
    getchar();
    fgets(note, MAX_NOTE_LENGTH, stdin);  // Get the user's input

    fprintf(file, "%s\n", note);  // Write the note to the file

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Diary entry added successfully!\n");

    fclose(file);  // Close the file when done
}

// Function to calculate days in a month for displayed calendar
int daysInMonth(int month, int year) {
    switch (month) {
    case 1: return 31;
    case 2: return (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) ? 29 : 28; // calculate leap year
    case 3: return 31;
    case 4: return 30;
    case 5: return 31;
    case 6: return 30;
    case 7: return 31;
    case 8: return 31;
    case 9: return 30;
    case 10: return 31;
    case 11: return 30;
    case 12: return 31;
    default: return -1;
    }

}

//Done by Doreen Gyimah
// Function to display a calendar
void displayCalendar(struct Calendar calendar[], int currentYear) {
    int displayDays;
    printf("\n\n");
    printf("Input Current Year: ");
    scanf("%d", &currentYear);

    printf("Input the starting day of the year (First day of 2025 is Wednesday (3): ");
    scanf("%d", &displayDays);

    for (int i = 0; i < MONTHSOFYEAR; i++)
    {
        calendar[i].year = currentYear;
        calendar[i].day = daysInMonth(calendar[i].monthNum, currentYear);
        calendar[i].displayDays = displayDays;


        printf("\n---------------%s  %d--------------\n", calendar[i].month, currentYear);
        printf("\n\n   Sun  Mon Tues  Wed Thur  Fri  Sat\n");

        for (int a = 0; a < displayDays; a++) {
            printf("     ");
        }


        for (int d = 1; d <= calendar[i].day; d++) {
            printf("%5d", d);
            displayDays = (displayDays + 1) % 7;
            if (displayDays == 0) {
                printf("\n");
            }
        }

        printf("\n \n \n");
    }

    printf("\n \n");

}

// Function to display all cycles
// Function to read and display cycles from a file
void displayAllData(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) { // if file doesn't open print error message
        printf("Error: Unable to open file '%s'.\n", filename);
        return;
    }
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Displaying all period cycles from file:\n");

    int s_day, s_month, s_year, duration;
    int count = 0;

    while (fscanf(file, "%d %d %d %d", &s_day, &s_month, &s_year, &duration) == 4) {
        count++;
        printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
        printf("Cycle %d:\n", count);
        printf("Start Date: %02d-%02d-%04d\n", s_day, s_month, s_year);
        printf("Duration: %d days\n", duration);
        
    }

    if (count == 0) {
        printf("No period cycles found in file.\n");
    }

    fclose(file);
}


// Function to save cycles to a new file
void saveCycleToFile() {
    FILE* inputFile = fopen(FILENAME, "r");
    if (!inputFile) { // if file doesn't open print error message
        printf("Error: Unable to open source file '%s'.\n", FILENAME);
        return;
    }
    const char* destinationFile = "saved_cycles.txt";
    FILE* outputFile = fopen(destinationFile, "w");
    if (!outputFile) { // if file doesn't open print error message
        printf("Error: Unable to create destination file '%s'.\n", destinationFile);
        fclose(inputFile);
        return;
    }
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Saving data from '%s' to '%s'...\n", FILENAME, destinationFile);

    char line[256];  // Buffer to store each line
    while (fgets(line, sizeof(line), inputFile)) { //reads a single line from inputFile and store it in the line buffer.
        fputs(line, outputFile);  // Write each line from source to destination
    }
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Data has been successfully saved to '%s'.\n", destinationFile);

    fclose(inputFile); //close file
    fclose(outputFile); // close file
}

// Function to save cycles to a new file
void displayAmount() {
    FILE* file = fopen("period_data.txt", "r");  // Open file in append and reading mode
    if (!file) {  // if file doesn't open print error message
        printf("Error opening file.\n");
        return;
    }

    int amount = 0;
    char periodEntry[100];

    while (fgets(periodEntry, sizeof(periodEntry), file) != NULL) { //reads one line from the file and stores it in the buffer periodEntry.txt
        amount++; //increment
    }

    fclose(file);
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("Total number of cycles: %d\n", amount);  // Display the total amount of period cycles
}

int main() {

    struct CycleData cycles[MAX_ENTRIES]; //stores max entries
    struct Calendar calendar[MONTHSOFYEAR] = { {"January", 1}, {"February", 2}, 
        {"March", 3}, {"April", 4}, {"May", 5}, {"June", 6}, {"July", 7}, 
        {"August", 8}, {"September", 9}, {"October", 10}, {"November", 11}, 
        {"December", 12} }; // stores 12 months into calendar struct
    int currentYear = 2025; //initializes calender year
    int displayDays; //declares int variable
    int searchMonth; //declares int variable
    int count = 0; //initializes count to 0

    printf("..####...##..##...####...##......######...####...##..##..##..##...####..\n");
    printf(".##..##...####...##..##..##......##......##.......####...###.##..##..##.\n");
    printf(".##........##....##......##......####.....####.....##....##.###..##.....\n");
    printf(".##..##....##....##..##..##......##..........##....##....##..##..##..##.\n");
    printf("..####.....##.....####...######..######...####.....##....##..##...####..\n");
    printf("========================================================================\n\n");

    struct Login login;
    printLoginStart(&login);  // Call the function to start the login process

    int choice;
    while (1) {
        printf("+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+-+\n");
        printf("|C|y|c|l|e|S|y|n|c| |P|e|r|i|o|d| |T|r|a|c|k|e|r|\n"); // Menu after login
        printf("+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+-+\n");
        printf("1. Add Cycle\n");
        printf("2. Update Cycle\n");
        printf("3. Delete Cycle\n");
        printf("4. Search Cycle\n");
        printf("5. Predict Next Period\n");
        printf("6. Diary Entry\n");
        printf("7. Display All Cycles\n");
        printf("8. Display Amount of Periods\n");
        printf("9. Save Cycles To New File\n");
        printf("10. Display Calendar\n");
        printf("11. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1: addCycle();
            break;
        case 2: updateCycle();
            break;
        case 3: deleteCycle();
            break;
        case 4: searchCycle();
            break;
        case 5: predictCycle();
            break;
        case 6: addDiaryEntry();
            break;
        case 7: displayAllData("period_data.txt");
            break;
        case 8:  displayAmount(); // dislay amount of entries
            break;
        case 9:
            printf("Saving cycles...\n");
            saveCycleToFile(); // save .txt to new .txt
            break;
        case 10:
            displayCalendar(calendar, currentYear);  // Display calendar
            break;

        case 11: exit(0);
        default: printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}