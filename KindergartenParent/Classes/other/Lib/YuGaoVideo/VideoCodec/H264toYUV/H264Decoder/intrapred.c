#include <assert.h>

#include "global.h"
#include "common.h"
#include "intrapred.h"
#include "access.h"


#define P_X (PredPel[0])
#define P_A (PredPel[1])
#define P_B (PredPel[2])
#define P_C (PredPel[3])
#define P_D (PredPel[4])
#define P_E (PredPel[5])
#define P_F (PredPel[6])
#define P_G (PredPel[7])
#define P_H (PredPel[8])
#define P_I (PredPel[9])
#define P_J (PredPel[10])
#define P_K (PredPel[11])
#define P_L (PredPel[12])

void intra_pred_luma_16x16(decoder_t *decoder,macroblock_t *cur_mb)
{
	long i,j;
	picture_t *pic = decoder->cur_pic;
	uchar **img_y = pic->img_y;
	long  x = decoder->mb_x << 4;
	long  y = decoder->mb_y << 4;

	switch( cur_mb->i16mode )
	{
	case VERT_PRED_16:
		for( j=0;j<16;j++ ){
			for( i=0;i<16;i++ ){
				decoder->mpr[j][i] = img_y[y-1][x+i]; 
			}
		}
		break;
	case HOR_PRED_16:
		for( j=0;j<16;j++ ){
			for( i=0;i<16;i++ ){
				decoder->mpr[j][i] = img_y[y+j][x-1];
			}
		}
		break;
	case DC_PRED_16:
		{
			long s0,s1=0,s2=0;
			mb_access_t *access = decoder->mb_access;
			long a , b;
			a = get_neighbour4x4(access,decoder->cur_mb_nr,0,0);
			b = get_neighbour4x4(access,decoder->cur_mb_nr,0,1);

			if ( decoder->active_pps->constrained_intra_pred_flag ){
				if( a>=0 && (decoder->mb_date[a>>4].mb_type!=I16MB || decoder->mb_date[a>>4].mb_type!=I4MB) ){
					a = -1;
				}
				if( b>=0 && (decoder->mb_date[b>>4].mb_type!=I16MB || decoder->mb_date[b>>4].mb_type!=I4MB) ){
					b = -1;
				}
			}

			if( (a>=0) ){
				for( i=0;i<16;i++ ){
					s1 += img_y[y+i][x-1];
				}
			}
			if( b>=0 ){
				for( i=0;i<16;i++ ){
					s2 += img_y[y-1][x+i];
				}
			}
			if( a>=0 && b>= 0 ){
                s0 = (s1+s2+16)>>5;
			} else if( a<0 && b<0 ){
				s0 = 128;
			} else {
				s0 = (s1+s2+8)>>4;
			}
			for( j=0;j<16;j++ ){
				for( i=0;i<16;i++ ){
					decoder->mpr[j][i] = s0;
				}
			}
		}
		break;
	case PLANE_16:
		{
			long iaa,ib,ic,ih,iv;
			long pred;
			ih = 0;
			iv = 0;
			for( i=0;i<8;i++ ){
                ih += (i+1)*(img_y[y-1][x+8+i]-img_y[y-1][x+6-i]);
				iv += (i+1)*(img_y[y+8+i][x-1]-img_y[y+6-i][x-1]);
			}
			ib = (5*ih+32)>>6;
			ic = (5*iv+32)>>6;
			iaa = 16*(img_y[y-1][x+15] + img_y[y+15][x-1]);
			for( j=0;j<16;j++ ){
				for( i=0;i<16;i++ ){
					pred = (iaa + ib*(i-7) + ic*(j-7) + 16)>>5;
					decoder->mpr[j][i] = CLIP3(0,255,pred);
				}
			}
		}
		break;
	}
}

void  intra_pred_luma_4x4(decoder_t *decoder,macroblock_t *cur_mb,long idx)
{
	long x,y;
	long sub_x,sub_y;
	picture_t *pic = decoder->cur_pic;
	uchar **img_y = pic->img_y;
	uchar  pred_mode = decoder->ipred_mode[(decoder->cur_mb_nr<<4)+idx];
	long i,j;
	long PredPel[13];
	long sub_a,sub_b,sub_c,sub_d;
	mb_access_t *access = decoder->mb_access;
    
	sub_x = submb_pos_x[idx]<<2;
	sub_y = submb_pos_y[idx]<<2;
	x = (decoder->mb_x << 4) + sub_x;
	y = (decoder->mb_y << 4) + sub_y;

	if ( pred_mode <= 2 ){
		if( pred_mode == VERT_PRED ){
			for( j=sub_y;j<sub_y+4;j++ ){
				for( i=sub_x;i<sub_x+4;i++ ){
					decoder->mpr[j][i] = img_y[y-1][x+i-sub_x];
				}
			}
		} else if( pred_mode == HOR_PRED ){
			for( j=sub_y;j<sub_y+4;j++ ){
				for( i=sub_x;i<sub_x+4;i++ ){
					decoder->mpr[j][i] = img_y[y+j-sub_y][x-1];
				}
			}	
		} else { // DC_RRED
			long s0,s1=0,s2=0;
			long a,b;			
			a = get_neighbour4x4(access,decoder->cur_mb_nr,idx,0);
			b = get_neighbour4x4(access,decoder->cur_mb_nr,idx,1);
			if( decoder->active_pps->constrained_intra_pred_flag ){
				if( a>=0 && (decoder->mb_date[a>>4].mb_type!=I16MB || decoder->mb_date[a>>4].mb_type!=I4MB) ){
					a = -1;
				}
				if( b>=0 && (decoder->mb_date[b>>4].mb_type!=I16MB || decoder->mb_date[b>>4].mb_type!=I4MB) ){
					b = -1;
				}
			}
			if( a>=0 ){
				for( i=0;i<4;i++ ){
					s1 += img_y[y+i][x-1];
				}
			}
			if( b>=0 ){
				for( i=0;i<4;i++ ){
					s2 += img_y[y-1][x+i];
				}
			}
			if( a>=0 && b>=0 ){
				s0 = (s1+s2+4)>>3;
			} else if( a<0 && b<0 ){
				s0 = 128;				
			} else {
				s0 = (s1+s2+2)>>2;
			}
			for( j=sub_y;j<sub_y+4;j++ ){
			    for( i=sub_x;i<sub_x+4;i++ ){
					decoder->mpr[j][i] = s0;
				}
			}
		}
		return;
	}

	sub_a = get_neighbour4x4(access,decoder->cur_mb_nr,idx,0);
	sub_b = get_neighbour4x4(access,decoder->cur_mb_nr,idx,1);
	sub_c = get_neighbour4x4(access,decoder->cur_mb_nr,idx,2);
	sub_d = get_neighbour4x4(access,decoder->cur_mb_nr,idx,3);

	if( sub_b>=0 ){
		P_A = img_y[y-1][x];
		P_B = img_y[y-1][x+1];
		P_C = img_y[y-1][x+2];
		P_D = img_y[y-1][x+3];
	} else {
		P_A = P_B = P_C = P_D = 128;
	}
	if( sub_c>=0 ){
		P_E = img_y[y-1][x+4];
		P_F = img_y[y-1][x+5];
		P_G = img_y[y-1][x+6];
		P_H = img_y[y-1][x+7];
	} else {
		P_E = P_F = P_G = P_H = P_D;
	}

	if( sub_a>=0 ){
		P_I = img_y[y][x-1];
		P_J = img_y[y+1][x-1];
		P_K = img_y[y+2][x-1];
		P_L = img_y[y+3][x-1];
	} else {
        P_I = P_J = P_K = P_L = 128;
	}

	if( sub_d>=0 ){
		P_X = img_y[y-1][x-1];
	} else {
		P_X = 128;
	}



	switch ( pred_mode )
	{	
	case DIAG_DOWN_LEFT_PRED:
		{
			decoder->mpr[sub_y][sub_x] = (P_A + P_C + 2*(P_B) + 2) / 4;;
			decoder->mpr[sub_y][sub_x+1] = decoder->mpr[sub_y+1][sub_x] = 
				(P_B + P_D + 2*(P_C) + 2) / 4;
			decoder->mpr[sub_y][sub_x+2] = decoder->mpr[sub_y+1][sub_x+1] = decoder->mpr[sub_y+2][sub_x] = 
				(P_C + P_E + 2*(P_D) + 2) / 4;
			decoder->mpr[sub_y][sub_x+3] = decoder->mpr[sub_y+1][sub_x+2] = decoder->mpr[sub_y+2][sub_x+1] = decoder->mpr[sub_y+3][sub_x] = 
				(P_D + P_F + 2*(P_E) + 2) / 4;
			decoder->mpr[sub_y+1][sub_x+3] = decoder->mpr[sub_y+2][sub_x+2] = decoder->mpr[sub_y+3][sub_x+1] = 
                (P_E + P_G + 2*(P_F) + 2) / 4;
			decoder->mpr[sub_y+2][sub_x+3] = decoder->mpr[sub_y+3][sub_x+2] = 
				(P_F + P_H + 2*(P_G) + 2) / 4;
			decoder->mpr[sub_y+3][sub_x+3] = (P_G + 3*(P_H) + 2) / 4;
		}
		break;
	case DIAG_DOWN_RIGHT_PRED:
		{
			decoder->mpr[sub_y+3][sub_x] = (P_L + 2*P_K + P_J + 2) / 4; 
            decoder->mpr[sub_y+2][sub_x] = decoder->mpr[sub_y+3][sub_x+1] = 
				(P_K + 2*P_J + P_I + 2) / 4;
			decoder->mpr[sub_y+1][sub_x] = decoder->mpr[sub_y+2][sub_x+1] = decoder->mpr[sub_y+3][sub_x+2] = 
				(P_J + 2*P_I + P_X + 2) / 4;
			decoder->mpr[sub_y][sub_x] = decoder->mpr[sub_y+1][sub_x+1] = decoder->mpr[sub_y+2][sub_x+2] = decoder->mpr[sub_y+3][sub_x+3] = 
				(P_I + 2*P_X + P_A + 2) / 4;
			decoder->mpr[sub_y][sub_x+1] = decoder->mpr[sub_y+1][sub_x+2] = decoder->mpr[sub_y+2][sub_x+3] = 
				(P_X + 2*P_A + P_B + 2) / 4;
			decoder->mpr[sub_y][sub_x+2] = decoder->mpr[sub_y+1][sub_x+3] = 
				(P_A + 2*P_B + P_C + 2) / 4;
			decoder->mpr[sub_y][sub_x+3] = (P_B + 2*P_C + P_D + 2) / 4;
		}
		break;
	case VERT_RIGHT_PRED:
		{
            decoder->mpr[sub_y][sub_x] = decoder->mpr[sub_y+2][sub_x+1] = 
				(P_X + P_A + 1) / 2;
			decoder->mpr[sub_y][sub_x+1] = decoder->mpr[sub_y+2][sub_x+2] = 
				(P_A + P_B + 1) / 2;
			decoder->mpr[sub_y][sub_x+2] = decoder->mpr[sub_y+2][sub_x+3] = 
				(P_B + P_C + 1) / 2;
			decoder->mpr[sub_y][sub_x+3] = 
				(P_C + P_D + 1) / 2;
			decoder->mpr[sub_y+1][sub_x] = decoder->mpr[sub_y+3][sub_x+1] = 
				(P_I + 2*P_X + P_A + 2) / 4;
			decoder->mpr[sub_y+1][sub_x+1] = decoder->mpr[sub_y+3][sub_x+2] = 
				(P_X + 2*P_A + P_B + 2) / 4;
			decoder->mpr[sub_y+1][sub_x+2] = decoder->mpr[sub_y+3][sub_x+3] = 
				(P_A + 2*P_B + P_C + 2) / 4;
            decoder->mpr[sub_y+1][sub_x+3] = (P_B + 2*P_C + P_D + 2) / 4;
			decoder->mpr[sub_y+2][sub_x] = (P_X + 2*P_I + P_J + 2) / 4;
			decoder->mpr[sub_y+3][sub_x] = (P_I + 2*P_J + P_K + 2) / 4;
		}
		break;
	case HOR_DOWN_PRED:
		{
			decoder->mpr[sub_y][sub_x] = decoder->mpr[sub_y+1][sub_x+2] = 
				(P_X + P_I + 1) / 2;
            decoder->mpr[sub_y][sub_x+1] = decoder->mpr[sub_y+1][sub_x+3] = 
				(P_I + 2*P_X + P_A + 2) / 4;
			decoder->mpr[sub_y][sub_x+2] = (P_X + 2*P_A + P_B + 2) / 4;
			decoder->mpr[sub_y][sub_x+3] = (P_A + 2*P_B + P_C + 2) / 4;
			decoder->mpr[sub_y+1][sub_x] = decoder->mpr[sub_y+2][sub_x+2] = 
				(P_I + P_J + 1) / 2;
			decoder->mpr[sub_y+1][sub_x+1] = decoder->mpr[sub_y+2][sub_x+3] = 
				(P_X + 2*P_I + P_J + 2) / 4;
			decoder->mpr[sub_y+2][sub_x] = decoder->mpr[sub_y+3][sub_x+2] = 
				(P_J + P_K + 1) / 2;
			decoder->mpr[sub_y+2][sub_x+1] = decoder->mpr[sub_y+3][sub_x+3] = 
				(P_I + 2*P_J + P_K + 2) / 4;
            decoder->mpr[sub_y+3][sub_x] = (P_K + P_L + 1) / 2;
			decoder->mpr[sub_y+3][sub_x+1] = (P_J + 2*P_K + P_L + 2) / 4;
		}
		break;
	case VERT_LEFT_PRED:
		{
            decoder->mpr[sub_y][sub_x] = (P_A + P_B + 1) / 2;
			decoder->mpr[sub_y][sub_x+1] = decoder->mpr[sub_y+2][sub_x] = 
                (P_B + P_C + 1) / 2;
			decoder->mpr[sub_y][sub_x+2] = decoder->mpr[sub_y+2][sub_x+1] = 
				(P_C + P_D + 1) / 2;
			decoder->mpr[sub_y][sub_x+3] = decoder->mpr[sub_y+2][sub_x+2] = 
				(P_D + P_E + 1) / 2;
			decoder->mpr[sub_y+2][sub_x+3] = (P_E + P_F + 1) / 2;
			decoder->mpr[sub_y+1][sub_x] = (P_A + 2*P_B + P_C + 2) / 4;
			decoder->mpr[sub_y+1][sub_x+1] = decoder->mpr[sub_y+3][sub_x] = 
				(P_B + 2*P_C + P_D + 2) / 4;
			decoder->mpr[sub_y+1][sub_x+2] = decoder->mpr[sub_y+3][sub_x+1] = 
				(P_C + 2*P_D + P_E + 2) / 4;
			decoder->mpr[sub_y+1][sub_x+3] = decoder->mpr[sub_y+3][sub_x+2] = 
				(P_D + 2*P_E + P_F + 2) / 4;
			decoder->mpr[sub_y+3][sub_x+3] = (P_E + 2*P_F + P_G + 2) / 4;
		}
		break;
	case HOR_UP_PRED:
		{
			decoder->mpr[sub_y][sub_x] = (P_I + P_J + 1) / 2;
			decoder->mpr[sub_y][sub_x+1] = (P_I + 2*P_J + P_K + 2) / 4;
			decoder->mpr[sub_y][sub_x+2] = decoder->mpr[sub_y+1][sub_x] = 
				(P_J + P_K + 1) / 2;
			decoder->mpr[sub_y][sub_x+3] = decoder->mpr[sub_y+1][sub_x+1] = 
				(P_J + 2*P_K + P_L + 2) / 4;
			decoder->mpr[sub_y+1][sub_x+2] = decoder->mpr[sub_y+2][sub_x+0] = 
				(P_K + P_L + 1) / 2;
			decoder->mpr[sub_y+1][sub_x+3] = decoder->mpr[sub_y+2][sub_x+1] = 
                (P_K + 2*P_L + P_L + 2) / 4;
			decoder->mpr[sub_y+2][sub_x+3] = decoder->mpr[sub_y+3][sub_x+1] = decoder->mpr[sub_y+3][sub_x] = 
				decoder->mpr[sub_y+2][sub_x+2] = decoder->mpr[sub_y+3][sub_x+2] = decoder->mpr[sub_y+3][sub_x+3] = P_L;			
		}
		break;
	}

}



void  intra_pred_chroma(decoder_t *decoder,macroblock_t *cur_mb,long uv)
{
	long x, y;
	picture_t *pic = decoder->cur_pic;
	uchar **img_uv = (uv==0) ? pic->img_u : pic->img_v;
	long  i,j;
	long  v = uv << 3;
	long  pred;

	x = decoder->mb_x << 3;
	y = decoder->mb_y << 3;
	switch( cur_mb->c_ipred_mode )
	{
	case INTRA_CHROMA_PRED_MODE_DC:
		{
			long js0=0,js1=0,js2=0,js3=0;
			long js[2][2];
			mb_access_t *access = decoder->mb_access;
			long a,b;
			long ii,jj;
			a = access->neighbour_mb_addr[decoder->cur_mb_nr][0];
			b = access->neighbour_mb_addr[decoder->cur_mb_nr][1];

			if( decoder->active_pps->constrained_intra_pred_flag ){
				if( a>=0 && (decoder->mb_date[a].mb_type!=I16MB || decoder->mb_date[a].mb_type!=I4MB) ){
					a = -1;
				}
				if( b>=0 && (decoder->mb_date[b].mb_type!=I16MB || decoder->mb_date[b].mb_type!=I4MB) ){
					b = -1;
				}
			}
			for( i=0;i<4;i++ ){
				if( b>=0 ){
                    js0 = js0 + img_uv[y-1][x+i];
					js1 = js1 + img_uv[y-1][x+i+4];
				}
				if( a>=0 ){
					js2 = js2 + img_uv[y+i][x-1];
					js3 = js3 + img_uv[y+i+4][x-1];
				}
			}

			if( a>=0 && b>=0 ){
				js[0][0] = (js0+js2+4)/8;
				js[0][1] = (js3+2)/4;
				js[1][0] = (js1+2)/4;
				js[1][1] = (js1+js3+4)/8;
			} else if( a<0 && b>=0 ){
				js[0][0] = js[0][1] = (js0+2)/4;
				js[1][0] = js[1][1] = (js1+2)/4;
			} else if( a>=0 && b<0 ){
				js[0][0] = js[1][0] = (js2+2)/4;
				js[0][1] = js[1][1] = (js3+2)/4;
			} else {
				js[0][0] = js[1][1] = js[0][1] = js[1][0] = 128;
			}
			for( jj=0;jj<2;jj++ ){
				for( ii=0;ii<2;ii++ ){
					for( j=0;j<4;j++ ){
						for( i=0;i<4;i++ ){
							decoder->mpr[(jj<<2)+j+16][(ii<<2)+i+v] = js[ii][jj];
						}
					}
				}
			}

		}
		break;
	case INTRA_CHROMA_PRED_MODE_HORIZONTAL:
		for( j=0;j<8;j++ ){
			pred = img_uv[y+j][x-1]; 
			for( i=0;i<8;i++ ){
				decoder->mpr[16+j][i+v] = pred;
			}
		}
		break;
	case INTRA_CHROMA_PRED_MODE_VERTICAL:
		for( j=0;j<8;j++ ){
			for( i=0;i<8;i++ ){
				decoder->mpr[16+j][i+v] = img_uv[y-1][x+i];
			}
		}
		break;
	case INTRA_CHROMA_PRED_MODE_PLANE:
		{
			long ih=0,iv=0,ib,ic,iaa;

			for( i=0;i<4;i++ ){
				ih += (i+1)*(img_uv[y-1][x+i+4]-img_uv[y-1][x+2-i]);
				iv += (i+1)*(img_uv[y+i+4][x-1]-img_uv[y+2-i][x-1]);
			}
			ib = (17*ih + 16) >> 5;
			ic = (17*iv + 16) >> 5;
			iaa=16*(img_uv[y-1][x+7]+img_uv[y+7][x-1]);
			for( j=0;j<8;j++ ){
				for( i=0;i<8;i++ ){
					pred = (iaa + (i-3)*ib + (j-3)*ic + 16)>>5;
					decoder->mpr[16+j][v+i] = CLIP3(0,255,pred);
				}
			}
		}
		break;
	default:
		assert(0==1);
	}
}
