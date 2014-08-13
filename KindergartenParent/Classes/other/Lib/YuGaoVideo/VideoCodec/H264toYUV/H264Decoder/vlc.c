

#include <assert.h>
#include <stdlib.h>

#include "global.h"
#include "common.h"
#include "bitstream.h"
#include "vlc.h"

__attribute__((aligned(16))) const static uchar  zero_table[256]  = {
	    8,  7,  6,  6,  5,  5,  5,  5,  4,  4,  4,  4,  4,  4,  4,  4,
		3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
		2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};



__attribute__((aligned(16))) static const long inc_vlc[] = {0,3,6,12,24,48,32768};
__attribute__((aligned(16))) static const long numcoeff_vlc_table[18] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};

__attribute__((aligned(16))) static const char total_zeros_table_for_one_coeff[256][4]= 
{
	{15,9,0,0},{13,9,1,0},{12,8,0,0},{11,8,0,0},{10,7,0,0},{10,7,0,0},{9,7,0,0},{9,7,0,0},
	{8,6,0,0},{8,6,0,0},{8,6,0,0},{8,6,0,0},{7,6,0,0},{7,6,0,0},{7,6,0,0},{7,6,0,0},
	{6,5,0,0},{6,5,0,0},{6,5,0,0},{6,5,0,0},{6,5,0,0},{6,5,0,0},{6,5,0,0},{6,5,0,0},
	{5,5,0,0},{5,5,0,0},{5,5,0,0},{5,5,0,0},{5,5,0,0},{5,5,0,0},{5,5,0,0},{5,5,0,0},
	{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},
	{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},{4,4,0,0},
	{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},
	{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},{3,4,0,0},
	{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},
	{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},
	{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},
	{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},{2,3,0,0},
	{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},
	{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},
	{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},
	{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},{1,3,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
	{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}
};


__attribute__((aligned(16))) static const char  coeff_token_table_for_cdc[][4]= {
	{4,3,7,0},{4,3,7,0},{4,2,8,0},{4,1,8,0},{3,2,7,0},{3,2,7,0},{3,1,7,0},{3,1,7,0},
	{4,0,6,0},{4,0,6,0},{4,0,6,0},{4,0,6,0},{3,0,6,0},{3,0,6,0},{3,0,6,0},{3,0,6,0},
	{2,0,6,0},{2,0,6,0},{2,0,6,0},{2,0,6,0},{3,3,6,0},{3,3,6,0},{3,3,6,0},{3,3,6,0},
	{2,1,6,0},{2,1,6,0},{2,1,6,0},{2,1,6,0},{1,0,6,0},{1,0,6,0},{1,0,6,0},{1,0,6,0},
	{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},
	{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},
	{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},
	{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},{2,2,3,0},
	{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},
	{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},
	{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},
	{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},
	{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},
	{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},
	{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},
	{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},{0,0,2,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},
	{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}
};


__attribute__((aligned(16))) static const uchar num_num_coeff_code_bits[3][16] =
{
	{0,0,0,1,1,2,2,2,2,3,3,3,3,2,0},
	{1,1,2,2,2,2,2,3,3,3,2,2,0},
	{3,3,3,3,3,3,2,2,1,0}	
};


__attribute__((aligned(16))) static const num_total_coeff_and_trailingones_t num_total_coeff_and_trones_noncdc[3][16][8] =
{
	// VLC 0
	{																	// Leading zeros
    {{0,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},    //0                                                                // 0
	{{1,1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	// 1
	{{2,2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	// 2
	{{17,-1},{3,3},{-1,-1},{-1,-1},{2,1},{1,0},{-1,-1},{-1,-1}},		// 3
	{{17,-1},{4,3},{-1,-1},{-1,-1},{5,3},{3,2},{-1,-1},{-1,-1}},		// 4
	{{6,3},{4,2},{3,1},{2,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},			// 5
	{{7,3},{5,2},{4,1},{3,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},			// 6
	{{8,3},{6,2},{5,1},{4,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},			// 7
	{{9,3},{7,2},{6,1},{5,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},			// 8
	{{8,0},{9,2},{8,1},{7,0},{10,3},{8,2},{7,1},{6,0}},					// 9
	{{12,3},{11,2},{10,1},{10,0},{11,3},{10,2},{9,1},{9,0}},			// 10
	{{14,3},{13,2},{12,1},{12,0},{13,3},{12,2},{11,1},{11,0}},			// 11
	{{16,3},{15,2},{15,1},{14,0},{15,3},{14,2},{14,1},{13,0}},			// 12
	{{16,0},{16,2},{16,1},{15,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},		// 13
	{{13,1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	// 14
	{{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}   // useless
	},

	// VLC 1
	{																	// Leading zeros
    {{1,1},{0,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},      // 0                                                                // 0
	{{17,-1},{2,2},{-1,-1},{-1,-1},{4,3},{3,3},{-1,-1},{-1,-1}},		// 1
	{{17,-1},{17,-1},{5,3},{2,1},{6,3},{3,2},{3,1},{1,0}},				// 2
	{{7,3},{4,2},{4,1},{2,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},			// 3
	{{8,3},{5,2},{5,1},{3,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},			// 4
	{{5,0},{6,2},{6,1},{4,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},			// 5
	{{9,3},{7,2},{7,1},{6,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},			// 6
	{{11,3},{9,2},{9,1},{8,0},{10,3},{8,2},{8,1},{7,0}},				// 7
	{{11,0},{11,2},{11,1},{10,0},{12,3},{10,2},{10,1},{9,0}},			// 8
	{{14,3},{13,2},{13,1},{13,0},{13,3},{12,2},{12,1},{12,0}},			// 9
	{{17,-1},{17,-1},{14,2},{14,0},{15,1},{15,0},{15,2},{14,1}},		// 10
	{{16,3},{16,2},{16,1},{16,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},		// 11
	{{15,3},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}	// 12
	},

	// VLC 2
	{																	// Leading zeros
    {{7,3},{6,3},{5,3},{4,3},{3,3},{2,2},{1,1},{0,0}},                  // 0                                                               // 0
	{{5,1},{5,2},{4,1},{4,2},{3,1},{8,3},{3,2},{2,1}},					// 1
	{{3,0},{7,2},{7,1},{2,0},{9,3},{6,2},{6,1},{1,0}},					// 2
	{{7,0},{6,0},{9,2},{5,0},{10,3},{8,2},{8,1},{4,0}},					// 3
	{{12,3},{11,2},{10,1},{9,0},{11,3},{10,2},{9,1},{8,0}},				// 4
	{{12,0},{13,2},{12,1},{11,0},{13,3},{12,2},{11,1},{10,0}},			// 5
	{{17,-1},{17,-1},{17,-1},{13,1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	// 6
	{{16,1},{15,0},{15,3},{15,2},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},		// 7
	{{16,3},{16,2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	// 8
	{{16,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	// 9
	},
};




__attribute__((aligned(16))) static const num_total_coeff_and_trailingones_t alt_vlc2_num_total_coeff_and_trones[8] = {
	{15,1},{14,0},{14,3},{14,2},{14,1},{13,0},{-1,-1},{-1,-1}
};


__attribute__((aligned(16))) static const uchar total_zeros_lookup_noncdc[14][64] = {
{
 0x6e, 0x6d, 0x6c, 0x6b, 0x5a, 0x5a, 0x59, 0x59, 0x48, 0x48, 0x48, 0x48, 0x47, 0x47, 0x47, 0x47,
 0x46, 0x46, 0x46, 0x46, 0x45, 0x45, 0x45, 0x45, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34,
 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30
},
{
 0x6d, 0x6b, 0x5c, 0x5c, 0x5a, 0x5a, 0x59, 0x59, 0x48, 0x48, 0x48, 0x48, 0x45, 0x45, 0x45, 0x45,
 0x44, 0x44, 0x44, 0x44, 0x40, 0x40, 0x40, 0x40, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31
},
{
 0x5c, 0x5c, 0x5b, 0x5b, 0x5a, 0x5a, 0x50, 0x50, 0x49, 0x49, 0x49, 0x49, 0x47, 0x47, 0x47, 0x47,
 0x43, 0x43, 0x43, 0x43, 0x42, 0x42, 0x42, 0x42, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35,
 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31
},
{
 0x5b, 0x5b, 0x59, 0x59, 0x4a, 0x4a, 0x4a, 0x4a, 0x48, 0x48, 0x48, 0x48, 0x42, 0x42, 0x42, 0x42,
 0x41, 0x41, 0x41, 0x41, 0x40, 0x40, 0x40, 0x40, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35,
 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33
},
{
 0x6a, 0x60, 0x51, 0x51, 0x48, 0x48, 0x48, 0x48, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39,
 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34,
 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32
},
{
 0x69, 0x60, 0x51, 0x51, 0x47, 0x47, 0x47, 0x47, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34,
 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25
},
{
 0x68, 0x60, 0x52, 0x52, 0x41, 0x41, 0x41, 0x41, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25,
 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24
},
{
 0x61, 0x60, 0x57, 0x57, 0x42, 0x42, 0x42, 0x42, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35,
 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26,
 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24,
 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23
},
{
 0x51, 0x51, 0x50, 0x50, 0x46, 0x46, 0x46, 0x46, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25,
 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24,
 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23
},
{
 0x40, 0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x41, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35,
 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14
},
{
 0x40, 0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x41, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34,
 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13
},
{
 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23,
 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12
},
{
 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12
},
{
 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
}
};





__attribute__((aligned(16))) static const uchar total_zeros_lookup_cdc[3][8]=
{
	{
		0x33,0x32,0x21,0x21,0x10,0x10,0x10,0x10
	},
	{
        0x22,0x22,0x21,0x21,0x10,0x10,0x10,0x10
	},
	{
        0x11,0x11,0x11,0x11,0x10,0x10,0x10,0x10
	}
};


__attribute__((aligned(16))) static const uchar run_before_lookup[6][8] = {
{
 0x11, 0x11, 0x11, 0x11, 0x10, 0x10, 0x10, 0x10,
},
{
 0x22, 0x22, 0x21, 0x21, 0x10, 0x10, 0x10, 0x10,
},
{
 0x23, 0x23, 0x22, 0x22, 0x21, 0x21, 0x20, 0x20,
},
{
 0x34, 0x33, 0x22, 0x22, 0x21, 0x21, 0x20, 0x20,
},
{
 0x35, 0x34, 0x33, 0x32, 0x21, 0x21, 0x20, 0x20,
},
{
 0x31, 0x32, 0x34, 0x33, 0x36, 0x35, 0x20, 0x20,
},
};

__attribute__((aligned(16))) static const uchar NTAB1[4][8][2] =
{
	{{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{1,1},{1,2},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{2,0},{1,3},{1,4},{1,5},{0,0},{0,0},{0,0},{0,0}},
	{{3,0},{2,1},{2,2},{1,6},{1,7},{1,8},{1,9},{4,0}},
};

__attribute__((aligned(16))) static const uchar LEVRUN1[16]=
{
	4,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,
};


__attribute__((aligned(16))) static const uchar NTAB2[4][8][2] =
{
	{{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{1,1},{2,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{1,2},{3,0},{4,0},{5,0},{0,0},{0,0},{0,0},{0,0}},
	{{1,3},{1,4},{2,1},{3,1},{6,0},{7,0},{8,0},{9,0}},
};

//! for the linfo_levrun__c2x2 routine
__attribute__((aligned(16))) static const uchar LEVRUN3[4] =
{
	2,1,0,0
};
__attribute__((aligned(16))) static const uchar NTAB3[2][2][2] =
{
	{{1,0},{0,0}},
	{{2,0},{1,1}},
};

__attribute__((aligned(16))) const uchar NCBP[48][2]=
{
  {47, 0},{31,16},{15, 1},{ 0, 2},{23, 4},{27, 8},{29,32},{30, 3},{ 7, 5},{11,10},{13,12},{14,15},
  {39,47},{43, 7},{45,11},{46,13},{16,14},{ 3, 6},{ 5, 9},{10,31},{12,35},{19,37},{21,42},{26,44},
  {28,33},{35,34},{37,36},{42,40},{44,39},{ 1,43},{ 2,45},{ 4,46},{ 8,17},{17,18},{18,20},{20,24},
  {24,19},{ 6,21},{ 9,26},{22,28},{25,23},{32,27},{33,29},{34,30},{36,22},{40,25},{38,38},{41,41},
};

__attribute__((aligned(16))) static const long trailingones_table[3][8][3] = {
		{ {0,0,1},{0,0,-1},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} },
		{ {0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1} },
		{ {1,1,1},{1,1,-1},{1,-1,1},{1,-1,-1},{-1,1,1},{-1,1,-1},{-1,-1,1},{-1,-1,-1} }
};



static inline long get_zero_number(ulong bit_buf)
{
	long  info;
	info = UBITS(bit_buf,8);
	if( info ){
		return zero_table[info];
	}
	info = UBITS(bit_buf,16);
	if( info ){
		return zero_table[info] + 8;
	}
    
	assert (0==1);
    
	return -1;
}


long golumb_u1(bs_t *bs)
{
	long rc = UBITS(bs->bit_buf,1);
	DUMPBITS(bs->bit_buf,bs->bit_ptr,bs->bits,1);
	return rc;
}

long golumb_u(bs_t *bs , long numbits)
{
	long rc = UBITS(bs->bit_buf,numbits);
	DUMPBITS(bs->bit_buf,bs->bit_ptr,bs->bits,numbits);
	return rc;
}


long golumb_ue(bs_t *bs)
{    
    long info = 0;
	long leading_zero_bits = get_zero_number(bs->bit_buf);
	DUMPBITS(bs->bit_buf,bs->bit_ptr,bs->bits,leading_zero_bits+1);
	if( leading_zero_bits ){
		info = UBITS(bs->bit_buf,leading_zero_bits);
		DUMPBITS(bs->bit_buf,bs->bit_ptr,bs->bits,leading_zero_bits);
	}

	return (1<<leading_zero_bits) + info - 1;
}


long golumb_se(bs_t *bs)
{
	long rc = golumb_ue(bs);	

	return (rc&0x01) ? (rc+1)>>1 : -(rc>>1);
}


long  read_intra4x4_pred_mode_cavlc(bs_t *bs)
{
	long  mode = -1;
	if( !golumb_u(bs,1) ){
		mode = golumb_u(bs,3);
	}
	return mode;
}

long  read_ref_idx_cavlc(bs_t *bs,long ref_idx_active_flag)
{
	if( ref_idx_active_flag ){
		return 1 - golumb_u(bs,1);
	} 
	return golumb_ue(bs);
}


long  read_cbp_intra_cavlc(bs_t *bs)
{
	long cbp_idx = golumb_ue(bs);
	return NCBP[cbp_idx][0];
}

long  read_cbp_inter_cavlc(bs_t *bs)
{
	long cbp_idx = golumb_ue(bs);
	return NCBP[cbp_idx][1];
}


long  read_coeffs_4x4_block_cavlc(bs_t *bs,long pred_nnz,long max_coeff_num,long cdc,long *levarr,long *runarr)
{
	long i,k,code,vlcnum;
	long numcoeffs = 0, numtrailingones, numcoeff_vlc;
	long level_two_or_higher;
	long totzeros, level;
	long zerosleft;	
	long inf = 0;	
    	
	uchar *bit_ptr = bs->bit_ptr;
	ulong bit_buf = bs->bit_buf;
	long bits = bs->bits;


	for( k=0;k<18;k++ )
		runarr[k] = 0;

	if( !cdc ){
		long nNumZeros;
		long nBitsToGet;
		long nCode;
		long inf;
        numcoeff_vlc = numcoeff_vlc_table[pred_nnz];
		if(numcoeff_vlc == 3)
		{		
			numcoeffs = UBITS(bit_buf,6);
			numtrailingones = numcoeffs&0x3;
			numcoeffs = numcoeffs>>2;
			DUMPBITS(bit_buf,bit_ptr,bits,6);
			if (!numcoeffs && numtrailingones == 3) 
			{
				numtrailingones = 0;
			}
			else
				numcoeffs++;				
		} else {
			nNumZeros = get_zero_number(bit_buf);
			assert(nNumZeros < 15);		
			DUMPBITS(bit_buf,bit_ptr,bits,nNumZeros+1);
			nBitsToGet = num_num_coeff_code_bits[numcoeff_vlc][nNumZeros];
			if(nBitsToGet)
			{				
				nCode = UBITS(bit_buf,nBitsToGet);
				DUMPBITS(bit_buf,bit_ptr,bits,nBitsToGet);
			}
			else 
				nCode = 0;			
			numcoeffs = num_total_coeff_and_trones_noncdc[numcoeff_vlc][nNumZeros][nCode].TotalCoeff;
			numtrailingones = num_total_coeff_and_trones_noncdc[numcoeff_vlc][nNumZeros][nCode].TrailingOnes;
			if(numcoeffs > 16)
			{
				nCode = (nCode << 1);			
				inf = UBITS(bit_buf,1);
				DUMPBITS(bit_buf,bit_ptr,bits,1);
				nCode |= inf;
				if(numcoeff_vlc < 2)
				{
					nCode |= 4;
					numcoeffs = num_total_coeff_and_trones_noncdc[numcoeff_vlc][nNumZeros][nCode].TotalCoeff;
					numtrailingones = num_total_coeff_and_trones_noncdc[numcoeff_vlc][nNumZeros][nCode].TrailingOnes;
				}
				else
				{
					numcoeffs = alt_vlc2_num_total_coeff_and_trones[nCode].TotalCoeff;
					numtrailingones = alt_vlc2_num_total_coeff_and_trones[nCode].TrailingOnes;
				}
			}		
		}
	} else {
		long nCode,nBitsToDump;
	    nCode = UBITS(bit_buf,8);
	    numcoeffs = coeff_token_table_for_cdc[nCode][0];
	    numtrailingones = coeff_token_table_for_cdc[nCode][1];
	    nBitsToDump = coeff_token_table_for_cdc[nCode][2];
	    DUMPBITS(bit_buf,bit_ptr,bits,nBitsToDump);
	}


	if (numcoeffs){
		if (numtrailingones){			
			code = UBITS(bit_buf,numtrailingones);
			DUMPBITS(bit_buf,bit_ptr,bits,numtrailingones);		
			levarr[numcoeffs-numtrailingones] = trailingones_table[numtrailingones-1][code][2];
			levarr[numcoeffs-numtrailingones+1] = trailingones_table[numtrailingones-1][code][1];
			levarr[numcoeffs-numtrailingones+2] = trailingones_table[numtrailingones-1][code][0]; 
		}	
		vlcnum = 0;	
		level_two_or_higher = !(numtrailingones==3);
		if (numcoeffs > 10 && numtrailingones < 3)
			vlcnum = 1;
		k = numcoeffs - 1 - numtrailingones;
		if(k >= 0) // read levels 
		{
			if (vlcnum == 0)
			{				
				long len, sign, code;	
				len = get_zero_number(bit_buf);
				len++;

				DUMPBITS(bit_buf,bit_ptr,bits,len);

				if (len < 15)
				{
					sign = (len - 1) & 1;
					inf = (len-1) / 2 + 1;
				}
				else if (len == 15)
				{				
					code = UBITS(bit_buf,4);
					DUMPBITS(bit_buf,bit_ptr,bits,4);
					sign = (code & 1);
					inf = (code >> 1) + 8;
				}
				else if (len == 16)
				{					
					code = UBITS(bit_buf,12);
					DUMPBITS(bit_buf,bit_ptr,bits,12);
					sign =  (code & 1);
					inf = (code >> 1) + 16;
				}

				if (sign)
					inf = -inf;					
			}
			else
			{								
				long levabs, sign;	
				long sb;

				long numPrefix;
				long shift = vlcnum-1;
				long escape = (15<<shift)+1;

				// read pre zeros
				numPrefix = get_zero_number(bit_buf);		
				DUMPBITS(bit_buf,bit_ptr,bits,numPrefix+1);

				if (numPrefix < 15)
				{
					levabs = (numPrefix<<shift) + 1;		
					sb = UBITS(bit_buf,vlcnum);
					DUMPBITS(bit_buf,bit_ptr,bits,vlcnum);
					sign = -(sb & 0x1);
					levabs += (sb >> 1);		
				}
				else  
				{ 		
					sb = UBITS(bit_buf,12);
					DUMPBITS(bit_buf,bit_ptr,bits,12);
					sign = -(sb & 0x1);
					levabs = (sb >> 1) + escape;		
				}
				inf = levabs;
				inf ^= sign;
				inf -= sign;
			}			
			if (level_two_or_higher)
			{
				if (inf > 0)
					inf ++;
				else
					inf --;
			}
			level = levarr[k] = inf;
			vlcnum = 1;
			if (abs(level)>3)
				vlcnum = 2;
			k--;
			for (; k >= 0; k--)
			{											
				long levabs, sign;	
				long sb;

				long numPrefix;
				long shift = vlcnum-1;
				long escape = (15<<shift)+1;

				// read pre zeros
				numPrefix = get_zero_number(bit_buf);		
				DUMPBITS(bit_buf,bit_ptr,bits,numPrefix+1);

				if (numPrefix < 15)
				{
					levabs = (numPrefix<<shift) + 1;		
					sb = UBITS(bit_buf,vlcnum);
					DUMPBITS(bit_buf,bit_ptr,bits,vlcnum);
					sign = -(sb & 0x1);
					levabs += (sb >> 1);		
				}
				else  
				{ 		
					sb = UBITS(bit_buf,12);
					DUMPBITS(bit_buf,bit_ptr,bits,12);
					sign = -(sb & 0x1);
					levabs = (sb >> 1) + escape;		
				}
				inf = levabs;
				inf ^= sign;
				inf -= sign;				

				level = levarr[k] = inf;
				if (abs(level)>inc_vlc[vlcnum])
					vlcnum++;
			}
		}

		totzeros = 0;
		if (numcoeffs < max_coeff_num)
		{
			vlcnum = numcoeffs-1;// read totalzeros
			if (!cdc)
			{				
				long nBitsToGet;
				long nCode;				
				if(numcoeffs > 1)
				{		
					nCode = UBITS(bit_buf,6);
					nCode = total_zeros_lookup_noncdc[numcoeffs - 2][nCode];
					totzeros = (nCode & 0xf);
					nBitsToGet = (nCode >> 4);				
					DUMPBITS(bit_buf,bit_ptr,bits,nBitsToGet);		
				}
				else
				{	
					nCode = UBITS(bit_buf,9);
					totzeros = total_zeros_table_for_one_coeff[nCode>>1][0];
					nBitsToGet = total_zeros_table_for_one_coeff[nCode>>1][1];
					if( total_zeros_table_for_one_coeff[nCode>>1][2] ){
						totzeros += 1 - (nCode&0x01);
					}
					DUMPBITS(bit_buf,bit_ptr,bits,nBitsToGet);
				}	
			}
			else
			{								
				long nCode;				
				nCode = UBITS(bit_buf,3);
				nCode = total_zeros_lookup_cdc[numcoeffs - 1][nCode];
				totzeros = (nCode & 0xf);	
				nCode>>=4;
				DUMPBITS(bit_buf,bit_ptr,bits,nCode);				
			}
		}
		zerosleft = totzeros;
		i = numcoeffs-1;
		if (zerosleft > 0 && i > 0)
		{
			do// read runbefore
			{
				vlcnum = zerosleft - 1;
				if (vlcnum > RUNBEFORE_NUM-1)
					vlcnum = RUNBEFORE_NUM-1;								
				{  //  
					long ZerosLeft = vlcnum+1;
					long nNumZeros;
					long nBitsToGet;
					long nCode;
					long RunBefore;
					if(ZerosLeft > 6)
					{		
						nCode = UBITS(bit_buf,3);
						DUMPBITS(bit_buf,bit_ptr,bits,3);
						if(nCode)
						{
							RunBefore = 7 - nCode;			
						}
						else
						{									
							nNumZeros = zero_table[UBITS(bit_buf,8)];
							RunBefore = 7 + nNumZeros;				
							DUMPBITS(bit_buf,bit_ptr,bits,nNumZeros + 1);
						}
					}
					else
					{
						nCode = UBITS(bit_buf,3);
						nCode = run_before_lookup[ZerosLeft - 1][nCode];
						RunBefore = (nCode & 0xf);
						nBitsToGet = (nCode >> 4);
						DUMPBITS(bit_buf,bit_ptr,bits,nBitsToGet);		
					}
					runarr[i] = RunBefore;
				}
				zerosleft -= runarr[i];
				i --;
			}
			while (zerosleft != 0 && i != 0);
		}
		runarr[i] = zerosleft;
	}	

	bs->bit_ptr = bit_ptr;
	bs->bit_buf = bit_buf;
	bs->bits = bits;
	return numcoeffs;
}
