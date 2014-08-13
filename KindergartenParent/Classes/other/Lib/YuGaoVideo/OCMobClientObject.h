//
//  OCMobClientObject.h
//  iViewer-New
//
//  Created by apple on 13-11-28.
//  Copyright 2013 HuangSiwen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <unistd.h>

////////////////////////////////////////////////////////////////////////////////
//data types define
typedef unsigned char   UCHAR;
typedef int				INT;
typedef char*	        STRING;
typedef long			LONG;
typedef unsigned long   DWORD;
typedef unsigned long   ULONG;
typedef const char*     LPCTSTR;
typedef char*           PCHAR;
typedef unsigned char*  PUCHAR;
typedef char			CHAR;
typedef unsigned char   BYTE;
typedef short			SHORT;
typedef unsigned short  WORD;
typedef unsigned short  USHORT;
typedef unsigned int	UINT;
////////////////////////////////////////////////////////////////////////////////


@interface UserInfo : NSObject {
    UINT	nPurviewInfo;	// 权限信息
    // bit0:
    // bit1:	回放功能
    // bit2:	拍照功能
    // bit3:	录像功能
    // bit4:	PTZ功能
    // bit5:	设置品质功能
    // bit6:	麦克风输入
    // bit7:	声音输出
	UINT	nPlayTime;		// 摄像机播放限制时间（0表示无限制）
    // 单位:秒
	UINT	bIsSMSCheck;	// 0:no 1:yes
	DWORD	dwSMSServerIP;	// 短信服务器地址 端口固定为3571
	NSData *szPhoneNo;    // 短信接收者号码
}
@property (nonatomic, assign) UINT nPurviewInfo;
@property (nonatomic, assign) UINT nPlayTime;
@property (nonatomic, assign) UINT bIsSMSCheck;
@property (nonatomic, assign) DWORD	dwSMSServerIP;
@property (nonatomic, retain) NSData *szPhoneNo;
@end


@interface CameraInfo : NSObject {
	DWORD       dwDeviceID;         // 平台数据库中ID
	DWORD       dwCameraID;         // 摄像机ID
	DWORD       dwParentID;         // 摄像机所属分组ID
    UINT        nExtendInfo;        // 摄像机扩展信息
    BYTE        IsOnline;           // 摄像机是否可以观看
    NSString    *szCameraName;		// 摄像机名称
	NSString    *szCameraSerialNO;	// 摄像机序列号
	NSData    *szCameraPassword;	// 摄像机密码
}
@property (nonatomic, assign) DWORD	dwDeviceID;
@property (nonatomic, assign) DWORD	dwCameraID;
@property (nonatomic, assign) DWORD	dwParentID;
@property (nonatomic, assign) UINT  nExtendInfo;
@property (nonatomic, assign) BYTE  IsOnline;
@property (nonatomic, retain) NSString *szCameraName;
@property (nonatomic, retain) NSString *szCameraSerialNO;
@property (nonatomic, retain) NSData *szCameraPassword;
@end


@interface GroupInfo : NSObject {
	DWORD       dwGroupID;		// 分组ID
	DWORD       dwParentID;		// 分组所属分组ID
    NSString    *strGroupName;	// 分组名称
	DWORD       dwSequence;		// 分组排序使用
}
@property (nonatomic, assign) DWORD	dwGroupID;
@property (nonatomic, assign) DWORD	dwParentID;
@property (nonatomic, assign) DWORD	dwSequence;
@property (nonatomic, retain) NSString *strGroupName;
@end


@interface CameraExInfo : NSObject {
	DWORD   dwDeviceID;		// 平台数据库中ID
	DWORD   dwCameraID;		// 摄像机ID
	UINT    nExtendInfo;	// 摄像机扩展信息
}
@property (nonatomic, assign) DWORD	dwDeviceID;
@property (nonatomic, assign) DWORD	dwCameraID;
@property (nonatomic, assign) UINT	nExtendInfo;
@end


@interface NvrInfo : NSObject {
	DWORD       dwNvrID;        // Nvr ID
    NSString    *szNvrName;		// Nvr名称
    NSString    *szSerialNo;	// Nvr序列号
	BYTE        IsOnline;       // Nvr是否在线
}
@property (nonatomic, assign) DWORD	dwNvrID;
@property (nonatomic, retain) NSString *szNvrName;
@property (nonatomic, retain) NSString *szSerialNo;
@property (nonatomic, assign) BYTE	IsOnline;
@end


@interface NetInfo : NSObject {
	DWORD	dwPublicIP;		// 公网IP
	WORD	wPublicPortTCP;	// 公网TCP端口
	WORD	wPublicPortUDP;	// 公网UDP端口
	WORD	wLocalPortUDP;	// 内网UDP端口
	NSArray *LocalIPs;      // 内网IP集
	WORD	wNetworkType;	// 网络类型 1-具有公网IP 2-UDP可用 3-两者都有
}
@property (nonatomic, assign) DWORD	dwPublicIP;
@property (nonatomic, assign) WORD	wPublicPortTCP;
@property (nonatomic, assign) WORD	wPublicPortUDP;
@property (nonatomic, assign) WORD	wLocalPortUDP;
@property (nonatomic, retain) NSArray	*LocalIPs;
@property (nonatomic, assign) WORD	wNetworkType;
@end


@interface DevConInfo : NSObject {
	DWORD       dwDeviceID;		// 设备ID 摄像机指dwCameraID / Nvr指dwNvrID
	NetInfo     *tNetInfo;		// 连接信息
	NSString    *strUserName;	// 连接使用-用户名
	NSString    *strPassword;	// 连接使用-密码
}
@property (nonatomic, assign) DWORD     dwDeviceID;
@property (nonatomic, retain) NetInfo   *tNetInfo;
@property (nonatomic, assign) NSString  *strUserName;
@property (nonatomic, assign) NSString  *strPassword;
@end

@interface DevConSuccInfo : NSObject {
    DWORD dwDeviceID;		// 设备ID 摄像机指dwCameraID / Nvr指dwNvrID
	DWORD dwIp;				// 连接IP
	WORD wPort;				// 连接端口
	int nType;				// 通信协议类型 0-UDP/1-TCP
	int nConType;			// 连接类型 0-直连(主动)/1-直连(被动)/2-转发
}
@property (nonatomic, assign) DWORD dwDeviceID;
@property (nonatomic, assign) DWORD dwIp;
@property (nonatomic, assign) WORD wPort;
@property (nonatomic, assign) int nType;
@property (nonatomic, assign) int nConType;
@end

enum { AUDIOGSM, VIDEOH264 };
@interface MediaData : NSObject {
    DWORD	dwCameraID;             // 摄像机ID
	struct timeval tCameraTime;     // 摄像机时间
	struct timeval tMediaTime;      // 录像时间
	WORD	usISeq;                 // I帧（GOP）序列号
	BYTE	bSubSeq;                // GOP子帧序列号，0表示I帧，其他表示P帧
    int     nFlipType;              // 图像翻转类型：0-无，1-左右，2-上下，3-上下左右（旋转180度）
	int		nMediaFormat;           // 数据类型：音频或视频
	BYTE*	pRawData;               // 媒体数据
	int		nRawLen;                // 数据长度
}
@property (nonatomic, assign) DWORD	dwCameraID;
@property (nonatomic, assign) struct timeval tCameraTime;
@property (nonatomic, assign) struct timeval tMediaTime;
@property (nonatomic, assign) WORD	usISeq;
@property (nonatomic, assign) BYTE	bSubSeq;
@property (nonatomic, assign) int   nFlipType;
@property (nonatomic, assign) int   nMediaFormat;
@property (nonatomic, assign) BYTE*	pRawData;
@property (nonatomic, assign) int	nRawLen;
@end
