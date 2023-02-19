#include "quantum.h"

#define XXX KC_NO

#define LAYOUT( \
    L00, L01, L02, L03, L04,                     R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,                     R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,                     R20, R21, R22, R23, R24, \
         L30, L31,                                         R30, R31, \
                   L40, L41, L42, L43,      R40,           R41, \
                             L50, L51,           R50, R51 \
) { \
    { L04, L03, L02, L01, L00 }, \
    { L14, L13, L12, L11, L10 }, \
    { L24, L23, L22, L21, L20 }, \
    { L41, L40, L31, L30, XXX }, \
    { L42, L50, L51, L43, XXX }, \
\
    { R00, R01, R02, R03, R04 }, \
    { R10, R11, R12, R13, R14 }, \
    { R20, R21, R22, R23, R24 }, \
    { XXX, XXX, R30, R31, XXX }, \
    { XXX, R41, R51, R50, R40 } \
}
