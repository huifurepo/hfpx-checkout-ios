//
//  HFCWeChatPaymentHandler.h
//  HFCheckout
//
//  Created by 张体宾 on 2026/3/2.
//  内置微信支付Handler - SDK已集成微信SDK
//

#import <Foundation/Foundation.h>
#import <HFCheckout/HFCPaymentHandler.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 内置微信支付Handler
 *
 * SDK已集成微信支付SDK，调用方无需自行集成。
 * 只需配置微信AppID即可使用。
 */
@interface HFCWeChatPaymentHandler : NSObject <HFCWeChatPaymentHandler>

/**
 * 初始化微信支付Handler
 * @param appId 微信AppID（在微信开放平台获取）
 */
- (instancetype)initWithAppId:(NSString *)appId;

/// 配置微信AppID（需要在调用收银台前设置）
+ (void)configureWithAppId:(NSString *)appId;

/// 配置微信AppID和UniversalLink（需要在调用收银台前设置）
/// @param appId 微信AppID（在微信开放平台获取）
/// @param universalLink 微信UniversalLink，传nil则使用URL Scheme方式回调
+ (void)configureWithAppId:(NSString *)appId universalLink:(NSString * _Nullable)universalLink;

@end

NS_ASSUME_NONNULL_END
