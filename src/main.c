#include "chat.h"

int main(int argc, char *argv[]) {
    init_chat(argc > 1 ? argv[1] : NULL);
    run_chat();
    cleanup_chat();
    return 0;
}

