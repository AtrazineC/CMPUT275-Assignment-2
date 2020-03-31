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

#include "serial_handling.h"

extern shared_vars shared;

/**
 * Given a string of two space-separated numbers, this function correctly
 * assigns the longitude and latitude of the correct spot in the waypoints
 * array.
 * 
 * @param line The line containing two space-separated integers
 * @param waypoint_counter Counter of which waypoint we're writing to
 */
void parseCoord(String& line, int waypoint_counter) {
  int i = 0;
  int end = line.length();

  // find the position of the space, which separates the two numbers
  while (line.charAt(i) != ' ') {
    i++;
  }

  // before the space
  int32_t lat = line.substring(0, i).toInt();

  //after the space
  int32_t lon = line.substring(i + 1, end - 1).toInt();

  shared.waypoints[waypoint_counter] = lon_lat_32(lon, lat);
}

/**
 * Reads the next line into the buffer and converts it to the arduino string
 * object, which we later process. It then returns the string, regardless
 * of timeout. If there was no timeout, it sets the pass-by-reference parameter
 * `success` to true.
 * 
 * @param success Reference to a bool that stores if the most recent line read 
 *                was successful
 * @param timeout Time to wait before timing out
 * 
 * @return Newly read line
 */
String readline(bool& success, unsigned long timeout = 1000) {
  success = false;

  char buffer[22];
  uint8_t buff_length = 0;
  
  // continously try and read input until timeout or end of line
  unsigned long start = millis();
  while ((millis() - start) <= timeout) { 
    char c = ' ';

    if (Serial.available()) {
      c = Serial.read();
      buffer[buff_length++] = c;
    }

    if (c == '\n' || c == '\r') { // new line -> end string
      buffer[buff_length] = '\0';
      success = true;
      break;
    }
  }

  // return the newly read line
  return String(buffer);
}

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
uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  // Send pathfinding request, then wait for a response
  Serial.print("R ");
  Serial.print(start.lat);
  Serial.print(" ");
  Serial.print(start.lon);
  Serial.print(" ");
  Serial.print(end.lat);
  Serial.print(" ");
  Serial.println(end.lon);

  bool success; // whether the most recent readline was successful
  String line = readline(success, 10000); // attempt reading in the line

  // Wait for server to calc path. Time limit 10 seconds.
  if (success && line[0] == 'N') {
    line = line.substring(2); // essentially strip off the first two characters
    shared.num_waypoints = line.toInt();
    Serial.println("A");

    for (int i = 0; i < shared.num_waypoints; i++) {
      line = readline(success);

      if (success && line[0] == 'W') {
        line = line.substring(2); // strip off the first two characters
        parseCoord(line, i);
        Serial.println("A");
      } else {
        return 0; // time out or error
      }
    }

    // Did not time out
    return 1;    
  }

  return 0;  
}
