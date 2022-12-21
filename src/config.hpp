#ifndef CONFIG_HPP
#define CONFIG_HPP

#define PI                      3.14159265
#define ROTATE_VAL              3.14159265358979323846 / 16
#define EPS 1e-3

#define SIZE_SC                 50
#define BRICK_TOP_HEIGHT        10
#define BRICK_TOP_SIZE_SC       0.3

#define PLATE_HEIGHT            20
#define BASE_Z                  8000
#define BASE_START              10, 10, BASE_Z

#define X_CENTER                500
#define Y_CENTER                390

#define SCALE_VAL               0.1

#define LIGHT_X                 1000
#define LIGHT_Y                 1000

#define MOVE_VAL                50

#define BLACK_COLOR            0, 0, 0

#define SHADOW_VAL              30

#define BASE_COLOR              220, 220, 220
#define BASE_COLOR_SHADOW       220 - SHADOW_VAL, 220 - SHADOW_VAL, 220 - SHADOW_VAL

#define BRICK_COLOR             255, 051, 051
#define BRICK_COLOR_SHADOW      255 - SHADOW_VAL, 051 - SHADOW_VAL, 051 - SHADOW_VAL

#define PLATE_COLOR             153, 255, 051
#define PLATE_COLOR_SHADOW      153 - SHADOW_VAL, 255 - SHADOW_VAL, 051 - SHADOW_VAL

#define TILE_COLOR              051, 255, 255
#define TILE_COLOR_SHADOW       051 - SHADOW_VAL, 255 - SHADOW_VAL, 255 - SHADOW_VAL

#define ARC_COLOR               255, 051, 255
#define ARC_COLOR_SHADOW        255 - SHADOW_VAL, 051 - SHADOW_VAL, 255 - SHADOW_VAL

#define CYLINDER_COLOR          255, 255, 051
#define CYLINDER_COLOR_SHADOW   255 - SHADOW_VAL, 255 - SHADOW_VAL, 051 - SHADOW_VAL

#endif
