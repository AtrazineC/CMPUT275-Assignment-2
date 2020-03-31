//::    //::    //::    //::    //::    //::    //::    //::    //::    //::
    //::    //::    //::    //::    //::    //::    //::    //::    //::    //:
//::
    //::    By:                 Benjamin Kong -- 1573684
//::                            Lora Ma -------- 1570935
    //::
//::        CMPUT 275:          Winter 2020
    //::    Assignment 2:       Part 2
//::
    //::    //::    //::    //::    //::    //::    //::    //::    //::    //:
//::    //::    //::    //::    //::    //::    //::    //::    //::    //::

#include "draw_route.h"
#include "map_drawing.h"

#define BLUE 0x001F

extern shared_vars shared;

/**
 * This function iterates through all the waypoints and connects them with a
 * line. It gets the coordinates of two adjacent waypoints, converts it to the
 * absolute position in terms of the screen, and draws a line using the tft
 * function `drawLine`.
 */
void draw_route() {
  int8_t zoomLevel = shared.map_number;
  int32_t map_x = shared.map_coords.x;
  int32_t map_y = shared.map_coords.y;

  for (int i = 0; i < shared.num_waypoints - 1; i++) {

    // Start point
    lon_lat_32 start = shared.waypoints[i];

    // Convert start point lat/lon to map x/y
    xy_pos xy_start = xy_pos(
      longitude_to_x(zoomLevel, start.lon),
      latitude_to_y(zoomLevel, start.lat)
    );

    // Convert to absolute screen x/y
    xy_pos abs_start = xy_pos(
      xy_start.x - map_x,
      xy_start.y - map_y
    );

    // End point
    lon_lat_32 end = shared.waypoints[i + 1];

    // Convert end point lat/lon to map x/y
    xy_pos xy_end = xy_pos(
      longitude_to_x(zoomLevel, end.lon),
      latitude_to_y(zoomLevel, end.lat)
    );

    // Convert to absolute screen x/y
    xy_pos abs_end = xy_pos(
      xy_end.x - map_x,
      xy_end.y - map_y
    );

    // Draw a line from the start absolute screen x/y 
    // to the end absolute screen x/y
    shared.tft->drawLine(abs_start.x, abs_start.y, 
                         abs_end.x, abs_end.y,
                         BLUE);
  }
}
