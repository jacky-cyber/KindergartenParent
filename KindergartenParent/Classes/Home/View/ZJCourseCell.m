//
//  ZJCourseCell.m
//  KindergartenParent
//
//  Created by define on 14-9-18.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJCourseCell.h"

@implementation ZJCourseCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        UIView *backView = [[UIView alloc] init];
        self.backgroundView = backView;
        [self addSubViews];
    }
    return self;
}

-(void)addSubViews{
    
    if (!_timeLabel) {
        _timeLabel = [[UILabel alloc] initWithFrame:CGRectMake(20, 10, 80, 20)];
        [self addSubview:_timeLabel];
        _timeLabel.font = kFont(14);
    }
    
    if (!_contentLabel) {
        _contentLabel = [[UILabel alloc] initWithFrame:CGRectMake(XW(_timeLabel)+10, Y(_timeLabel), 280-W(_timeLabel)-10, 20)];
        [self addSubview:_contentLabel];
        _contentLabel.backgroundColor = [UIColor clearColor];
        _contentLabel.numberOfLines = 0;
        _contentLabel.font = kFont(14);
    }

}

-(void)setModel:(ZJCourseModel *)model
{
    _model = model;
    _timeLabel.text = _model.timestamp;
    _contentLabel.text = _model.content;
    CGFloat contentH = [_model.content getHeightByWidth:200 font:kFont(14)];
    contentH = contentH<20?20:contentH;
    CGRect frame = _contentLabel.frame;
    frame.size.height = contentH;
    _contentLabel.frame = frame;
  
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

@end
