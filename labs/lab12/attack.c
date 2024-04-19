#include "lab.h"
#include <string.h>
/* forward function for "attack 0" case.
   change this code to implement your attack
 */
void forward_attack_0(struct message *message) {
    if (strcmp(message->data, "PAY $0438 TO M") == 0){
        strcpy(message->data, "PAY $10000438 TO M");
        message->data_size = strlen(message->data);
        
    }
    send_message(message);
}

/* forward function for "attack 1" case.
   change this code to implement your attack
 */
void forward_attack_1(struct message *message) {
        char buffer[1024];
        decrypt_message_for_M(message, buffer, sizeof(buffer));

        if (strstr(buffer, "PAY $0438 TO M") != NULL) {
            strcpy(buffer, "PAY $10000438 TO M");
            struct message *new_msg = new_message(message->from, message->to, buffer, true, false);
            send_message(new_msg);
        } else {
            send_message(message);
    }
    send_message(message);
}

/* forward function for "attack 2" case.
   change this code to implement your attack
 */
void forward_attack_2(struct message *message) {
    send_message(message);
}

/* forward function for "attack 3" case.
   change this code to implement your attack
 */
void forward_attack_3(struct message *message) {
    send_message(message);
}

/* forward function for "attack 4" case.
   change this code to implement your attack */
void forward_attack_4(struct message *message) {
    send_message(message);
}

/* forward function for "attack 5" case.
   I did not intend this one to be possible. */
void forward_attack_5(struct message *message) {
    send_message(message);
}
