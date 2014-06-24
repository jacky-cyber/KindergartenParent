//
//  ZJUserInfoCell.m
//  ybparents
//
//  Created by 郑敬 on 14-6-13.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJUserInfoCell.h"
#import "ZJUserInfoModel.h"
@implementation ZJUserInfoCell

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
-(void)addSubviews
{
    
    //1.标题
    _title = [[UILabel alloc] init];
    _title.font = kFont(14);
    _title.backgroundColor = [UIColor colorWithRed:0.800 green:1.000 blue:0.400 alpha:1.000];
    [self.contentView addSubview:_title];
    //2.创建时间
    _property= [[UILabel alloc] init];
    _property.font = kFont(14);
    _property.numberOfLines= 0;
    _property.textAlignment = NSTextAlignmentRight;
    _property.backgroundColor = [UIColor colorWithRed:0.400 green:1.000 blue:0.800 alpha:1.000];
    [self.contentView addSubview:_property];

}

- (void)awakeFromNib
{
    // Initialization code
}

-(void)setModel:(ZJUserInfoModel *)model
{
    _model = model;
    
    MyLog(@"%@",model.name);
    
}
static int row =0;
-(void)layoutSubviews
{
    row +=1;
    NSLog(@"%d",row);
    [super layoutSubviews];
    //NSLog(@"%f %f",(H(self))/2-10,H(self));
    _title.frame = CGRectMake(20, (H(self)-10)/2, 70, 20);
    
   }
- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
