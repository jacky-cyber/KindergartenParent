//
//  ChatMessageCell.h
//  企信通
//
//  Created by apple on 13-12-4.
//  Copyright (c) 2013年 itcast. All rights reserved.
//

#import <UIKit/UIKit.h>
@class XMPPMessageArchiving_Message_CoreDataObject;
@interface ChatMessageCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UIImageView *headImageView;

@property (weak, nonatomic) IBOutlet UIButton *messageButton;
@property (weak, nonatomic) IBOutlet UILabel *soundTimeLabel;

@property (weak, nonatomic) IBOutlet NSLayoutConstraint *messageHeightConstraint;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *messageWeightConstraint;
@property (weak, nonatomic) IBOutlet UILabel *timestamp;

- (void)setMessage:(XMPPMessageArchiving_Message_CoreDataObject *)message isOutgoing:(BOOL)isOutgoing isImageTag:(NSInteger)tag;

@end
