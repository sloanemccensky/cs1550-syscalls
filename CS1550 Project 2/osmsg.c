#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// user input error statements
void usage(char* program_name){

    fprintf(stderr, "Usage is: %s [-r|-s] <username> <msg>", program_name);
    fprintf(stderr, "-r : Receive a message");
    fprintf(stderr, "-s: Send a message <username> and <msg> required!");
    exit(-1);
    //printf(STDERR, "Not a valid input!");

}

// struct to take in message information
typedef struct message_s{

    char contents[255];
    char from_user[33];
    char to_user[33];

} message;

int send_msg(const char *to, char *msg, char *from) {
    syscall(441, to, msg, from);
}

int get_msg(char *to, char *msg, long msg_len, char *from, long from_len) {
    syscall(442, to, msg, msg_len, from, from_len);
}

int main(int argc, char *argv[]) {

    // current user's username
    char* user = getenv("USER");

    if( user == NULL ){
        // printf("Couldn't get the username :y");
        exit(-1);
    }

    // number of command line arguments is not 2 or 4
    if( argc != 2 && argc != 4 ){
	usage(argv[0]);
    }

    // checks to make sure argv[1] is either '-r' or '-s'
    if( argv[1][0] == '-' ){

	// if argv[1] is '-r', display user's messages
        if( argv[1][1] == 'r' ){

		message* new_m = (message*) malloc(sizeof(message));

		while( get_msg(user, new_m->contents, 255, new_m->from_user, 33) != -1 ){
			printf("%s said: \"%s\"\n", new_m->from_user, new_m->contents);
		}

		free(new_m);

	// if argv[1] is '-s', send message using user input information to specified recipient
	}else if( argv[1][1] == 's' ){
		send_msg(argv[2], argv[3], user);
	// else, command line input was incorrect
        }else{
		usage(argv[0]);
        }
    // incorrect command line input
    }else{
	usage(argv[0]);
    }

    return 0;

}
