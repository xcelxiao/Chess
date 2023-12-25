#include "dbg.h"

#include <debug.h>

void dbg_bitboard(u64 bitboard)
{
    dbg_printf("\n");

    for (u8 rank = 0; rank < RANK_CNT; ++rank)
    {
        for (u8 file = 0; file < FILE_CNT; ++file)
        {
            u8 square = GET_SQUARE(rank, file);

            if (!file) // prints ranks ↓
                dbg_printf("  %d  ", RANK_CNT - rank);

            dbg_printf(" %d", GET_BIT(bitboard, square) ? 1 : 0);
        }

        dbg_printf("\n");
    }

    // prints files
    dbg_printf("\n     A B C D E F G H \n\n");

    // bitboard ull value
    dbg_printf("Bitboard: %llud\n\n", bitboard);
}