//
//  HFCCheckout.h
//  HFCheckout
//
//  Created by 张体宾 on 2026/2/28.
//  收银台SDK主入口
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <HFCheckout/HFCPaymentHandler.h>

NS_ASSUME_NONNULL_BEGIN

/// 环境类型枚举
typedef NS_ENUM(NSInteger, HFCCheckoutEnvironment) {
    HFCCheckoutEnvironmentIntegration = 0,  // 集成环境（测试环境）
    HFCCheckoutEnvironmentProduction = 1    // 生产环境
};

/**
 * 收银台SDK主类
 *
 * 提供打开收银台、注册支付Handler等方法。
 * SDK不直接集成第三方支付SDK，通过注册Handler的方式解耦。
 */
@interface HFCCheckout : NSObject

/**
 * 打开收银台（新版回调方式）
 * @param tokenId 预下单返回参数tokenId
 * @param viewController 当前ViewController（用于present收银台），传nil则自动获取当前显示的ViewController
 * @param onSuccess 支付成功回调，result包含订单状态等信息
 * @param onError 支付错误回调，code为错误码，message为错误描述
 * @param onCancel 用户取消支付回调
 */
+ (void)startPaymentWithTokenId:(NSString *)tokenId
             fromViewController:(nullable UIViewController *)viewController
                      onSuccess:(nullable HFCPaymentSuccessBlock)onSuccess
                        onError:(nullable HFCPaymentErrorBlock)onError
                      onCancel:(nullable HFCPaymentCancelBlock)onCancel;

/**
 * 注册支付Handler
 * @param handler 支付Handler对象
 * @discussion 调用方自行集成第三方SDK后，需要注册对应的Handler
 *
 * 示例：
 * HFCWeChatPaymentHandlerImpl *wechatHandler = [[HFCWeChatPaymentHandlerImpl alloc] init];
 * [HFCCheckout registerPaymentHandler:wechatHandler];
 */
+ (void)registerPaymentHandler:(id<HFCPaymentHandler>)handler;

/**
 * 注册支付Handler（指定类型）
 * @param handler 支付Handler对象
 * @param type 支付类型
 */
+ (void)registerPaymentHandler:(id<HFCPaymentHandler>)handler forType:(HFCPaymentType)type;

/**
 * 移除已注册的支付Handler
 * @param type 支付类型
 */
+ (void)unregisterPaymentHandlerForType:(HFCPaymentType)type;

/**
 * 处理URL回调
 * @param url 回调URL
 * @return 是否处理了该URL
 * @discussion 需要在AppDelegate的openURL方法中调用
 */
+ (BOOL)handleOpenURL:(NSURL *)url;

/**
 * 获取当前可用的支付方式列表
 * @return 可用的支付Handler数组
 */
+ (NSArray<id<HFCPaymentHandler>> *)availablePaymentHandlers;

/**
 * 设置可用的支付方式
 * @param types 需要显示的支付方式类型数组
 * @discussion 默认显示所有已注册且可用的支付方式，调用此方法可以限制显示的支付方式
 */
+ (void)setEnabledPaymentTypes:(NSArray<NSNumber *> *)types;

/**
 * 配置微信支付AppId
 * @param appId 微信开放平台获取的AppId
 * @discussion 必须在调用收银台前配置，否则微信支付不可用。使用URL Scheme方式回调。
 *
 * 示例：
 * [HFCCheckout configureWeChatAppId:@"wx1234567890abcdef"];
 */
+ (void)configureWeChatAppId:(NSString *)appId;

/**
 * 配置微信支付AppId和UniversalLink
 * @param appId 微信开放平台获取的AppId
 * @param universalLink 微信开发者Universal Link，传nil则使用URL Scheme方式回调
 * @discussion 必须在调用收银台前配置，否则微信支付不可用
 *
 * 示例：
 * [HFCCheckout configureWeChatAppId:@"wx1234567890abcdef" universalLink:@"https://yourdomain.com/"];
 */
+ (void)configureWeChatAppId:(NSString *)appId universalLink:(NSString * _Nullable)universalLink;

/**
 * 配置支付宝URL Scheme
 * @param scheme 支付宝回调的URL Scheme
 * @discussion 默认使用 "hfccheckoutdemo"，如需自定义请在调用收银台前配置
 */
+ (void)configureAlipayScheme:(NSString *)scheme;

/**
 * 配置云闪付URL Scheme
 * @param scheme 云闪付回调的URL Scheme
 * @discussion 默认使用 "hfccheckoutdemo"，如需自定义请在调用收银台前配置
 */
+ (void)configureUnionPayScheme:(NSString *)scheme;

/**
 * 配置日志开关
 * @param enabled 是否开启日志开关
 */
+ (void)setLogsEnabled:(BOOL)enabled;

/**
 * 配置屏幕适配开关
 * @param enabled 是否启用屏幕适配（默认为NO）
 * @discussion 启用后，收银台页面所有UI组件和文字大小将根据当前屏幕宽度相对于设计稿宽度（375）进行等比例缩放
 *
 * 示例：
 * [HFCCheckout setScreenAdaptationEnabled:YES];
 */
+ (void)setScreenAdaptationEnabled:(BOOL)enabled;

/**
 * 获取屏幕适配是否启用
 */
+ (BOOL)isScreenAdaptationEnabled;

/**
 * 配置屏幕适配的设计稿基准尺寸
 * @param width 设计稿基准宽度（默认375）
 * @param height 设计稿基准高度（默认812）
 * @discussion 仅在启用屏幕适配时生效
 */
+ (void)setScreenAdaptationBaseSize:(CGFloat)width height:(CGFloat)height;

#pragma mark - 环境配置

/**
 * 切换SDK环境
 * @param environment 环境类型（集成环境/生产环境）
 * @discussion 切换环境后，API域名、密钥、系统ID、银联支付环境等都会自动切换
 *
 * 示例：
 * [HFCCheckout setEnvironment:HFCCheckoutEnvironmentProduction];
 */
+ (void)setEnvironment:(HFCCheckoutEnvironment)environment;

/**
 * 切换到集成环境（测试环境）
 *
 * 示例：
 * [HFCCheckout switchToIntegrationEnvironment];
 */
+ (void)switchToIntegrationEnvironment;

/**
 * 切换到生产环境
 *
 * 示例：
 * [HFCCheckout switchToProductionEnvironment];
 */
+ (void)switchToProductionEnvironment;

/**
 * 获取当前环境类型
 * @return 当前环境类型
 */
+ (HFCCheckoutEnvironment)currentEnvironment;

/**
 * 获取当前环境名称
 * @return 环境名称字符串
 */
+ (NSString *)currentEnvironmentName;

/**
 * 检查当前是否为生产环境
 * @return YES表示生产环境，NO表示集成环境
 */
+ (BOOL)isProductionEnvironment;

/**
 * 检查当前是否为集成环境
 * @return YES表示集成环境，NO表示生产环境
 */
+ (BOOL)isIntegrationEnvironment;

/**
 * 获取当前环境的API域名
 * @return 当前环境的API基础URL
 * @discussion 仅用于调试和配置查看，不建议在生产环境中使用
 */
+ (nullable NSString *)currentBaseURL;

/**
 * 获取当前环境的系统ID
 * @return 当前环境的系统ID
 * @discussion 仅用于调试和配置查看，不建议在生产环境中使用
 */
+ (nullable NSString *)currentSysId;

/**
 * 获取当前环境的商户密钥预览
 * @return 商户密钥预览字符串（仅包含前12位和后12位）
 * @discussion 仅用于调试和配置查看，不会暴露完整的密钥信息
 */
+ (nullable NSString *)currentMerchantKeyPreview;

/**
 * 设置集成环境的系统ID
 * @param sysId 系统ID
 * @discussion 仅在需要使用自定义系统ID时调用，大多数情况下不需要调用此方法
 */
+ (void)setIntegrationSysId:(NSString *)sysId;

/**
 * 设置生产环境的系统ID
 * @param sysId 系统ID
 * @discussion 仅在需要使用自定义系统ID时调用，大多数情况下不需要调用此方法
 */
+ (void)setProductionSysId:(NSString *)sysId;

@end

NS_ASSUME_NONNULL_END
