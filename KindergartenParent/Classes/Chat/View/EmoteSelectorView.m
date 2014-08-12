

#import "EmoteSelectorView.h"



#define kRowCount   4
#define kColCount   7
#define kStartPoint CGPointMake(6, 20)
#define kButtonSize CGSizeMake(44, 44)
@interface EmoteSelectorView (){
    NSDictionary *_dict;
    NSArray *_keys;
    UIPageControl *_pageController;
}
@property(nonatomic,retain) NSArray *allEmojis;
@end
@implementation EmoteSelectorView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.pagingEnabled = YES;
        NSString *plistPath = [[NSBundle mainBundle] pathForResource:@"expressionImage_custom" ofType:@"plist"];
        _dict = [[NSDictionary alloc] initWithContentsOfFile:plistPath];
        _keys = [_dict allKeys];
        self.showsHorizontalScrollIndicator = NO;
        
        _pageController = [[UIPageControl alloc] initWithFrame:CGRectMake((320-50)/2, 185, 50, 20)];
        _pageController.numberOfPages = 3;
        [self addSubview:_pageController];
        
        //_pageController.backgroundColor = [UIColor orangeColor];
        
        // 设置背景颜色
        [self setBackgroundColor:[UIColor colorWithWhite:0.824 alpha:1.000]];
        

    }
    
    return self;
}


-(void)loadEmojiItem:(int)page
{
    _pageController.frame = CGRectMake(((320-50)/2)+(page*320), 185, 50, 20);
    _pageController.currentPage =  page;
    for (int row = 0; row<kRowCount; row++) {
        for (int col = 0; col<kColCount; col++) {
            UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
            // 2) 设置位置，自定义视图中没有使用AutoLayout，因此，可以使用setFrame
            NSInteger x = kStartPoint.x + col * kButtonSize.width+(320*page);
            
            NSInteger y = kStartPoint.y + row * kButtonSize.height;
//            button.layer.borderWidth = 1;
//            button.layer.borderColor = [[UIColor blackColor] CGColor];
            [button setFrame:CGRectMake(x, y, kButtonSize.width, kButtonSize.height)];
            NSInteger index = row * kColCount + col;
            button.tag = index;
            [self addSubview:button];
            
            if(row==kRowCount-1 && col == kColCount-1){
                [button setImage:[UIImage imageNamed:@"DeleteEmoticonBtn"] forState:UIControlStateNormal];
                button.tag = 10000;
            }else{

                NSString *imageName =   _dict[_keys[col+(row*kColCount)+(page*27)]];
                
                button.tag = col+(row*kColCount)+(page*27);
                [button setImage:[UIImage imageNamed:imageName] forState:UIControlStateNormal];
            }
            [button addTarget:self action:@selector(clickEmote:) forControlEvents:UIControlEventTouchUpInside];
            [self addSubview:button];
            // 6) 添加到临时数组
            //[array addObject:button];
        }
    }
}

- (NSString *)emoteStringWithIndex:(NSInteger)index
{
    return _keys[index];
}

#pragma mark - 表情按钮点击事件
- (void)clickEmote:(UIButton *)button
{
   
    
    if (button.tag != 10000) {
         NSString *string = [self emoteStringWithIndex:button.tag];
        // 通知代理接收用户选择的表情字符串
        [_delegates emoteSelectorViewSelectEmoteString:string];
    } else {
        // 通知代理处理删除字符功能
        [_delegates emoteSelectorViewRemoveChar];
    }
}


@end
