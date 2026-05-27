//
//  HFCAlipayPaymentHandler.h
//  HFCheckout
//
//  Created by 张体宾 on 2026/3/2.
//  内置支付宝支付Handler - SDK已集成支付宝SDK
//

#import <Foundation/Foundation.h>
#import <HFCheckout/HFCPaymentHandler.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 内置支付宝支付Handler
 *
 * SDK已集成支付宝SDK，调用方无需自行集成。
 * 支付回调通过URL Scheme处理。
 */
@interface HFCAlipayPaymentHandler : NSObject <HFCAlipayHandler>

@end

NS_ASSUME_NONNULL_END
