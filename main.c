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

typedef struct {
    char username[MAX_LEN];
    char password[ARRAY_SIZE];
    int wpm[ARRAY_SIZE];
    float accuracy[ARRAY_SIZE];
    int no;
    float average_wpm;
} User;


void clearConsole(void); // clears the terminal
int validatepass(char *); // password validation function
int login(char*, char*); // login function
int signup(char*, char*); // signup function
void gamewords(char*); // go to NO_OF_WORDS test mode
void gametimer(void); // go to TIMER based test mode
void messagetimer(int); // function for display a timer for n seconds
void selectTest(int,int,char*); // function to select the type of test (-- words , -- difficulty)
void startTest(char*,int,char*); // function to start the test
void handleTypingTest(const char*,char*); // important function to display the acuuracy and wpm
void displayTextWithColor(const char*, int); // for green and red output
void setConsoleTextColor(int);
float calculateWPM(int, float, int); // to calculate wpm using formula
float calAccuracy(int,int); // to calculate accuracy
void makechangestouser(const char*,float); // update the user structure after each test
void history(char*); // function to display user's past performamce
void saveuser(char*); // save the username separately (for using it in leaderboard)
void showleaderboard(); // Shows the Name of users and Their Average WPM in Descending Order 
void sortuser(User[], int);


int main() {
    int authmode, i, gamemode;
    char usrname[25], password[25];
    printf("WELCOME TO QuickFingers \n");
    printf("Press 1 If Existing User \nPress 2 If You Dont Have An Account \nPress 3 To Check QuickFingers LeadebBoard ");
    scanf("%d", &authmode);
    clearConsole();
    while (1) {
        if (authmode == 1) {
            printf("---Login---\n");
            printf("Please Enter UserName : ");
            scanf("%s", usrname);

            printf("Please Enter Password : ");
            scanf("%s", password);

            int loginResult = login(usrname, password);
            if (loginResult == 1) {
                printf("Login Successful\n");
                break; 
            } else if (loginResult == 2) {
                printf("No Account Exists \n Please Create An Account !! \n");
                authmode = 2; 
            } else {
                printf("Invalid Password! Try Again\n");
                //clearConsole();
            }
        }
        else if(authmode == 2) {
            if (signup(usrname, password)) {
                authmode = 1;
            } else {
                printf("Error Creating Account! Try Again\n");
            } 
        }else if(authmode == 3)
        {
            showleaderboard();
            exit(0); // (When i entred 1 it showed abnormal Termination, so use 0 () indicates sucessful termination of program to the OS)
        }else{
            printf("Invalid Option !! \n");
        }
    }

    while (1) {
        clearConsole();
        printf(" TEST YOUR TYPING SKILLS !! \n Press 1 For Words \n Press 2 For Timed Test : ");
        scanf("%d", &gamemode);

        if (gamemode == 1) {
            gamewords(usrname);
            //printf("Bye game mode 1");
            break;
        } else if (gamemode == 2) {
            //gametimer();
            //printf("bye");
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

    printf("---Creating New Account---\n\n");

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
            newuser.average_wpm = 0.0;
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
                saveuser(strcat(usrname,".txt\n"));
                messagetimer(3);
                clearConsole();
                return 1;
            }
        } else {
            printf("Retype The Password Which Satisfies the Conditions !!\n");
        }
    }
    clearConsole();
    return 0; 
}


void gamewords(char* usrname)
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
    //printf("--- %d Words (%d) Difficulty ---\n", words, diff);
    printf("Your Test Starts In 3 Seconds !!\n");
    messagetimer(3);
    clearConsole();
    
    selectTest(words, diff,usrname);
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

void selectTest(int words,int diff,char* usrname)
{
    char filename[MAX_LEN + 4]; // +4 for ".txt" and null terminator
    if(words == 1)
    {
        // 60 words
        strcpy(filename, "files\\");
        strcat(filename, "words\\");
        strcat(filename, "60 words\\");
        
        startTest(filename,diff,usrname);    
    }
    else if(words == 2)
    {
        // 90 words
        strcpy(filename, "files\\");
        strcat(filename, "words\\");
        strcat(filename, "90 words\\");
        
        startTest(filename,diff,usrname);
    }

    else if(words == 3)
    {
        // 120 words
        strcpy(filename, "files\\");
        strcat(filename, "words\\");
        strcat(filename, "120 words\\");
        
        startTest(filename,diff,usrname);
    }

}

void setConsoleTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void startTest(char* filename, int diff,char* usrname) {
    FILE *fp;
    char readarray[READ_TEXT_SIZE];
    char *paragraphs[MAX_PARAGRAPHS];
    int paragraph_count = 0;

    
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
        handleTypingTest(text_to_type,usrname);
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

void handleTypingTest(const char* correct_text,char* usrname) {
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
            printf("Test Incomplete !!");
            exit(1);
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
    //printf("\n%s",usrname);

    makechangestouser(usrname,wpm);
    history(usrname);

}

float calAccuracy(int totalchars, int errorchars) {
    if (totalchars == 0) return 100.0; // Avoid division by zero
    float correct_chars = totalchars - errorchars;
    float acc = (correct_chars / totalchars) * 100.0f;
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

void makechangestouser(const char* usrname, float wpm) {
    User user1;
    FILE* fp;
    char filename[MAX_LEN + 4]; // +4 for ".txt" and null terminator

    // Create the filename
    strcpy(filename, "auth/");
    strcat(filename, usrname);
    strcat(filename, ".txt");

    // Open the file for reading
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("PROGRAM ERROR (updating user records) TRY AGAIN LATER !! SORRY FOR THE INCONVENIENCE \n");
        return;
    }

    // Read the user data from the file
    fread(&user1, sizeof(User), 1, fp);
    fclose(fp);  // Close the file after reading

    printf("Before Test : %d \n",user1.no);
    // Update the WPM and calculate the new average
    if (user1.no < ARRAY_SIZE) {
        user1.wpm[user1.no] = (int)wpm;
        user1.no++;
        printf("After Test : %d \n",user1.no);
    } else {
        // Optional: Handle the case when the array is full
        printf("WPM array is full. No further WPM can be added.\n");
        return;
    }

    float sum = 0.0;
    for (int i = 0; i < user1.no; i++) {
        sum += user1.wpm[i];
    }
    user1.average_wpm = sum / user1.no;

    // Open the file for writing
    fp = fopen(filename, "w");
    if (fp != NULL) {
        fwrite(&user1, sizeof(User), 1, fp);
        fclose(fp);  // Close the file after writing
    } else {
        printf("PROGRAM ERROR (updating user records) TRY AGAIN LATER !! SORRY FOR THE INCONVENIENCE \n");
    }
}

void history(char* usrname) {
    User user1;
    FILE* fp;
    char filename[MAX_LEN + 4]; // +4 for ".txt" and null terminator

    // Create the filename
    strcpy(filename, "auth/");
    strcat(filename, usrname);
    strcat(filename, ".txt");

    // Open the file for reading
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("PROGRAM ERROR (updating user records) TRY AGAIN LATER !! SORRY FOR THE INCONVENIENCE \n");
        return;
    }

    // Read the user data from the file
    fread(&user1, sizeof(User), 1, fp);
    fclose(fp); // Close the file after reading

    printf("\nAverage WPM for user %s is %.2f\n", usrname, user1.average_wpm);

    if (user1.no > 0) {
        int prev = user1.no - 1;
        printf("Your Previous Performance: %d WPM\n", user1.wpm[prev]);
    } else {
        printf("No previous performance data available.\n");
    }
}


void saveuser(char* usrname)
{
    FILE *fp;

    fp = fopen("user.txt","a");

    if(fp == NULL)
    {
        printf("\nError While Saving The User !! \n");
    }else
    {
        fprintf(fp,"%s",usrname);
    }
    fclose(fp);
}


void showleaderboard()
{
    FILE *fp = fopen("user.txt", "r");  // Open the file for reading
    User allusers[100];
    int count = 0;
    if (fp == NULL) {
        printf("\n\nError Loading The Leaderboard ! Sorry For The Incovenience !! \n\n");  // Handle the case where the file cannot be opened
        return;
    }
    char line[256];  
    while(fgets(line,sizeof(line),fp))
    {
        line[strcspn(line, "\n")] = '\0';
        
        char path[300];
        snprintf(path, sizeof(path), "auth\\%s", line);
       
       FILE *fp1 = fopen(path,"r");
       if (fp1 == NULL) {
            printf("\nError opening file: %s\n", path);
            continue;  // Skip this file and continue with the next one
        }

        if(fread(&allusers[count],sizeof(User),1,fp1) == 1)
        {
            count++;
        }else{
            printf("\n\nError Loading The Leaderboard ! Sorry For The Incovenience !! \n\n");
        }

        fclose(fp1);
    }
    fclose(fp);

    // SORTING THE STRUCTURE BASED ON AVERAGE WPM
    sortuser(allusers,count);
    printf("/n");
    // PRINTING THE SORTED ARRAY
    //clearConsole();
    printf("=============== QUICKFINGERS LEADERBOARD ===============\n\n");
    printf("USERNAME\t\tAVERAGE WPM");
    for(int i =0;i<count;i++)
    {
        printf("\n%s\t\t\t%f\n",allusers[i].username,allusers[i].average_wpm);
    } 
}

void sortuser(User allusers[],int count)
{
    int i, j, minIndex;
    User temp;
    for (i = 0; i < count - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < count; j++) {
            if (allusers[j].average_wpm > allusers[minIndex].average_wpm) {  // Sort in descending order
                minIndex = j;
            }
        }
        // Swap the found minimum element with the first element
        temp = allusers[minIndex];
        allusers[minIndex] = allusers[i];
        allusers[i] = temp;
    }
}