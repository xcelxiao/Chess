#include "atks.h"

#include <stdlib.h>

/* See header file for prototype descriptions */

const u8 bishop_relevant_bits[RANK_CNT * FILE_CNT] =
{
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

const u8 rook_relevant_bits[RANK_CNT * FILE_CNT] = 
{
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12    
};

u64 atks__pawn_table[PLYR_CNT][SQUARE_CNT];
u64 atks__knight_table[SQUARE_CNT];
u64 atks__king_table[SQUARE_CNT];
u64 atks__bishop_table[SQUARE_CNT];
u64 atks__rook_table[SQUARE_CNT];

u64 atks__pawn_mask(const u8 side, u8 square);
u64 atks__knight_mask(u8 square);
u64 atks__king_mask(u8 square);
u64 atks__bishop_mask(u8 square);
u64 atks__rook_mask(u8 square);

u64 atks__bishop_on_the_fly(u8 square, u64 block);
u64 atks__rook_on_the_fly(u8 square, u64 block);

void atks__init_leapers();

u64 atks__set_occupancy(u8 idx, int8_t bits_in_mask, u64 atk_mask);

static u8 update_atks(u64 *atks, const u64 *block, u8 rank, u8 file);

/**
 * Top Left: (0,0); Bottom Right: (FILE_CNT, RANK_CNT)
 * All movement logic is defined through commenting at end of line
*/

u64 atks__pawn_mask(u8 side, u8 square)
{
    // result attacks bitboard
    u64 atks = 0ull;

    // piece bitboard
    u64 bb = 0ull;

    // set piece on board
    SET_BIT(bb, square);
    
    // white pawns
    if (!side)
    {
        // edge guards white's pawn captures on either column end of the board
        if(bb >> (FILE_CNT - 1) & NOT_A_FILE) atks |= (bb >> (FILE_CNT - 1)); // (-1, -1)
        if(bb >> (FILE_CNT + 1) & NOT_H_FILE) atks |= (bb >> (FILE_CNT + 1)); // (+1, -1)
    }
    // black pawn
    else
    {
        // ... and same for black's pawn captures
        if(bb << (FILE_CNT - 1) & NOT_H_FILE) atks |= (bb << (FILE_CNT - 1)); // (+1, +1)
        if(bb << (FILE_CNT + 1) & NOT_A_FILE) atks |= (bb << (FILE_CNT + 1)); // (-1, +1)
    }

    // return attack map
    return atks;
}

u64 atks__knight_mask(u8 square)
{
    // result attacks bitboard
    u64 atks = 0ull;

    // piece bitboard
    u64 bb = 0ull;

    // set piece on board
    SET_BIT(bb, square);

    // knight attacks moving up, edge guarding moves on either column end of board
    // uses double guard if moving two squares left or right
    if ((bb >> (FILE_CNT * 2 + 1)) & NOT_H_FILE ) atks |= (bb >> (FILE_CNT * 2 + 1)); // (+1, +2)
    if ((bb >> (FILE_CNT * 2 - 1)) & NOT_A_FILE ) atks |= (bb >> (FILE_CNT * 2 - 1)); // (-1. +2)
    if ((bb >> (FILE_CNT     + 2)) & NOT_HG_FILE) atks |= (bb >> (FILE_CNT     + 2)); // (+2. +1)
    if ((bb >> (FILE_CNT     - 2)) & NOT_AB_FILE) atks |= (bb >> (FILE_CNT     - 2)); // (-2, +1)

    // knight attacks moving down ..
    if ((bb << (FILE_CNT * 2 + 1)) & NOT_A_FILE ) atks |= (bb << (FILE_CNT * 2 + 1)); // (-1, -2)
    if ((bb << (FILE_CNT * 2 - 1)) & NOT_H_FILE ) atks |= (bb << (FILE_CNT * 2 - 1)); // (+1. -2)
    if ((bb << (FILE_CNT     + 2)) & NOT_AB_FILE) atks |= (bb << (FILE_CNT     + 2)); // (-2. -1)
    if ((bb << (FILE_CNT     - 2)) & NOT_HG_FILE) atks |= (bb << (FILE_CNT     - 2)); // (+2, -1)

    return atks;    
}

u64 atks__king_mask(u8 square)
{
    // result attacks bitboard
    u64 atks = 0ull;

    // piece bitboard
    u64 bb = 0ull;

    // set piece on board
    SET_BIT(bb, square);

    // king attacks moving up, edge guarding moves on either column end of board
    if ( bb >>  FILE_CNT                   ) atks |= (bb >> (FILE_CNT    )); // (-0, +1)
    if ((bb >> (FILE_CNT + 1)) & NOT_H_FILE) atks |= (bb >> (FILE_CNT + 1)); // (-1, +1)
    if ((bb >> (FILE_CNT - 1)) & NOT_A_FILE) atks |= (bb >> (FILE_CNT - 1)); // (-1, -1)
    if ((bb >> 1             ) & NOT_H_FILE) atks |= (bb >> (           1)); // (-1, -0)
    // king attacks moving down ..
    if ( bb <<  FILE_CNT                   ) atks |= (bb << (FILE_CNT    )); // (+0, -1)
    if ((bb << (FILE_CNT + 1)) & NOT_A_FILE) atks |= (bb << (FILE_CNT + 1)); // (+1, -1)
    if ((bb << (FILE_CNT - 1)) & NOT_H_FILE) atks |= (bb << (FILE_CNT - 1)); // (+1, +1)
    if ((bb << 1             ) & NOT_A_FILE) atks |= (bb << (           1)); // (+1, +0)

    return atks;   
}

u64 atks__bishop_mask(u8 square)
{
    // result attacks bitboard
    u64 atks = 0ull;

    // rank, file
    int8_t  r, f;

    // target ranks and files
    u8  tr = square / FILE_CNT;
    u8  tf = square % FILE_CNT;

    // mask relveant bishop occupancy bits
    for (r = tr + 1, f = tf + 1; r <= RANK_CNT - 2 && f <= FILE_CNT - 2; ++r, ++f) atks |= (GET_SQUARE_BIT(r, f)); // (+.., +..)
    for (r = tr - 1, f = tf + 1; r >=            1 && f <= FILE_CNT - 2; --r, ++f) atks |= (GET_SQUARE_BIT(r, f)); // (+.., -..)
    for (r = tr + 1, f = tf - 1; r <= RANK_CNT - 2 && f >=            1; ++r, --f) atks |= (GET_SQUARE_BIT(r, f)); // (-.., +..)
    for (r = tr - 1, f = tf - 1; r >=            1 && f >=            1; --r, --f) atks |= (GET_SQUARE_BIT(r, f)); // (-.., -..)

    return atks;  
}

u64 atks__rook_mask(u8 square)
{
    // result attacks bitboard
    u64 atks = 0ull;

    // rank, file
    int8_t  r, f;

    // target ranks and files
    u8  tr = square / FILE_CNT;
    u8  tf = square % FILE_CNT;

    // mask relveant bishop occupancy bits
    for (r = tr + 1; r <= RANK_CNT - 2; ++r) atks |= (GET_SQUARE_BIT(r, tf)); // (  0, +..)
    for (r = tr - 1; r >=            1; --r) atks |= (GET_SQUARE_BIT(r, tf)); // (  0, -..)
    for (f = tf + 1; f <= RANK_CNT - 2; ++f) atks |= (GET_SQUARE_BIT(tr, f)); // (+..,   0)
    for (f = tf - 1; f >=            1; --f) atks |= (GET_SQUARE_BIT(tr, f)); // (-..,   0)

    return atks;
}

u64 atks__bishop_on_the_fly(u8 square, u64 block)
{
    // result attacks bitboard
    u64 atks = 0ull;

    // rank, file
    int8_t  r, f;

    // target ranks and files
    u8  tr = square / FILE_CNT;
    u8  tf = square % FILE_CNT;

    // generates all bishop attacks, stopping in a direction if subroutine finds space bishop
    // is tyring to move to is already occupied
    for (r = tr + 1, f = tf + 1; r <= RANK_CNT - 1 && f <= FILE_CNT - 1; ++r, ++f) if(!update_atks(&atks, &block, r, f)) break;
    for (r = tr - 1, f = tf + 1; r >=            0 && f <= FILE_CNT - 1; --r, ++f) if(!update_atks(&atks, &block, r, f)) break;
    for (r = tr + 1, f = tf - 1; r <= RANK_CNT - 1 && f >=            0; ++r, --f) if(!update_atks(&atks, &block, r, f)) break;
    for (r = tr - 1, f = tf - 1; r >=            0 && f >=            0; --r, --f) if(!update_atks(&atks, &block, r, f)) break;

    return atks;  
}

u64 atks__rook_on_the_fly(u8 square, u64 block)
{
    // result attacks bitboard
    u64 atks = 0ull;

    // rank, file
    int8_t  r, f;

    // target ranks and files
    u8  tr = square / FILE_CNT;
    u8  tf = square % FILE_CNT;

    // generates all rook attacks, stopping in a direction if subroutine finds space bishop
    // is tyring to move to is already occupied
    for (r = tr + 1; r <= RANK_CNT - 1; ++r) if(!update_atks(&atks, &block, r, tf)) break;
    for (r = tr - 1; r >=            0; --r) if(!update_atks(&atks, &block, r, tf)) break;
    for (f = tf + 1; f <= RANK_CNT - 1; ++f) if(!update_atks(&atks, &block, tr, f)) break;
    for (f = tf - 1; f >=            0; --f) if(!update_atks(&atks, &block, tr, f)) break;

    return atks;
}

void atks__init_leapers()
{
    for (u8 square = 0; square < SQUARE_CNT; ++square)
    {
        atks__pawn_table[white][square] = atks__pawn_mask(white, square);
        atks__pawn_table[black][square] = atks__pawn_mask(black, square);

        atks__knight_table[square]      = atks__knight_mask(square);

        atks__king_table[square]        = atks__king_mask(square);
    }
}

u64 atks__set_occupancy(u8 idx, int8_t bits_in_mask, u64 atk_mask)
{
    // occupany map
    u64 occupancy = 0ull;

    for (u8 cnt = 0; cnt < bits_in_mask; ++cnt)
    {
        // get least significant 1st bit idx of atk mask
        u8 square = GET_LS1B_IDX(atk_mask);

        POP_BIT(atk_mask, square);

        if (idx & (1 << cnt))
        {
            // populate occupany mask
            occupancy |= (1ull << square);
        }
    }

    return occupancy;
}

static u8 update_atks(u64 *atks, const u64 *block, u8 rank, u8 file)
{
    const u64 square_bit = GET_SQUARE_BIT(rank, file);

    *atks |= square_bit;

    if (square_bit & *block)
        return 0; // need to break
    else 
        return 1; // will continue loop
}