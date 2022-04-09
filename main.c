#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define REVERSE_ALPHABET "zyxwvutsrqponmlkjihgfedcba"

unsigned short state = 0; // 16 bits
// we have 3 rotors-r1,r2,r3 of size 27 and rotor is the final rotor
// the size 27 -26 alphabets + 1 null char at the last
char r1[27] = "", r2[27] = "", r3[27] = "", rotor[27] = "";
FILE *file;

// read 3 rotors from enigma.txt file
void ReadRotors()
{
    //access()-comes under <unistd.h>-->> checks the file availability , F_OK checks its existence:-1 if file doesn't exist
    if (access(".enigma", F_OK) == -1)
    {
        printf("Key File is missing\n");
        exit(1);
    }
    // we are accepting rotor values from enigma.txt and storing it in r1,r2,r3
    FILE *FRotors = fopen(".enigma", "r"); //r reads the text file specifically whereas rb reads any type of file(pdf file, text file, etc)
    fscanf(FRotors, "%s %s %s", r1, r2, r3);  //we are accepting rotor values from enigma.txt and storing it in r1,r2,r3
    fclose(FRotors);                          // closing the stream and not the file
}

char FindIndex(const char *arr, char search)
{
    //  to find the size of a particular character array

    for (char i = 0; i < strlen(arr); i++)
        if (search == arr[i])
            return i;
    return -1;
    // return i returns the size of the char array
}

void WriteToFile(const char *format)
{
    bool CanAdd = true;
    register char i = 0;
    // we ignore 27th index as it contains a null char
    for (i = 0; i < 26; i++)
    {
        char random = (rand() % ('z' - 'a' + 1)) + 'a';
        for (register char j = 0; j < i; j++)
            if (random == rotor[j])
            {
                CanAdd = false;
                break;
            }
        if (CanAdd == true)
            rotor[i] = random;
        else
            i--;
        CanAdd = true;
    }
    rotor[i + 1] = '\0';
    fprintf(file, format, rotor);
}

// Enigma function: converts a normal character to an enigma character
// this func returns the enigma char
// this is where encryption happens
char Enigma(char character)
{
    if (character == 32)
        return character;
    char c1 = r1[character - 97];
    char c2 = r2[c1 - 97];
    char c3 = r3[c2 - 97];
    char reflect = REVERSE_ALPHABET[c3 - 97];
    c3 = ALPHABET[FindIndex(r3, reflect)];
    c2 = ALPHABET[FindIndex(r2, c3)];
    c1 = ALPHABET[FindIndex(r1, c2)];

    return c1;
}

//shift array converts 'abcdef' to 'bcdefa'
void ShiftArray(char *r)
{
    char temp[27];
    strcpy(temp, r);
    char tmp = r[0];
    // we ignore 27th index as it contains a null char
    for (char i = strlen(r) - 1; i > 0; i--)
        r[i - 1] = temp[i];
    r[strlen(r) - 1] = tmp;
}

//used to rotate the rotors
void RotateRotors()
{
    ShiftArray(r1);
    if (state % 24 == 0)
        ShiftArray(r2);
    if (state % (24 * 24) == 0)
        ShiftArray(r3);
}

//prints borders

void borders()
{
    for (int i = 1; i <= 11; i++)
    {

        printf("- * -\t");
    }
}

// write new rotors in enigma.txt

int main()
{
    for (;;)
    {
        char str[10000];
        char ch;
        FILE *fpointer1 = fopen("input.txt", "r");
        if (fpointer1 == NULL)
        {
            printf("Please Enter the text in input.txt file");
            exit(0);
        }
        ReadRotors();
        for (unsigned short i = 0; i < 10000; i++)
        {

            state++;
            ch = Enigma(fgetc(fpointer1));
            strncat(str, &ch, 1);
            RotateRotors();
        }
        fclose(fpointer1);

        
        int choice;
        // Part 1
        borders();
        printf("\nPlease Enter Your Choice -> \n");
        printf("\t 1. Encryption or Decryption\n");
        printf("\t 2. Save Data\n");
        printf("\t 3. Exit\n");
        printf("\t 4. Help\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:

        {
          
            borders();
            printf("\nThe Encrypted text is -->");
            printf("\n\t%s\n", str);
            printf("\n");
        }
        
        break;
        case 2:
        {
            FILE *filepointer = fopen("output.txt", "w");
            fprintf(filepointer, "%s", str);
            printf("Data has been saved in output.txt\n");
            fclose(filepointer);
            break;
        }
        case 3:
        {
            exit(0);
            break;
        }
        case 4:
        {
          borders();
          printf("\n");
          FILE *help = fopen("help.txt", "r");
          char ch;
    while ((ch = fgetc(help)) != EOF)
        putchar(ch);

        printf("\n");

    // close the file
    fclose(help);
    break;

}

        
        default:
        {
            printf("Wrong option\n");
        }
        break;
        }
    }
}





/*
->exit(0) indicates that the program terminated without errors. exit(1) indicates that there was an error.

*/