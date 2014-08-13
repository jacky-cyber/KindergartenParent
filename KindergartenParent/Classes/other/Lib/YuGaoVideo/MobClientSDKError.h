#ifndef _MOBCLIENT_SDK_ERROR_H_
#define _MOBCLIENT_SDK_ERROR_H_

//////////////////////////////////////////////////////////////////////////
// Err No
// 10001~20000 for NetCon
#define ERR_NC_CREATE_CON_FAILED						10001	// 创建连接失败
#define ERR_NC_CONNECT_DSERVER_FAILED					10002	// 建立DServer连接失败
#define ERR_NC_CONNECT_MEDIASERVER_FAILED				10003	// 建立MediaServer连接失败

// 20001~30000 for DServer
// 未知错误
#define ERR_DS_UNKOWN									20001
// 与D-Server连接断开
#define ERR_DS_DISCONNECT								20002
// 没有D-Server服务器连接
#define ERR_DS_DSVR_NOT_CONNECTED						20003
// D-Server应答: 协议头长度太小
#define ERR_DS_REP_HEAD_LEN_TOO_SHORT					20021
// D-Server应答: 协议头记录的Payload长度与实际Payload长度不一致
#define ERR_DS_REP_HEAD_PAYLOAD_LEN_DIFF				20022
// D-Server应答: 命令没有相应的处理函数
#define ERR_DS_REP_CMD_NO_HANDLER						20023
// 插件用户登录
// D-Server请求: 《插件用户登录》命令发送失败
#define ERR_DS_ACTIVEXLOGIN_REQ_SEND_FAILED				20101
// D-Server应答: Challenge的Payload长度不正确
#define ERR_DS_CHALLENGE_REP_PAYLOAD_INCORRECT			20102
// D-Server请求: Respond Challenge命令发送失败
#define ERR_DS_RESPONDCHALLENGE_REQ_SEND_FAILED			20103
// D-Server应答: Respond Login的Payload长度不正确
#define ERR_DS_RESPONDLOGIN_REP_PAYLOAD_INCORRECT		20104
// D-Server应答: Respond Login的用户名错误
#define ERR_DS_RESPONDLOGIN_REP_WRONG_USERNAME			20105
// D-Server应答: Respond Login的密码错误
#define ERR_DS_RESPONDLOGIN_REP_WRONG_PASSWORD			20106
// D-Server应答: 超过限制
#define ERR_DS_RESPONDLOGIN_REP_BEYOND_SYSTEM_LIMIT		20107
// D-Server请求: Report Network Type Ex命令发送失败
#define ERR_DS_REPORTNETWORKEX_REQ_SEND_FAILED			20108
//D-Server应答：Respond Login的账号锁定
#define ERR_DS_RESPONDLOGIN_REP_USER_LOCKED             20109
//D-Server应答：Respond Login的账号过期
#define ERR_DS_RESPONDLOGIN_REP_USER_EXPIRED            20110

// 获取分组信息
// D-Server请求: 《获取分组信息》命令发送失败
#define ERR_DS_GETGROUPTREE_REQ_SEND_FAILED				20201
// D-Server应答: 《获取分组信息》的Payload长度不正确
#define ERR_DS_GETGROUPTREE_REP_PAYLOAD_INCORRECT		20202
// 获取摄像机与分组所属关系
// D-Server请求: 《获取摄像机与分组所属关系》命令发送失败
#define ERR_DS_GETCAMERAGROUP_REQ_SEND_FAILED			20301
// D-Server应答: 《获取摄像机与分组所属关系》的Payload长度不正确
#define ERR_DS_GETCAMERAGROUP_REP_PAYLOAD_INCORRECT		20302
// 插件用户获取设备列表
// D-Server请求: 《插件用户获取设备列表》命令发送失败
#define ERR_DS_GETCAMERALIST_REQ_SEND_FAILED			20401
// D-Server应答: 《插件用户获取设备列表》的Payload长度不正确
#define ERR_DS_GETCAMERALIST_REP_PAYLOAD_INCORRECT		20402
// 没有匹配到分组
#define ERR_DS_GETCAMERALIST_REP_NO_GROUP				20403
// 获取摄像机扩展信息
// D-Server请求: 《获取摄像机扩展信息》命令发送失败
#define ERR_DS_GETCAMEXTENDINFO_REQ_SEND_FAILED			20501
// D-Server应答: 《获取摄像机扩展信息》的Payload长度不正确
#define ERR_DS_GETCAMEXTENDINFO_REP_PAYLOAD_INCORRECT	20502
// 连接摄像机
// D-Server请求: 《连接摄像机》命令发送失败
#define ERR_DS_CONCAM_REQ_SEND_FAILED					20601
// D-Server请求: 《连接摄像机》摄像头序列号无效
#define ERR_DS_CONCAM_INVALID_REQ_SERIALNO				20602
// D-Server应答: 《连接摄像机》的Payload长度不正确
#define ERR_DS_CONCAM_REP_PAYLOAD_INCORRECT				20603
// D-Server应答: 《连接摄像机》超出摄像机使用期限
#define ERR_DS_CONCAM_REP_CAMERA_EXPIRED			    20604
// D-Server应答: 《连接摄像机》该时段内不允许播放
#define ERR_DS_CONCAM_REP_OUT_VIEWTIME				    20605
// D-Server应答: 《连接摄像机》摄像机不可用
#define ERR_DS_CONCAM_REP_CAMERA_FORBIDDEN              20606

// 获取Nvr列表
// D-Server请求: 《获取Nvr列表》摄像头序列号无效
#define ERR_DS_GETNVRLIST_REQ_SEND_FAILED				20701
// D-Server应答: 《获取Nvr列表》的Payload长度不正确
#define ERR_DS_GETNVRLIST_REP_PAYLOAD_INCORRECT			20702
// 连接NVR
// D-Server请求: 《连接NVR》命令发送失败
#define ERR_DS_CONNVR_REQ_SEND_FAILED					20801
// D-Server应答: 《连接NVR》的Payload长度不正确
#define ERR_DS_CONNVR_REP_PAYLOAD_INCORRECT				20802
// 请求连接摄像机（用于新手机客户端）
// D-Server请求: 命令发送失败
#define ERR_DS_CONCAM4MOB_REQ_SEND_FAILED				20901
// D-Server请求: 摄像头序列号无效
#define ERR_DS_CONCAM4MOB_INVALID_REQ_SERIALNO			20902
// D-Server应答: Payload长度不正确
#define ERR_DS_CONCAM_4MOBREP_PAYLOAD_INCORRECT			20903
// D-Server应答: 《连接摄像机》超出摄像机使用期限
#define ERR_DS_CONCAM4MOB_REP_CAMERA_EXPIRED			20904
// D-Server应答: 《连接摄像机》该时段内不允许播放
#define ERR_DS_CONCAM4MOB_REP_OUT_VIEWTIME				20905
// D-Server应答: 《连接摄像机》摄像机不可用
#define ERR_DS_CONCAM4MOB_REP_CAMERA_FORBIDDEN          20906

// D-Server请求: 上报推送信息失败!
#define ERR_DS_REPORT_PUSHINFO_FAILED                   21005
// D-Server应答: 账号禁止推送
#define ERR_DS_PUSH_FORBIDDEN				            21006
// D-Server应答: 账号禁止推送
#define ERR_DS_PUSH_SCUCCED				                21007



// 30001~40000 for MediaServer
#define ERR_MS_UNKNOWN									30001   // 未知错误
#define ERR_MS_DISCONNECT								30002   // 连接断开

#define ERR_MS_USER_NOT_EXISTS							31001   // 用户名不存在            
#define ERR_MS_AUTHEN_FAILED							31002   // 认证失败                
#define ERR_MS_INVALID_SESSION_ID	 					31003   // 无效的会话id            
#define ERR_MS_INVALID_PRIVILEGES						31004   // 无效的权限              
#define ERR_MS_INVALID_BUFFER_SIZE						31005   // 无效的缓冲大小          
#define ERR_MS_INVALID_PLAY_TYPE                        31006   // 无效的播放类型          
#define ERR_MS_INVALID_PLAY_TIME                        31007   // 无效的播放时间          
#define ERR_MS_INVALID_SESSION_TYPE	 					31008   // 无效的会话类型          
#define ERR_MS_ALLOC_FAILED								31009   // 内存申请失败            
#define ERR_MS_INVALID_CAMERAID							31010   // 无效的摄像机            
#define ERR_MS_PACKET_LEN     							31011   // 无效的数据包大小        
#define ERR_MS_ONLINE_LEN     							31012   // 无效的ONLINE CMD长度    
#define ERR_MS_INVALID_BUFF_SIZE						31013   // 无效的缓冲大小          
#define ERR_MS_PLAY_FAILED   							31014   // 播放失败                
#define ERR_MS_STOP_FAILED   							31015   // 停止失败                
#define ERR_MS_SWITCH_AUDIO_FAILED  					31016   // 摄像机麦克风打开失败    
#define ERR_MS_MAX_PALYBACK_COUNT_REACH					31017   // 录像回放数量达到上限    
#define ERR_MS_MAX_REALTIME_COUNT_REACH					31018   // 实时播放数量达到上限    
#define ERR_MS_NET_POINTER_NULL      					31019   // 网络连接指针为空        
#define ERR_MS_AUTHENTICATE_PACKET      				31020   // 无效的认证数据包        
#define ERR_MS_GET_CFG_VIEWER_POINTER_FAILED            31021   // 获取viewer设置失败      
#define ERR_MS_NULL_AUTHENTICATE_USER_NAME   			31022   // 用户名为空              
#define ERR_MS_INVALID_COMMAND_ID             			31023   // 无效的命令              
#define ERR_MS_INVALID_GROUP_CODE	           			31024   // 无效的GROUP CODE        
#define ERR_MS_MALLOC_COMMAND_SESSION_FAILED   			31025   // 命令会话申请失败        
#define ERR_MS_REGISTER_SYCN_QUERY_POINTER_FAILED		31026   // 异步查询申请失败        
#define ERR_MS_MALLOC_MEDIA_SESSION_FAILED   			31027   // 媒体会话申请失败        
#define ERR_MS_REGISTER_NULL_MEDIA_POINTER   			31028   // 注册媒体返回为空        
#define ERR_MS_INVALID_PACKET_FORMATE		   			31029   // 无效的数据包格式        
#define ERR_MS_INVALID_USR_LEVEL  						31030   // 无效的用户等级          
#define ERR_MS_NO_MOTION_PRI      						31031   // 没有观看动态画面权限    
#define ERR_MS_NO_QUICK_DWON_PRI   						31032   // 没有快速下载权限        
#define ERR_MS_NO_QUICK_DWON_AND_MOTION_PRI   			31033   // 没有快速下载动态画面权限
#define ERR_MS_COMMON_FAILED                 			31034   // 一般错误                
#define ERR_MS_DOWNLOAD_REDOWNLOAD						31035   // 下载失败                
#define ERR_MS_SEEK_FAILED								31036   // 回放定位失败            
#define ERR_MS_GET_RECORD_QUALITY_FAILED				31037   // 录像品质获取失败        
#define ERR_MS_INVALID_SEEK_TYPE						31038   // 录像定位类型无效        
#define ERR_MS_INVALID_SEEK_TIME						31039   // 录像定位时间无效        
#define ERR_MS_INVALID_SET_REPORTLOG_PARAM              31040   // 错误上报的参数无效      
#define ERR_MS_INVALID_SERIALNO                         31041   // 无效的序列号            
#define ERR_MS_SET_RECTYPE_FAILED            			31042   // 录像类型设置失败        
#define ERR_MS_INVALID_RECTYPE      					31043   // 无效的录像类型          
#define ERR_MS_SAME_RECTYPE								31044   // 设置了相同的录像类型    
#define ERR_MS_NOT_OWNER_ACCOUNT						31045   // 没有所属账户            
#define ERR_MS_INVALID_FTP_ACCOUNT						31046   // 无效的ftp账户           
#define ERR_MS_CON_FTPSVR_FAILED                 	    31047   // ftp服务器连接失败       
#define ERR_MS_GET_CAMEXTENDINFO                        31048   // 摄像机扩展信息获取失败  
#define ERR_MS_INVALID_IMAGE_PATH						31049   // 无效的image路径         
#define ERR_MS_OPEN_IMAGE_FILE_FAILED					31050   // 打开摄像机文件失败      
#define ERR_MS_OTHER_UPDATE								31051   // 其他更新失败            
#define ERR_MS_SEEK										31052   // 录像定位失败            
#define ERR_MS_UPDATE_IMAGE_FAILED                      31053   // 更新image失败           
#define ERR_MS_SET_VIGOLOGEVENT_FAILED                  31054   // 设置日志事件失败        

#define ERR_MS_MAX_COMMAND_SESSION_REACH				31224   // 命令会话达到上限          
#define ERR_MS_MAX_MEDIA_SESSION_REACH					31225   // 媒体会话达到上限          
#define ERR_MS_MAX_MEDIA_SESSION_CON_REACH				31226   // 媒体会话连接达到上限      
#define ERR_MS_GET_GROUPS_FAILED						31227   // 分组列表获取失败          
#define ERR_MS_INVALID_CAMERA_NUM						31228   // 无效的摄像机数量          
#define ERR_MS_NOT_AUTHEN       						31229   // 还没认证                  
#define ERR_MS_SET_BUFF_FAILED    						31230   // 设置缓冲失败              
#define ERR_MS_GET_CAMERA_FAILED						31231   // 摄像机列表获取失败        
#define ERR_MS_CAMERAS_FOR_USER_TOO_LARGE				31232   // 用户的摄像机数量太多了    
#define ERR_MS_PLAY_SPEED     		     				31233   // 播放速度设置失败          
#define ERR_MS_PLAY_TYPE     		     				31234   // 播放类型设置失败          
#define ERR_MS_GROUPS_FOR_USER_TOO_LARGE    			31235   // 用户的摄像机分组数量太多了
#define ERR_MS_CREATE_GROUP_INFO_POINTER    			31236   // 分组信息创建错误          
#define ERR_MS_CREATE_CAMERA_INFO_POINTER   			31237   // 摄像机信息创建错误        
#define ERR_MS_CREATE_CAMERA_COUNT_POINTER   			31238   // 摄像机数量创建错误        
#define ERR_MS_CREATE_EX_BUFFER_POINTER      			31239   // 扩展缓冲创建错误          
                                                                                         
#define ERR_MS_GET_CFGIO_POINTER_FAILED     			31112   // 获取IO配置失败            
#define ERR_MS_UPDATE_LOCALE_CFG_FAILED     			31113   // 更新本地配置失败          
#define ERR_MS_GET_NVR_BUFFER_FAILED     				31114   // 获取NVR缓冲失败           
#define ERR_MS_GET_DISK_INFO_FAILED     		    	31115   // 获取磁盘信息失败          
#define ERR_MS_MALLOC_BUFFER_SIZE_FAILED     	   		31116   // 内存申请失败              
#define ERR_MS_INVALID_DRIVE_TAG                	   	31117   // 无效的驱动标记            

#define ERR_MS_RPS_UNKNOWN								32001
#define ERR_MS_RPS_OPEN_FILE							32002   // open file error (record 1. configure 2 minute file)
#define ERR_MS_RPS_NOT_FIND_FILE						32003   // can't find file 
#define ERR_MS_RPS_NOT_FIND_DIR							32004   // directory is not exist (1.Search Day 2.Search Hour 3.SearchMin 4.Search Size)
#define ERR_MS_RPS_CREATE_DIR							32005   // create directory error
#define ERR_MS_RPS_CREATE_FILE							32006   // create file error
#define ERR_MS_RPS_READ_CFG_FILE						32007   // read configure file error 
#define ERR_MS_RPS_READ_MIN_FILE						32008   // read minute file error
#define ERR_MS_RPS_OUT_OF_MEMORY						32009   // out of memory
#define ERR_MS_RPS_PARSING_ELEMENT						32010   // parser error 
#define ERR_MS_RPS_PARSING_BUFFER						32011   // parser RINGBUFFER error   
#define ERR_MS_RPS_OUT_OF_SPACE							32012   // disk out of space (record)
#define ERR_MS_RPS_DOCUMENT_EMPTY						32013   // document empty
#define ERR_MS_RPS_NO_FRAME								32014   // lose frame 
#define ERR_MS_RPS_RECORDING_IO							32015   // I/O error 
#define ERR_MS_RPS_VERSION								32016   // version error (old version does not support playback)
#define ERR_MS_RPS_TIME									32017   // time error 
#define ERR_MS_RPS_PACKET								32018   // packet error  (read frame packet)
#define ERR_MS_RPS_ADJUST_DISK							32019   // adjust disk error 
#define ERR_MS_RPS_PARAM								32020   // param error     
#define ERR_MS_RPS_INSERT_PACKET						32021   // insert packet error 
#define ERR_MS_RPS_FULL_STOP							32022   // full stop 
#define ERR_MS_RPS_MODIFY_CAMERA_NAME					32023   // modify camera name error 
#define ERR_MS_RPS_SAVE_DAY_CFG							32024   // save day configure error 
#define ERR_MS_RPS_UPDATE_TO_TIME						32025   // update time error 
#define ERR_MS_RPS_RENAME_FILE							32026   // update file name error 
#define ERR_MS_RPS_SAVE_FILE_HEADER						32027   // save file header error 
#define ERR_MS_RPS_NOTACCORD_PLAN						32028   // not according to record plan 
#define ERR_MS_RPS_IN_OF_SPACE							32029   // disk in space 
#define ERR_MS_RPS_DIR_IS_SON							32030   // this dir is sub dir of other 
#define ERR_MS_RPS_DIR_IS_PARENT						32031   // this dir is parent of other 
#define ERR_MS_RPS_DEL_FINISH							32032   // delete record file finish
#define ERR_MS_RPS_REACH_MAX_PATH_COUNT					32033   // SetManagePath max path count (10)
#define ERR_MS_RPS_NOT_IN_SCHEDULE						32034   // not in schedule
#define ERR_MS_RPS_IN_SCHEDULE							32035   // in schedule
#define ERR_MS_RPS_PLAY_FINISH							32036   // play finish
#define ERR_MS_RPS_MAX_CHAN_REACH						32037   // max channel reach
#define ERR_MS_RPS_NEWBUFFER							32038   // new buffer eeor
#define ERR_MS_RPS_NO_DOWNLOADBUFFER					32039   // no download buffer
#define ERR_MS_RPS_PLAYTYPE								32040   // play type error
#define ERR_MS_RPS_NO_READ_POINT						32041   // read point error

// 40001~50000 for Dispatch
#define ERR_DP_INVALID_DSERVERIP		                40001	// 无效的注册服务器地址
#define ERR_DP_INVALID_USERNAME			                40002	// 无效的用户名
#define ERR_DP_INVALID_PASSWORD			                40003	// 无效的密码
#define ERR_DP_USER_LOGIN_NOTYET		                40004	// 用户尚未登录
#define ERR_DP_REG_NETCON_FAILED		                40005	// 注册NetCon模块失败
#define ERR_DP_REG_DSERVER_FAILED		                40006	// 注册DServer模块失败
#define ERR_DP_REG_MEDIASERVER_FAILED	                40007	// 注册MeidaServer模块失败
#define ERR_DP_INVALID_NVR				                40008	// 无效的NVR
#define ERR_DP_INVALID_VIEW				                40009	// 无效的View
#define ERR_DP_NEW_FAILED				                40010	// 动态内存分配失败
#define ERR_DP_GETINFO_TIMEOUT			                40011	// DServer连接信息获取超时
#define ERR_DP_INVALID_SN				                40012	// 无效的序列号

// 50001~60000 for 7620 Control
// 未知错误
#define ERR_7620_UNKNOWN                                30001
// 断开连接
#define ERR_7620_DISCONNECT                             30002
// 获取SSID列表失败
#define ERR_7620_GETSSIDLIST_FAILED                     30003
// 连接SSID失败
#define ERR_7620_CONNECTSSID_FAILED                     30004
// 设置SSID失败
#define ERR_7620_SETSSID_FAILED                         30005
//////////////////////////////////////////////////////////////////////////

#endif // _MOBCLIENT_SDK_ERROR_H_
