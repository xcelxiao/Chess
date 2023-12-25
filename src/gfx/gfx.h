#ifndef gfx_appvar_include_file
#define gfx_appvar_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define sizeof_palette 64
#define palette (gfx_appvar[0])
#define sprites_palette_offset 0
#define white_king_width 30
#define white_king_height 30
#define gfx_sprites_white_king_compressed_index 1
#define white_king_compressed gfx_appvar[1]
#define white_pawn_width 30
#define white_pawn_height 30
#define gfx_sprites_white_pawn_compressed_index 2
#define white_pawn_compressed gfx_appvar[2]
#define white_knight_width 30
#define white_knight_height 30
#define gfx_sprites_white_knight_compressed_index 3
#define white_knight_compressed gfx_appvar[3]
#define white_bishop_width 30
#define white_bishop_height 30
#define gfx_sprites_white_bishop_compressed_index 4
#define white_bishop_compressed gfx_appvar[4]
#define white_rook_width 30
#define white_rook_height 30
#define gfx_sprites_white_rook_compressed_index 5
#define white_rook_compressed gfx_appvar[5]
#define white_queen_width 30
#define white_queen_height 30
#define gfx_sprites_white_queen_compressed_index 6
#define white_queen_compressed gfx_appvar[6]
#define black_king_width 30
#define black_king_height 30
#define gfx_sprites_black_king_compressed_index 7
#define black_king_compressed gfx_appvar[7]
#define black_pawn_width 30
#define black_pawn_height 30
#define gfx_sprites_black_pawn_compressed_index 8
#define black_pawn_compressed gfx_appvar[8]
#define black_knight_width 30
#define black_knight_height 30
#define gfx_sprites_black_knight_compressed_index 9
#define black_knight_compressed gfx_appvar[9]
#define black_bishop_width 30
#define black_bishop_height 30
#define gfx_sprites_black_bishop_compressed_index 10
#define black_bishop_compressed gfx_appvar[10]
#define black_rook_width 30
#define black_rook_height 30
#define gfx_sprites_black_rook_compressed_index 11
#define black_rook_compressed gfx_appvar[11]
#define black_queen_width 30
#define black_queen_height 30
#define gfx_sprites_black_queen_compressed_index 12
#define black_queen_compressed gfx_appvar[12]
#define gfx_entries_num 13
extern unsigned char *gfx_appvar[13];
unsigned char gfx_init(void);

#ifdef __cplusplus
}
#endif

#endif
