#include <stdio.h>
#include <io.h>
#include "PShader.h"
#include "CTextures.h"
#include "CEffect.h"
#include "types.h"

//*******************************************************************************************************
// Constants to improve code readability
//*******************************************************************************************************

#define MAX_LAND_TESTS  10
#define TEST_SWITCH     (MAX_LAND_TESTS/2)
#define HLSL_TECHNIQUE  1
#define ASM_TECHNIQUE   0

//*******************************************************************************************************
//    Landscape static vertex and index information.
//*******************************************************************************************************

typedef struct
{
    float   x;
    float   y;
    float   z;
    float   nx;
    float   ny;
    float   nz;
} LANDVERTEX, *PLANDVERTEX;

//*******************************************************************************************************
//      Begin Landcape Vertices
//*******************************************************************************************************

LANDVERTEX  LandVertex [] = {
    {-100.000000f,  -100.000000f,   1.157579f,  0.0f,   0.0f,   0.0f},    {100.000000f,   -100.000000f,   52.271778f, 0.0f,   0.0f,   0.0f},    {100.000000f,   100.000000f,    45.954170f, 0.0f,   0.0f,   0.0f},    {-100.000000f,  100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},
    {0.000000f,     -100.000000f,   32.992500f, 0.0f,   0.0f,   0.0f},    {-100.000000f,  0.000000f,      0.000000f,  0.0f,   0.0f,   0.0f},    {100.000000f,   0.000000f,      56.760860f, 0.0f,   0.0f,   0.0f},    {0.000000f,     100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},
    {0.000000f,     0.000000f,      17.351452f, 0.0f,   0.0f,   0.0f},    {-50.000000f,   -100.000000f,   15.364509f, 0.0f,   0.0f,   0.0f},    {-100.000000f,  -50.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {0.000000f,     -50.000000f,    15.412491f, 0.0f,   0.0f,   0.0f},
    {-50.000000f,   0.000000f,      0.948729f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   -50.000000f,    2.429963f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   -100.000000f,   6.990741f,  0.0f,   0.0f,   0.0f},    {-100.000000f,  -75.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},
    {-50.000000f,   -75.000000f,    4.817202f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   -50.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   -75.000000f,    3.324356f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   -100.000000f,   5.707117f,  0.0f,   0.0f,   0.0f},
    {-100.000000f,  -87.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   -87.500000f,    6.283494f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   -75.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   -87.500000f,    3.160995f,  0.0f,   0.0f,   0.0f},
    {-100.000000f,  -62.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   -62.500000f,    0.173565f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   -50.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   -62.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},
    {-62.500000f,   -100.000000f,   12.583229f, 0.0f,   0.0f,   0.0f},    {-50.000000f,   -87.500000f,    9.982474f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   -75.000000f,    2.979867f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   -87.500000f,    9.817303f,  0.0f,   0.0f,   0.0f},
    {-50.000000f,   -62.500000f,    1.925946f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   -50.000000f,    0.280297f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   -62.500000f,    1.161214f,  0.0f,   0.0f,   0.0f},    {-100.000000f,  -25.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},
    {-50.000000f,   -25.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   0.000000f,      0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   -25.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-100.000000f,  -37.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},
    {-75.000000f,   -37.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   -25.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   -37.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-100.000000f,  -12.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},
    {-75.000000f,   -12.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   0.000000f,      0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   -12.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   -37.500000f,    0.773163f,  0.0f,   0.0f,   0.0f},
    {-62.500000f,   -25.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   -37.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   -12.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   0.000000f,      0.000000f,  0.0f,   0.0f,   0.0f},
    {-62.500000f,   -12.500000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-25.000000f,   -100.000000f,   21.924986f, 0.0f,   0.0f,   0.0f},    {0.000000f,     -75.000000f,    23.386967f, 0.0f,   0.0f,   0.0f},    {-25.000000f,   -50.000000f,    6.131757f,  0.0f,   0.0f,   0.0f},
    {-25.000000f,   -75.000000f,    9.359191f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   -100.000000f,   17.601990f, 0.0f,   0.0f,   0.0f},    {-25.000000f,   -87.500000f,    14.448404f, 0.0f,   0.0f,   0.0f},    {-37.500000f,   -75.000000f,    5.100697f,  0.0f,   0.0f,   0.0f},
    {-37.500000f,   -87.500000f,    13.003797f, 0.0f,   0.0f,   0.0f},    {-25.000000f,   -62.500000f,    9.809659f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   -50.000000f,    5.785961f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   -62.500000f,    4.665762f,  0.0f,   0.0f,   0.0f},
    {-12.500000f,   -100.000000f,   30.180357f, 0.0f,   0.0f,   0.0f},    {0.000000f,     -87.500000f,    30.414499f, 0.0f,   0.0f,   0.0f},    {-12.500000f,   -75.000000f,    15.237624f, 0.0f,   0.0f,   0.0f},    {-12.500000f,   -87.500000f,    24.194473f, 0.0f,   0.0f,   0.0f},
    {0.000000f,     -62.500000f,    20.063745f, 0.0f,   0.0f,   0.0f},    {-12.500000f,   -50.000000f,    10.410098f, 0.0f,   0.0f,   0.0f},    {-12.500000f,   -62.500000f,    12.811353f, 0.0f,   0.0f,   0.0f},    {0.000000f,     -25.000000f,    17.593008f, 0.0f,   0.0f,   0.0f},
    {-25.000000f,   0.000000f,      8.149773f,  0.0f,   0.0f,   0.0f},    {-25.000000f,   -25.000000f,    10.021423f, 0.0f,   0.0f,   0.0f},    {-25.000000f,   -37.500000f,    8.377707f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   -25.000000f,    4.048883f,  0.0f,   0.0f,   0.0f},
    {-37.500000f,   -37.500000f,    3.793490f,  0.0f,   0.0f,   0.0f},    {-25.000000f,   -12.500000f,    7.225012f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   0.000000f,      4.913954f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   -12.500000f,    3.130815f,  0.0f,   0.0f,   0.0f},
    {0.000000f,     -37.500000f,    18.370600f, 0.0f,   0.0f,   0.0f},    {-12.500000f,   -25.000000f,    11.703689f, 0.0f,   0.0f,   0.0f},    {-12.500000f,   -37.500000f,    13.497591f, 0.0f,   0.0f,   0.0f},    {0.000000f,     -12.500000f,    16.475233f, 0.0f,   0.0f,   0.0f},
    {-12.500000f,   0.000000f,      10.921152f, 0.0f,   0.0f,   0.0f},    {-12.500000f,   -12.500000f,    13.729685f, 0.0f,   0.0f,   0.0f},    {-100.000000f,  50.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {0.000000f,     50.000000f,     10.051336f, 0.0f,   0.0f,   0.0f},
    {-50.000000f,   100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   50.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-100.000000f,  25.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   25.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-75.000000f,   50.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   25.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-100.000000f,  12.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   12.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-87.500000f,   25.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   12.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-100.000000f,  37.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   37.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-87.500000f,   50.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   37.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   12.500000f,     0.234917f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   25.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-62.500000f,   12.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   37.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   50.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   37.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-100.000000f,  75.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   75.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   75.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-100.000000f,  62.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   62.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   75.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   62.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-100.000000f,  87.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-75.000000f,   87.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-87.500000f,   87.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-50.000000f,   62.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   75.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   62.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-50.000000f,   87.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-62.500000f,   100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-62.500000f,   87.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {0.000000f,     25.000000f,     14.691806f, 0.0f,   0.0f,   0.0f},    {-25.000000f,   50.000000f,     3.455203f,  0.0f,   0.0f,   0.0f},
    {-25.000000f,   25.000000f,     5.211050f,  0.0f,   0.0f,   0.0f},    {-25.000000f,   12.500000f,     7.163913f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   25.000000f,     1.597536f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   12.500000f,     1.731950f,  0.0f,   0.0f,   0.0f},
    {-25.000000f,   37.500000f,     2.215325f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   50.000000f,     0.792790f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   37.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {0.000000f,     12.500000f,     17.904356f, 0.0f,   0.0f,   0.0f},
    {-12.500000f,   25.000000f,     9.813177f,  0.0f,   0.0f,   0.0f},    {-12.500000f,   12.500000f,     12.413867f, 0.0f,   0.0f,   0.0f},    {0.000000f,     37.500000f,     11.943870f, 0.0f,   0.0f,   0.0f},    {-12.500000f,   50.000000f,     8.484420f,  0.0f,   0.0f,   0.0f},
    {-12.500000f,   37.500000f,     7.852427f,  0.0f,   0.0f,   0.0f},    {0.000000f,     75.000000f,     3.899586f,  0.0f,   0.0f,   0.0f},    {-25.000000f,   100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-25.000000f,   75.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-25.000000f,   62.500000f,     0.642029f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   75.000000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   62.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {-25.000000f,   87.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {-37.500000f,   100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-37.500000f,   87.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},    {0.000000f,     62.500000f,     5.195411f,  0.0f,   0.0f,   0.0f},    {-12.500000f,   75.000000f,     1.417528f,  0.0f,   0.0f,   0.0f},
    {-12.500000f,   62.500000f,     1.182346f,  0.0f,   0.0f,   0.0f},    {0.000000f,     87.500000f,     1.987746f,  0.0f,   0.0f,   0.0f},    {-12.500000f,   100.000000f,    0.000000f,  0.0f,   0.0f,   0.0f},    {-12.500000f,   87.500000f,     0.000000f,  0.0f,   0.0f,   0.0f},
    {50.000000f,    -100.000000f,   38.569641f, 0.0f,   0.0f,   0.0f},    {100.000000f,   -50.000000f,    60.485474f, 0.0f,   0.0f,   0.0f},    {50.000000f,    0.000000f,      33.739082f, 0.0f,   0.0f,   0.0f},    {50.000000f,    -50.000000f,    40.672543f, 0.0f,   0.0f,   0.0f},
    {25.000000f,    -100.000000f,   39.045784f, 0.0f,   0.0f,   0.0f},    {50.000000f,    -75.000000f,    41.636353f, 0.0f,   0.0f,   0.0f},    {25.000000f,    -50.000000f,    31.420074f, 0.0f,   0.0f,   0.0f},    {25.000000f,    -75.000000f,    39.600311f, 0.0f,   0.0f,   0.0f},
    {12.500000f,    -100.000000f,   35.600201f, 0.0f,   0.0f,   0.0f},    {25.000000f,    -87.500000f,    37.465511f, 0.0f,   0.0f,   0.0f},    {12.500000f,    -75.000000f,    30.049545f, 0.0f,   0.0f,   0.0f},    {12.500000f,    -87.500000f,    35.347530f, 0.0f,   0.0f,   0.0f},
    {25.000000f,    -62.500000f,    33.787262f, 0.0f,   0.0f,   0.0f},    {12.500000f,    -50.000000f,    24.771652f, 0.0f,   0.0f,   0.0f},    {12.500000f,    -62.500000f,    28.646317f, 0.0f,   0.0f,   0.0f},    {37.500000f,    -100.000000f,   37.518265f, 0.0f,   0.0f,   0.0f},
    {50.000000f,    -87.500000f,    39.310745f, 0.0f,   0.0f,   0.0f},    {37.500000f,    -75.000000f,    41.729397f, 0.0f,   0.0f,   0.0f},    {37.500000f,    -87.500000f,    39.315006f, 0.0f,   0.0f,   0.0f},    {50.000000f,    -62.500000f,    41.621479f, 0.0f,   0.0f,   0.0f},
    {37.500000f,    -50.000000f,    37.890419f, 0.0f,   0.0f,   0.0f},    {37.500000f,    -62.500000f,    35.958008f, 0.0f,   0.0f,   0.0f},    {50.000000f,    -25.000000f,    40.205597f, 0.0f,   0.0f,   0.0f},    {25.000000f,    0.000000f,      21.173775f, 0.0f,   0.0f,   0.0f},
    {25.000000f,    -25.000000f,    20.995077f, 0.0f,   0.0f,   0.0f},    {25.000000f,    -37.500000f,    25.226471f, 0.0f,   0.0f,   0.0f},    {12.500000f,    -25.000000f,    20.522392f, 0.0f,   0.0f,   0.0f},    {12.500000f,    -37.500000f,    22.934565f, 0.0f,   0.0f,   0.0f},
    {25.000000f,    -12.500000f,    22.247755f, 0.0f,   0.0f,   0.0f},    {12.500000f,    0.000000f,      20.391790f, 0.0f,   0.0f,   0.0f},    {12.500000f,    -12.500000f,    17.715191f, 0.0f,   0.0f,   0.0f},    {50.000000f,    -37.500000f,    41.894646f, 0.0f,   0.0f,   0.0f},
    {37.500000f,    -25.000000f,    29.470762f, 0.0f,   0.0f,   0.0f},    {37.500000f,    -37.500000f,    34.501816f, 0.0f,   0.0f,   0.0f},    {50.000000f,    -12.500000f,    36.052460f, 0.0f,   0.0f,   0.0f},    {37.500000f,    0.000000f,      26.134695f, 0.0f,   0.0f,   0.0f},
    {37.500000f,    -12.500000f,    28.894424f, 0.0f,   0.0f,   0.0f},    {75.000000f,    -100.000000f,   43.459721f, 0.0f,   0.0f,   0.0f},    {100.000000f,   -75.000000f,    54.544830f, 0.0f,   0.0f,   0.0f},    {75.000000f,    -50.000000f,    43.375935f, 0.0f,   0.0f,   0.0f},
    {75.000000f,    -75.000000f,    40.488541f, 0.0f,   0.0f,   0.0f},    {62.500000f,    -100.000000f,   43.614632f, 0.0f,   0.0f,   0.0f},    {75.000000f,    -87.500000f,    40.558365f, 0.0f,   0.0f,   0.0f},    {62.500000f,    -75.000000f,    43.328812f, 0.0f,   0.0f,   0.0f},
    {62.500000f,    -87.500000f,    40.615658f, 0.0f,   0.0f,   0.0f},    {75.000000f,    -62.500000f,    44.351147f, 0.0f,   0.0f,   0.0f},    {62.500000f,    -50.000000f,    42.537094f, 0.0f,   0.0f,   0.0f},    {62.500000f,    -62.500000f,    43.513424f, 0.0f,   0.0f,   0.0f},
    {87.500000f,    -100.000000f,   45.793594f, 0.0f,   0.0f,   0.0f},    {100.000000f,   -87.500000f,    52.236824f, 0.0f,   0.0f,   0.0f},    {87.500000f,    -75.000000f,    47.164669f, 0.0f,   0.0f,   0.0f},    {87.500000f,    -87.500000f,    46.559139f, 0.0f,   0.0f,   0.0f},
    {100.000000f,   -62.500000f,    55.198303f, 0.0f,   0.0f,   0.0f},    {87.500000f,    -50.000000f,    52.387867f, 0.0f,   0.0f,   0.0f},    {87.500000f,    -62.500000f,    49.579601f, 0.0f,   0.0f,   0.0f},    {100.000000f,   -25.000000f,    55.059322f, 0.0f,   0.0f,   0.0f},
    {75.000000f,    0.000000f,      46.537136f, 0.0f,   0.0f,   0.0f},    {75.000000f,    -25.000000f,    43.372223f, 0.0f,   0.0f,   0.0f},    {75.000000f,    -37.500000f,    45.127895f, 0.0f,   0.0f,   0.0f},    {62.500000f,    -25.000000f,    40.407207f, 0.0f,   0.0f,   0.0f},
    {62.500000f,    -37.500000f,    42.782894f, 0.0f,   0.0f,   0.0f},    {75.000000f,    -12.500000f,    46.864132f, 0.0f,   0.0f,   0.0f},    {62.500000f,    0.000000f,      37.659222f, 0.0f,   0.0f,   0.0f},    {62.500000f,    -12.500000f,    40.444347f, 0.0f,   0.0f,   0.0f},
    {100.000000f,   -37.500000f,    54.959045f, 0.0f,   0.0f,   0.0f},    {87.500000f,    -25.000000f,    50.833523f, 0.0f,   0.0f,   0.0f},    {87.500000f,    -37.500000f,    52.860363f, 0.0f,   0.0f,   0.0f},    {100.000000f,   -12.500000f,    55.874489f, 0.0f,   0.0f,   0.0f},
    {87.500000f,    0.000000f,      51.832466f, 0.0f,   0.0f,   0.0f},    {87.500000f,    -12.500000f,    52.151649f, 0.0f,   0.0f,   0.0f},    {100.000000f,   50.000000f,     46.820431f, 0.0f,   0.0f,   0.0f},    {50.000000f,    100.000000f,    22.392103f, 0.0f,   0.0f,   0.0f},
    {50.000000f,    50.000000f,     38.405960f, 0.0f,   0.0f,   0.0f},    {50.000000f,    25.000000f,     31.180256f, 0.0f,   0.0f,   0.0f},    {25.000000f,    50.000000f,     18.765841f, 0.0f,   0.0f,   0.0f},    {25.000000f,    25.000000f,     20.965261f, 0.0f,   0.0f,   0.0f},
    {25.000000f,    12.500000f,     23.090967f, 0.0f,   0.0f,   0.0f},    {12.500000f,    25.000000f,     18.880741f, 0.0f,   0.0f,   0.0f},    {12.500000f,    12.500000f,     18.854727f, 0.0f,   0.0f,   0.0f},    {25.000000f,    37.500000f,     19.720770f, 0.0f,   0.0f,   0.0f},
    {12.500000f,    50.000000f,     15.590965f, 0.0f,   0.0f,   0.0f},    {12.500000f,    37.500000f,     16.143496f, 0.0f,   0.0f,   0.0f},    {50.000000f,    12.500000f,     32.283207f, 0.0f,   0.0f,   0.0f},    {37.500000f,    25.000000f,     24.315207f, 0.0f,   0.0f,   0.0f},
    {37.500000f,    12.500000f,     29.066223f, 0.0f,   0.0f,   0.0f},    {50.000000f,    37.500000f,     33.711517f, 0.0f,   0.0f,   0.0f},    {37.500000f,    50.000000f,     29.814749f, 0.0f,   0.0f,   0.0f},    {37.500000f,    37.500000f,     27.216986f, 0.0f,   0.0f,   0.0f},
    {50.000000f,    75.000000f,     35.935272f, 0.0f,   0.0f,   0.0f},    {25.000000f,    100.000000f,    10.514590f, 0.0f,   0.0f,   0.0f},    {25.000000f,    75.000000f,     12.202639f, 0.0f,   0.0f,   0.0f},    {25.000000f,    62.500000f,     16.490923f, 0.0f,   0.0f,   0.0f},
    {12.500000f,    75.000000f,     6.504263f,  0.0f,   0.0f,   0.0f},    {12.500000f,    62.500000f,     9.251613f,  0.0f,   0.0f,   0.0f},    {25.000000f,    87.500000f,     12.286880f, 0.0f,   0.0f,   0.0f},    {12.500000f,    100.000000f,    4.682819f,  0.0f,   0.0f,   0.0f},
    {12.500000f,    87.500000f,     8.120445f,  0.0f,   0.0f,   0.0f},    {50.000000f,    62.500000f,     38.706146f, 0.0f,   0.0f,   0.0f},    {37.500000f,    75.000000f,     21.521910f, 0.0f,   0.0f,   0.0f},    {37.500000f,    62.500000f,     30.027344f, 0.0f,   0.0f,   0.0f},
    {50.000000f,    87.500000f,     28.740870f, 0.0f,   0.0f,   0.0f},    {37.500000f,    100.000000f,    18.220098f, 0.0f,   0.0f,   0.0f},    {37.500000f,    87.500000f,     19.631273f, 0.0f,   0.0f,   0.0f},    {100.000000f,   25.000000f,     57.925961f, 0.0f,   0.0f,   0.0f},
    {75.000000f,    50.000000f,     39.985836f, 0.0f,   0.0f,   0.0f},    {75.000000f,    25.000000f,     42.519089f, 0.0f,   0.0f,   0.0f},    {75.000000f,    12.500000f,     44.924366f, 0.0f,   0.0f,   0.0f},    {62.500000f,    25.000000f,     37.000488f, 0.0f,   0.0f,   0.0f},
    {62.500000f,    12.500000f,     36.771801f, 0.0f,   0.0f,   0.0f},    {75.000000f,    37.500000f,     41.835995f, 0.0f,   0.0f,   0.0f},    {62.500000f,    50.000000f,     37.406384f, 0.0f,   0.0f,   0.0f},    {62.500000f,    37.500000f,     36.005566f, 0.0f,   0.0f,   0.0f},
    {100.000000f,   12.500000f,     59.287617f, 0.0f,   0.0f,   0.0f},    {87.500000f,    25.000000f,     50.004478f, 0.0f,   0.0f,   0.0f},    {87.500000f,    12.500000f,     52.399055f, 0.0f,   0.0f,   0.0f},    {100.000000f,   37.500000f,     55.134460f, 0.0f,   0.0f,   0.0f},
    {87.500000f,    50.000000f,     43.542706f, 0.0f,   0.0f,   0.0f},    {87.500000f,    37.500000f,     49.251568f, 0.0f,   0.0f,   0.0f},    {100.000000f,   75.000000f,     42.673733f, 0.0f,   0.0f,   0.0f},    {75.000000f,    100.000000f,    28.527487f, 0.0f,   0.0f,   0.0f},
    {75.000000f,    75.000000f,     36.332592f, 0.0f,   0.0f,   0.0f},    {75.000000f,    62.500000f,     40.224876f, 0.0f,   0.0f,   0.0f},    {62.500000f,    75.000000f,     36.871021f, 0.0f,   0.0f,   0.0f},    {62.500000f,    62.500000f,     40.684044f, 0.0f,   0.0f,   0.0f},
    {75.000000f,    87.500000f,     32.368046f, 0.0f,   0.0f,   0.0f},    {62.500000f,    100.000000f,    24.078209f, 0.0f,   0.0f,   0.0f},    {62.500000f,    87.500000f,     29.004343f, 0.0f,   0.0f,   0.0f},    {100.000000f,   62.500000f,     44.468121f, 0.0f,   0.0f,   0.0f},
    {87.500000f,    75.000000f,     37.275261f, 0.0f,   0.0f,   0.0f},    {87.500000f,    62.500000f,     41.723854f, 0.0f,   0.0f,   0.0f},    {100.000000f,   87.500000f,     44.050758f, 0.0f,   0.0f,   0.0f},    {87.500000f,    100.000000f,    40.201881f, 0.0f,   0.0f,   0.0f},
    {87.500000f,    87.500000f,     38.967472f, 0.0f,   0.0f,   0.0f}};

//*******************************************************************************************************
//      Begin Landcape indices
//*******************************************************************************************************

WORD    LandIndices[] = {0,19,23,   0,23,20,   20,23,22,   20,22,15,   19,14,21,   19,21,23,   23,21,18,   23,18,22,   15,22,27,   15,27,24,   24,27,26,   24,26,10,
                        22,18,25,   22,25,27,   27,25,17,   27,17,26,   14,28,31,   14,31,21,   21,31,30,   21,30,18,   28,9,29,   28,29,31,   31,29,16,   31,16,30,
                        18,30,34,   18,34,25,   25,34,33,   25,33,17,   30,16,32,   30,32,34,   34,32,13,   34,13,33,   10,26,42,   10,42,39,   39,42,41,   39,41,35,
                        26,17,40,   26,40,42,   42,40,38,   42,38,41,   35,41,46,   35,46,43,   43,46,45,   43,45,5,   41,38,44,   41,44,46,   46,44,37,   46,37,45,
                        17,33,49,   17,49,40,   40,49,48,   40,48,38,   33,13,47,   33,47,49,   49,47,36,   49,36,48,   38,48,52,   38,52,44,   44,52,51,   44,51,37,
                        48,36,50,   48,50,52,   52,50,12,   52,12,51,   9,57,60,   9,60,29,   29,60,59,   29,59,16,   57,53,58,   57,58,60,   60,58,56,   60,56,59,
                        16,59,63,   16,63,32,   32,63,62,   32,62,13,   59,56,61,   59,61,63,   63,61,55,   63,55,62,   53,64,67,   53,67,58,   58,67,66,   58,66,56,
                        64,4,65,   64,65,67,   67,65,54,   67,54,66,   56,66,70,   56,70,61,   61,70,69,   61,69,55,   66,54,68,   66,68,70,   70,68,11,   70,11,69,
                        13,62,76,   13,76,47,   47,76,75,   47,75,36,   62,55,74,   62,74,76,   76,74,73,   76,73,75,   36,75,79,   36,79,50,   50,79,78,   50,78,12,
                        75,73,77,   75,77,79,   79,77,72,   79,72,78,   55,69,82,   55,82,74,   74,82,81,   74,81,73,   69,11,80,   69,80,82,   82,80,71,   82,71,81,
                        73,81,85,   73,85,77,   77,85,84,   77,84,72,   81,71,83,   81,83,85,   85,83,8,   85,8,84,   5,45,97,   5,97,94,   94,97,96,   94,96,90,
                        45,37,95,   45,95,97,   97,95,93,   97,93,96,   90,96,101,   90,101,98,   98,101,100,   98,100,86,   96,93,99,   96,99,101,   101,99,92,
                        101,92,100,   37,51,104,   37,104,95,   95,104,103,   95,103,93,   51,12,102,   51,102,104,   104,102,91,   104,91,103,   93,103,107,   93,107,99,
                        99,107,106,   99,106,92,   103,91,105,   103,105,107,   107,105,89,   107,89,106,   86,100,115,   86,115,112,   112,115,114,   112,114,108,   100,92,113,
                        100,113,115,   115,113,111,   115,111,114,   108,114,119,   108,119,116,   116,119,118,   116,118,3,   114,111,117,   114,117,119,   119,117,110,   119,110,118,
                        92,106,122,   92,122,113,   113,122,121,   113,121,111,   106,89,120,   106,120,122,   122,120,109,   122,109,121,   111,121,125,   111,125,117,   117,125,124,
                        117,124,110,   121,109,123,   121,123,125,   125,123,88,   125,88,124,   12,78,131,   12,131,102,   102,131,130,   102,130,91,   78,72,129,   78,129,131,
                        131,129,128,   131,128,130,   91,130,134,   91,134,105,   105,134,133,   105,133,89,   130,128,132,   130,132,134,   134,132,127,   134,127,133,   72,84,137,
                        72,137,129,   129,137,136,   129,136,128,   84,8,135,   84,135,137,   137,135,126,   137,126,136,   128,136,140,   128,140,132,   132,140,139,   132,139,127,
                        136,126,138,   136,138,140,   140,138,87,   140,87,139,   89,133,146,   89,146,120,   120,146,145,   120,145,109,   133,127,144,   133,144,146,   146,144,143,
                        146,143,145,   109,145,149,   109,149,123,   123,149,148,   123,148,88,   145,143,147,   145,147,149,   149,147,142,   149,142,148,   127,139,152,   127,152,144,
                        144,152,151,   144,151,143,   139,87,150,   139,150,152,   152,150,141,   152,141,151,   143,151,155,   143,155,147,   147,155,154,   147,154,142,   151,141,153,
                        151,153,155,   155,153,7,   155,7,154,   4,164,167,   4,167,65,   65,167,166,   65,166,54,   164,160,165,   164,165,167,   167,165,163,   167,163,166,
                        54,166,170,   54,170,68,   68,170,169,   68,169,11,   166,163,168,   166,168,170,   170,168,162,   170,162,169,   160,171,174,   160,174,165,   165,174,173,
                        165,173,163,   171,156,172,   171,172,174,   174,172,161,   174,161,173,   163,173,177,   163,177,168,   168,177,176,   168,176,162,   173,161,175,   173,175,177,
                        177,175,159,   177,159,176,   11,169,183,   11,183,80,   80,183,182,   80,182,71,   169,162,181,   169,181,183,   183,181,180,   183,180,182,   71,182,186,
                        71,186,83,   83,186,185,   83,185,8,   182,180,184,   182,184,186,   186,184,179,   186,179,185,   162,176,189,   162,189,181,   181,189,188,   181,188,180,
                        176,159,187,   176,187,189,   189,187,178,   189,178,188,   180,188,192,   180,192,184,   184,192,191,   184,191,179,   188,178,190,   188,190,192,   192,190,158,
                        192,158,191,   156,197,200,   156,200,172,   172,200,199,   172,199,161,   197,193,198,   197,198,200,   200,198,196,   200,196,199,   161,199,203,   161,203,175,
                        175,203,202,   175,202,159,   199,196,201,   199,201,203,   203,201,195,   203,195,202,   193,204,207,   193,207,198,   198,207,206,   198,206,196,   204,1,205,
                        204,205,207,   207,205,194,   207,194,206,   196,206,210,   196,210,201,   201,210,209,   201,209,195,   206,194,208,   206,208,210,   210,208,157,   210,157,209,
                        159,202,216,   159,216,187,   187,216,215,   187,215,178,   202,195,214,   202,214,216,   216,214,213,   216,213,215,   178,215,219,   178,219,190,   190,219,218,
                        190,218,158,   215,213,217,   215,217,219,   219,217,212,   219,212,218,   195,209,222,   195,222,214,   214,222,221,   214,221,213,   209,157,220,   209,220,222,
                        222,220,211,   222,211,221,   213,221,225,   213,225,217,   217,225,224,   217,224,212,   221,211,223,   221,223,225,   225,223,6,   225,6,224,   8,185,234,
                        8,234,135,   135,234,233,   135,233,126,   185,179,232,   185,232,234,   234,232,231,   234,231,233,   126,233,237,   126,237,138,   138,237,236,   138,236,87,
                        233,231,235,   233,235,237,   237,235,230,   237,230,236,   179,191,240,   179,240,232,   232,240,239,   232,239,231,   191,158,238,   191,238,240,   240,238,229,
                        240,229,239,   231,239,243,   231,243,235,   235,243,242,   235,242,230,   239,229,241,   239,241,243,   243,241,228,   243,228,242,   87,236,249,   87,249,150,
                        150,249,248,   150,248,141,   236,230,247,   236,247,249,   249,247,246,   249,246,248,   141,248,252,   141,252,153,   153,252,251,   153,251,7,   248,246,250,
                        248,250,252,   252,250,245,   252,245,251,   230,242,255,   230,255,247,   247,255,254,   247,254,246,   242,228,253,   242,253,255,   255,253,244,   255,244,254,
                        246,254,258,   246,258,250,   250,258,257,   250,257,245,   254,244,256,   254,256,258,   258,256,227,   258,227,257,   158,218,264,   158,264,238,   238,264,263,
                        238,263,229,   218,212,262,   218,262,264,   264,262,261,   264,261,263,   229,263,267,   229,267,241,   241,267,266,   241,266,228,   263,261,265,   263,265,267,
                        267,265,260,   267,260,266,   212,224,270,   212,270,262,   262,270,269,   262,269,261,   224,6,268,   224,268,270,   270,268,259,   270,259,269,   261,269,273,
                        261,273,265,   265,273,272,   265,272,260,   269,259,271,   269,271,273,   273,271,226,   273,226,272,   228,266,279,   228,279,253,   253,279,278,   253,278,244,
                        266,260,277,   266,277,279,   279,277,276,   279,276,278,   244,278,282,   244,282,256,   256,282,281,   256,281,227,   278,276,280,   278,280,282,   282,280,275,
                        282,275,281,   260,272,285,   260,285,277,   277,285,284,   277,284,276,   272,226,283,   272,283,285,   285,283,274,   285,274,284,   276,284,288,   276,288,280,
                        280,288,287,   280,287,275,   284,274,286,   284,286,288,   288,286,2,   288,2,287};

//*******************************************************************************************************
                        
CLandscape2_0::CLandscape2_0(LPTSTR str) : CPShaderX()
{
	m_Options.D3DOptions.fMinDXVersion = 9.f; // !!! 8 maybe?
	m_Options.D3DOptions.dwBehaviors  = DEVICECREATE_MIXED | DEVICECREATE_SW_OR_HW_VP;
	m_Options.D3DOptions.dwRefBehaviors = DEVICECREATE_MIXED | DEVICECREATE_SW_OR_HW_VP;
	
	m_szCommandKey="Land20";
	m_szTestName="Landscape, Pixel Shader 2.0";
	
	m_Options.ModeOptions.TextureFmt = FMT_ALL;
	m_Options.ModeOptions.dwTexExclude  |= PF_BUMPDUDV | PF_BUMPLUMINANCE | PF_METN | PF_ALPHA;
	
    m_pLandColorTex = NULL;
    m_pRockCoverTex = NULL;
    m_pDetailTex = NULL;
    m_pGroundCoverTex = NULL;
    m_pRockMaskTex = NULL;
    m_pWaterTex = NULL;
    m_pEffect = NULL;
    
    m_dwFXCreateFlags = 0;  // Perform optimization, else the HLSL compiled landscape is too big.
}

//*******************************************************************************************************

bool CLandscape2_0::CapsCheck(void)
{
    if (!CPShaderX::CapsCheck())
        return false;
    
    DWORD dwSupported = m_pSrcDevice->GetCaps()->dwPixelShaderVersion;
    
    // Check major version
    
    if (D3DSHADER_VERSION_MAJOR(dwSupported) < 2)
    {
        WriteToLog(_T("Pixel shader version 2.0 required! Device supports %d.%d!\n"),
                   D3DSHADER_VERSION_MAJOR(dwSupported), D3DSHADER_VERSION_MINOR(dwSupported));
                   
        return false;
    }
/*    
    dwSupported = m_pSrcDevice->GetCaps()->dwVertexShaderVersion;
    
    // Check major version
    
    if (D3DSHADER_VERSION_MAJOR(dwSupported) < 2)
    {
        WriteToLog(_T("Vertex shader version 2.0 required! Device supports %d.%d!\n"),
                   D3DSHADER_VERSION_MAJOR(dwSupported), D3DSHADER_VERSION_MINOR(dwSupported));
                   
        return false;
    }
*/    
    return true;
}

//*******************************************************************************************************

UINT CLandscape2_0::TestInitialize(void)
{
    // Before we start, determine if the current texture formats can handle linear filtering
    // AND wrapping.

    LRESULT lResult = m_pD3D->CheckDeviceFormat(m_pSrcDevice->GetAdapterID(), 
                                                m_pSrcDevice->GetDevType(),
                                                m_pCurrentMode->pDisplay->Format,
                                                USAGE_QUERY_FILTER, 
                                                RTYPE_TEXTURE, 
                                                m_pCommonTextureFormats[m_pCurrentMode->nTextureFormat]);


    if (FAILED(lResult))
    {
        WriteToLog("Test requires linear filtering support\n");
        return D3DTESTINIT_SKIPALL;
    }

    if( m_pD3D->GetRuntimeVersion() >= 9.029f)
    {
        lResult = m_pD3D->CheckDeviceFormat(m_pSrcDevice->GetAdapterID(), 
                                            m_pSrcDevice->GetDevType(),
                                            m_pCurrentMode->pDisplay->Format,
                                            USAGE_QUERY_WRAPANDMIP, 
                                            RTYPE_TEXTURE, 
                                            m_pCommonTextureFormats[m_pCurrentMode->nTextureFormat]);

        if (FAILED(lResult))
        {
            WriteToLog("Test requires texture wrapping\n");
            return D3DTESTINIT_SKIPALL;
        }
    }

    // Load the .FX file.
    
    LPD3DXBUFFER    pErrors;

    DWORD dwSupported = m_pSrcDevice->GetCaps()->dwVertexShaderVersion;
    
    // Check major version
    
    if (D3DSHADER_VERSION_MAJOR(dwSupported) < 2)
    {
        m_dwFXCreateFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
        if ( FAILED( m_pSrcDevice->SetSoftwareVertexProcessing(TRUE)) )
		{
			WriteToLog("SetSoftwareVertexProcessing() failed.\n");
			return D3DTESTINIT_SKIPALL;
		}
    }
    
    bool bResult = CreateEffectFromResource(NULL, "Landscape_ps2_0.fx", m_pMacros, &m_Include, m_dwFXCreateFlags, NULL, &m_pEffect, &pErrors);


	if (!bResult)
	{
		WriteToLog( "CreateEffectFromResource() failed.\n" );
		if( pErrors )
		{
			LPCTSTR errorString = (LPCTSTR) pErrors->GetBufferPointer();
			if( errorString )
				WriteToLog( "%s\n", errorString );
		}
		return D3DTESTINIT_ABORT;
	}    
    // Load the textures in the current supported format.

    FORMAT *pTexFormat = &m_pCommonTextureFormats[m_pCurrentMode->nTextureFormat];

    D3DXCreateTextureFromResourceExA(NULL,"LandColors.PNG",64,64,0,0,pTexFormat->d3dfFormat,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pLandColorTex);
    D3DXCreateTextureFromResourceExA(NULL,"RockCover.BMP",256,256,0,0,pTexFormat->d3dfFormat,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pRockCoverTex);
    D3DXCreateTextureFromResourceExA(NULL,"Detail.PNG",128,128,0,0,pTexFormat->d3dfFormat,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pDetailTex);
    D3DXCreateTextureFromResourceExA(NULL,"GroundCover.BMP",256,256,0,0,pTexFormat->d3dfFormat,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pGroundCoverTex);
    D3DXCreateTextureFromResourceExA(NULL,"RockMask.PNG",64,64,0,0,pTexFormat->d3dfFormat,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pRockMaskTex);
    D3DXCreateTextureFromResourceExA(NULL,"water.PNG",256,256,0,0,pTexFormat->d3dfFormat,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pWaterTex);
    
    // Generate vertex normals
    
    for (int i=0; i<sizeof(LandIndices)/sizeof(WORD); i+=3)
    {
        D3DXVECTOR3 V1(LandVertex[LandIndices[i]].x,LandVertex[LandIndices[i]].y,LandVertex[LandIndices[i]].z);
        D3DXVECTOR3 V2(LandVertex[LandIndices[i+1]].x,LandVertex[LandIndices[i+1]].y,LandVertex[LandIndices[i+1]].z);
        D3DXVECTOR3 V3(LandVertex[LandIndices[i+2]].x,LandVertex[LandIndices[i+2]].y,LandVertex[LandIndices[i+2]].z);
        
        D3DXVECTOR3 R1 = V2-V1;
        D3DXVECTOR3 R2 = V3-V1;
        
        D3DXVECTOR3 N;
        
        D3DXVec3Cross(&N,&R1,&R2);
        D3DXVec3Normalize(&N,&N);

        for (int x=i; x<i+3; ++x)
        {
            LandVertex[LandIndices[x]].nx += N.x;
            LandVertex[LandIndices[x]].ny += N.y;
            LandVertex[LandIndices[x]].nz += N.z;
        }
    }

    // Normalize the vertex normals

    for (int i = 0; i<sizeof(LandVertex)/sizeof(LANDVERTEX); ++i)
    {
        D3DXVECTOR3 N(LandVertex[i].nx,LandVertex[i].ny,LandVertex[i].nz);

        D3DXVec3Normalize(&N,&N);

        LandVertex[i].nx = N.x;
        LandVertex[i].ny = N.y;
        LandVertex[i].nz = N.z;
    }
    
    // Set the test range
    
    SetTestRange(1,MAX_LAND_TESTS);

    // All set, let's go.

    return D3DTESTINIT_RUN;
}

//*******************************************************************************************************

bool CLandscape2_0::ExecuteTest(UINT uTestNumber)
{
    float 		fAspect = 1.33f;
    float		fFOV = pi/4.0f;
    D3DXMATRIX  Projection;
    D3DXMATRIX  View;
    D3DXMATRIX  World;
    D3DXVECTOR3 vCamera(-150.0f,150.0f,125.0f);
    D3DXVECTOR3 vAt(0.0f,0.0f,0.0f);
    D3DXVECTOR3 vUp(0.0f,0.0f,1.0f);

    // Begin the test case

    if (uTestNumber > TEST_SWITCH)
        BeginTestCase("Landscape 2.0 HLSL Rendering",uTestNumber);
    else
        BeginTestCase("Landscape 2.0 ASM Rendering",uTestNumber);
    
    // Setup the camera

    D3DXMatrixPerspectiveFovRH(&Projection, fFOV, fAspect, 1.0f, 4000.0f);
    D3DXMatrixLookAtRH(&View,  &vCamera, &vAt, &vUp);
    D3DXMatrixIdentity(&World);

    // Build the final "projected" matrix used for vertex transformation.
    
    D3DXMatrixMultiply(&Projection,&View,&Projection);

    m_pEffect->SetValue("Projection",&Projection,sizeof(D3DXMATRIX));
    
    // Set the textures in the pixel shader.
    
    m_pEffect->SetTexture("Tex0",m_pLandColorTex);
    m_pEffect->SetTexture("Tex1",m_pRockCoverTex);
    m_pEffect->SetTexture("Tex2",m_pDetailTex);
    m_pEffect->SetTexture("Tex3",m_pGroundCoverTex);
    m_pEffect->SetTexture("Tex4",m_pRockMaskTex);
    m_pEffect->SetTexture("Tex5",m_pWaterTex);
    
    // Define cloud and water motion multipliers

    float   CloudMotion[] = {uTestNumber * 0.02f, uTestNumber * 0.02f, 0.0f, 0.0f};
    float   WaterMotion[] = {-sinf(uTestNumber * 0.5f) * 0.05f, -cosf(uTestNumber * 0.5f) * 0.05f, 0.0f, 0.0f};

    // .ASM constants

    SetPixelShaderConstantF(11,CloudMotion,1);
    SetPixelShaderConstantF(12,WaterMotion,1);

    // HLSL constants
    
    m_pEffect->SetValue("WaterOffset",WaterMotion,sizeof(WaterMotion));
    m_pEffect->SetValue("CloudOffset",CloudMotion,sizeof(CloudMotion));

    // Specify the technique/pass in the effect
    
    UINT uPasses = 0;
    
    if (uTestNumber > TEST_SWITCH)
        m_pEffect->SetTechniqueByIndex(HLSL_TECHNIQUE);
    else
        m_pEffect->SetTechniqueByIndex(ASM_TECHNIQUE);

    m_pEffect->Begin(&uPasses,0);
    m_pEffect->BeginPass(0);
    
    // Render the landscape

    BeginScene();

    SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
    SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);
    SetRenderState(D3DRS_LIGHTING,FALSE);
    
    DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 
                            0, 
                            sizeof(LandVertex)/sizeof(LANDVERTEX), 
                            sizeof(LandIndices)/sizeof(WORD) / 3,
                            LandIndices, 
                            FMT_INDEX16, 
                            LandVertex,
                            sizeof(LANDVERTEX));

    // Done with this effect.

    m_pEffect->EndPass();
    m_pEffect->End();

    // All done.

    EndScene();

    return true;
}

//*******************************************************************************************************

void CLandscape2_0::Cleanup(void)
{
    RELEASE(m_pEffect);
    RELEASE(m_pLandColorTex);
    RELEASE(m_pRockCoverTex);
    RELEASE(m_pDetailTex);
    RELEASE(m_pGroundCoverTex);
    RELEASE(m_pRockMaskTex);
    RELEASE(m_pWaterTex);
    
    CPShaderX::Cleanup();
}