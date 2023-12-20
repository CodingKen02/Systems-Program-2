/*
----------------------------------------------------------
Program: Smain.c
Date: August 29, 2023
Student Name & NetID: Kennedy Keyes kfk38
Description: This program allows the user to open the accelerometer
data file to then create the output binary file by reading the given dataset.
Then the program processes 80 sets of data triples, combines the bytes
and then convert/print/write the acceleration values until the file is closed.
----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define BUF_SIZE 1024
#define DATA_FILE "raw.dat"
#define OUTPUT_FILE "accl.dat"

int main(int argc, char *argv[]) {
    // Open the accelerometer data file
    int fd = open(DATA_FILE, O_RDONLY);
    if (fd == -1) {
        perror("There's an error opening data file");
        exit(EXIT_FAILURE);
    }
    // Generate the binary file
    int pd = open(OUTPUT_FILE, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (pd == -1) {
        perror("There's an error creating output file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    unsigned char L, H;
    short val;
    double accl;
    
    // Process 80 sets of data triples
    for (int i = 0; i < 80; ++i) {
        // Handle X, Y, Z axes for each data triple
        for (int j = 0; j < 3; ++j) {
            // Read the lowest and highest order bytes
            ssize_t rdL = read(fd, &L, 1);
            ssize_t rdH = read(fd, &H, 1);
            if (rdL != 1 || rdH != 1) {
                perror("There's an error reading data");
                close(pd);
                close(fd);
                exit(EXIT_FAILURE);
            }
            // Combine the bytes to form a signed 16-bit value
            val = (H << 8) | L;

            // Convert the value to acceleration along the axis in multiples of g
            accl = (val / 32768.0) * 16;

            // Print the acceleration value and axis label to the console
            char axis = 'X' + j;
            printf("%c: %.6lfg", axis, accl);

            // Write the acceleration value to the binary output file
            ssize_t wr = write(pd, &accl, sizeof(double));
            if (wr == -1) {
                perror("There' an error writing data");
                close(pd);
                close(fd);
                exit(EXIT_FAILURE);
            }
            // Print commas, spacing, and newlines for dataset
            if (j < 2) {
                printf(", ");
            }
        }
        printf("\n");
    }
    // Close the files & exit program
    close(fd);
    close(pd);
    exit(EXIT_SUCCESS);
}
