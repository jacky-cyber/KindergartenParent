//
//  ZJCookBookViewController.m
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJCookBooksViewController.h"
#import "ZJCookBookTableViewCell.h"
#import "CookBookModel.h"
#import "UIImageView+MJWebCache.h"
#import "MJPhotoBrowser.h"
#import "MJPhoto.h"
@interface ZJCookBooksViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
    NSMutableArray *_dataArr;
    NSArray *_images;//图片
}
@end

@implementation ZJCookBooksViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, W(self.view),height) style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    //_tableView.sectionFooterHeight = 0;
    //_tableView.rowHeight = [ZJCookBookTableViewCell cellHeight];
    
    [self.view addSubview:_tableView];
    
    [self initData];
}
#pragma mark 初始化数据
-(void)initData
{
    
    //初始化数组
    _dataArr = [NSMutableArray array];
    
    [HttpTool getWithPath:@"recipe" params:nil success:^(id JSON) {
        if ([JSON[@"code"] intValue] == 0) {
            NSArray *data = JSON[@"data"][@"everyday"];
            for (NSDictionary *dict in data) {
        
                 CookBookModel *model = [CookBookModel objectWithKeyValues:dict];
                [model setKeyValues:dict];
                [_dataArr addObject:model];
            }
            [_tableView reloadData];
            
            if (JSON[@"data"][@"images"]) {
                _images = JSON[@"data"][@"images"];
            
                UIButton *btnR = [UIButton buttonWithType:UIButtonTypeSystem];
                btnR.frame = CGRectMake(0, 2, 50, 25);
                [btnR setBackgroundImage:[UIImage imageNamed:@"nav_rightbackbround_image"] forState:UIControlStateNormal];
                [btnR addTarget:self action:@selector(imagesAction:) forControlEvents:UIControlEventTouchUpInside];
                [btnR setTitle:@"图片" forState:UIControlStateNormal];
                [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
                UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
                ItemR.tag = 10;
                self.navigationItem.rightBarButtonItem = ItemR;
                
            }
            
            
        }
        
//        CookBookModel *dict = _dataArr[1];
//        NSLog(@"%d",dict.count);
        
    } failure:^(NSError *error) {
        MyLog(@"%@",error.localizedDescription);
    }];
}
#pragma mark 跳转到图片页面
-(void)imagesAction:(UIButton *)sender{
 
        int count = _images.count;
    
    if (!count) {
        kPE(@"本周食谱没有图片", 1);
    }
    
    
        // 1.封装图片数据
        NSMutableArray *photos = [NSMutableArray arrayWithCapacity:count];
        for (int i = 0; i<count; i++) {
            // 替换为中等尺寸图片
            
            MJPhoto *photo = [[MJPhoto alloc] init];
            photo.url = [NSURL URLWithString:_images[i]]; // 图片路径
           // photo.srcImageView = self.view.subviews[i]; // 来源于哪个UIImageView
            [photos addObject:photo];
        }
        
        // 2.显示相册
        MJPhotoBrowser *browser = [[MJPhotoBrowser alloc] init];
        //browser.currentPhotoIndex = sender.tag; // 弹出相册时显示的第一张图片是？
        browser.photos = photos; // 设置所有的图片
        [browser show];
    
    
    //[self pushController:[ZJCookBookImagesViewController class] withInfo:_images withTitle:@"食谱配置"];
    
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    
   // NSDictionary *dict = [_dataArr[section] keyValues];
    
    return _dataArr.count ;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    ZJCookBookTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:[ZJCookBookTableViewCell ID]];
    if (cell == nil) {
        cell = [[ZJCookBookTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:[ZJCookBookTableViewCell ID]];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    NSString *imageName = [NSString stringWithFormat:@"zhou%d",indexPath.row+1];
    cell.weekImg.image = [UIImage imageNamed:imageName];
    CookBookModel *model = _dataArr[indexPath.row];
    
    [cell setModel:model];  
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CookBookModel *model = _dataArr[indexPath.row];
    
    CGFloat height = 60;
    
    height += [self getHeight:model.breakfast];
    height += [self getHeight:model.lunch];
    height += [self getHeight:model.supper];
    height += [self getHeight:model.jiacan];
    
    return height;
}
#pragma mark 计算没餐的高度
-(CGFloat)getHeight:(NSString*)str
{
    CGFloat h = 0;
     h = [str getHeightByWidth:230 font:kFont(17)];
     h = h>21?h:21;
    return h;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
