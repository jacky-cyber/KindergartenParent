#ifndef _MOBCLIENT_SDK_ERROR_H_
#define _MOBCLIENT_SDK_ERROR_H_

//////////////////////////////////////////////////////////////////////////
// Err No
// 10001~20000 for NetCon
#define ERR_NC_CREATE_CON_FAILED						10001	// ��������ʧ��
#define ERR_NC_CONNECT_DSERVER_FAILED					10002	// ����DServer����ʧ��
#define ERR_NC_CONNECT_MEDIASERVER_FAILED				10003	// ����MediaServer����ʧ��

// 20001~30000 for DServer
// δ֪����
#define ERR_DS_UNKOWN									20001
// ��D-Server���ӶϿ�
#define ERR_DS_DISCONNECT								20002
// û��D-Server����������
#define ERR_DS_DSVR_NOT_CONNECTED						20003
// D-ServerӦ��: Э��ͷ����̫С
#define ERR_DS_REP_HEAD_LEN_TOO_SHORT					20021
// D-ServerӦ��: Э��ͷ��¼��Payload������ʵ��Payload���Ȳ�һ��
#define ERR_DS_REP_HEAD_PAYLOAD_LEN_DIFF				20022
// D-ServerӦ��: ����û����Ӧ�Ĵ�����
#define ERR_DS_REP_CMD_NO_HANDLER						20023
// ����û���¼
// D-Server����: ������û���¼�������ʧ��
#define ERR_DS_ACTIVEXLOGIN_REQ_SEND_FAILED				20101
// D-ServerӦ��: Challenge��Payload���Ȳ���ȷ
#define ERR_DS_CHALLENGE_REP_PAYLOAD_INCORRECT			20102
// D-Server����: Respond Challenge�����ʧ��
#define ERR_DS_RESPONDCHALLENGE_REQ_SEND_FAILED			20103
// D-ServerӦ��: Respond Login��Payload���Ȳ���ȷ
#define ERR_DS_RESPONDLOGIN_REP_PAYLOAD_INCORRECT		20104
// D-ServerӦ��: Respond Login���û�������
#define ERR_DS_RESPONDLOGIN_REP_WRONG_USERNAME			20105
// D-ServerӦ��: Respond Login���������
#define ERR_DS_RESPONDLOGIN_REP_WRONG_PASSWORD			20106
// D-ServerӦ��: ��������
#define ERR_DS_RESPONDLOGIN_REP_BEYOND_SYSTEM_LIMIT		20107
// D-Server����: Report Network Type Ex�����ʧ��
#define ERR_DS_REPORTNETWORKEX_REQ_SEND_FAILED			20108
//D-ServerӦ��Respond Login���˺�����
#define ERR_DS_RESPONDLOGIN_REP_USER_LOCKED             20109
//D-ServerӦ��Respond Login���˺Ź���
#define ERR_DS_RESPONDLOGIN_REP_USER_EXPIRED            20110

// ��ȡ������Ϣ
// D-Server����: ����ȡ������Ϣ�������ʧ��
#define ERR_DS_GETGROUPTREE_REQ_SEND_FAILED				20201
// D-ServerӦ��: ����ȡ������Ϣ����Payload���Ȳ���ȷ
#define ERR_DS_GETGROUPTREE_REP_PAYLOAD_INCORRECT		20202
// ��ȡ����������������ϵ
// D-Server����: ����ȡ����������������ϵ�������ʧ��
#define ERR_DS_GETCAMERAGROUP_REQ_SEND_FAILED			20301
// D-ServerӦ��: ����ȡ����������������ϵ����Payload���Ȳ���ȷ
#define ERR_DS_GETCAMERAGROUP_REP_PAYLOAD_INCORRECT		20302
// ����û���ȡ�豸�б�
// D-Server����: ������û���ȡ�豸�б������ʧ��
#define ERR_DS_GETCAMERALIST_REQ_SEND_FAILED			20401
// D-ServerӦ��: ������û���ȡ�豸�б���Payload���Ȳ���ȷ
#define ERR_DS_GETCAMERALIST_REP_PAYLOAD_INCORRECT		20402
// û��ƥ�䵽����
#define ERR_DS_GETCAMERALIST_REP_NO_GROUP				20403
// ��ȡ�������չ��Ϣ
// D-Server����: ����ȡ�������չ��Ϣ�������ʧ��
#define ERR_DS_GETCAMEXTENDINFO_REQ_SEND_FAILED			20501
// D-ServerӦ��: ����ȡ�������չ��Ϣ����Payload���Ȳ���ȷ
#define ERR_DS_GETCAMEXTENDINFO_REP_PAYLOAD_INCORRECT	20502
// ���������
// D-Server����: ������������������ʧ��
#define ERR_DS_CONCAM_REQ_SEND_FAILED					20601
// D-Server����: �����������������ͷ���к���Ч
#define ERR_DS_CONCAM_INVALID_REQ_SERIALNO				20602
// D-ServerӦ��: ���������������Payload���Ȳ���ȷ
#define ERR_DS_CONCAM_REP_PAYLOAD_INCORRECT				20603
// D-ServerӦ��: ����������������������ʹ������
#define ERR_DS_CONCAM_REP_CAMERA_EXPIRED			    20604
// D-ServerӦ��: ���������������ʱ���ڲ�������
#define ERR_DS_CONCAM_REP_OUT_VIEWTIME				    20605
// D-ServerӦ��: ������������������������
#define ERR_DS_CONCAM_REP_CAMERA_FORBIDDEN              20606

// ��ȡNvr�б�
// D-Server����: ����ȡNvr�б�����ͷ���к���Ч
#define ERR_DS_GETNVRLIST_REQ_SEND_FAILED				20701
// D-ServerӦ��: ����ȡNvr�б���Payload���Ȳ���ȷ
#define ERR_DS_GETNVRLIST_REP_PAYLOAD_INCORRECT			20702
// ����NVR
// D-Server����: ������NVR�������ʧ��
#define ERR_DS_CONNVR_REQ_SEND_FAILED					20801
// D-ServerӦ��: ������NVR����Payload���Ȳ���ȷ
#define ERR_DS_CONNVR_REP_PAYLOAD_INCORRECT				20802
// ����������������������ֻ��ͻ��ˣ�
// D-Server����: �����ʧ��
#define ERR_DS_CONCAM4MOB_REQ_SEND_FAILED				20901
// D-Server����: ����ͷ���к���Ч
#define ERR_DS_CONCAM4MOB_INVALID_REQ_SERIALNO			20902
// D-ServerӦ��: Payload���Ȳ���ȷ
#define ERR_DS_CONCAM_4MOBREP_PAYLOAD_INCORRECT			20903
// D-ServerӦ��: ����������������������ʹ������
#define ERR_DS_CONCAM4MOB_REP_CAMERA_EXPIRED			20904
// D-ServerӦ��: ���������������ʱ���ڲ�������
#define ERR_DS_CONCAM4MOB_REP_OUT_VIEWTIME				20905
// D-ServerӦ��: ������������������������
#define ERR_DS_CONCAM4MOB_REP_CAMERA_FORBIDDEN          20906

// D-Server����: �ϱ�������Ϣʧ��!
#define ERR_DS_REPORT_PUSHINFO_FAILED                   21005
// D-ServerӦ��: �˺Ž�ֹ����
#define ERR_DS_PUSH_FORBIDDEN				            21006
// D-ServerӦ��: �˺Ž�ֹ����
#define ERR_DS_PUSH_SCUCCED				                21007



// 30001~40000 for MediaServer
#define ERR_MS_UNKNOWN									30001   // δ֪����
#define ERR_MS_DISCONNECT								30002   // ���ӶϿ�

#define ERR_MS_USER_NOT_EXISTS							31001   // �û���������            
#define ERR_MS_AUTHEN_FAILED							31002   // ��֤ʧ��                
#define ERR_MS_INVALID_SESSION_ID	 					31003   // ��Ч�ĻỰid            
#define ERR_MS_INVALID_PRIVILEGES						31004   // ��Ч��Ȩ��              
#define ERR_MS_INVALID_BUFFER_SIZE						31005   // ��Ч�Ļ����С          
#define ERR_MS_INVALID_PLAY_TYPE                        31006   // ��Ч�Ĳ�������          
#define ERR_MS_INVALID_PLAY_TIME                        31007   // ��Ч�Ĳ���ʱ��          
#define ERR_MS_INVALID_SESSION_TYPE	 					31008   // ��Ч�ĻỰ����          
#define ERR_MS_ALLOC_FAILED								31009   // �ڴ�����ʧ��            
#define ERR_MS_INVALID_CAMERAID							31010   // ��Ч�������            
#define ERR_MS_PACKET_LEN     							31011   // ��Ч�����ݰ���С        
#define ERR_MS_ONLINE_LEN     							31012   // ��Ч��ONLINE CMD����    
#define ERR_MS_INVALID_BUFF_SIZE						31013   // ��Ч�Ļ����С          
#define ERR_MS_PLAY_FAILED   							31014   // ����ʧ��                
#define ERR_MS_STOP_FAILED   							31015   // ֹͣʧ��                
#define ERR_MS_SWITCH_AUDIO_FAILED  					31016   // �������˷��ʧ��    
#define ERR_MS_MAX_PALYBACK_COUNT_REACH					31017   // ¼��ط������ﵽ����    
#define ERR_MS_MAX_REALTIME_COUNT_REACH					31018   // ʵʱ���������ﵽ����    
#define ERR_MS_NET_POINTER_NULL      					31019   // ��������ָ��Ϊ��        
#define ERR_MS_AUTHENTICATE_PACKET      				31020   // ��Ч����֤���ݰ�        
#define ERR_MS_GET_CFG_VIEWER_POINTER_FAILED            31021   // ��ȡviewer����ʧ��      
#define ERR_MS_NULL_AUTHENTICATE_USER_NAME   			31022   // �û���Ϊ��              
#define ERR_MS_INVALID_COMMAND_ID             			31023   // ��Ч������              
#define ERR_MS_INVALID_GROUP_CODE	           			31024   // ��Ч��GROUP CODE        
#define ERR_MS_MALLOC_COMMAND_SESSION_FAILED   			31025   // ����Ự����ʧ��        
#define ERR_MS_REGISTER_SYCN_QUERY_POINTER_FAILED		31026   // �첽��ѯ����ʧ��        
#define ERR_MS_MALLOC_MEDIA_SESSION_FAILED   			31027   // ý��Ự����ʧ��        
#define ERR_MS_REGISTER_NULL_MEDIA_POINTER   			31028   // ע��ý�巵��Ϊ��        
#define ERR_MS_INVALID_PACKET_FORMATE		   			31029   // ��Ч�����ݰ���ʽ        
#define ERR_MS_INVALID_USR_LEVEL  						31030   // ��Ч���û��ȼ�          
#define ERR_MS_NO_MOTION_PRI      						31031   // û�йۿ���̬����Ȩ��    
#define ERR_MS_NO_QUICK_DWON_PRI   						31032   // û�п�������Ȩ��        
#define ERR_MS_NO_QUICK_DWON_AND_MOTION_PRI   			31033   // û�п������ض�̬����Ȩ��
#define ERR_MS_COMMON_FAILED                 			31034   // һ�����                
#define ERR_MS_DOWNLOAD_REDOWNLOAD						31035   // ����ʧ��                
#define ERR_MS_SEEK_FAILED								31036   // �طŶ�λʧ��            
#define ERR_MS_GET_RECORD_QUALITY_FAILED				31037   // ¼��Ʒ�ʻ�ȡʧ��        
#define ERR_MS_INVALID_SEEK_TYPE						31038   // ¼��λ������Ч        
#define ERR_MS_INVALID_SEEK_TIME						31039   // ¼��λʱ����Ч        
#define ERR_MS_INVALID_SET_REPORTLOG_PARAM              31040   // �����ϱ��Ĳ�����Ч      
#define ERR_MS_INVALID_SERIALNO                         31041   // ��Ч�����к�            
#define ERR_MS_SET_RECTYPE_FAILED            			31042   // ¼����������ʧ��        
#define ERR_MS_INVALID_RECTYPE      					31043   // ��Ч��¼������          
#define ERR_MS_SAME_RECTYPE								31044   // ��������ͬ��¼������    
#define ERR_MS_NOT_OWNER_ACCOUNT						31045   // û�������˻�            
#define ERR_MS_INVALID_FTP_ACCOUNT						31046   // ��Ч��ftp�˻�           
#define ERR_MS_CON_FTPSVR_FAILED                 	    31047   // ftp����������ʧ��       
#define ERR_MS_GET_CAMEXTENDINFO                        31048   // �������չ��Ϣ��ȡʧ��  
#define ERR_MS_INVALID_IMAGE_PATH						31049   // ��Ч��image·��         
#define ERR_MS_OPEN_IMAGE_FILE_FAILED					31050   // ��������ļ�ʧ��      
#define ERR_MS_OTHER_UPDATE								31051   // ��������ʧ��            
#define ERR_MS_SEEK										31052   // ¼��λʧ��            
#define ERR_MS_UPDATE_IMAGE_FAILED                      31053   // ����imageʧ��           
#define ERR_MS_SET_VIGOLOGEVENT_FAILED                  31054   // ������־�¼�ʧ��        

#define ERR_MS_MAX_COMMAND_SESSION_REACH				31224   // ����Ự�ﵽ����          
#define ERR_MS_MAX_MEDIA_SESSION_REACH					31225   // ý��Ự�ﵽ����          
#define ERR_MS_MAX_MEDIA_SESSION_CON_REACH				31226   // ý��Ự���Ӵﵽ����      
#define ERR_MS_GET_GROUPS_FAILED						31227   // �����б��ȡʧ��          
#define ERR_MS_INVALID_CAMERA_NUM						31228   // ��Ч�����������          
#define ERR_MS_NOT_AUTHEN       						31229   // ��û��֤                  
#define ERR_MS_SET_BUFF_FAILED    						31230   // ���û���ʧ��              
#define ERR_MS_GET_CAMERA_FAILED						31231   // ������б��ȡʧ��        
#define ERR_MS_CAMERAS_FOR_USER_TOO_LARGE				31232   // �û������������̫����    
#define ERR_MS_PLAY_SPEED     		     				31233   // �����ٶ�����ʧ��          
#define ERR_MS_PLAY_TYPE     		     				31234   // ������������ʧ��          
#define ERR_MS_GROUPS_FOR_USER_TOO_LARGE    			31235   // �û����������������̫����
#define ERR_MS_CREATE_GROUP_INFO_POINTER    			31236   // ������Ϣ��������          
#define ERR_MS_CREATE_CAMERA_INFO_POINTER   			31237   // �������Ϣ��������        
#define ERR_MS_CREATE_CAMERA_COUNT_POINTER   			31238   // �����������������        
#define ERR_MS_CREATE_EX_BUFFER_POINTER      			31239   // ��չ���崴������          
                                                                                         
#define ERR_MS_GET_CFGIO_POINTER_FAILED     			31112   // ��ȡIO����ʧ��            
#define ERR_MS_UPDATE_LOCALE_CFG_FAILED     			31113   // ���±�������ʧ��          
#define ERR_MS_GET_NVR_BUFFER_FAILED     				31114   // ��ȡNVR����ʧ��           
#define ERR_MS_GET_DISK_INFO_FAILED     		    	31115   // ��ȡ������Ϣʧ��          
#define ERR_MS_MALLOC_BUFFER_SIZE_FAILED     	   		31116   // �ڴ�����ʧ��              
#define ERR_MS_INVALID_DRIVE_TAG                	   	31117   // ��Ч���������            

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
#define ERR_DP_INVALID_DSERVERIP		                40001	// ��Ч��ע���������ַ
#define ERR_DP_INVALID_USERNAME			                40002	// ��Ч���û���
#define ERR_DP_INVALID_PASSWORD			                40003	// ��Ч������
#define ERR_DP_USER_LOGIN_NOTYET		                40004	// �û���δ��¼
#define ERR_DP_REG_NETCON_FAILED		                40005	// ע��NetConģ��ʧ��
#define ERR_DP_REG_DSERVER_FAILED		                40006	// ע��DServerģ��ʧ��
#define ERR_DP_REG_MEDIASERVER_FAILED	                40007	// ע��MeidaServerģ��ʧ��
#define ERR_DP_INVALID_NVR				                40008	// ��Ч��NVR
#define ERR_DP_INVALID_VIEW				                40009	// ��Ч��View
#define ERR_DP_NEW_FAILED				                40010	// ��̬�ڴ����ʧ��
#define ERR_DP_GETINFO_TIMEOUT			                40011	// DServer������Ϣ��ȡ��ʱ
#define ERR_DP_INVALID_SN				                40012	// ��Ч�����к�

// 50001~60000 for 7620 Control
// δ֪����
#define ERR_7620_UNKNOWN                                30001
// �Ͽ�����
#define ERR_7620_DISCONNECT                             30002
// ��ȡSSID�б�ʧ��
#define ERR_7620_GETSSIDLIST_FAILED                     30003
// ����SSIDʧ��
#define ERR_7620_CONNECTSSID_FAILED                     30004
// ����SSIDʧ��
#define ERR_7620_SETSSID_FAILED                         30005
//////////////////////////////////////////////////////////////////////////

#endif // _MOBCLIENT_SDK_ERROR_H_
