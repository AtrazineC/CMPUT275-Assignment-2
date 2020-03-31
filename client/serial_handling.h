#ifndef __SERIAL_HANDLING_H
#define __SERIAL_HANDLING_H

#include "consts_and_types.h"
#include "map_drawing.h"

/**
 * This function gets all the waypoints from the server until there are no more
 * waypoints. To determine if successful, the function returns 1 if it was
 * successful and 0 if it was unsuccessful. If successful, the number of
 * waypoints and the waypoints themselves are stored into the appropriate 
 * fields of the external variable `shared`.
 * 
 * If the number of waypoints is greater than max_waypoints, a message is 
 * printed indicating the path is too long.
 * 
 * @param start The start point
 * @param end The end point
 * 
 * @return 1 if successful, 0 if unsuccessful.
 */
uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end);

#endif
