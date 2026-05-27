//
//  HFCAliFenqiPaymentHandler.h
//  HFCheckout
//
//  Created by 张体宾 on 2026/3/6.
//  内置花呗分期支付Handler - SDK已集成支付宝SDK
//

#import <Foundation/Foundation.h>
#import <HFCheckout/HFCPaymentHandler.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 内置花呗分期支付Handler
 *
 * SDK已集成支付宝SDK，花呗分期通过支付宝SDK发起。
 * 支付回调通过URL Scheme处理。
 */
@interface HFCAliFenqiPaymentHandler : NSObject <HFCAliFenqiHandler>

@end

NS_ASSUME_NONNULL_END
