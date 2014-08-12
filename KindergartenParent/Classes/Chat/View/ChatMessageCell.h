

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
