// +-+ Attack Tables Header File +-+

#ifndef ATKS_H
#define ATKS_H

#include "defs.h"

// Bitfield representation of board states hardcoded for an 8x8 bitboard  ↓↓↓

// bitboard decimal equivalent to all bits set in a u64 except lefmost file
// ("A file") - guards against piece movement search indexing on edge of board
#define NOT_A_FILE  18374403900871474942ull
// bitboard decimal equivalent of all bits set in a u64 except rightmost file
// ("H file") - guards against piece movement search indexing on edge of board
#define NOT_H_FILE  9187201950435737471ull
// bitboard decimal equivalent to all bits set in a u64 except two lefmost files
// ("AB file") - guards against knight movement search indexing on edge of board
#define NOT_AB_FILE 18229723555195321596ull
// bitboard decimal equivalent of all bits set in a u64 except two rightmost file
// ("HG file") - guards against knight movement search indexing on edge of board
#define NOT_HG_FILE 4557430888798830399ull

// -- relevancy occupancy bit count for every square on board --
// generated by iterating through every square and
// counting the bits produced by atks__bishop_mask(square)
extern const u8 bishop_relevant_bits[RANK_CNT * FILE_CNT];

// -- relevancy occupancy bit count for every square on board --
// generated by iterating through every square and
// counting the bits produced by atks__rook_mask(square)
extern const u8 rook_relevant_bits[RANK_CNT * FILE_CNT];

// all possible pawn attacks for white or black ("side") on a given square
u64 atks__pawn_mask(u8 side, u8 square);
// all possible knight attacks on a given square
u64 atks__knight_mask(u8 square);
// all possible king attacks on a given square
u64 atks__king_mask(u8 square);
// all possible bishop attacks on a given square
u64 atks__bishop_mask(u8 square);
// all possible rook attacks on a given square
u64 atks__rook_mask(u8 square);
// generate bishop attacks from a given square accouting for blocked spaces
u64 atks__bishop_on_the_fly(u8 square, u64 block);
// generate rook attacks from a given square accouting for blocked spaces
u64 atks__rook_on_the_fly(u8 square, u64 block);

// pawns, knights, kings
void atks__init_leapers();

// -- pawn attack bitboard table [side][board_square] --
// white pawns can only atk up and black pawns can only atk down, hence need
// for two pawn atk tables; contains an array of all possible pawn attacks for
// each square
extern u64 atks__pawn_table[PLYR_CNT][SQUARE_CNT];

// -- knight attack bitboard table --
// contains an array of all possible knight attacks for each square
extern u64 atks__knight_table[SQUARE_CNT];

// -- king attack btiboard table --
// contains an array of all possible king attacks for each square
extern u64 atks__king_table[SQUARE_CNT];

// -- bishop attack btiboard table --
// contains an array of all possible bishop attacks for each square
extern u64 atks__bishop_table[SQUARE_CNT];

// rook attack btiboard table
// contains an array of all possible rook attacks for each square
extern u64 atks__rook_table[SQUARE_CNT];

// Populates then returns occupany mask by popping each consecutive least
// significant 1st bit in atk_mask until all bits_in_mask have been read.
// I don't really know how to call the first function parameter, but it
// represents the range of all possible bit states for an occupancy mask
// ranging from [0, 64^2], i.e., (0, 10, 11, 100, 101, ...) for a 64 bit-mask.
u64 atks__set_occupancy(u8 idx, int8_t bits_in_mask, u64 atk_mask);

#endif // ATKS_H