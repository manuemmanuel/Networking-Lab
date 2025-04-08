#include <stdio.h>
#include <stdlib.h>

struct frame {
    int info;
    int seq;
};

int ak;
int t = 5, k;
int disconnect = 0;
struct frame p;
char turn = 's';          // Initialize first turn as sender
int errorframe = 1;       // No error initially
int errorack = 1;

void sender();
void receiver();

void main() {
    p.info = 0;  // Data part
    p.seq = 0;   // Sequence number

    while (!disconnect) {
        sender(); // Call sender

        for (k = 1; k <= 10000000; k++); // Simulate delay

        receiver(); // Call receiver after delay
    }
}

void sender() {
    static int flag = 0;

    if (turn == 's') { // Sender's turn
        if (errorack == 0) { // ACK didn't arrive
            printf("SENDER: sent packet with seq NO: %d\n", p.seq);
            errorframe = rand() % 4; // Randomly pick error frame
            printf("%s\n", (errorframe == 0 ? "Error While sending Packet" : ""));
            turn = 'r'; // Set turn to receiver
        }
        else {
            if (flag == 1)
                printf("SENDER: Received ACK for packet %d\n", ak);

            if (p.seq == 5) {
                disconnect = 1;
                return;
            }

            p.info = p.info + 1;
            p.seq = p.seq + 1;

            printf("SENDER: sent packet with seq NO: %d\n", p.seq);
            errorframe = rand() % 4;
            printf("%s\n", (errorframe == 0 ? "Error While sending Packet" : ""));

            turn = 'r'; // Set next turn as receiver
            flag = 1;
        }
    }
    else { // Receiver's turn (simulating timeout)
        t--;
        printf("SENDER time reducing\n");

        if (t == 0) {
            turn = 's';
            errorack = 0;
            t = 5;
        }
    }
}

void receiver() {
    static int frexp = 1;

    if (turn == 'r') {
        if (errorframe != 0) {
            if (p.seq == frexp) { // Expected frame received
                printf("RECEIVER: Received packet with seq %d\n", p.seq);
                ak = p.seq;
                frexp = frexp + 1;
                turn = 's';
                errorack = rand() % 4;
                printf("%s\n", (errorack == 0 ? "Error While sending ACK" : ""));
            }
            else { // Duplicated frame
                printf("RECEIVER: Duplicated packet with seq %d\n", frexp - 1);
                ak = frexp - 1;
                turn = 's';
                errorack = rand() % 4;
                printf("%s\n", (errorack == 0 ? "Error While sending ACK" : ""));
            }
        }
    }
}
