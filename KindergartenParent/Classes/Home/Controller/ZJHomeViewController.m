//
//  ZJHomeViewController.m
//  KindergartenParent
//
//  Created by define on 14-7-25.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJHomeViewController.h"
#import "ZJSystemMsgViewController.h"
#import "ZJCookBooksViewController.h"
#import "ZJMonthCommentViewController.h"
#import "ZJDayAndWeekReportListViewController.h"
#import "ZJNotificationListViewController.h"
#import "ZJUserInfoViewController.h"
#import "ZJHonorViewController.h"
#import "DockItem.h"
#import "InsetsLabel.h"
#import "ZJCourseViewController.h"
#import "ZJLoginViewController.h"
#import "APService.h"
#define kBtnW 106.66666

@interface ZJHomeViewController ()<IChatManagerDelegate>
{
    UIImageView *_headerImageView;//顶部图片
    
    UIView *_cateBgView;//分类背景按钮
    
    NSArray *_cateTitleArr;//标题
    
    UIImageView *_profileImageView;//头像
    
    UILabel *_nickName;//昵称
    
    InsetsLabel *_parentName;//
    
    
    UILabel *_noReadMsgLabel;//未读消息数目
}

@end

@implementation ZJHomeViewController


-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    if (_parentName !=nil) {
        [_parentName setAttributedText:[self setParentNameAttrText]];
    }
    
    
    
    //获取未读消息数，此时并没有把self注册为SDK的delegate，读取出的未读数是上次退出程序时的
    [self didUnreadMessagesCountChanged];
    _noReadMsgLabel.hidden = NO;
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"家长中心";
    
    // Do any additional setup after loading the view from its nib.
    //分类标题
    _cateTitleArr = @[@"通知公告",@"每周食谱",@"签到/签退",@"每日一报",@"每周一报",@"课程安排",@"家长月评",@"服药提醒",@"园宝记录"];
    //设置头部图片
    [self setParentView];
    
    //设置分类图片
    [self setupButton];
    
    //设置头像
    
    [self setProfileImg];
    
    
//#warning 把self注册为SDK的delegate
    [self registerNotifications];
    
//    //进来重新登录一遍
//    [self loginAction];
}

-(void)setParentView
{
    //设置头部
    if (!_headerImageView) {
        //ihone4 和5区别
        CGFloat height = 130;
        if (iPhone5) {
            height = 160;
        }
        _headerImageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, W(self.view),height)];
        _headerImageView.image = [UIImage imageNamed:@"beijing"];
        [self.view addSubview:_headerImageView];
    }
    
    //设置头像
    
    
    //设置分类背景
    if (!_cateBgView) {
        CGFloat height = H(self.view)-H(_headerImageView)-kNavH-20;
        if (ISIOS7) {
            height -=20;
        }
       // MyLog(@"%f",height);
        _cateBgView = [[UIView alloc] initWithFrame:CGRectMake(0, YH(_headerImageView)+30, W(self.view), height-2)];
        [self.view addSubview:_cateBgView];

    }
}

-(NSMutableAttributedString*)setParentNameAttrText
{
   // MyLog(@"%@----%@",[LoginUser sharedLoginUser].nickname,[LoginUser sharedLoginUser].name);
    NSString *name = ![[LoginUser sharedLoginUser].nickname isEqualToString:@""]?[LoginUser sharedLoginUser].nickname:[LoginUser sharedLoginUser].name;
    
    NSString *str  = [NSString stringWithFormat:@"%@  %@",name,[LoginUser sharedLoginUser].classes];
    CGSize size = [str sizeWithFont:kFont(16)];
    CGRect frame = _parentName.frame;
    frame.size.width = size.width+10;
    _parentName.frame = frame;
    
    //班级的长度
    int classesLeng  = [LoginUser sharedLoginUser].classes.length;
    
    // 如果想要改变部份文本内容的风格，我们就需要用到NSAttributedString NSMutableAttributedString
    NSMutableAttributedString *attributeString = [[NSMutableAttributedString alloc] initWithString:str];
    //设置属性
    [attributeString addAttribute:NSFontAttributeName value:kFont(12) range:NSMakeRange(str.length-classesLeng, classesLeng)];
    
    return attributeString;
}

-(void)setProfileImg
{
    //单击昵称
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self action:@selector(updateProfileAction)];
    
     NSString *name = ![[LoginUser sharedLoginUser].nickname isEqualToString:@""]?[LoginUser sharedLoginUser].nickname:[LoginUser sharedLoginUser].name;
    NSString *str  = [NSString stringWithFormat:@"%@  %@",name,[LoginUser sharedLoginUser].classes];
    CGSize size = [str sizeWithFont:kFont(16)];
    _parentName=[[InsetsLabel alloc] initWithFrame:CGRectMake(60,H(_headerImageView)-25,size.width+10,25)];
    _parentName.userInteractionEnabled = YES;
    _parentName.insets = UIEdgeInsetsMake(0, 15, 0, 0);
    _parentName.font = kFont(16);
    _parentName.backgroundColor =  [UIColor colorWithWhite:0.000 alpha:0.200];
    _parentName.textColor = [UIColor colorWithWhite:0.996 alpha:1.000];
    _parentName.layer.cornerRadius = 5;
    _parentName.layer.masksToBounds = YES;

    [_parentName setAttributedText:[self setParentNameAttrText]];
    
    //parentName.text = str;
    //单击头像
    UITapGestureRecognizer *tap1 = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self action:@selector(updateProfileAction)];
    [_parentName  addGestureRecognizer:tap1];
    [self.view addSubview:_parentName];
    
    
    
    
    _profileImageView = [[UIImageView alloc] initWithFrame:CGRectMake(10, H(_headerImageView)-40, 60, 60)];
    _profileImageView.layer.borderColor = [UIColor whiteColor].CGColor;
    _profileImageView.layer.borderWidth = 2.0;
    _profileImageView.tag = 1;

    //设置头像
    [_profileImageView setImageWithURL:[NSURL URLWithString:[LoginUser sharedLoginUser].profilImg] placeholderImage:[UIImage imageNamed:@"profile"]];
    _profileImageView.layer.cornerRadius = 30;
    _profileImageView.layer.masksToBounds = YES;
    _profileImageView.userInteractionEnabled = YES;
    //单击头像
    [_profileImageView addGestureRecognizer:tap];
    [self.view addSubview:_profileImageView];
    
    
    //荣誉榜
    UIButton *btn = [[UIButton alloc] initWithFrame:CGRectMake(290,H(_headerImageView)-25, 25, 25)];
    [btn setImage:[UIImage imageNamed:@"honor"] forState:UIControlStateNormal];
    // btn.backgroundColor = [UIColor colorWithRed:0.400 green:1.000 blue:1.000 alpha:1.000];
    [btn addTarget:self action:@selector(honorAction) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:btn];

    //    bringSubviewToFront
   // [self.view bringSubviewToFront:prifileImg];
    
}

-(CGFloat)returnBtnH{
    CGFloat height = H(self.view)-H(_headerImageView)-kNavH-30;
    if (ISIOS7) {
        height -=20;
    }
    return height/3;
}

#pragma mark 设置分类按钮
-(void)setupButton
{
    int columns = 3;
    // 每个表情之间的间距 = (控制器view的宽度 - 列数 * 表情的宽度) / (列数 + 1)
    CGFloat margin = (W(self.view) - columns * kBtnW) / (columns + 1);
    
    // 2.定义第一个表情的位置
    // 第一个表情的Y值
    CGFloat oneY = 0;
    // 第一个表情的x值
    CGFloat oneX = 0;
    
    // 3.创建所有的表情
    for (int i = 0; i<9; i++) {
        
        // i这个位置对应的列数
        int col = i % columns;
        // i这个位置对应的行数
        int row = i / columns;
        
        // 列数（col）决定了x
        CGFloat x = oneX + col * (kBtnW + margin);
        // 行数（row）决定了y
        CGFloat y = oneY + row * ([self returnBtnH] + margin);
        [self addImg:nil x:x y:y tag:i+1];
    }
    
}

#pragma mark 添加表情 icon:表情图片名
- (void)addImg:(NSString *)icon x:(CGFloat)x y:(CGFloat)y tag:(int)tag;
{
    DockItem *item = [[DockItem alloc] init];
    NSString *imageName = [NSString stringWithFormat:@"home_type_%d",tag];
    [item setImage:[UIImage imageNamed:imageName] forState:UIControlStateNormal];
    item.tag = tag;
    [item addTarget:self action:@selector(cateAction:) forControlEvents:UIControlEventTouchUpInside];
    item.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
    item.tag = tag;
    item.frame = CGRectMake(x, y,kBtnW-1,[self returnBtnH]-1);
    [item setTitle:_cateTitleArr[tag-1] forState:UIControlStateNormal];
    [_cateBgView addSubview:item];
}

-(void)cateAction:(UIButton*)sender
{
    
     //_cateTitleArr = @[@"通知公告",@"每周食谱",@"签到/签退",@"每日一报",@"没周一报",@"课程安排",@"家长月评",@"服药提醒",@"园宝记录"];
    int tag  = sender.tag;
    
    if (tag == 1) {
         [self pushController:[ZJNotificationListViewController class] withInfo:@"2,8,9,10" withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 2){//每周食谱
        [self pushController:[ZJCookBooksViewController class] withInfo:nil withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 3){//签到
        [self pushController:[ZJNotificationListViewController class] withInfo:@"11" withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 4){//月报
        [self pushController:[ZJDayAndWeekReportListViewController class] withInfo:@"day" withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 5){//周报
        [self pushController:[ZJDayAndWeekReportListViewController class] withInfo:@"week" withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 6){//课程
        [self pushController:[ZJCourseViewController class] withInfo:nil withTitle:@"课程表"];
    }else if(tag == 7){//月评
        //[self pushController:[ZJMonthCommentViewController class] withInfo:nil withTitle:_cateTitleArr[tag-1]];
        [self monthAction];
    }else if(tag == 8){//医务通知
        [self pushController:[ZJNotificationListViewController class] withInfo:@"6" withTitle:_cateTitleArr[tag-1]];
    }else if (tag == 9) {
        [self pushController:[ZJSystemMsgViewController class] withInfo:nil withTitle:_cateTitleArr[tag-1]];
    }
    
    MyLog(@"%d",sender.tag);
}

#pragma makr 判断是否月评
-(void)monthAction{
    
    [HttpTool getWithPath:@"month_cmt_status" params:@{@"username":[LoginUser sharedLoginUser].userName} success:^(id JSON) {
        MyLog(@"%@",JSON[@"data"]);
        if ([JSON[@"data"][@"status"] intValue] == 0) {
            [self pushController:[ZJMonthCommentViewController class] withInfo:nil withTitle:@"家长月评"];
        }else{
            kPE(@"您本月已经提交月评，请下月再评", 1.5);
        }
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
    }];
    
}

//// 实现接收消息的委托
// int i = 0;
//#pragma mark - IChatManagerDelegate
//-(void)didReceiveMessage:(EMMessage *)message{
//    MyLog(@"-----%@",message.description);
//    
//    UILabel *label = [[UILabel alloc] init];
//    i++;
//    label.text = [NSString stringWithFormat:@"%d",i];
//    label.frame = CGRectMake(10, 0, 15, 15);
//    
//    [self.navigationController.navigationBar addSubview:label];
//    
//    
//}
#pragma mark 查看个人信息
-(void)updateProfileAction
{
   // NSLog(@"%@",_uinfo);
    [self pushController:[ZJUserInfoViewController class] withInfo:nil withTitle:@"详情"];
    
}



//#pragma mark 荣誉版
-(void)honorAction
{
    [self pushController:[ZJHonorViewController class] withInfo:nil withTitle:@"荣誉榜"];
}






// 统计未读消息数
-(void)setupUnreadMessageCount
{
    NSArray *conversations = [[[EaseMob sharedInstance] chatManager] conversations];
    NSInteger unreadCount = 0;
    for (EMConversation *conversation in conversations) {
        unreadCount += conversation.unreadMessagesCount;
    }
    //    UIViewController *vc = [self.viewControllers objectAtIndex:0];
    //    if (unreadCount > 0) {
    //        vc.tabBarItem.badgeValue = [NSString stringWithFormat:@"%d",unreadCount];
    //    }else{
    //        vc.tabBarItem.badgeValue = nil;
    //    }
    
    MyLog(@"unreadCount:%d",unreadCount);
    //设置头像
    [_profileImageView setImageWithURL:[NSURL URLWithString:[LoginUser sharedLoginUser].profilImg] placeholderImage:[UIImage imageNamed:@"profile"]];
    if (!_noReadMsgLabel) {
        _noReadMsgLabel = [[UILabel alloc] initWithFrame:CGRectMake(300, 5, 15, 15)];
        _noReadMsgLabel.backgroundColor = [UIColor redColor];
        _noReadMsgLabel.layer.cornerRadius = 7;
        _noReadMsgLabel.font = kFont(11);
        _noReadMsgLabel.textAlignment = NSTextAlignmentCenter;
        _noReadMsgLabel.layer.masksToBounds = YES;
        _noReadMsgLabel.textColor = [UIColor whiteColor];
        [self.navigationController.navigationBar addSubview:_noReadMsgLabel];
       // MyLog(@"_noReadMsgLabel is nil");
    }
    if (unreadCount==0) {
        _noReadMsgLabel.alpha = 0;
    }else{
        _noReadMsgLabel.alpha = 1;
    }

    _noReadMsgLabel.text = [NSString stringWithFormat:@"%d",unreadCount];
}
-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    _noReadMsgLabel.hidden = YES;
}
#pragma mark - IChatManagerDelegate 消息变化

// 未读消息数量变化回调
-(void)didUnreadMessagesCountChanged
{
    [self setupUnreadMessageCount];
}



-(void)registerNotifications
{
    [self unregisterNotifications];
    [[EaseMob sharedInstance].chatManager addDelegate:self delegateQueue:nil];
}


-(void)unregisterNotifications
{
    [[EaseMob sharedInstance].chatManager removeDelegate:self];
}


- (void)dealloc
{
    [self unregisterNotifications];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark login
- (void)loginAction {
    
    NSString *username = [LoginUser sharedLoginUser].userName;
    NSString *pwd = [LoginUser sharedLoginUser].password;
    
    
    [HttpTool getWithPath:@"login" params:@{@"username":username,@"pwd":pwd} success:^(id JSON) {
        if (JSON[@"data"]) {
            
            // MyLog(@"%@",JSON);
            
            [SVProgressHUD showSuccessWithStatus:@"登录成功" duration:0.5];
            
            //[LoginUser sharedLoginUser].userName = username;
            
            ZJUserInfoModel *user = [[ZJUserInfoModel alloc] init];
            
            [user setKeyValues:JSON[@"data"]];
            [LoginUser sharedLoginUser].password = pwd;
            [[LoginUser sharedLoginUser] saveInfo:user];
            
            // MyLog(@"%@",[LoginUser sharedLoginUser].description);
            
            [APService setAlias:user.username callbackSelector:nil object:nil];
                       
        }else{
            [SVProgressHUD showErrorWithStatus:@"用户名/密码错误" duration:1];
            [self gologinControll];
        }
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
        MyLog(@"%@",error);
    }];
    
    
}
-(void)gologinControll
{
    ZJLoginViewController *login =  [[ZJLoginViewController alloc] init];
    self.view.window.rootViewController =login ;
}

@end
