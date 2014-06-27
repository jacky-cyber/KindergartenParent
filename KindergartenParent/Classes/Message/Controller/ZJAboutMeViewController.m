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
