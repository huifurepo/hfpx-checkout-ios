//
//  HFCPaymentHandler.h
//  HFCheckout
//
//  Created by 张体宾 on 2026/2/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// 支付结果状态
typedef NS_ENUM(NSInteger, HFCPaymentResultStatus) {
    HFCPaymentResultStatusSuccess = 1,     // 支付成功
    HFCPaymentResultStatusCancel = 2,      // 用户取消
    HFCPaymentResultStatusFail = 3,        // 支付失败
    HFCPaymentResultStatusProcessing = 4,  // 订单处理中
    HFCPaymentResultStatusRepeat = 5       // 订单重复
};

/**
 * 支付错误Domain
 *
 * 用于NSError的domain，标识错误来源为HFCheckout SDK
 */
FOUNDATION_EXPORT NSErrorDomain const HFCPaymentErrorDomain;

/**
 * 支付错误码
 *
 * 定义SDK返回给调用方的各类错误码，用于NSError的code
 */
typedef NS_ERROR_ENUM(HFCPaymentErrorDomain, HFCPaymentErrorCode) {
    /** 网络错误 */
    HFCPaymentErrorCodeNetwork = -1,

    /** 解析错误 */
    HFCPaymentErrorCodeParse = -2,

    /** 参数错误 */
    HFCPaymentErrorCodeInvalidParam = -3,

    /** 不支持的配置 */
    HFCPaymentErrorCodeUnsupported = -4,

    /** 支付执行异常 */
    HFCPaymentErrorCodeExecution = -5,

    /** 指定支付方式流程暂不支持 */
    HFCPaymentErrorCodePayTypeUnsupported = -6,

    /** 支付超时 */
    HFCPaymentErrorCodeTimeout = -7,

    /** 会话异常 */
    HFCPaymentErrorCodeSession = -8,

    /** 微信组件不可用 */
    HFCPaymentErrorCodeProviderWxPayNotAvailable = -10,

    /** 微信支付异常 */
    HFCPaymentErrorCodeProviderWxPayError = -11,

    /** 网络请求异常 */
    HFCPaymentErrorCodeDirectNetworkError = -12,

    /** 支付处理异常 */
    HFCPaymentErrorCodeDirectPaymentError = -20,

    /** 支付宝组件不可用 */
    HFCPaymentErrorCodeProviderAlipayNotAvailable = -30,

    /** 支付宝支付异常 */
    HFCPaymentErrorCodeProviderAlipayError = -31,

    /** 支付组件不可用 */
    HFCPaymentErrorCodeProviderIndirectNotAvailable = -50,

    /** 支付异常 */
    HFCPaymentErrorCodeProviderIndirectError = -51
};

/**
 * 错误码名称常量
 * 用于在回调中传递可读的错误码标识
 */
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeNetwork;              // "-1" - ERR_NETWORK
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeParse;                // "-2" - ERR_PARSE
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeInvalidParam;         // "-3" - ERR_INVALID_PARAM
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeUnsupported;          // "-4" - ERR_UNSUPPORTED
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeExecution;            // "-5" - ERR_EXECUTION
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodePayTypeUnsupported;   // "-6" - ERR_PAY_TYPE_UNSUPPORTED
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeTimeout;              // "-7" - ERR_TIMEOUT
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeSession;              // "-8" - ERR_SESSION
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeProviderWxPayNotAvailable; // "-10" - ERR_PROVIDER_WXPAY_NOT_AVAILABLE
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeProviderWxPayError;   // "-11" - ERR_PROVIDER_WXPAY_ERROR
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeDirectNetworkError;   // "-12" - ERR_DIRECT_NETWORK_ERROR
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeDirectPaymentError;   // "-20" - ERR_DIRECT_PAYMENT_ERROR
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeProviderAlipayNotAvailable; // "-30" - ERR_PROVIDER_ALIPAY_NOT_AVAILABLE
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeProviderAlipayError;  // "-31" - ERR_PROVIDER_ALIPAY_ERROR
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeProviderIndirectNotAvailable; // "-50" - ERR_PROVIDER_INDIRECT_NOT_AVAILABLE
FOUNDATION_EXPORT NSString * const kHFCPaymentErrorCodeProviderIndirectError; // "-51" - ERR_PROVIDER_INDIRECT_ERROR

/**
 * NSError的userInfo key
 *
 * 用于从NSError.userInfo中获取额外的错误信息
 */
FOUNDATION_EXPORT NSString * const HFCPaymentErrorDetailsKey;  // 详细信息字典

/**
 * 创建支付错误
 * @param code 错误码
 * @param message 错误描述信息
 * @return NSError对象
 */
NS_INLINE NSError * HFCPaymentErrorMake(HFCPaymentErrorCode code, NSString *message) {
    return [NSError errorWithDomain:HFCPaymentErrorDomain
                               code:code
                           userInfo:@{NSLocalizedDescriptionKey: message ?: @""}];
}

/**
 * 创建带额外信息的支付错误
 * @param code 错误码
 * @param message 错误描述信息
 * @param details 额外信息字典
 * @return NSError对象
 */
NS_INLINE NSError * HFCPaymentErrorMakeWithDetails(HFCPaymentErrorCode code, NSString *message, NSDictionary *details) {
    NSMutableDictionary *userInfo = [NSMutableDictionary dictionaryWithDictionary:@{
        NSLocalizedDescriptionKey: message ?: @""
    }];
    if (details) {
        userInfo[HFCPaymentErrorDetailsKey] = details;
    }
    return [NSError errorWithDomain:HFCPaymentErrorDomain
                               code:code
                           userInfo:[userInfo copy]];
}

/// 支付结果回调
/// @param status 支付状态（成功/失败/取消/处理中）
/// @param message 结果描述信息
/// @param error 错误对象（失败时包含具体错误码和原因，成功时为nil）
typedef void(^HFCPaymentCompletionBlock)(HFCPaymentResultStatus status, NSString * _Nullable message, NSError * _Nullable error);

/**
 * 新的支付回调方式 - 分离成功、错误、取消三种回调
 */

/// 支付成功回调
/// @param result 支付结果数据，包含订单状态等信息
typedef void(^HFCPaymentSuccessBlock)(NSDictionary * _Nullable result);

/// 支付错误回调
/// @param code 错误码（如 "-1", "-2" 等）
/// @param message 错误描述信息
typedef void(^HFCPaymentErrorBlock)(NSString *code, NSString *message);

/// 支付取消回调
typedef void(^HFCPaymentCancelBlock)(void);

/// 支付方式类型
typedef NS_ENUM(NSInteger, HFCPaymentType) {
    HFCPaymentTypeWeChat = 1,    // 微信支付
    HFCPaymentTypeAlipay = 2,    // 支付宝
    HFCPaymentTypeUnionPay = 3,  // 银联支付
    HFCPaymentTypeAliFenqi = 4,  // 花呗分期
};

/**
 * 支付处理器协议
 *
 * 调用方需要实现此协议来处理具体的支付逻辑。
 * SDK内部不直接集成微信、支付宝、银联SDK，而是通过此协议调用。
 */
@protocol HFCPaymentHandler <NSObject>

@required

/// 支付方式类型
@property (nonatomic, assign, readonly) HFCPaymentType paymentType;

/// 是否可用（运行时检测对应SDK是否已集成）
@property (nonatomic, assign, readonly) BOOL isAvailable;

/// 支付方式名称（用于UI显示，如："微信支付"）
@property (nonatomic, copy, readonly) NSString *paymentName;

/// 支付方式图标名称（用于UI显示，可以是本地图片名或URL）
@property (nonatomic, copy, readonly) NSString * iconName;

/**
 * 发起支付
 * @param paymentData 支付数据（具体格式由各支付方式约定，如微信的支付参数JSON字符串）
 * @param completion 支付结果回调
 * @return 是否成功发起支付（返回NO表示参数错误或SDK未集成）
 */
- (BOOL)startPaymentWithData:(NSString *)paymentData completion:(HFCPaymentCompletionBlock)completion;

@optional

/// 处理URL回调（用于处理支付宝/微信等通过URL Scheme的回调）
/// @param url 回调URL
/// @return 是否处理了该URL
- (BOOL)handleOpenURL:(NSURL *)url;

@end

/**
 * 微信支付Handler协议
 *
 * 调用方实现此协议来处理微信支付。
 * SDK会在运行时检测是否有类实现了此协议。
 */
@protocol HFCWeChatPaymentHandler <HFCPaymentHandler>

@end

/**
 * 支付宝支付Handler协议
 *
 * 调用方实现此协议来处理支付宝支付。
 */
@protocol HFCAlipayHandler <HFCPaymentHandler>

@end

/**
 * 银联支付Handler协议
 *
 * 调用方实现此协议来处理银联支付。
 */
@protocol HFCUnionPayHandler <HFCPaymentHandler>

@end

/**
 * 花呗分期支付Handler协议
 *
 * 调用方实现此协议来处理花呗分期支付。
 */
@protocol HFCAliFenqiHandler <HFCPaymentHandler>

@end

NS_ASSUME_NONNULL_END
