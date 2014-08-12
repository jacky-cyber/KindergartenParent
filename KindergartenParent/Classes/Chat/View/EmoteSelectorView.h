

#import <UIKit/UIKit.h>

@protocol EmoteSelectorViewDelegate <NSObject>

// 选择表情字符串
- (void)emoteSelectorViewSelectEmoteString:(NSString *)emote;
// 删除字符
- (void)emoteSelectorViewRemoveChar;

@end

@interface EmoteSelectorView : UIScrollView

-(void)loadEmojiItem:(int)page;//加载每一页数据
@property (weak, nonatomic) id <EmoteSelectorViewDelegate> delegates;

@end
