//
//  HFCLogger.h
//  HFCheckout
//
//  Created by 张体宾 on 2026/3/4.
//  SDK 日志配置
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// SDK 日志配置类
@interface HFCLogger : NSObject

/// 是否启用日志输出（默认：NO，不输出）
@property (class, nonatomic, assign) BOOL enabled;

/// 日志级别
typedef NS_ENUM(NSInteger, HFCLogLevel) {
    HFCLogLevelNone = 0,      // 不输出
    HFCLogLevelError = 1,     // 只输出错误
    HFCLogLevelInfo = 2,      // 输出信息和错误
    HFCLogLevelDebug = 3,     // 输出所有日志
};

/// 当前日志级别（默认：HFCLogLevelNone）
@property (class, nonatomic, assign) HFCLogLevel logLevel;

/// 日志前缀（默认：[HFCheckout]）
@property (class, nonatomic, copy) NSString *logPrefix;

/// 日志回调（用于将SDK日志输出到外部，如Demo界面）
/// @param message 日志消息
@property (class, nonatomic, copy, nullable) void(^logCallback)(NSString *message);

/// 快捷设置：启用所有日志
+ (void)enableAllLogs;

/// 快捷设置：禁用所有日志
+ (void)disableAllLogs;

/// 输出日志
/// @param level 日志级别
/// @param format 格式化字符串
+ (void)logWithLevel:(HFCLogLevel)level format:(NSString *)format, ... NS_FORMAT_FUNCTION(2, 3);

/// 输出错误日志
/// @param format 格式化字符串
+ (void)logError:(NSString *)format, ... NS_FORMAT_FUNCTION(1, 2);

/// 输出信息日志
/// @param format 格式化字符串
+ (void)logInfo:(NSString *)format, ... NS_FORMAT_FUNCTION(1, 2);

/// 输出调试日志
/// @param format 格式化字符串
+ (void)logDebug:(NSString *)format, ... NS_FORMAT_FUNCTION(1, 2);

/// 输出JSON格式的日志
/// @param title 日志标题
/// @param jsonObject 要格式化的JSON对象
+ (void)logJSONWithTitle:(NSString *)title jsonObject:(id)jsonObject;

@end

NS_ASSUME_NONNULL_END
