// +-+ Defines Header File +-+

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

// iterators, sprite indices, etc.
#define u8  uint8_t
// define bitboard data type
#define u64 unsigned long long

// avoid dangling pointers
#define FREE(ptr) free(ptr); ptr=NULL;

/*******************************************************************************
*                             BOARD REPRESENTATIONS                            *
*******************************************************************************/

// 0- white, 1- black, 2- number of players ;)
enum { white, black, PLYR_CNT };

// Although the game is correctly factored around rank_cnt and file_cnt
// (as far as I know), the fundamental nature of bitboards relies on 8-byte
// numbers to represent the state of each board square, so modifying the
// variables below will certainly result in undefined behavior ↓↓↓

// Technically if you modified all the hardcoded 8x8 bitboard values to a
// corresponding (ranks,files) "bitboard" value with 64 or fewer squares, at
// least the bitmasks should still work as intended.

// number of board square rows
#define RANK_CNT    8
// number of board square columns
#define FILE_CNT    8
// numbers of squares on the board
#define SQUARE_CNT  (RANK_CNT * FILE_CNT)
// converts 2d board representation into an index to the specific square on a board
#define GET_SQUARE(rank, file) (rank * FILE_CNT + file)
// converts 2d board representation into bit representation of the specific square
#define GET_SQUARE_BIT(rank, file) (1ull << GET_SQUARE(rank, file))

/*******************************************************************************
*                           BITBOARD REPRESENTATIONS                           *
*******************************************************************************/

// bitboard (file, rank) enumeration
enum
{
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

// converts a board square index to human-readable short-hand chess notation
extern const char *SQUARE_TO_SHORT_HAND[];

/*******************************************************************************
*                               BIT MANIPULATIONS                              *
*******************************************************************************/

// set/get/pop macros
#define SET_BIT(bitboard, square) (bitboard |= (1ull << square))
#define GET_BIT(bitboard, square) (bitboard &  (1ull << square))
#define POP_BIT(bitboard, square) (GET_BIT(bitboard, square) ? bitboard ^= (1ull << square) : 0)

// counts bits within bitboard by consecutively resetting the least
// significant 1st bit ... it's kinda magical ngl ...
static inline u8 COUNT_BITS(u64 bitboard)
{
    u8 cnt = 0;
    while(bitboard)
    {
        ++cnt;
        bitboard &= bitboard - 1;
    }
    return cnt;
}

// gets the least significant 1st bit's index within bitboard
static inline int8_t GET_LS1B_IDX(u64 bitboard)
{
    if (bitboard)
        // count trailing bits before ls1b
        return COUNT_BITS((bitboard & -bitboard) - 1);
    else
        // returns illegal index
        return -1;
}

#endif // DEFS_H