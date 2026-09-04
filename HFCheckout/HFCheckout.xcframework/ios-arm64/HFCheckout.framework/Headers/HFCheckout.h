//
//  HFCheckout.h
//  HFCheckout
//
//  Created by 张体宾 on 2026/2/28.
//

#import <Foundation/Foundation.h>

//! Project version number for HFCheckout.
FOUNDATION_EXPORT double HFCheckoutVersionNumber;

//! Project version string for HFCheckout.
FOUNDATION_EXPORT const unsigned char HFCheckoutVersionString[];

// Import public headers for framework
#import <HFCheckout/HFCPaymentHandler.h>
#import <HFCheckout/HFCCheckout.h>
#import <HFCheckout/HFCLogger.h>

// Import optional public payment handlers
#if __has_include(<HFCheckout/HFCWeChatPaymentHandler.h>)
#import <HFCheckout/HFCWeChatPaymentHandler.h>
#endif
#if __has_include(<HFCheckout/HFCAlipayPaymentHandler.h>)
#import <HFCheckout/HFCAlipayPaymentHandler.h>
#endif
#if __has_include(<HFCheckout/HFCUnionPayPaymentHandler.h>)
#import <HFCheckout/HFCUnionPayPaymentHandler.h>
#endif
#if __has_include(<HFCheckout/HFCAliFenqiPaymentHandler.h>)
#import <HFCheckout/HFCAliFenqiPaymentHandler.h>
#endif
