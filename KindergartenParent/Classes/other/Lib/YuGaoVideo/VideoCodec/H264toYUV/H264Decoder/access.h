

#ifndef _ACCESS_H_
#define _ACCESS_H_


__attribute__((aligned(16))) static const sub_mb_position_t submb_4x4[][16] = 
{  
  {  // left
	{ 0 , 5 } , { 4 , 0 } , { 0 , 7 } , { 4 , 2 } ,
	{ 4 , 1 } , { 4 , 4 } , { 4 , 3 } , { 4 , 6 } ,
	{ 0 , 13 } , { 4 , 8 } , { 0 , 15 } , { 4 , 10 } ,
	{ 4 , 9 } , { 4 , 12 } , { 4 , 11 } , { 4 , 14 }
  } , 
  {  // up
	{ 1 , 10 } , { 1 , 11 } , { 4 , 0 } , { 4 , 1 } ,
	{ 1 , 14 } , { 1 , 15 } , { 4 , 4 } , { 4 , 5 } , 
	{ 4 , 2 } , { 4 , 3 } , { 4 , 8 } , { 4 , 9 } ,
	{ 4 , 6 } , { 4 , 7 } , { 4 , 12 } , { 4 , 13 }
  } ,
  {  // upright
	{ 1 , 11 } , { 1 , 14 } , { 4 , 1 } , { 5 , 4 } ,
	{ 1 , 15 } , { 2 , 10 } , { 4 , 5 } , { 5 , 0 } , 
	{ 4 , 3 } , { 4 , 6 } , { 4 , 9 } , { 5 , 12 } ,
	{ 4 , 7 } , { 5 , 2 } , { 4 , 13 } , { 5 , 8 }
  } ,
   {  // upleft
	{ 3 , 15 } , { 1 , 10 } , { 0 , 5 } , { 4 , 0 } ,
	{ 1 , 11 } , { 1 , 14 } , { 4 , 1 } , { 4 , 4 } , 
	{ 0 , 7 } , { 4 , 2 } , { 0 , 13 } , { 4 , 8 } ,
	{ 4 , 3 } , { 4 , 6 } , { 4 , 9 } , { 4 , 12 }
  }
};


__attribute__((aligned(16))) static const long map_to_submb_idx[4][4] = 
{
	{0,1,4,5},{2,3,6,7},{8,9,12,13},{10,11,14,15}
};

__attribute__((aligned(16))) static const long submb_pos_x[] = { 0 , 1 , 0 , 1 , 2 , 3 , 2 , 3 , 0 , 1 , 0 , 1 , 2 , 3 , 2 , 3 };
__attribute__((aligned(16))) static const long submb_pos_y[] = { 0 , 0 , 1 , 1 , 0 , 0 , 1 , 1 , 2 , 2 , 3 , 3 , 2 , 2 , 3 , 3 };
__attribute__((aligned(16))) static const long submb_pos_rasterscanning[]={0, 1, 4, 5, 2, 3, 6, 7, 8, 9, 12, 13, 10, 11, 14, 15};


long  init_mb_access(decoder_t *decoder,mb_access_t *access);

static inline long get_neighbour_mb_addr(mb_access_t *access,long cur_mb_addr,long position)
{
	return access->neighbour_mb_addr[cur_mb_addr][position];
}

static inline void get_mb_pos(mb_access_t *access,ulong mb_addr,long *mb_x,long *mb_y)
{
	*mb_x = access->mb_pos[mb_addr].x;
	*mb_y = access->mb_pos[mb_addr].y;
}

static inline long get_neighbour4x4(mb_access_t *access,long cur_mb_addr,long cur_sub_idx,long position)
{
	long mb_addr , sub_idx;
	long mb_offset;

	mb_offset = submb_4x4[position][cur_sub_idx].mb_offset;
	sub_idx = submb_4x4[position][cur_sub_idx].sub_idx;
	mb_addr = access->neighbour_mb_addr[cur_mb_addr][mb_offset];

    return ((mb_addr<<4)+sub_idx);
}



#endif