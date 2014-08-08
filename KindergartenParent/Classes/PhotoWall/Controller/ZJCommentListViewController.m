//
//  ZJTableViewController.m
//  ybparents
//
//  Created by define on 14-6-20.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJCommentListViewController.h"
#import "ZJCommentCell.h"
#import "ZJCommentModel.h"

int page = 1;

@interface ZJCommentListViewController ()<UITextFieldDelegate>
{
    NSMutableArray *_dataArr;
}
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *inputViewConstraint;
@property (weak, nonatomic) IBOutlet UITextField *commentTextField;
@end


@implementation ZJCommentListViewController

-(void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    page = 1;
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    
    //[self addData];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    // 1. 利用通知中心监听键盘的变化（打开、关闭、中英文切换）
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardChangeFrame:) name:UIKeyboardWillChangeFrameNotification object:nil];
    
    _commentTextField.layer.borderColor = [UIColor colorWithRed:0.109 green:0.502 blue:0.273 alpha:1.000].CGColor;
    _commentTextField.layer.borderWidth = 1.0;
    //初始化数组
    _dataArr = [NSMutableArray array];

    //加载数据
    [self headerRereshing];
    
    
    // 集成刷新控件
    [self setupRefresh];
    
}

/**
 *  集成刷新控件
 */
- (void)setupRefresh
{
    // 1.下拉刷新(进入刷新状态就会调用self的headerRereshing)
    [self.tableView addHeaderWithTarget:self action:@selector(headerRereshing)];
    //#warning 自动刷新(一进入程序就下拉刷新)
    //[self.tableView headerBeginRefreshing];
    
    // 2.上拉加载更多(进入刷新状态就会调用self的footerRereshing)
    [self.tableView  addFooterWithTarget:self action:@selector(footerRereshing)];
}

-(void)headerRereshing
{
    [self getDataForHeaderOrFooter:@"header"];
}

- (void)footerRereshing
{
    
    [self getDataForHeaderOrFooter:@"footer"];
}

-(void)getDataForHeaderOrFooter:(NSString*)str
{
    if ([str isEqualToString:@"header"]) {
        self.page = 1;
    }
    kPBlack(@"数据加载中");
    //182.18.23.244:8080/kindergarten/service/app!ntfilist.action?page=1&username=hzl&role=3
     NSDictionary *params = @{@"pwallid":self.userInfo,@"page":@(self.page)};
    [HttpTool getWithPath:@"commentlist" params:params success:^(id JSON) {
        MyLog(@"%@",JSON[@"data"]);
        
        if ([JSON[@"code"] intValue] == 0) {
            //删除所有的数据先
            if ([str isEqualToString:@"header"]) {
                [_dataArr removeAllObjects];
            }
            for (NSDictionary *dict in JSON[@"data"]) {
                ZJCommentModel *model = [[ZJCommentModel alloc] init];
                [model setKeyValues:dict];
                [_dataArr addObject:model];
                
            }
            [self.tableView reloadData];
            //model setKeyValues:JSON
            
            //滚动到顶部
            if (page == 1) {
                [self scrollToTableBottom];
            }
            if (((NSArray*)JSON[@"data"]).count) {
               self.page++;
            }
            
            kPdismiss;
            [self.tableView reloadData];
        }else{
            kPE(JSON[@"msg"], 0.5);
        }
        if ([str isEqualToString:@"header"]) {
            [self.tableView headerEndRefreshing];
        }else{
            [self.tableView footerEndRefreshing];
        }
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
        MyLog(@"%@",error.localizedDescription);
    }];
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    
    return _dataArr.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    NSString *Id  = [NSString stringWithFormat:@"%d",indexPath.row];
    ZJCommentCell *cell = [tableView dequeueReusableCellWithIdentifier:Id];
    
    if (!cell) {
        cell = [[ZJCommentCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:Id];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    ZJCommentModel *model = _dataArr[indexPath.row];
    
    //判断是否最后一个，让后去掉分割线
    if ([model isEqual:_dataArr.lastObject]) {
        cell.isLast = true;
    }
    
    cell.tag = indexPath.row;
    [cell setCmmodel:model];
    
    // Configure the cell...
    //如果是第一行，调整bgVie的高度顶部的高度是20
    if (indexPath.row == 0) {
        CGRect bgF = cell.bgView.frame;
        bgF.origin.y = 20;
        cell.bgView.frame = bgF;
    }
    
    
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ZJCommentModel *model = _dataArr[indexPath.row];
    
    
    CGFloat flag = 40;
    if (indexPath.row == 0) {
        flag = 60;
    }
    CGFloat height = 20 ;
    
    CGFloat contentH = [model.content getHeightByWidth:230 font:kFont(17)];
    
    //如果高于内容的高度，就加上
    if (contentH > height ) {
        height = contentH ;
    }
    
    
    return height + flag;
}


#pragma mark - 键盘边框大小变化
- (void)keyboardChangeFrame:(NSNotification *)notification
{
    // 根据跟踪发现，使用userInfo的UIKeyboardFrameEndUserInfoKey数据可以判断键盘的大小和目标位置
    
    // 1. 获取键盘的目标区域
    NSDictionary *info = notification.userInfo;
    CGRect rect = [info[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    CGFloat duration = [info[UIKeyboardAnimationDurationUserInfoKey] floatValue];
    
    // 2. 根据rect的orgion.y可以判断键盘是开启还是关闭
    if (rect.origin.y == [UIScreen mainScreen].bounds.size.height) {
        // 关闭键盘
        _inputViewConstraint.constant = 0.0;
    } else {
        // 打开键盘
        _inputViewConstraint.constant = rect.size.height;
    }
    
    // 用自动布局系统实现动画，调整位置
    [UIView animateWithDuration:duration animations:^{
        [self.view layoutIfNeeded];
    } completion:^(BOOL finished) {
        //[self scrollToTableBottom];
    }];
}
#pragma mark -评论
- (IBAction)sendCommentAction:(id)sender {
    
    [_commentTextField resignFirstResponder];
    
    NSString *commentStr = _commentTextField.text.trimString;
    if (commentStr.isEmptyString) {
        [SVProgressHUD showErrorWithStatus:@"评论类容不能为空" duration:1.0];
        return;
    }
    
    [SVProgressHUD showWithStatus:@"正在评论" maskType:SVProgressHUDMaskTypeBlack];
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"pwallid":self.userInfo,
                             @"content":commentStr};
    [HttpTool getWithPath:@"comment" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0) {
            
            [SVProgressHUD showSuccessWithStatus:@"评论成功" duration:1];
            
            //先删除数据
            [_dataArr removeAllObjects];
            //重新加载数据
            [self headerRereshing];
            
            //将文本内容清空
            _commentTextField.text = @"";
            
        }else{
            kPS(JSON[@"msg"], 0.5);
        }
    } failure:^(NSError *error) {
        kPdismiss;
    }];
    
}
#pragma mark 滚动到表格的末尾
- (void)scrollToTableBottom
{
    // 要选中滚动到最末一条记录
    // 1. 知道所有的记录行数
    NSInteger count = _dataArr.count;
    
    if (count <= 0) {
        return;
    }
    
    // 2. 根据行数实例化indexPath
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:0 inSection:0];
    // 3. 选中并滚动到末尾
    [self.tableView selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionBottom];
}
-(BOOL)textFieldShouldReturn:(UITextField *)textField{
    [self sendCommentAction:nil];
    return YES;
}

-(void)dealloc
{
   
    [[NSNotificationCenter
      defaultCenter] removeObserver:self name:@"keyboardChangeFrame"
     
     object:nil];

}

@end
