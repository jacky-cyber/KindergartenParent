
// 1.判断是否为iPhone5的宏
#define iPhone5 ([UIScreen mainScreen].bounds.size.height == 568)

// 2.日志输出宏定义
#ifdef DEBUG
// 调试状态
#define MyLog(...) NSLog(__VA_ARGS__)
#else
// 发布状态
#define MyLog(...)
#endif
//判断是否为ios7
#define ISIOS7 ([[UIDevice currentDevice].systemVersion floatValue] >= 7.0f)
#define kFont11 [UIFont systemFontOfSize:11]//11字体大小
#define kFont12 [UIFont systemFontOfSize:12]//12字体大小
#define kFont13 [UIFont systemFontOfSize:13]//13字体大小
#define kFont14 [UIFont systemFontOfSize:14]//14字体大小
#define kFont15 [UIFont systemFontOfSize:15]//15字体大小
#define kFont16 [UIFont systemFontOfSize:16]//16字体大小
#define kFont17 [UIFont systemFontOfSize:17]//17字体大小

#define kFont(x) [UIFont systemFontOfSize:x]//自定义font大小
#define W(obj)   (!obj?0:(obj).frame.size.width)
#define H(obj)   (!obj?0:(obj).frame.size.height)
#define X(obj)   (!obj?0:(obj).frame.origin.x)
#define Y(obj)   (!obj?0:(obj).frame.origin.y)
#define XW(obj) (X(obj)+W(obj))
#define YH(obj) (Y(obj)+H(obj))
#define BoldFont(x) [UIFont boldSystemFontOfSize:x]

#define kNavH 44

#define kPBlack(obj)  [SVProgressHUD showWithStatus:obj maskType:SVProgressHUDMaskTypeBlack];

#define kPS(obj,time) [SVProgressHUD showSuccessWithStatus:obj duration:time?time:0.5];

#define kPE(obj,time) [SVProgressHUD showErrorWithStatus:obj duration:time?time:0.5];

#define kPdismiss [SVProgressHUD dismiss];

#define kHttpErrorMsg @"网络连接错误"

#define kScreenHeight [[UIScreen mainScreen] bounds].size.height
#define kScreenWidth [[UIScreen mainScreen] bounds].size.width

#define kBaseUrl @"http://182.18.23.244:8090/"


#define kHostName @"182.18.23.244"

#define kFileServerURL @"http://182.18.23.244" //文件服务器路径