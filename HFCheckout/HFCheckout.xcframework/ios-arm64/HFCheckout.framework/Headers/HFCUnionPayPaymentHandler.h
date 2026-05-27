//
//  HFCUnionPayPaymentHandler.h
//  HFCheckout
//
//  Created by 张体宾 on 2026/3/2.
//  内置云闪付（银联）支付Handler - SDK已集成银联SDK
//

#import <Foundation/Foundation.h>
#import <HFCheckout/HFCPaymentHandler.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 内置云闪付（银联）支付Handler
 *
 * SDK已集成银联支付SDK，调用方无需自行集成。
 */
@interface HFCUnionPayPaymentHandler : NSObject <HFCUnionPayHandler>

@end

NS_ASSUME_NONNULL_END
