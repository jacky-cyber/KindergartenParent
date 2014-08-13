

#include "global.h"
#include "common.h"
#include "access.h"





static inline long valid_mb_addr(ulong mb_addr,ulong pic_size_in_mbs)
{
	return ((mb_addr>=0) && (mb_addr<pic_size_in_mbs)) ? mb_addr : -1;
}


long   init_mb_access(decoder_t *decoder,mb_access_t *access)
{
	ulong  pic_width_in_mbs = decoder->pic_width_in_mbs;
	ulong  pic_size_in_mbs = decoder->pic_size_in_mbs;
	ulong  i;

	for( i=0;i<pic_size_in_mbs;i++ ){
		access->mb_pos[i].x = i%pic_width_in_mbs;
		access->mb_pos[i].y = i/pic_width_in_mbs;
	
		access->neighbour_mb_addr[i][0] = valid_mb_addr(i-1,pic_size_in_mbs);
		access->neighbour_mb_addr[i][1] = valid_mb_addr(i-pic_width_in_mbs,pic_size_in_mbs);
		access->neighbour_mb_addr[i][2] = valid_mb_addr(i-pic_width_in_mbs+1,pic_size_in_mbs);
		access->neighbour_mb_addr[i][3] = valid_mb_addr(i-pic_width_in_mbs-1,pic_size_in_mbs);
		access->neighbour_mb_addr[i][4] = i;
		access->neighbour_mb_addr[i][5] = -1;

		if ( access->mb_pos[i].x == 0 ){
			access->neighbour_mb_addr[i][0] = -1;
			access->neighbour_mb_addr[i][3] = -1;
		}
		if( (i+1)%pic_width_in_mbs == 0 ){
			access->neighbour_mb_addr[i][2] = -1;
		}
	}
	return 0;
}