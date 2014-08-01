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
#define kBtnW 106.66666

@interface ZJHomeViewController ()
{
    UIImageView *_headerImageView;//顶部图片
    
    UIView *_cateBgView;//分类背景按钮
    
    NSArray *_cateTitleArr;//标题
    
    UIImageView *_profileImageView;//头像
    
    UILabel *_nickName;//昵称
}

@end

@implementation ZJHomeViewController



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

-(void)setProfileImg
{
    

    InsetsLabel * parentName=[[InsetsLabel alloc] initWithFrame:CGRectMake(60,H(_headerImageView)-25,180,25)];
    parentName.insets = UIEdgeInsetsMake(0, 10, 0, 0);
    
    parentName.backgroundColor =  [UIColor colorWithWhite:0.000 alpha:0.200];
    parentName.textColor = [UIColor colorWithWhite:0.996 alpha:1.000];
    parentName.layer.cornerRadius = 5;
    parentName.layer.masksToBounds = YES;
    
   NSString *str  = [NSString stringWithFormat:@"%@    %@",[LoginUser sharedLoginUser].nickname,[LoginUser sharedLoginUser].classes];
    //班级的长度
    int classesLeng  = [LoginUser sharedLoginUser].classes.length;
    
    // 如果想要改变部份文本内容的风格，我们就需要用到NSAttributedString NSMutableAttributedString
    NSMutableAttributedString *attributeString = [[NSMutableAttributedString alloc] initWithString:str];
    //设置属性
    [attributeString addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:12.0f] range:NSMakeRange(str.length-classesLeng, classesLeng)];
    parentName.font = kFont(16);
    [parentName setAttributedText:attributeString];
    
    //parentName.text = str;
    [self.view addSubview:parentName];

    
    _profileImageView = [[UIImageView alloc] initWithFrame:CGRectMake(10, H(_headerImageView)-40, 60, 60)];
    _profileImageView.layer.borderColor = [UIColor whiteColor].CGColor;
    _profileImageView.layer.borderWidth = 2.0;
    _profileImageView.tag = 1;

    //设置头像
    [_profileImageView setImageWithURL:[NSURL URLWithString:[LoginUser sharedLoginUser].profilImg] placeholderImage:[UIImage imageNamed:@"profile"]];
    _profileImageView.layer.cornerRadius = 10;
    _profileImageView.layer.masksToBounds = YES;
    _profileImageView.userInteractionEnabled = YES;
    //单击头像
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self action:@selector(updateProfileAction)];
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

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
