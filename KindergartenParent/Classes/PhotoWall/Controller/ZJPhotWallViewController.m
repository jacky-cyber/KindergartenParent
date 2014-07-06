//
//  ZJPhotWallViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJPhotWallViewController.h"
#import "ZJCommentListViewController.h"
#import "ZJPotoWallModel.h"
#import "UIImageView+MJWebCache.h"
#import "MJPhotoBrowser.h"
#import "MJPhoto.h"
static int page = 1;
@interface ZJPhotWallViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
    NSMutableArray *_dataArr;//数据数组
    
    NSArray *_images;//图片数组
}

@end

@implementation ZJPhotWallViewController


-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
   // page = 1;
}
-(void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:YES];
}
-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:YES];
}
-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [super viewWillAppear:animated];
    page = 1;

}
- (void)viewDidLoad
{
    [super viewDidLoad];

    self.title = @"成长记录";
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        //height -=20;
    }
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0,0,self.view.frame.size.width,height) style:UITableViewStyleGrouped];
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.sectionFooterHeight = 0;
    [self.view addSubview:_tableView];
    //设置背景，ios6里面不然就很乱
    UIView *backView = [[UIView alloc] init];
    backView.backgroundColor = [UIColor whiteColor];
    [_tableView setBackgroundView:backView];
    
    
    
    self.navigationController.navigationItem.rightBarButtonItem = nil;
    
    
    
    //看自己
    
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 4, 65, 25);
    [btnR addTarget:self action:@selector(initData:) forControlEvents:UIControlEventTouchUpInside];
    
    UIImage *backgroundImg= [UIImage resizedImage:@"nav_rightbackbround_image"];
    
    [btnR setBackgroundImage:backgroundImg forState:UIControlStateNormal];
    [btnR setTitle:@"看自己" forState:UIControlStateNormal];
    [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;

    
    //初始化数组
    _dataArr = [NSMutableArray array];
    
    //加载数据
    [self initData:nil];
}

-(void)initData:(UIButton*)btn{
    

    
//    classesid	int	班级id
//    catid	分类	分类id
//    username	String	看自己
//    page	int	页数
    kPBlack(@"正在加载成长记录");
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
        //@{@"classesid":@"1",@"page":@"1"}
   
    //如果有分类
    if (btn) {
        [params setValue:@(btn.tag) forKey:@"catid"];
        //删除数据并且初始化page
        [_dataArr removeAllObjects];
        page = 1;
    }
    [params setObject:@"1" forKey:@"classesid"];
    [params setObject:@(page) forKey:@"page"];
    
    [HttpTool getWithPath:@"potowall" params:params success:^(id JSON) {
        for (NSDictionary *dict in JSON[@"data"]) {
            ZJPotoWallModel *model = [[ZJPotoWallModel alloc] init];
            [model setKeyValues:dict];
            model.praisecount = [NSString stringWithFormat:@"%@",dict[@"praisecount"]];
            model.comcount = [NSString stringWithFormat:@"%@",dict[@"comcount"]];
            [_dataArr addObject:model];
            //MyLog(@"%@",model.images);
        }
        kPdismiss;
        page ++;
        [_tableView reloadData];
    } failure:^(NSError *error) {
        kPdismiss
        MyLog(@"%@",error.debugDescription);
    }];
    //kPdismiss
}


#define kMargin 10
-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    
    
   ZJPotoWallModel *model = _dataArr[section];
    
    UIView *view = [[UIView alloc] init];
    view.backgroundColor  = [UIColor whiteColor];
    
    //父视图
    UIView *superView = [[UIView alloc] init];
    
    //设置顶部距离，第一条距离顶部20
    CGFloat marginTop= 10;
    if (section == 0) {
        marginTop = 20;
    }
    
    superView.frame = CGRectMake(kMargin, marginTop, 300,210);
    superView.layer.borderColor = [UIColor colorWithRed:0.820 green:0.867 blue:0.773 alpha:1.000].CGColor;
    superView.layer.borderWidth = 1;
    [view addSubview:superView];
    //标题
    UILabel *title = [[UILabel alloc] initWithFrame:CGRectMake(kMargin, kMargin, 280, 21)];
    title.text = model.title;
    [superView addSubview:title];
    
    
    //给图片加一个视图
    UIView *imageBgView = [[UIView alloc] initWithFrame:CGRectMake(0, kMargin+YH(title), 280, 150)];
    imageBgView.tag = section+100;
    [superView addSubview:imageBgView];

    if (model.images.count) {
        //if (section == 0) {
            _images = model.images;

        //}
        UIImage *placeholder = [UIImage imageNamed:@"timeline_image_loading.png"];

        //取出图片的张数
        int count = model.images.count;
        
        for (int i = 0 ; i<count; i++) {
            
            CGFloat width = 280;
            CGFloat margin = 0;
            //如果大于一张的时候就显示张图片
            if (count>1) {
                width = 135;
                margin = 140*i;
            }
            
            UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(kMargin+margin, 0, width, 150)];
            [imageBgView addSubview:imageView];

            [imageView setImageURLStr:model.images[i] placeholder:placeholder];
            //if (count==1) {
                imageView.contentMode = UIViewContentModeScaleAspectFill ;
                imageView.clipsToBounds = YES;
            //}
             imageView.tag = i;
            imageView.userInteractionEnabled = YES;
            [imageView addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapImage:)]];
           
            if (i == 1) {
                break;
            }
        }
        
    }
    

    
    //工具条view
    UIView *toolView = [[UIView alloc] init];
    toolView.frame = CGRectMake(kMargin,  kMargin+YH(imageBgView), 280, 16);
    toolView.backgroundColor = [UIColor whiteColor];
    [superView addSubview:toolView];
    
    //时间图标
    UIImageView *timeImg  = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"p_time"]];
    timeImg.frame = CGRectMake(0, 0, 16, 16);
    [toolView addSubview:timeImg];
    //时间
    UILabel *timeLabel = [[UILabel alloc] initWithFrame:CGRectMake(XW(timeImg)+5, 0, 80, 16)];
    timeLabel.text = model.createtime;
    timeLabel.textColor = [UIColor colorWithWhite:0.725 alpha:1.000];
    timeLabel.font = kFont(12);
    [toolView addSubview:timeLabel];
    
    UIButton *praiseBtn = [ZJUIMethods creatButton:nil frame:CGRectMake(150, 0, 16, 16) delegate:self selector:@selector(praiseAction) tag:0];
    [praiseBtn setImage:[UIImage imageNamed:@"p_parise"] forState:UIControlStateNormal];
    [toolView addSubview:praiseBtn];
    
    //赞
    UILabel *praiseLable = [ZJUIMethods creatLabel:model.praisecount frame:CGRectMake(XW(praiseBtn)+5, 0, 30, 16) font:kFont(12) textColor:nil];
    praiseLable.textColor = [UIColor colorWithWhite:0.725 alpha:1.000];
    //praiseLable.backgroundColor = [UIColor redColor];
    [toolView addSubview:praiseLable];
    
    //评论按钮
    UIButton *commentBtn = [ZJUIMethods creatButton:nil frame:CGRectMake(XW(praiseLable)+2*kMargin, 0, 16, 16) delegate:self selector:@selector(commentAction:) tag:[model.id intValue]];
    [commentBtn setImage:[UIImage imageNamed:@"p_comment"] forState:UIControlStateNormal];
    [toolView addSubview:commentBtn];
    
    //评论
    UILabel *commentLb = [ZJUIMethods creatLabel:model.comcount frame:CGRectMake(XW(commentBtn)+5, 0, 30, 16) font:kFont(12) textColor:nil];
    commentLb.textColor = [UIColor colorWithWhite:0.725 alpha:1.000];
    //commentLb.backgroundColor = [UIColor redColor];
    [toolView addSubview:commentLb];
    
    //标签按钮
    UIView *cateView = [ZJUIMethods creatView:CGRectMake(kMargin, YH(toolView)+kMargin, 280, 20) bgColor:[UIColor whiteColor]];
    
    //按钮
    NSArray *cateArr = @[@"健康",@"语言",@"科学",@"艺术",@"社会"];
    for (int i = 0; i<5; i++) {
        UIButton *button = [ZJUIMethods creatButton:cateArr[i] frame:CGRectMake(i*58, 0, 48, 20) delegate:self selector:@selector(initData:) tag:i+1];
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


#pragma mark 跳转到图片页面
- (void)tapImage:(UITapGestureRecognizer *)tap
{
    //计算出父视图tag值
    int tag = tap.view.superview.tag;
    ZJPotoWallModel *model = _dataArr[tag-100];
    

    int count = model.images.count;
    // 1.封装图片数据
    NSMutableArray *photos = [NSMutableArray arrayWithCapacity:count];
    for (int i = 0; i<count; i++) {
        // 替换为中等尺寸图片
        //NSString *url = [model.images[i] stringByReplacingOccurrencesOfString:@"thumbnail" withString:@"bmiddle"];
        MJPhoto *photo = [[MJPhoto alloc] init];
        photo.url = [NSURL URLWithString:model.images[i]]; // 图片路径
        UIView *imageBgView  = [self.view viewWithTag:tag];
        //只放两张图片，所以i<2
        if (i<2) {
             photo.srcImageView = imageBgView.subviews[i]; // 来源于哪个UIImageView
        }
       
        [photos addObject:photo];
    }
    
    // 2.显示相册
    MJPhotoBrowser *browser = [[MJPhotoBrowser alloc] init];
    browser.currentPhotoIndex = tap.view.tag; // 弹出相册时显示的第一张图片是？
    browser.photos = photos; // 设置所有的图片
    [browser show];
}

-(void)commentAction:(UIButton*)btn{
    [self pushController:[ZJCommentListViewController class] withInfo:@(btn.tag) withTitle:@"评论"];
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
    CGFloat h = 285;
    if (section >0) {
        h = 275;
    }
    return h;
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1;
}
-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    
    return _dataArr.count;
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
    
    ZJPotoWallModel *model = _dataArr[indexPath.section];
    //自定义View
    
    
   // NSLog(@"%@",NSStringFromCGRect(cell.frame));
    if (ISIOS7) {
        
    }
    //cell.backgroundColor = [UIColor brownColor];
    UIView *view = [ZJUIMethods creatView:CGRectMake(kMargin, 0, 300, 100) bgColor:[UIColor colorWithWhite:0.969 alpha:1.000]];
    [cell addSubview:view];
    CGRect viewFrame = view.frame;
    
    //防止重复 删除姿势图
    NSArray *subViews = view.subviews;
    for (UIView *subView in subViews) {
        [subView removeFromSuperview];
    }
    
    CGFloat height = 0;
    for (int i = 0; i < model.comment.count; i ++) {
        NSDictionary *commDict = model.comment[i];
  
        NSString *cmStr = [NSString stringWithFormat:@"%@:%@",commDict[@"cmnickname"],commDict[@"cmcontent"]];
        // 如果想要改变部份文本内容的风格，我们就需要用到NSAttributedString NSMutableAttributedString
        NSMutableAttributedString *attrString = [[NSMutableAttributedString alloc] initWithString:cmStr];
        
        int nickNameLengt = ((NSString*)commDict[@"cmnickname"]).length;
        [attrString addAttribute:(NSString *)kCTForegroundColorAttributeName
                            value:[UIColor redColor]
                            range:NSMakeRange(0,nickNameLengt )];
         [attrString addAttribute:NSForegroundColorAttributeName value:[UIColor colorWithRed:0.627 green:0.827 blue:0.149 alpha:1.000] range:NSMakeRange(0,nickNameLengt)];
       
        UILabel *coment = [ZJUIMethods creatLabel:nil frame:CGRectMake(10, height, 280, 16) font:kFont(13) textColor:nil];
        coment.lineBreakMode = NSLineBreakByWordWrapping;
        coment.backgroundColor = [UIColor clearColor];
        [coment setAttributedText:attrString];
        [view addSubview:coment];

        //重新计算label 的高度
        CGRect frame = coment.frame;
    
        //CGSize optimumSize = coment.optimumSize;
        //重新计算label 的高度
        frame.size.height = [self getRTLabelH:cmStr]+5;
    
        height += frame.size.height+10;
    
        [coment setFrame:frame];
        //添加分割线
        if (i<model.comment.count -1) {
            UIView *line = [[UIView alloc] initWithFrame:CGRectMake(X(coment), YH(coment)+5, 280, 1)];
            line.backgroundColor = [UIColor colorWithWhite:0.941 alpha:1.000];
            [view addSubview:line];
        }
 
        //重新设置view frame
        viewFrame.size.height = YH(coment);
        view.frame = viewFrame;

        
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
    
     ZJPotoWallModel *model = _dataArr[indexPath.section];
    
    CGFloat height = 0;
    
    if (model.comment.count==0) {
        height = 0;
    }
    
    for (int i = 0; i < model.comment.count; i ++) {
        height +=10;
        if (i>0) {
           // height +=10;
        }
        NSDictionary *commDict = model.comment[i];
        
        NSString *commStr = [NSString stringWithFormat:@"%@:%@",commDict[@"cmnickname"],commDict[@"cmcontent"]];
        height += [self getRTLabelH:commStr];
         // MyLog(@"%f------%@",height,commStr);
    }
    //MyLog(@"%f------",height);
    
    return height;
}

#pragma 返回每个rtlable的高度
-(CGFloat)getRTLabelH:(NSString *)str
{
    CGFloat h = 0;
    
    h = [str getHeightByWidth:280 font:kFont(13)];
     //MyLog(@"%@--%f",str,h);
    return h;
    
    
}

@end
