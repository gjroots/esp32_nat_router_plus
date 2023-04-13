/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define MOUNT_PATH "/data"
#define HISTORY_PATH MOUNT_PATH "/history.txt"

    void initialize_console(void);
    void start_console(void);

#ifdef __cplusplus
}
#endif
