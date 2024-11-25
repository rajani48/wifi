#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 100
#define LINE_LENGTH 128

int main()
{
    FILE *fd = NULL;
    char line[LINE_LENGTH + 1];
    int lc = 0;

    fd = fopen("/dev/chdbg", "r");
    if (fd == NULL) {
        printf("File open error\n");
	return 1;
    }

    while (lc < MAX_LINES) {
        if (fgets(line, sizeof(line), fd) != NULL) {
            line[LINE_LENGTH] = '\0';
            printf("%s", line);
	    lc ++;
        } else {
            printf("End of file\n");
	    return 0;
        }
    }

    fclose(fd);
    return 0;
}
