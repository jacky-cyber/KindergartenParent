//
//  ZJPhotWallViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJPhotWallViewController.h"
#import "ZJCommentListViewController.h"
@interface ZJPhotWallViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
}
@end

@implementation ZJPhotWallViewController


- (void)viewDidLoad
{
    [super viewDidLoad];

    self.title = @"成长记录";
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0,0,self.view.frame.size.width,height) style:UITableViewStyleGrouped];
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.sectionFooterHeight = 0;
    //设置背景，ios6里面不然就很乱
    UIView *backView = [[UIView alloc] init];
    backView.backgroundColor = [UIColor whiteColor];
    [_tableView setBackgroundView:backView];
    [self.view addSubview:_tableView];
    
    
    self.navigationController.navigationItem.rightBarButtonItem = nil;
    
    
    
    //看自己
    
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 4, 65, 25);
    [btnR addTarget:self action:@selector(initData) forControlEvents:UIControlEventTouchUpInside];
    
    UIImage *backgroundImg= [UIImage resizedImage:@"nav_rightbackbround_image"];
    
    [btnR setBackgroundImage:backgroundImg forState:UIControlStateNormal];
    [btnR setTitle:@"看自己" forState:UIControlStateNormal];
    [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;

    
    
    //加载数据
    [self initData];
}

-(void)initData{
    
//    classesid	int	班级id
//    catid	分类	分类id
//    username	String	看自己
//    page	int	页数
    
    NSDictionary *params = @{@"classesid":@"LS13436871757",@"catid":@"1",@"page":@"1"};
    
    [HttpTool getWithPath:@"potowall" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
    } failure:^(NSError *error) {
        MyLog(@"%@",error.debugDescription);
    }];
}


#define kMargin 10
-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    UIView *view = [[UIView alloc] init];
    view.backgroundColor  = [UIColor whiteColor];
    
    //父视图
    UIView *superView = [[UIView alloc] init];
    superView.frame = CGRectMake(kMargin, 20, 300,210);
    superView.layer.borderColor = [UIColor colorWithRed:0.820 green:0.867 blue:0.773 alpha:1.000].CGColor;
    superView.layer.borderWidth = 1;
    [view addSubview:superView];
    //标题
    UILabel *title = [[UILabel alloc] initWithFrame:CGRectMake(kMargin, kMargin, 280, 21)];
    title.text = @"这只猫很萌，有木有，有木有?";
    [superView addSubview:title];
    
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(kMargin, kMargin+YH(title), 280, 110)];
    
    NSString *string = @"http://img.icoin.cn/_files/201405/13/fd25c3295ddc4873a81567e1120fb91b.jpg";
    NSURL *url = [NSURL URLWithString:string];
    
    [imageView setImageWithURL:url placeholderImage:nil];
    [superView addSubview:imageView];
    
    //工具条view
    UIView *toolView = [[UIView alloc] init];
    toolView.frame = CGRectMake(kMargin,  kMargin+YH(imageView), 280, 16);
    toolView.backgroundColor = [UIColor whiteColor];
    [superView addSubview:toolView];
    
    //时间图标
    UIImageView *timeImg  = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"p_time"]];
    timeImg.frame = CGRectMake(0, 0, 16, 16);
    [toolView addSubview:timeImg];
    //时间
    UILabel *timeLabel = [[UILabel alloc] initWithFrame:CGRectMake(XW(timeImg)+5, 0, 80, 16)];
    timeLabel.text = @"2014-05-15";
    timeLabel.textColor = [UIColor colorWithWhite:0.725 alpha:1.000];
    timeLabel.font = kFont(12);
    [toolView addSubview:timeLabel];
    
    UIButton *praiseBtn = [ZJUIMethods creatButton:nil frame:CGRectMake(150, 0, 16, 16) delegate:self selector:@selector(praiseAction) tag:0];
    [praiseBtn setImage:[UIImage imageNamed:@"p_parise"] forState:UIControlStateNormal];
    [toolView addSubview:praiseBtn];
    
    //赞
    UILabel *praiseLable = [ZJUIMethods creatLabel:@"1234" frame:CGRectMake(XW(praiseBtn)+5, 0, 30, 16) font:kFont(12) textColor:nil];
    praiseLable.textColor = [UIColor colorWithWhite:0.725 alpha:1.000];
    //praiseLable.backgroundColor = [UIColor redColor];
    [toolView addSubview:praiseLable];
    
    //评论按钮
    UIButton *commentBtn = [ZJUIMethods creatButton:nil frame:CGRectMake(XW(praiseLable)+2*kMargin, 0, 16, 16) delegate:self selector:@selector(commentAction) tag:0];
    [commentBtn setImage:[UIImage imageNamed:@"p_comment"] forState:UIControlStateNormal];
    [toolView addSubview:commentBtn];
    
    //赞
    UILabel *commentLb = [ZJUIMethods creatLabel:@"1234" frame:CGRectMake(XW(commentBtn)+5, 0, 30, 16) font:kFont(12) textColor:nil];
    commentLb.textColor = [UIColor colorWithWhite:0.725 alpha:1.000];
    //commentLb.backgroundColor = [UIColor redColor];
    [toolView addSubview:commentLb];
    
    //标签按钮
    UIView *cateView = [ZJUIMethods creatView:CGRectMake(kMargin, YH(toolView)+kMargin, 280, 20) bgColor:[UIColor whiteColor]];
    
    //按钮
    NSArray *cateArr = @[@"健康",@"语言",@"科学",@"艺术",@"社会"];
    for (int i = 0; i<5; i++) {
        UIButton *button = [ZJUIMethods creatButton:cateArr[i] frame:CGRectMake(i*58, 0, 48, 20) delegate:self selector:@selector(cateAction) tag:i+1];
        button.titleLabel.font = kFont(14);
        [button setBackgroundImage:[UIImage imageNamed:@"categor_bg"] forState:UIControlStateNormal];
        [cateView addSubview:button];
    }
    [superView addSubview:cateView];
    
    //重新调整superView的高度
    CGRect frame = superView.frame;
    frame.size.height = YH(cateView) +kMargin;
    superView.frame = frame;
    
    
    
    
    return view;
}

-(void)commentAction{
    [self pushController:[ZJCommentListViewController class] withInfo:@"1" withTitle:@"评论"];
}

-(void)praiseAction
{
    
    NSDictionary *params = @{@"pwallid":@"1",
                             @"userName":[LoginUser sharedLoginUser].userName,
                             @"content":@""
                             };
    
    [HttpTool getWithPath:@"praise" params:params success:^(id JSON) {
        NSLog(@"%@",JSON);
    } failure:^(NSError *error) {
        
    }];

}
-(void)cateAction
{
    
}
-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 245;
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1;
}
-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    //设置背景，ios6里面不然就很乱
    UIView *backView = [[UIView alloc] init];
    [cell setBackgroundView:backView];
    
    
    //自定义View
    
    NSLog(@"%@",NSStringFromCGRect(cell.frame));
    if (ISIOS7) {
        
    }
    UIView *view = [ZJUIMethods creatView:CGRectMake(kMargin, 10, 300, 100) bgColor:[UIColor colorWithWhite:0.969 alpha:1.000]];
    [cell addSubview:view];
    CGRect viewFrame = view.frame;
    
    NSString *name = @"<a href='a' size>果果果果</a>:<font color='gray'>的家长的家长的家长的家长的家长的家长的家长的家长的家长的家长的家长的家长</font>";
    
    RTLabel *coment = [[RTLabel alloc] initWithFrame:CGRectMake(10, 10, 280, 21)];
    coment.text = name;
    coment.font = kFont(13);
    [view addSubview:coment];
    //coment.backgroundColor = [UIColor colorWithRed:0.758 green:0.967 blue:1.000 alpha:1.000];
    coment.linkAttributes = @{@"color":@"#78a40e"};
    CGSize optimumSize = [coment optimumSize];
	CGRect frame = [coment frame];
	frame.size.height = (int)optimumSize.height+5; // +5 to fix height issue, this should be automatically fixed in iOS5
	[coment setFrame:frame];
    //重新设置view frame
    viewFrame.size.height = YH(coment)+10;
    view.frame = viewFrame;
    
    
    
    
    //添加分割线
    UIView *line = [[UIView alloc] initWithFrame:CGRectMake(X(coment), YH(coment)+5, 280, 1)];
    line.backgroundColor = [UIColor colorWithWhite:0.941 alpha:1.000];
    [view addSubview:line];
    
    
    NSString *name1 = @"<a href='a'>果果果果</a> <font color='gray'>的家长的家长的家长长的家长的家长的家长的家长</font>";
    MyLog(@"----%@----%f",NSStringFromCGRect(coment.frame),YH(coment));
    RTLabel *coment1 = [[RTLabel alloc] initWithFrame:CGRectMake(10, YH(line)+5, 280, 21)];
    coment1.text = name1;
    coment1.font = kFont(13);
    [view addSubview:coment1];
    //coment1.backgroundColor = [UIColor colorWithRed:0.522 green:1.000 blue:0.531 alpha:1.000];
    coment1.linkAttributes = @{@"color":@"#78a40e"};
    CGSize optimumSize1 = [coment1 optimumSize];
	CGRect frame1 = [coment1 frame];
	frame1.size.height = (int)optimumSize1.height; //
	[coment1 setFrame:frame1];
    //重新设置view frame
    viewFrame.size.height = YH(coment1)+10;
    view.frame = viewFrame;
    
    
    if (indexPath.row <2) {
//        UIView *bottomLine = [ZJUIMethods creatView:CGRectMake(X(view), YH(view)+3, W(view), 1) bgColor:[UIColor colorWithRed:0.965 green:0.000 blue:0.965 alpha:1.000]];
       // [cell addSubview:bottomLine];
    }
    
    
    return cell;
}

#pragma  mark 去评论列表
-(void)userAction
{
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 105;
}

@end
