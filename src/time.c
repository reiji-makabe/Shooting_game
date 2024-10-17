#include "shooting.h"

void	sleep_for_milliseconds(int milliseconds) {
    struct timespec req = {0};
    req.tv_sec = milliseconds / 1000;
    req.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&req, NULL);
}
