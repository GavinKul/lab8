
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

volatile sig_atomic_t largest = 2;
volatile sig_atomic_t interrupted = 0;

void handler(int sig) {
    interrupted = 1;
}

int main() {
    struct termios old, new;
    int num = 3;

    signal(SIGINT, handler);

    while (1) {
        if (interrupted) {
            char answer;

            interrupted = 0;

            printf("\n%d  Quit [y/n]? ", (int)largest);
            fflush(stdout);

            tcgetattr(STDIN_FILENO, &old);
            new = old;
            new.c_lflag &= ~(ICANON | ECHO);
            new.c_cc[VMIN] = 1;
            new.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &new);

            if (read(STDIN_FILENO, &answer, 1) != 1)
                answer = 'n';

            tcsetattr(STDIN_FILENO, TCSANOW, &old);

            printf("%c\n", answer);

            if (answer == 'y' || answer == 'Y')
                return 0;
        }

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

