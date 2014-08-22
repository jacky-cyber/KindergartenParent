//
//  NewfeatureController.m
//  新浪微博
//
//  Created by apple on 13-10-26.
//  Copyright (c) 2013年 itcast. All rights reserved.
//

#import "NewfeatureController.h"
#import "ZJLoginViewController.h"

#define kCount 3

@interface NewfeatureController () <UIScrollViewDelegate>
{
    UIPageControl *_page;
    UIScrollView *_scroll;
}
@end

@implementation NewfeatureController



/*
 一个控件无法显示：
 1.没有设置宽高(或者宽高为0)
 2.位置不对
 3.hidden=YES
 4.没有添加到控制器的view上面
 
 一个UIScrollView无法滚动：
 1.contentSize没有值
 2.不能接收到触摸事件
 */
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    
    // 1.添加UIScrollView
    [self addScrollView];
    
    // 2.添加图片
    [self addScrollImages];
    
    // 3.添加UIPageControl
    [self addPageControl];
}

#pragma mark - UI界面初始化
#pragma mark 添加滚动视图
- (void)addScrollView
{
    UIScrollView *scroll = [[UIScrollView alloc] init];
    scroll.frame = self.view.bounds;
    scroll.showsHorizontalScrollIndicator = NO; // 隐藏水平滚动条
    CGSize size = scroll.frame.size;
    scroll.contentSize = CGSizeMake(size.width * kCount, 0); // 内容尺寸
    scroll.pagingEnabled = YES; // 分页
    scroll.delegate = self;
    [self.view addSubview:scroll];
    _scroll = scroll;
}

#pragma mark 添加滚动显示的图片
- (void)addScrollImages
{
    CGSize size = _scroll.frame.size;
    
    for (int i = 0; i<kCount; i++) {
        UIImageView *imageView = [[UIImageView alloc] init];
        // 1.显示图片
        NSString *name = [NSString stringWithFormat:@"new_feature_%d.png", i + 1];
        imageView.image = [UIImage fullscrennImage:name];
        // 2.设置frame
        imageView.frame = CGRectMake(i * size.width, 0, size.width, size.height);
        [_scroll addSubview:imageView];
        
        if (i == kCount - 1) { // 最后一页，添加2个按钮
            // 3.立即体验（开始）
            UIButton *start = [UIButton buttonWithType:UIButtonTypeCustom];
            UIImage *startNormal = [UIImage imageNamed:@"button_start"];
            [start setBackgroundImage:startNormal forState:UIControlStateNormal];
            [start setBackgroundImage:[UIImage imageNamed:@"button_start"] forState:UIControlStateHighlighted];
            start.center = CGPointMake(size.width * 0.5, size.height * 0.83);
            //            start.bounds = CGRectMake(0, 0, startNormal.size.width, startNormal.size.height);
            start.bounds = (CGRect){CGPointZero, startNormal.size};
            [start addTarget:self action:@selector(start) forControlEvents:UIControlEventTouchUpInside];
            [imageView addSubview:start];
            
            imageView.userInteractionEnabled = YES;
        }
    }
}

#pragma mark 添加分页指示器
- (void)addPageControl
{
    CGSize size = self.view.frame.size;
    UIPageControl *page = [[UIPageControl alloc] init];
    page.center = CGPointMake(size.width * 0.5, size.height * 0.92);
    page.numberOfPages = kCount;
    //    page.currentPageIndicatorTintColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"new_feature_pagecontrol_checked_point.png"]];
    //    page.pageIndicatorTintColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"new_feature_pagecontrol_point.png"]];
    page.bounds = CGRectMake(0, 0, 150, 0);
    [self.view addSubview:page];
    _page = page;
}

#pragma mark - 监听按钮点击
#pragma mark 开始
- (void)start
{
    MyLog(@"开始微博");
    
    //    [UIApplication sharedApplication].keyWindow;
    
    /*
     有显示状态栏时创建控制器，控制器view的高度：460
     没有显示状态栏时创建控制器，控制器view的高度：480
     */
    // 显示状态栏
    [UIApplication sharedApplication].statusBarHidden = NO;
    
    // 控制器的view是延迟加载：需要显示的\用到的时候才会加载
    self.view.window.rootViewController = [[ZJLoginViewController alloc] init];
}


#pragma mark - 滚动代理方法
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    _page.currentPage = scrollView.contentOffset.x / scrollView.frame.size.width;
}

- (void)dealloc
{
    MyLog(@"new----销毁");
}

@end
