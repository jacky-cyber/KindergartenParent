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
#define kBtnW 106.66666

@interface ZJHomeViewController ()
{
    UIImageView *_headerImageView;//顶部图片
    
    UIView *_cateBgView;//分类背景按钮
    
    NSArray *_cateTitleArr;//标题
}

@end

@implementation ZJHomeViewController



- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"家长中心";
    
    // Do any additional setup after loading the view from its nib.
    //分类标题
    _cateTitleArr = @[@"通知公告",@"每周食谱",@"签到/签退",@"每日一报",@"没周一报",@"课程安排",@"家长月评",@"服药提醒",@"园宝记录"];
    //设置头部图片
    [self setParentView];
    
    //设置分类图片
    [self setupButton];
    
    
}

-(void)setParentView
{
    //设置头像
    if (!_headerImageView) {
        _headerImageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, W(self.view), H(self.view)*0.25)];
        _headerImageView.image = [UIImage imageNamed:@"beijing"];
        [self.view addSubview:_headerImageView];
    }
    //设置分类背景
    if (!_cateBgView) {
        
        CGFloat height = H(self.view)-H(_headerImageView)-kNavH-20;
        if (ISIOS7) {
            height -=20;
        }
        MyLog(@"%f",height);
        _cateBgView = [[UIView alloc] initWithFrame:CGRectMake(0, YH(_headerImageView)+20, W(self.view), height-2)];
        _cateBgView.backgroundColor = [UIColor redColor];
        [self.view addSubview:_cateBgView];

    }
}

-(CGFloat)returnBtnH{
    CGFloat height = H(self.view)-H(_headerImageView)-kNavH-20;
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
    UIButton *item = [[UIButton alloc] init];
    NSString *imageName = [NSString stringWithFormat:@"home_type_%d",tag];
    [item setImage:[UIImage imageNamed:imageName] forState:UIControlStateNormal];
    item.tag = tag;
    [item addTarget:self action:@selector(cateAction:) forControlEvents:UIControlEventTouchUpInside];
    item.backgroundColor = [UIColor brownColor];
    item.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
    item.tag = tag;
    [item setImageEdgeInsets:UIEdgeInsetsMake(-40, 30, 0.0, 0.0)];
    [item setTitleEdgeInsets:UIEdgeInsetsMake(40, -30, 0, 0)];
    item.frame = CGRectMake(x, y,kBtnW-1,[self returnBtnH]-1);
    MyLog(@"%@",_cateTitleArr[tag-1]);
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
//        [self pushController:[ZJCookBooksViewController class] withInfo:nil withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 3){
        [self pushController:[ZJSystemMsgViewController class] withInfo:nil withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 4){
        [self pushController:[ZJDayAndWeekReportListViewController class] withInfo:@"day" withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 5){
        [self pushController:[ZJDayAndWeekReportListViewController class] withInfo:@"week" withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 6){
        [self pushController:[ZJSystemMsgViewController class] withInfo:nil withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 7){//月评
        [self pushController:[ZJMonthCommentViewController class] withInfo:nil withTitle:_cateTitleArr[tag-1]];
    }else if(tag == 8){//医务通知
        [self pushController:[ZJNotificationListViewController class] withInfo:@"6" withTitle:_cateTitleArr[tag-1]];
    }else if (tag == 9) {
        [self pushController:[ZJSystemMsgViewController class] withInfo:nil withTitle:_cateTitleArr[tag-1]];
    }
    
    MyLog(@"%d",sender.tag);
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
