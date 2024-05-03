// Implementing a text editor following https://viewsourcecode.org/snaptoken/kilo

/* includes */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/* data */
// store terminal attr in struct
struct termios orig_termios;


/* terminal */
// func to gracefully crash
void die(const char *s){
	perror(s);
	exit(1);
}

// func to reset terminal attr to original state
void disableRawMode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1){
		die("tcsetattr");
	};
}

// func to get terminal attrs and disable ECHO
void enableRawMode() { 
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag &= ~(CS8);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}



/* init */
int main() {
	enableRawMode();
	// while loop reads one char and stores it in c
	// read returns no. of bytes read and writes one byte to c
	while(1) {
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == 'q') break;

	}
	return 0;
}






