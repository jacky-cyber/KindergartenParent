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
@interface ZJCommentListViewController ()
{
    NSMutableArray *_dataArr;
}
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *inputViewConstraint;
@property (weak, nonatomic) IBOutlet UITextField *commentTextField;
@end


@implementation ZJCommentListViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    _dataArr = [NSMutableArray array];
    
    [self addData];
    
    
    // 1. 利用通知中心监听键盘的变化（打开、关闭、中英文切换）
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardChangeFrame:) name:UIKeyboardWillChangeFrameNotification object:nil];
    
}
#pragma mark 添加数据
-(void)addData{
    
    
    for (int i = 0; i<10; i++) {
        ZJCommentModel *model = [[ZJCommentModel alloc] init];
        model.cmtime = @"2014-06-18 12:12:12";
        model.content = @"你是一个美丽的小女孩儿，哈哈哈你是一个美丽的小女孩儿，哈哈哈你是一个美丽的小女孩儿，哈哈哈";
        model.cmnickname = @"张三";
        model.cmuimgurl = @"http://static.oschina.net/uploads/user/98/196012_100.jpg?t=1378866193000";
        [_dataArr addObject:model];
        
        ZJCommentModel *model1 = [[ZJCommentModel alloc] init];
        model1.cmtime = @"2014-05-18 12:12:12";
        model1.content = @"你是一个美丽的小女孩儿，哈哈哈你是一个美丽的小女孩儿，哈哈哈";
        model1.cmnickname = @"李四";
        model1.cmuimgurl = @"http://static.oschina.net/uploads/user/98/196012_100.jpg?t=1378866193000";
        [_dataArr addObject:model1];
        
    }
    
    
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
    cell.tag = indexPath.row;
    [cell setCmmodel:model];
    
    // Configure the cell...
    //如果是第一行，调整bgVie的高度
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

- (IBAction)sendCommentAction:(id)sender {
}

/*
 // Override to support conditional editing of the table view.
 - (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
 {
 // Return NO if you do not want the specified item to be editable.
 return YES;
 }
 */

/*
 // Override to support editing the table view.
 - (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
 {
 if (editingStyle == UITableViewCellEditingStyleDelete) {
 // Delete the row from the data source
 [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
 } else if (editingStyle == UITableViewCellEditingStyleInsert) {
 // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
 }
 }
 */

/*
 // Override to support rearranging the table view.
 - (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
 {
 }
 */

/*
 // Override to support conditional rearranging of the table view.
 - (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
 {
 // Return NO if you do not want the item to be re-orderable.
 return YES;
 }
 */

/*
 #pragma mark - Table view delegate
 
 // In a xib-based application, navigation from a table can be handled in -tableView:didSelectRowAtIndexPath:
 - (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
 {
 // Navigation logic may go here, for example:
 // Create the next view controller.
 <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:<#@"Nib name"#> bundle:nil];
 
 // Pass the selected object to the new view controller.
 
 // Push the view controller.
 [self.navigationController pushViewController:detailViewController animated:YES];
 }
 */

@end
