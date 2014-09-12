//
//  ZJUserInfoViewController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-16.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJUserInfoViewController.h"
#import "ZJUserInfoModel.h"
#import "ZJEditPwdViewController.h"
#import "ZJEditUserViewController.h"
#import "ZJAppDelegate.h"
@interface ZJUserInfoViewController ()<EditUserInfoViewControllerDelegate,UINavigationControllerDelegate,UIImagePickerControllerDelegate,UIActionSheetDelegate>
{
    ZJUserInfoModel *_userModel;//用户模型
    
    NSArray *_titleArr;//title数组
}
@property (weak, nonatomic) IBOutlet UIView *footView;
@property (weak, nonatomic) IBOutlet UIImageView *prifileImg;
@property (weak, nonatomic) IBOutlet UILabel *praise;
@property (weak, nonatomic) IBOutlet UILabel *guli;
@property (weak, nonatomic) IBOutlet UILabel *jiayou;

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@end

@implementation ZJUserInfoViewController



- (void)viewDidLoad
{
    [super viewDidLoad];
   
    _userModel = [[ZJUserInfoModel alloc] init];

    
    //设置footView
    _footView.layer.borderWidth = 1;
    _footView.layer.cornerRadius = 3;
    _footView.layer.masksToBounds = YES;
    _footView.layer.borderColor = [UIColor colorWithRed:0.000 green:0.678 blue:0.427 alpha:1.000].CGColor;

    
    //设置头像
    [self.prifileImg setImageWithURL:[NSURL URLWithString:_userModel.profileimg] placeholderImage:[UIImage imageNamed:@"profile"]];
    self.prifileImg.tag = 100;
    self.prifileImg.layer.cornerRadius = 30;
    self.prifileImg.layer.masksToBounds = YES;
    self.prifileImg.userInteractionEnabled = YES;
    //单击头像
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self action:@selector(updateProfileAction:)];
    [self.prifileImg addGestureRecognizer:tap];
    //加载个人信息
    [self loadData];

    
     //[self addSubViews];
}
#pragma mark 加载个人信息
-(void)loadData
{
    
    
    [SVProgressHUD showWithStatus:@"加载数据" maskType:SVProgressHUDMaskTypeBlack];
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName};
    [HttpTool getWithPath:@"getuserinfo" params:params success:^(id JSON) {
       // MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0) {
            [_userModel setKeyValues:JSON[@"data"]];
            
            [self.prifileImg setImageWithURL:[NSURL URLWithString:_userModel.profileimg] placeholderImage:[UIImage imageNamed:@"profile"]];
            [[LoginUser sharedLoginUser] saveInfo:_userModel];
            kPdismiss;
            [self addSubViews];
        }
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
    }];
}

-(void)addSubViews
{
    //地址高度
    CGFloat addH = [_userModel.address getHeightByWidth:200 font:kFont(14)];
    
    //过敏史高度

    CGFloat guominshiH = [_userModel.guominshi getHeightByWidth:200 font:kFont(14)];
    //备注高度

    CGFloat remarkH = [_userModel.remark getHeightByWidth:200 font:kFont(14)];
    _titleArr = @[@"昵       称",@"学生姓名",@"家长姓名",@"联系电话",@"家庭住址",@"过  敏  史",@"备注信息",@"修改密码"];
    
    //所有parentView高度
    CGFloat temp = 0;
    int count = 7;
    for (int i = 0; i<=count; i++) {
        //parentView.backgroundColor = [UIColor colorWithRed:0.193 green:1.000 blue:0.789 alpha:1.000];
        //俯视图高度
        CGFloat parentY = 0;
        CGFloat btnY = 30;
        
        NSString *title= nil;
        switch (i) {
            case 0:
            {
                //title = _userModel.nickname;
                title = !_userModel.nickname.isEmptyString ?_userModel.nickname:@"未填写";
            }
            break;
            case 1:
            {
                title = _userModel.name;
            }
                break;
            case 2:
            {
                title = _userModel.parentname;
            }
                break;
            case 3:
            {
                title = _userModel.tel;
            }
                break;
            case 4:
            {
                //title = _userModel.address;
                title = !_userModel.address.isEmptyString ?_userModel.address:@"未填写";
                btnY = addH<btnY?btnY:addH;
                parentY = btnY + 20;
            }
                break;
            case 5:
            {
                //title = _userModel.guominshi;
                title = !_userModel.guominshi.isEmptyString ?_userModel.guominshi:@"未填写";
                btnY = guominshiH<btnY?btnY:guominshiH;
                parentY = btnY +20;
            }
                break;
            case 6:
            {
                title = !_userModel.remark.isEmptyString ?_userModel.remark:@"未填写";
                btnY = remarkH<btnY?btnY:remarkH;
                parentY = btnY +20;

            }
                break;
            
            default:
                break;
        }
        
        if (i<4 || i==7) {
            parentY = 50;
        }
        
        
        
        //MyLog(@"%f",temp);
        
        UIView *parentView = [[UIView alloc] initWithFrame:CGRectMake(5, temp, 270, parentY)];
        temp = YH(parentView);

        [_footView addSubview:parentView];
        
        //title
        UILabel *titleLable = [[UILabel alloc] init];
        titleLable.frame = CGRectMake(0, (H(parentView)-20)/2,60, 20);
        titleLable.font = kFont(14);
        titleLable.text = _titleArr[i];
        [parentView addSubview:titleLable];
        
        //按钮
        
        UILabel *titleLabel = [ZJUIMethods creatLabel:title frame:CGRectMake(70, 10, 200, btnY) font:kFont(14) textColor:[UIColor grayColor]];
        titleLabel.numberOfLines = 0;
        titleLabel.userInteractionEnabled = YES;
        titleLabel.textAlignment = NSTextAlignmentRight;
        titleLabel.tag = i+1;
        
        if (i <1 || i >3 ) {
            titleLabel.textColor = [[UIColor blackColor] colorWithAlphaComponent:1.0];
            UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(editAction:)];
            
            [titleLabel addGestureRecognizer:tap];
            
        }
        if (i==count) {
            //[titleBtn setBackgroundImage:[UIImage imageNamed:@"profile_detial"] forState:UIControlStateNormal];
            //titleBtn.frame = CGRectMake(240, 10, 25, 25);
        }
         [parentView addSubview:titleLabel];
        
        
    
        //分割线
        UIView *lineView = [[UIView alloc] initWithFrame:CGRectMake(0, H(parentView)-1, W(parentView), 1)];
        lineView.backgroundColor = [UIColor colorWithWhite:0.902 alpha:1.000];
        [parentView addSubview:lineView];
        
        if (i == count) {
            
            CGRect footViewFrame = _footView.frame;
            footViewFrame.size.height = YH(parentView);
            _footView.frame = footViewFrame;
            _scrollView.contentSize = CGSizeMake(W(self.view), YH(_footView)+10);
            //MyLog(@"%@",NSStringFromCGSize(size));
        }
    }
    
    _praise.text = [NSString stringWithFormat:@"%@",_userModel.totalpraise];
    _guli.text = [NSString stringWithFormat:@"%@",_userModel.totalflower];
    _jiayou.text = [NSString stringWithFormat:@"%@",_userModel.totalsun];
    
}

-(void)editAction:(UITapGestureRecognizer*)tap
{
    UILabel *sender = (UILabel*)tap.view;
    if (sender.tag == 8) {
        [self pushController:[ZJEditPwdViewController class] withInfo:_userModel.username withTitle:@"修改密码"];
    }else{
        
        ZJEditUserViewController *controller = [[ZJEditUserViewController alloc] init];

        // 1. 设置标题
        controller.contentTitle = sender.text;
        // 2. 传递内容标签
       controller.contentLable = _titleArr[sender.tag-1];
        
        //通过点击的tag值确定要修改的参数
        
//         _titleArr = @[@"昵       称",@"学生姓名",@"家长姓名",@"联系电话",@"家庭住址",@"过  敏  史",@"备注信息",@"修改密码"];
        NSString *param = nil;
        
        if (sender.tag == 1) {
            param = @"nickname";
        }else if(sender.tag  == 5){
            param = @"address";
        }else if(sender.tag  == 6){
            param = @"guominshi";
        }else if(sender.tag  == 7){
            param = @"remark";
        }
        controller.param = param;
        
        
        // 3. 设置代理
        controller.delegate = self;
        [self.navigationController pushViewController:controller animated:YES];
        //[self pushController:[ZJEditPwdViewController class] withInfo:_userModel.username withTitle:@"修改密码"];
    }
    
}


#pragma mark 修改个人信息

-(void)editUserInfoViewControllerDidFinished:(NSString *)contentStr withLabel:(NSString *)labelStr
{
    
//   _titleArr = @[@"昵       称",@"学生姓名",@"家长姓名",@"联系电话",@"家庭住址",@"过  敏  史",@"备注信息",@"修改密码"];
    
    if ([labelStr isEqualToString:_titleArr[4]]) {
        _userModel.address = contentStr;  
    }else if ([labelStr isEqualToString:_titleArr[0]]){
        _userModel.nickname = contentStr;
    }
    else if ([labelStr isEqualToString:_titleArr[5]]){
         _userModel.guominshi = contentStr;
    }
    else if ([labelStr isEqualToString:_titleArr[6]]){
         _userModel.remark = contentStr;
    }
    
    
    //现将俯视图删除，然后重新布局
    NSArray *arr = [_footView subviews];
    for (UIView *view in arr) {
        [view removeFromSuperview];
    }
    [self addSubViews];
    MyLog(@"%@----%@",contentStr,labelStr);
}


//修改头像

#pragma mark 修改头像
-(void)updateProfileAction:(UITapGestureRecognizer*)tapGeture
{

    UIActionSheet *sheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle:@"拍照" otherButtonTitles:@"选择照片", nil];
    
    [sheet showInView:self.view];
    
}
-(void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    
    NSLog(@"%d", buttonIndex);
    if (buttonIndex == 2) {
        return;
    }
    
    UIImagePickerController *controller = [[UIImagePickerController alloc] init];
    
    if (buttonIndex == 0) {
        NSString *deviceModel=[UIDevice currentDevice].model;
        if ([deviceModel isEqualToString:@"iPhone Simulator"]) {
            [SVProgressHUD showErrorWithStatus:@"模拟器不支持拍照" duration:2];
            return;
        }
        controller.sourceType = UIImagePickerControllerSourceTypeCamera;
    }else{
        controller.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    }
    
    controller.allowsEditing = YES;
    controller.delegate = self;
    
    [self presentViewController:controller animated:YES completion:nil];
    
    
}
#pragma mark UIImagePickerController代理方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    // 提示：UIImage不能为空
    // NSData *data = UIImagePNGRepresentation(self.imageView.image);
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    
    //UIImage *img = [UIImage imageNamed:@"some.png"];
    //_imageData = UIImageJPEGRepresentation(image, 1.0);
    
    UIImageView *img = (UIImageView*)[self.view viewWithTag:100];
    img.image = image;
    
    [self updateImage:image];
    
    
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)updateImage:(UIImage *)img
{
    //username=xuesheng&role=0&isParse=false&profileimg=123
    NSDictionary *params = @{@"role":@"0",
                            @"username":[LoginUser sharedLoginUser].userName,
                            };
    
    // profileimg
    [HttpTool updateFileWithPath:@"updateuserinfo" params:params withImag:img success:^(id JSON) {
        //重新加载数据
        [self loadData];
        [self savevCard:img];
    } failure:^(NSError *error) {
        
    }];
    
  }

#pragma mark 修改xmpp 头像
-(void)savevCard:(UIImage*)img
{
   
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
