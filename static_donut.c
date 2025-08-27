#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 20
#define R1 1.0f // Circle radius
#define R2 2.0f // Donut radius , distance of circle from the center
#define K2 5.0f // How much the donut is away in perception

// Calculate K1 based on screen size: the maximum x-distance occurs
// roughly at the edge of the torus, which is at x=R1+R2, z=0.  we
// want that to be displaced 3/8ths of the width of the screen, which
// is 3/4th of the way from the center to the side of the screen.
// SCREEN_WIDTH*3/8 = K1*(R1+R2)/(K2+0)
// SCREEN_WIDTH*K2*3/(8*(R1+R2)) = K1
#define K1 (SCREEN_WIDTH * K2 * 3) / (8 * (R1 + R2))

int main() {
  float z_buffer[SCREEN_WIDTH][SCREEN_HEIGHT] = {0};
  char output[SCREEN_WIDTH][SCREEN_HEIGHT];

  memset(output, 32, SCREEN_WIDTH * SCREEN_HEIGHT);
  memset(z_buffer, 0, 4 * SCREEN_WIDTH * SCREEN_HEIGHT);

  float phi = 0.0f, theta = 0.0f, theta_diff = 0.07, phi_diff = 0.02;
  float sin_theta, cos_theta, sin_phi, cos_phi;

  float circle_x, circle_y, circle_z = 0;

  // x,y,z in 3D plane after rotation
  float x3d, y3d, z3d;

  // Coordinates to plot on the screen
  int x, y;

  // Iterators for the output loop
  int i, j;

  float L;

  int luminance_index;
  float inverse_z;

  bool loop_condition = true;

  for (theta = 0.0; theta < 2 * PI; theta += theta_diff)
    for (phi = 0.0; phi < 2 * PI; phi += phi_diff) {
      sin_theta = sin(theta);
      cos_theta = cos(theta);
      sin_phi = sin(phi);
      cos_phi = cos(phi);

      circle_x = R2 + (R1 * cos_theta);
      circle_y = R1 * sin_theta;
      // x,y,z after 3d rotations
      x3d = circle_x * cos_phi;
      y3d = circle_y;
      z3d = K2 - ((circle_x)*sin_phi);

      // Calculate one over z
      inverse_z = 1.0f / z3d;

      // x,y to 2d space using similarity of triangles
      x = (int)((SCREEN_WIDTH / 2) + (x3d * inverse_z * K1));
      y = (int)((SCREEN_HEIGHT / 2) - (y3d * inverse_z * K1 / 2));

      // Luminascanse
      L = sin_theta + (cos_theta * sin_phi);

      if (L > 0)
        if (z_buffer[x][y] < inverse_z) {
          luminance_index = L * 8;
          z_buffer[x][y] = inverse_z;
          if (luminance_index >= 0 && luminance_index < 12)
            output[x][y] = ".,-~:;=!*#$@"[luminance_index];
        }
    }
  for (j = 0; j < SCREEN_HEIGHT; j++) {
    for (i = 0; i < SCREEN_WIDTH; i++) {
      putchar(output[i][j]);
    }
    putchar('\n');
  }
  return 0;
}
