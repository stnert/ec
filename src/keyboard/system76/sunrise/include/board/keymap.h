// SPDX-License-Identifier: GPL-3.0-only

#ifndef _BOARD_KEYMAP_H
#define _BOARD_KEYMAP_H

// Keymap layers (normal, Fn)
#define KM_LAY 2
// Keymap output pins
#define KM_OUT 12
// Keymap input pins
#define KM_IN 8

// common/keymap.h requires KM_LAY, KM_OUT, and KM_IN definitions
#include <common/keymap.h>

// Conversion of physical ANSI layout to keyboard matrix
// K0E is power button, it is not included
#define LAYOUT( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, K0D,   K0F, \
    K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, K1D,   K1E, \
    K20,   K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, K2D, K2E, \
    K30,    K31, K32, K33, K34, K35, K36, K37, K38, K39, K3A, K3B, K3C,     K3D, \
    K40,      K41, K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B,        K4C, \
    K50, K51, K52, K53, K54,                     K55, K56, K57, K58, K59,   K5A, \
                                                                     K60 \
) { \
    { K41, K21, K31, K11, K10, K03, K02, ___ }, \
    { K42, K22, K32, K12, K30, ___, K04, ___ }, \
    { K43, K23, K33, K13, K00, K01, K20, K05 }, \
    { ___, K0D, K06, K1E, ___, K60, K54, ___ }, \
    { K2A, ___, K3A, K1A, K1B, K2C, K08, K4A }, \
    { K25, K24, K14, K34, K45, K35, K44, K15 }, \
    { K26, K27, K17, K37, K46, K36, K47, K16 }, \
    { K48, K28, K38, K18, K4C, K3D, K07, K3B }, \
    { K49, K29, K39, K19, ___, K52, ___, ___ }, \
    { K2D, K0F, K1C, K09, K5A, K58, K2E, ___ }, \
    { K3C, K56, K2B, K0A, K0C, K1D, K0B, K59 }, \
    { K40, K4B, K53, K55, K51, K57, K50, ___ } \
}

// Position of physical Esc key in the matrix
#define MATRIX_ESC_INPUT    4
#define MATRIX_ESC_OUTPUT   2

// Position of physical Fn key in the matrix
#define MATRIX_FN_INPUT     4
#define MATRIX_FN_OUTPUT    11

#endif // _BOARD_KEYMAP_H
