
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>

// Number of circles
#define NUM_CIRCLES 500

// Circle Struct
struct Circle {
  int x;  // x coordinate
  int y;  // y coordinate
  int r;  // circle radius
  int cr; // color - RED
  int cg; // color - GREEN
  int cb; // color - BLUE
};

// Circle Animation Data Struct
struct CircleAnimationData {
  int x;  // x coordinate
  int y;  // y coordinate
  int r;  // circle radius
  int xv; // x - Axis velocity
  int yv; // y - Axis velocity
  int xd; // x - axis direction (1 = foward)
  int yd; // y - axis direction (1 = foward)
};


// Circles variable
struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];

// Random number generator
int getRand(max) {
  return (rand() % max);
}

// Init circle data and start render - JS
int main() {

  // Seed random number generator
  srand(time(NULL));

  // Create circles
  for( int i = 0; i < NUM_CIRCLES; i++ ) {

    // Radius
    int radius = getRand(50);

    // Coordinates
    int x = getRand(1000) + radius;
    int y = getRand(1000) + radius;

    // fill animation data struct - i
    animationData[i].x = x;
    animationData[i].y = y;
    animationData[i].r = radius;
    animationData[i].xv = getRand(10);
    animationData[i].yv = getRand(10);
    animationData[i].xd = 1;
    animationData[i].yd = 1;

    // Fill circle struct - i
    circles[i].x = x;
    circles[i].y = y;
    circles[i].r = radius;
    circles[i].cr = getRand(255);
    circles[i].cg = getRand(255);
    circles[i].cb = getRand(255);

  }

  // Start JS rendering
  EM_ASM({ render($0, $1); }, NUM_CIRCLES*6, 6 );
}

// Return animated circles to JS
struct Circle * getCircles(int canvasWidth, int canvasHeight) {

  // Update circle data
  for (int i = 0; i < NUM_CIRCLES; i++) {
    // Collsion RIGHT - set x direction backwards 0
    if( (animationData[i].x + animationData[i].r) >= canvasWidth ) animationData[i].xd = 0;

    // Collsion Left - set x direction backwards 1
    if( (animationData[i].x - animationData[i].r) <= 0 ) animationData[i].xd = 1;

    // Collsion TOP - set y direction backwards 1
    if( (animationData[i].y - animationData[i].r) <= 0 ) animationData[i].yd = 1;

    // Collsion BOTTOM - set y direction backwards 0
    if( (animationData[i].y + animationData[i].r) >= canvasHeight ) animationData[i].yd = 0;

    // Move circle in specific direction
    if (animationData[i].xd == 1) {
      animationData[i].x += animationData[i].xv;
    } else {
      animationData[i].x -= animationData[i].xv;
    }

    if (animationData[i].yd == 1) {
      animationData[i].y += animationData[i].yv;
    } else {
      animationData[i].y -= animationData[i].yv;
    }

    // Update matching circles
    circles[i].x = animationData[i].x;
    circles[i].y = animationData[i].y;
  }
  return circles;
}
