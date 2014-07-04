//
//  ZJHonorCell.m
//  ybparents
//
//  Created by 郑敬 on 14-6-17.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJHonorCell.h"
#import "ZJHonorModel.h"


#define kImgWH 30

@interface ZJHonorCell ()
{
    UILabel *_leftLabel;
   
}
@end

@implementation ZJHonorCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
        
        //添加子视图
        [self addSubviews];
        
        
    }
    return self;
}


#pragma mark 设置模型
-(void)setModel:(ZJHonorModel *)model
{
    _model = model;
    
    //计算出高度
    CGFloat height = 50;
    int sum  = _model.flower.intValue+ _model.sun.intValue + _model.praise.intValue;
    int colNum = sum/4;
    if (sum%4 !=0) {
        colNum +=1;
    }
   // NSLog(@"%d---%d",sum,colNum);
    
    height = 42 * colNum;
    
    _leftLabel.text = _model.date;
    
    _leftLabel.frame = CGRectMake(20, 5, 100,height);
    
    _rightView.frame = CGRectMake(XW(_leftLabel), Y(_leftLabel), 180,height);
    // 每行3列
    int columns = 4;
    // 每个表情之间的间距 = (控制器view的宽度 - 列数 * 表情的宽度) / (列数 + 1)
    CGFloat margin = (W(_rightView) - columns * kImgWH) / (columns + 1);
    
    // 2.定义第一个表情的位置
    // 第一个表情的Y值
    CGFloat oneY = 5;
    // 第一个表情的x值
    CGFloat oneX = margin;
    
    // 3.创建所有的表情
    for (int i = 0; i<sum; i++) {

        NSString *imgName = nil;
        if (i <_model.flower.intValue) {
            imgName = [NSString stringWithFormat:@"guli"];
        }else if (i < _model.flower.intValue + _model.sun.intValue && i >= _model.flower.intValue) {
            imgName = [NSString stringWithFormat:@"jiayou"];
        }else if (i >=_model.flower.intValue + _model.sun.intValue ) {
            imgName = [NSString stringWithFormat:@"praise"];
        }

        // i这个位置对应的列数
        int col = i % columns;
        // i这个位置对应的行数
        int row = i / columns;
        
        // 列数（col）决定了x
        CGFloat x = oneX + col * (kImgWH + margin);
        // 行数（row）决定了y
        CGFloat y = oneY + row * (kImgWH + margin);
        //MyLog(@"%f",y);
        [self addImg:imgName x:x y:y];
    }
    

}



#pragma mark 添加表情 icon:表情图片名
- (void)addImg:(NSString *)icon x:(CGFloat)x y:(CGFloat)y
{
    UIImageView *one = [[UIImageView alloc] init];
    one.image = [UIImage imageNamed:icon];
    one.frame = CGRectMake(x, y, kImgWH, kImgWH);
    [_rightView addSubview:one];
}

#pragma mark 添加子视图
-(void)addSubviews
{
    
    //左边视图
    _leftLabel = [[UILabel alloc] init];
    _leftLabel.backgroundColor = [UIColor colorWithRed:0.349 green:0.757 blue:0.855 alpha:1.000];
    _leftLabel.textColor = [UIColor whiteColor];
    _leftLabel.font = kFont(14);
    _leftLabel.textAlignment = NSTextAlignmentCenter;
    [self.contentView addSubview:_leftLabel];
    //右边视图
    _rightView = [[UIView alloc] init];
    _rightView.layer.borderColor = [UIColor colorWithRed:0.349 green:0.757 blue:0.855 alpha:1.000].CGColor;
    _rightView.layer.borderWidth = 1.0f;
    [self.contentView addSubview:_rightView];
    
}

-(void)layoutSubviews
{
    [super layoutSubviews];
    
   

}
- (void)settingSubviewFrame
{
  }

+(NSString *)ID
{
    return @"Cell";
}
+(CGFloat)cellHeight
{
    return 70;
}

@end
