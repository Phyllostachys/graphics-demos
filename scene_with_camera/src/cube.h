//        y
//        |
//        |
//        4-------5
//       /       /|
//      /   t   / |
//     /       /  |
//    7-------6 r 1----- x
//    |       |  /
//    |   f   | /
//    |       |/
//    3-------2
//   /
//  /
// z

#define NUM_CUBE_VERTS (3 * 8)
#define NUM_CUBE_IND   (2 * 3 * 6)

float cubeVerts[NUM_CUBE_VERTS] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f
};

unsigned int cubeIndices[NUM_CUBE_IND] = {
    // bottom
    0, 1, 2,
    2, 3, 0,

    // front
    3, 2, 6,
    6, 7, 3,

    // left
    0, 3, 7,
    7, 4, 0,

    // back
    0, 4, 5,
    5, 1, 0,

    // right
    1, 5, 6,
    6, 2, 1,

    // top
    4, 7, 6,
    6, 5, 4
};
