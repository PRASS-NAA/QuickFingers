#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MAX_LEN 50
#define ARRAY_SIZE 100
#define READ_TEXT_SIZE 2048
#define MAX_PARAGRAPHS 11

void clearConsole(void);
int validatepass(char *);
int login(char*, char*);
int signup(char*, char*);
void gamewords(void);
void gametimer(void);
void messagetimer(int);
void selectTest(int,int);
void startTest(char*,int);
void handleTypingTest(const char*);
void displayTextWithColor(const char*, int);
void setConsoleTextColor(int);
float calculateWPM(int, float, int);
float calAccuracy(int,int);


typedef struct {
    char username[MAX_LEN];
    char password[ARRAY_SIZE];
    int wpm[ARRAY_SIZE];
    float accuracy[ARRAY_SIZE];
    int no;
} User;

int main() {
    int authmode, i, gamemode;
    char usrname[25], password[25];
    printf("WELCOME TO QuickFingers \n");
    printf("Press 1 If Existing User \nPress 2 If You Dont Have An Account \n");
    scanf("%d", &authmode);
    clearConsole();
    while (1) {
        if (authmode == 1) {
            printf("---Login---");
            printf("Please Enter UserName : ");
            scanf("%s", usrname);

            printf("Please Enter Password : ");
            scanf("%s", password);

            int loginResult = login(usrname, password);
            if (loginResult == 1) {
                printf("Login Successful\n");
                break; // Exit the while loop after successful login
            } else if (loginResult == 2) {
                printf("No Account Exists \n Please Create An Account !! \n");
                authmode = 2; // Set authmode to 2 to call the signup process
            } else {
                printf("Invalid Password! Try Again\n");
                //clearConsole();
            }
        }
        if (authmode == 2) {
            if (signup(usrname, password)) {
                authmode = 1;
            } else {
                printf("Error Creating Account! Try Again\n");
            } 
        }
    }

    while (1) {
        clearConsole();
        printf(" TEST YOUR TYPING SKILLS !! \n Press 1 For Words \n Press 2 For Timed Test : ");
        scanf("%d", &gamemode);

        if (gamemode == 1) {
            gamewords();
            printf("Bye game mode 1");
            break;
        } else if (gamemode == 2) {
            //gametimer();
            printf("bye");
            break;
        } else {
            printf("Enter A Valid Choice \n");
        }
    }
    return 0;
}

void clearConsole() {
    system("cls");
}

int validatepass(char* password) {
    int len = 0;
    int i;
    int hasCap = 0, hasSpl = 0, hasNum = 0;
    for (i = 0; password[i] != '\0'; i++) {
        len++;
    }

    for (i = 0; i < len; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') {
            hasCap = 1;
        } else if (password[i] >= '0' && password[i] <= '9') {
            hasNum = 1;
        } else if ((password[i] >= 33 && password[i] <= 38) || (password[i] == 64)) {
            hasSpl = 1;
        }
    }

    if (hasCap == 1 && hasNum == 1 && hasSpl == 1) {
        return 1;
    } else {
        return 0;
    }
}

int login(char* username, char* password) {
    FILE *fp;
    char filename[MAX_LEN + 4]; // +4 for ".txt" and null terminator
    strcpy(filename, "auth/");
    strcat(filename, username);
    strcat(filename, ".txt");

    fp = fopen(filename, "r");
    if (fp != NULL) {
        User user;
        fread(&user, sizeof(User), 1, fp);
        fclose(fp);
        if (strcmp(user.password, password) == 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 2;
    }
}

int signup(char* usrname, char* password) {
    FILE *fp;

    printf("---Creating New Account---");

    printf("Please Enter UserName : ");
    scanf("%s", usrname);

    while (1) {
        printf("Please Enter Password (Password Must Include A capital letter, A Special Character, A Number) : ");
        scanf("%s", password);

        int valid = validatepass(password);

        if (valid == 1) {
            User newuser;
            strcpy(newuser.username, usrname);
            strcpy(newuser.password, password);
            newuser.no = 0;
            for (int i = 0; i < ARRAY_SIZE; i++) {
                newuser.wpm[i] = 0;
            }
            for (int i = 0; i < ARRAY_SIZE; i++) {
                newuser.accuracy[i] = 0;
            }

            char filename[MAX_LEN + 4]; // +4 for ".txt" and null terminator
            strcpy(filename, "auth/");
            strcat(filename, usrname);
            strcat(filename, ".txt");

            fp = fopen(filename, "w");
            if (fp == NULL) {
                printf("\n ERROR CREATING ACCOUNT");
                return 0;
            } else {
                fwrite(&newuser, sizeof(User), 1, fp);
                fclose(fp);

                printf("Account Created Successfully\n Redirecting You To Login Screen !! \n");
                messagetimer(3);
                clearConsole();
                return 1;
            }
        } else {
            printf("Retype The Password Which Satisfies the Conditions !!\n");
        }
    }
    clearConsole();
    return 0; // Added to avoid compilation warning
}


void gamewords()
{
    clearConsole();
    int words, diff;
    printf("\t\t\t SpeedScribbler\n");
    
    while(1)
    {
        printf("Press \n1. 60 Word Test\n2.90 Word Test\n3.120 Word Test\n");
        scanf("%d", &words);
        if (words == 1 || words == 2 || words == 3)
        {
            printf("\nEnter Difficulty \n1. EASY\n2. MODERATE\n3. HARD \n");
            scanf("%d", &diff);
            if ((diff == 1) || (diff == 2) || (diff == 3))
            {
                break;
            }
            else
            {
                printf("Enter Valid Difficulty Choice !!\n");
            }
        }
        else
        {
            printf("Enter Valid Word Count Choice !!\n");
        }
    }
    
    clearConsole();
    printf("--- %d Words (%d) Difficulty ---\n", words, diff);
    printf("Your Test Starts In 3 Seconds !!\n");
    messagetimer(3);
    clearConsole();
    
    selectTest(words, diff);
}


void messagetimer(int time){
    //clearConsole();
    for (int i = time; i >0; i--) 
    {
        printf("%d", i);
        fflush(stdout);
        Sleep(1000);
        printf("\r");
    }
}

void selectTest(int words,int diff)
{
    char filename[MAX_LEN + 4]; // +4 for ".txt" and null terminator
    if(words == 1)
    {
        // 60 words
        strcpy(filename, "files\\");
        strcat(filename, "words\\");
        strcat(filename, "60 words\\");
        
        startTest(filename,diff);    
    }
    else if(words == 2)
    {
        // 90 words
        strcpy(filename, "files\\");
        strcat(filename, "words\\");
        strcat(filename, "90 words\\");
        
        startTest(filename,diff);
    }

    else if(words == 3)
    {
        // 120 words
        strcpy(filename, "files\\");
        strcat(filename, "words\\");
        strcat(filename, "120 words\\");
        
        startTest(filename,diff);
    }

}

void setConsoleTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void startTest(char* filename, int diff) {
    FILE *fp;
    char readarray[READ_TEXT_SIZE];
    char *paragraphs[MAX_PARAGRAPHS];
    int paragraph_count = 0;

    // Add difficulty level to filename
    if (diff == 1) {
        strcat(filename, "easy.txt");
    } else if (diff == 2) {
        strcat(filename, "medium.txt");
    } else {
        strcat(filename, "hard.txt");
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    // Read the entire file into readarray
    size_t n = fread(readarray, 1, READ_TEXT_SIZE - 1, fp);
    readarray[n] = '\0';  // Null-terminate the read content

    fclose(fp);

    // Split the content into paragraphs based on double newline characters
    char *token = strtok(readarray, "\n\n");
    while (token != NULL && paragraph_count < MAX_PARAGRAPHS) {
        paragraphs[paragraph_count++] = token;
        token = strtok(NULL, "\n\n");
    }

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Randomly select a paragraph
    if (paragraph_count > 0) {
        int random_index = rand() % paragraph_count;

        // Display the randomly selected paragraph
        printf("Type the following text:\n");
        const char* text_to_type = paragraphs[random_index];
        displayTextWithColor(text_to_type, strlen(text_to_type));
        handleTypingTest(text_to_type);
    } else {
        printf("No paragraphs found.\n");
    }
}

void displayTextWithColor(const char* text, int text_length) {
    // Set text color to default (white)
    setConsoleTextColor(0x07);// white

    // Print the text to type
    printf("%s\n", text);
}

void handleTypingTest(const char* correct_text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char user_input[READ_TEXT_SIZE];
    int index = 0;
    int length = strlen(correct_text);
    int ch;
    int errors= 0;
    //int accerror = 0;
    clock_t start_time, end_time;

    start_time = clock();

    while (index < length) {
        ch = _getch(); // Read a single character without echo
        if (ch == 8) { // Backspace
            if (index > 0) {
                //errors--;
                index--;
                printf("\b \b"); // Move cursor back, print space, move cursor back again
                fflush(stdout);
            }
        } else if (ch == 13) { // Enter
            printf("Test INcomplete !!");
            break; // End input on Enter
        } else {
            user_input[index] = ch;
            user_input[index + 1] = '\0';
            index++;

            // Compare input with correct text
            if (user_input[index - 1] == correct_text[index - 1]) {
                // Correct character
                setConsoleTextColor(0x0A);//green
            } else {
                // Incorrect character
                errors++;
                //accerror++;
                setConsoleTextColor(0x0C);
            }

            // Print the character
            printf("%c", user_input[index - 1]);
            fflush(stdout);

            // Reset text color to default
            setConsoleTextColor(0x07);
        }
    }
    end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Reset text color to default
    setConsoleTextColor(0x07);
    printf("\n");

    float acc = calAccuracy(length, errors);
    float wpm = calculateWPM(length, elapsed_time, errors);

    printf("\nYour Accuracy: %.2f%%\n", acc);
    printf("Net WPM: %.2f\n", wpm);

}

float calAccuracy(int totalchars, int errorchars) {
    if (totalchars == 0) return 100.0; // Avoid division by zero
    float correct_chars = totalchars - errorchars;
    float acc = (correct_chars / totalchars) * 100.0f; // Ensure floating-point division
    return acc;
}


float calculateWPM(int totalchars, float elapsed_time, int errors) {
    // Convert elapsed time to minutes
    float elapsed_time_minutes = elapsed_time / 60.0;
    if (elapsed_time_minutes == 0) return 0.0; // Avoid division by zero

    // Convert characters to words (assuming 5 characters per word)
    float word_count = totalchars / 5.0;

    // Gross WPM
    float gross_wpm = word_count / elapsed_time_minutes;

    // Net WPM calculation
    float net_wpm = gross_wpm - (errors / elapsed_time_minutes);

    return net_wpm;
}