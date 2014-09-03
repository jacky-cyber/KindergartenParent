//
//  ZJCommentCell.m
//  ybparents
//
//  Created by define on 14-6-20.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJCommentCell.h"
#import "ZJCommentModel.h"
#define kMargin 10


@interface ZJCommentCell ()
{
    //头像地址
    UIImageView *_imgView;
    UILabel *_unameLabel;//评论人
    UILabel *_commTime;//评论时间
    UILabel *_contentLabel;//评论内容
   
    
}
@end

@implementation ZJCommentCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
        
        //添加姿势图
        [self addSubViews];
    }
    return self;
}

-(void)addSubViews{
    //1.添加背景图片
    UIView *bgView = [ZJUIMethods creatView:CGRectMake(kMargin,0, 300, 44) bgColor:[UIColor colorWithWhite:0.969 alpha:1.000]];
    [self.contentView addSubview:bgView];
    //头像
    _imgView = [ZJUIMethods creatImageView:CGRectMake(kMargin, kMargin, 40, 40) imageName:@"201277441619594489.jpg"];
    [bgView addSubview:_imgView];
    //发布人
    _unameLabel = [[UILabel alloc] initWithFrame:CGRectMake( XW(_imgView)+kMargin, Y(_imgView),120,20)];
    //_unameLabel.backgroundColor = [UIColor redColor];
    [bgView addSubview:_unameLabel];
    
    //发布时间
    _commTime = [ZJUIMethods creatLabel:nil frame:CGRectMake(XW(_unameLabel), Y(_imgView), 100, 20) font:kFont(10) textColor:nil];
    //_commTime.backgroundColor = [UIColor purpleColor];
    _commTime.textAlignment = NSTextAlignmentRight;
    [bgView addSubview:_commTime];
    //评论内容
    _contentLabel = [ZJUIMethods creatLabel:nil frame:CGRectMake(XW(_imgView)+kMargin, YH(_unameLabel), 230,20) font:kFont(17) textColor:[UIColor grayColor]];
    _contentLabel.numberOfLines = 0;
    [bgView addSubview:_contentLabel];
    
    //重新设置背景
    _bgView = bgView;
    _bgView.backgroundColor = [UIColor clearColor];
//    CGRect bgF = bgView.frame;
//    bgF.size.height = XW(_contentLabel)+10;
//    bgView.frame = bgF;
    
    
    
    
}
-(void)setCmmodel:(ZJCommentModel *)cmmodel
{
    
    _cmmodel = cmmodel;
    
    [_imgView setImageWithURL:[NSURL URLWithString:_cmmodel.cmuimgurl] placeholderImage:[UIImage imageNamed:@"profile"]];
    
    
    //发布人
    //NSString *uname = [NSString stringWithFormat:@"<a href='www.baidu.com'>%@</a> 的家长",_cmmodel.cmnickname];
    _unameLabel.text = _cmmodel.cmnickname;
    //发布时间
    TimeFormatTools *timef = [[TimeFormatTools alloc] init];
    NSString *timeStr = [timef timeToNow:_cmmodel.cmtime];
    _commTime.text = timeStr;
    
    //内容
    NSString *contentStr = _cmmodel.content;
    CGFloat contentH = [contentStr getHeightByWidth:230 font:kFont(17)];
    _contentLabel.text = contentStr;
    //调整高度
    CGRect contentF = _contentLabel.frame;
    contentF.size.height = contentH;
    _contentLabel.frame = contentF;
    
    //重新设置背景
    CGRect bgF = _bgView.frame;
    bgF.size.height = YH(_contentLabel)+10;
    _bgView.frame = bgF;
    
    //判断是否最后一段，不加分割线
    if (_isLast == false) {
        UIView *lineView = [[UIView alloc] initWithFrame:CGRectMake(kMargin, H(_bgView), 280, 0.5)];
        [_bgView addSubview:lineView];
        lineView.backgroundColor = [UIColor lightGrayColor];
    }
    
    
}


- (void)awakeFromNib
{
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}
-(CGFloat)cellH
{
    return 105;
}

+(NSString *)ID
{
    return @"Cell";
}
@end
