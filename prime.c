
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

volatile sig_atomic_t largest = 2;

void handler(int sig) {
    struct termios old, new;
    char answer;

    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);

    printf("\n%d  Quit [y/n]? ", (int)largest);
    fflush(stdout);

    if (read(STDIN_FILENO, &answer, 1) == 1) {
        printf("%c\n", answer);
    } else {
        answer = 'n';
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    if (answer == 'y' || answer == 'Y')
        exit(0);
}

int main() {
    int num = 3;

    signal(SIGINT, handler);

    while (1) {
        int prime = 1;

        for (int i = 2; i < num; i++) {
            if (num % i == 0) {
                prime = 0;
                break;
            }
        }

        if (prime)
            largest = num;

        num++;
    }

    return 0;
}
