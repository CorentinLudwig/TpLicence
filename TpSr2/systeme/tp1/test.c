#include <stdio.h>
#include <time.h>

int main() {
    time_t currentTime;
    struct tm *localTime;

    // Get the current time
    currentTime = time(NULL);

    // Convert the current time to the local time
    localTime = localtime(&currentTime);

    // Format and print the date and time
    char formattedTime[100];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", localTime);
    printf("Current date and time: %s\n", formattedTime);

    return 0;
}