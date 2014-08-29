//
//  ZJAboutMeViewController.m
//  ybparents
//
//  Created by define on 14-6-20.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJAboutMeViewController.h"
#import "UIImage+ZJ.h"
@interface ZJAboutMeViewController ()
{
    UILabel *_version;//版本
}
@end

@implementation ZJAboutMeViewController

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
    // Do any additional setup after loading the view.
    
    
    // Do any additional setup after loading the view from its nib.
    
    UIImage *bgImage = [UIImage imageNamed:@"about_me"];
    if (iPhone5) {
        bgImage = [UIImage imageNamed:@"aboutme"];
    }
    self.view.backgroundColor = [UIColor colorWithPatternImage:bgImage];
    
    NSString *key = (NSString *)kCFBundleVersionKey;
    // 1.从Info.plist中取出版本号
    NSString *version = [NSBundle mainBundle].infoDictionary[key];
    NSString *versionStr = [NSString stringWithFormat:@"版本号 V%@",version];
    if (!_version) {
        CGFloat height = 190;
        if (iPhone5) {
            height+=30;
        }
        _version = [ZJUIMethods creatLabel:versionStr frame:CGRectMake(100, height, 120, 21) font:kFont(17) textColor:[UIColor lightGrayColor]];
        _version.textAlignment = NSTextAlignmentCenter;
        _version.backgroundColor = [UIColor clearColor];
        [self.view addSubview:_version];
    }

}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
