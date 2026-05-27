# HFCheckout iOS SDK 集成指南

## 1. 概述

HFCheckout 是汇付天下收银台 iOS SDK，提供简洁的收银台 UI，支持微信支付、支付宝、花呗分期、云闪付等支付方式。通过 CocoaPods subspec 机制实现按需引入，避免引入不必要的第三方支付 SDK。

### 主要特性

- 按需引入支付 SDK，减小包体积
- 内置收银台 UI，开箱即用
- 自动注册已引入的支付 Handler
- 支持自定义支付方式扩展
- 支持集成环境/生产环境切换
- 支持屏幕适配

### 系统要求

- iOS 12.0+
- Xcode 15.0+
- CocoaPods 1.10+

---

## 2. 安装

### 2.1 按需引入（推荐）

在 `Podfile` 中根据需要选择对应的 subspec：

```ruby
# 仅引入核心收银台（不含任何第三方支付 SDK）
pod 'HFCheckout'

# 引入微信支付
pod 'HFCheckout/WechatPaySDK'

# 引入支付宝（含花呗分期）
pod 'HFCheckout/AlipaySDK'

# 引入云闪付
pod 'HFCheckout/UnionPaySDK'
```

组合引入示例：

```ruby
# 引入支付宝 + 微信支付
pod 'HFCheckout/AlipaySDK'
pod 'HFCheckout/WechatPaySDK'

# 引入全部支付方式
pod 'HFCheckout/All'
```

### 2.2 本地路径引入（开发调试）

```ruby
pod 'HFCheckout/AlipaySDK', :path => '/path/to/hfpx-checkout-ios'
```

### 2.3 安装

```bash
pod install
```

> **注意**：如果遇到 `Sandbox: rsync deny` 错误，在 Podfile 中添加以下 post_install 钩子：
>
> ```ruby
> post_install do |installer|
>   installer.pods_project.targets.each do |target|
>     target.build_configurations.each do |config|
>       config.build_settings['ENABLE_USER_SCRIPT_SANDBOXING'] = 'NO'
>     end
>   end
> end
> ```

---

## 3. 项目配置

### 3.1 Info.plist 配置

#### URL Schemes

在 `Info.plist` 的 `CFBundleURLTypes` 中添加以下 URL Scheme：

```xml
<key>CFBundleURLTypes</key>
<array>
    <!-- 自定义 URL Scheme（用于支付宝/云闪付回调） -->
    <dict>
        <key>CFBundleTypeRole</key>
        <string>Editor</string>
        <key>CFBundleURLName</key>
        <string>com.yourcompany.yourapp</string>
        <key>CFBundleURLSchemes</key>
        <array>
            <string>yourappscheme</string>
        </array>
    </dict>
    <!-- 支付宝 URL Scheme（格式：ap + 支付宝AppId） -->
    <dict>
        <key>CFBundleTypeRole</key>
        <string>Editor</string>
        <key>CFBundleURLName</key>
        <string>com.alipay</string>
        <key>CFBundleURLSchemes</key>
        <array>
            <string>ap2021006136691695</string>
        </array>
    </dict>
</array>
```

#### 白名单配置

在 `Info.plist` 的 `LSApplicationQueriesSchemes` 中添加：

```xml
<key>LSApplicationQueriesSchemes</key>
<array>
    <string>weixin</string>
    <string>weixinULAPI</string>
    <string>alipays</string>
    <string>uppaywallet</string>
</array>
```

#### ATS 配置

```xml
<key>NSAppTransportSecurity</key>
<dict>
    <key>NSAllowsArbitraryLoads</key>
    <true/>
</dict>
```

### 3.2 URL 回调处理

#### iOS 13+（SceneDelegate 方式）

在 `SceneDelegate.m` 中处理 URL 回调：

```objc
#import <HFCheckout/HFCCheckout.h>

- (void)scene:(UIScene *)scene openURLContexts:(NSSet<UIOpenURLContext *> *)URLContexts {
    for (UIOpenURLContext *context in URLContexts) {
        [HFCCheckout handleOpenURL:context.URL];
    }
}
```

同时在 `AppDelegate.m` 中保留兼容处理：

```objc
#import <HFCheckout/HFCCheckout.h>

- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
    return [HFCCheckout handleOpenURL:url];
}
```

#### iOS 12 及以下（AppDelegate 方式）

```objc
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    return [HFCCheckout handleOpenURL:url];
}
```

---

## 4. SDK 初始化

在 `AppDelegate.m` 的 `didFinishLaunchingWithOptions:` 中初始化：

```objc
#import <HFCheckout/HFCCheckout.h>

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // 1. 开启日志（调试阶段建议开启，上线前关闭）
    [HFCCheckout setLogsEnabled:YES];

    // 2. 配置环境（默认集成环境，生产环境需切换）
    [HFCCheckout switchToIntegrationEnvironment];
    // 或
    // [HFCCheckout switchToProductionEnvironment];

    // 3. 配置支付宝 URL Scheme（必须，与 Info.plist 中的 URL Scheme 一致）
    [HFCCheckout configureAlipayScheme:@"yourappscheme"];

    // 4. 配置云闪付 URL Scheme（必须，与 Info.plist 中的 URL Scheme 一致）
    [HFCCheckout configureUnionPayScheme:@"yourappscheme"];

    // 5. 配置微信支付（仅引入 WechatPaySDK 时需要）
    [HFCCheckout configureWeChatAppId:@"wxYourAppId" universalLink:@"https://yourdomain.com/"];

    return YES;
}
```

### 配置说明

| 配置项 | 必选/可选 | 说明 |
|--------|----------|------|
| `setLogsEnabled:` | 可选 | 调试阶段建议开启，上线前关闭 |
| `switchTo*Environment` | 可选 | 默认集成环境，生产环境需手动切换 |
| `configureAlipayScheme:` | 引入 AlipaySDK 时必选 | 与 Info.plist 中配置的 URL Scheme 一致 |
| `configureUnionPayScheme:` | 引入 UnionPaySDK 时必选 | 与 Info.plist 中配置的 URL Scheme 一致 |
| `configureWeChatAppId:universalLink:` | 引入 WechatPaySDK 时必选 | 微信开放平台获取的 AppId 和 Universal Link |

---

## 5. 发起支付

### 5.1 基本用法

```objc
#import <HFCheckout/HFCCheckout.h>

[HFCCheckout startPaymentWithTokenId:@"your_token_id"
                  fromViewController:self
                           onSuccess:^(NSDictionary * _Nullable result) {
    // 支付成功
    NSLog(@"支付成功: %@", result);
    // result 示例: { "order_stat": "S", "message": "支付成功" }
}
                             onError:^(NSString *code, NSString *message) {
    // 支付错误
    NSLog(@"支付错误 [%@]: %@", code, message);
}
                            onCancel:^{
    // 用户取消支付
    NSLog(@"用户取消支付");
}];
```

### 5.2 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| `tokenId` | NSString | 预下单接口返回的 tokenId，**必传** |
| `viewController` | UIViewController | 用于 present 收银台的控制器，传 nil 则自动获取当前显示的 ViewController |
| `onSuccess` | Block | 支付成功回调，result 包含订单状态等信息 |
| `onError` | Block | 支付错误回调，code 为错误码，message 为错误描述 |
| `onCancel` | Block | 用户取消支付回调 |

### 5.3 自动获取 ViewController

`fromViewController` 传 nil 时，SDK 会自动获取当前最顶层的 ViewController 来 present 收银台：

```objc
[HFCCheckout startPaymentWithTokenId:@"your_token_id"
                  fromViewController:nil
                           onSuccess:nil
                             onError:nil
                            onCancel:nil];
```

---

## 6. 环境配置

### 6.1 环境切换

```objc
// 切换到集成环境（测试环境）
[HFCCheckout switchToIntegrationEnvironment];

// 切换到生产环境
[HFCCheckout switchToProductionEnvironment];

// 通过枚举切换
[HFCCheckout setEnvironment:HFCCheckoutEnvironmentProduction];
```

### 6.2 环境查询

```objc
// 获取当前环境类型
HFCCheckoutEnvironment env = [HFCCheckout currentEnvironment];

// 获取当前环境名称
NSString *envName = [HFCCheckout currentEnvironmentName];

// 判断是否为生产环境
BOOL isProd = [HFCCheckout isProductionEnvironment];

// 判断是否为集成环境
BOOL isIntg = [HFCCheckout isIntegrationEnvironment];

// 获取当前 API 域名（调试用）
NSString *baseURL = [HFCCheckout currentBaseURL];
```

---

## 7. 日志配置

### 7.1 基本配置

```objc
// 开启/关闭日志
[HFCCheckout setLogsEnabled:YES];

// 设置日志级别
HFCLogger.logLevel = HFCLogLevelDebug;  // 输出所有日志
HFCLogger.logLevel = HFCLogLevelInfo;   // 输出信息和错误
HFCLogger.logLevel = HFCLogLevelError;  // 只输出错误

// 快捷方法
[HFCLogger enableAllLogs];   // 启用所有日志
[HFCLogger disableAllLogs];  // 禁用所有日志
```

### 7.2 自定义日志回调

```objc
HFCLogger.logCallback = ^(NSString *message) {
    // 将日志输出到自定义位置（如 UI 界面、文件等）
    NSLog(@"[SDK Log] %@", message);
};
```

---

## 8. 屏幕适配

```objc
// 启用屏幕适配（根据屏幕宽度等比缩放 UI 元素）
[HFCCheckout setScreenAdaptationEnabled:YES];

// 自定义设计稿基准尺寸（默认 375x812）
[HFCCheckout setScreenAdaptationBaseSize:375 height:812];

// 查询是否启用
BOOL enabled = [HFCCheckout isScreenAdaptationEnabled];
```

---

## 9. 支付方式管理

### 9.1 查询可用支付方式

```objc
NSArray<id<HFCPaymentHandler>> *handlers = [HFCCheckout availablePaymentHandlers];
for (id<HFCPaymentHandler> handler in handlers) {
    NSLog(@"支付方式: %@, 类型: %ld, 可用: %@",
          handler.paymentName,
          (long)handler.paymentType,
          handler.isAvailable ? @"YES" : @"NO");
}
```

### 9.2 限制显示的支付方式

```objc
// 只显示微信支付和支付宝
[HFCCheckout setEnabledPaymentTypes:@[@(HFCPaymentTypeWeChat), @(HFCPaymentTypeAlipay)]];
```

### 9.3 支付类型枚举

| 枚举值 | 值 | 说明 |
|--------|-----|------|
| `HFCPaymentTypeWeChat` | 1 | 微信支付 |
| `HFCPaymentTypeAlipay` | 2 | 支付宝 |
| `HFCPaymentTypeUnionPay` | 3 | 云闪付（银联） |
| `HFCPaymentTypeAliFenqi` | 4 | 花呗分期 |

---

## 10. 错误码

| 错误码 | 常量 | 说明 |
|--------|------|------|
| -1 | `kHFCPaymentErrorCodeNetwork` | 网络错误 |
| -2 | `kHFCPaymentErrorCodeParse` | 解析错误 |
| -3 | `kHFCPaymentErrorCodeInvalidParam` | 参数错误 |
| -4 | `kHFCPaymentErrorCodeUnsupported` | 不支持的配置 |
| -5 | `kHFCPaymentErrorCodeExecution` | 支付执行异常 |
| -6 | `kHFCPaymentErrorCodePayTypeUnsupported` | 支付方式暂不支持 |
| -7 | `kHFCPaymentErrorCodeTimeout` | 支付超时 |
| -8 | `kHFCPaymentErrorCodeSession` | 会话异常 |
| -10 | `kHFCPaymentErrorCodeProviderWxPayNotAvailable` | 微信组件不可用 |
| -11 | `kHFCPaymentErrorCodeProviderWxPayError` | 微信支付异常 |
| -12 | `kHFCPaymentErrorCodeDirectNetworkError` | 网络请求异常 |
| -20 | `kHFCPaymentErrorCodeDirectPaymentError` | 支付处理异常 |
| -30 | `kHFCPaymentErrorCodeProviderAlipayNotAvailable` | 支付宝组件不可用 |
| -31 | `kHFCPaymentErrorCodeProviderAlipayError` | 支付宝支付异常 |
| -50 | `kHFCPaymentErrorCodeProviderIndirectNotAvailable` | 云闪付组件不可用 |
| -51 | `kHFCPaymentErrorCodeProviderIndirectError` | 云闪付支付异常 |

---

## 11. 自定义支付方式扩展

SDK 支持通过实现 `HFCPaymentHandler` 协议来自定义支付方式：

```objc
@interface MyCustomPaymentHandler : NSObject <HFCPaymentHandler>
@end

@implementation MyCustomPaymentHandler

- (HFCPaymentType)paymentType {
    return (HFCPaymentType)100; // 自定义类型
}

- (BOOL)isAvailable {
    return YES;
}

- (NSString *)paymentName {
    return @"自定义支付";
}

- (NSString *)iconName {
    return @"custom_pay_icon";
}

- (BOOL)startPaymentWithData:(NSString *)paymentData completion:(HFCPaymentCompletionBlock)completion {
    // 实现支付逻辑
    // 支付成功时：
    // completion(HFCPaymentResultStatusSuccess, @"支付成功", nil);
    // 支付失败时：
    // completion(HFCPaymentResultStatusFail, @"支付失败", error);
    return YES;
}

- (BOOL)handleOpenURL:(NSURL *)url {
    // 处理 URL 回调（可选）
    return NO;
}

@end
```

注册自定义 Handler：

```objc
MyCustomPaymentHandler *handler = [[MyCustomPaymentHandler alloc] init];
[HFCCheckout registerPaymentHandler:handler];
```

移除 Handler：

```objc
[HFCCheckout unregisterPaymentHandlerForType:(HFCPaymentType)100];
```

---

## 12. Subspec 与支付方式对应关系

| Subspec | 包含的支付方式 | 额外依赖 |
|---------|---------------|---------|
| `HFCheckout` (Core) | 无（仅核心收银台） | GMObjC, OpenSSL |
| `HFCheckout/WechatPaySDK` | 微信支付 | WechatOpenSDK-XCFramework |
| `HFCheckout/AlipaySDK` | 支付宝、花呗分期 | AlipaySDK-iOS |
| `HFCheckout/UnionPaySDK` | 云闪付 | UPPaymentControlMini |
| `HFCheckout/All` | 微信支付、支付宝、花呗分期、云闪付 | 以上全部 |

> **注意**：只引入 `HFCheckout/AlipaySDK` 时，收银台只会显示支付宝和花呗分期，不会显示微信支付和云闪付。

---

## 13. 完整接入示例

### Podfile

```ruby
use_frameworks!

platform :ios, '12.0'

target 'YourApp' do
  pod 'HFCheckout/AlipaySDK'
  pod 'HFCheckout/WechatPaySDK'
end

post_install do |installer|
  installer.pods_project.targets.each do |target|
    target.build_configurations.each do |config|
      config.build_settings['ENABLE_USER_SCRIPT_SANDBOXING'] = 'NO'
    end
  end
end
```

### AppDelegate.m

```objc
#import <HFCheckout/HFCCheckout.h>

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [HFCCheckout setLogsEnabled:YES];
    [HFCCheckout switchToProductionEnvironment];
    [HFCCheckout configureAlipayScheme:@"yourappscheme"];
    [HFCCheckout configureUnionPayScheme:@"yourappscheme"];
    [HFCCheckout configureWeChatAppId:@"wxYourAppId" universalLink:@"https://yourdomain.com/"];
    return YES;
}

- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
    return [HFCCheckout handleOpenURL:url];
}
```

### SceneDelegate.m（iOS 13+）

```objc
#import <HFCheckout/HFCCheckout.h>

- (void)scene:(UIScene *)scene openURLContexts:(NSSet<UIOpenURLContext *> *)URLContexts {
    for (UIOpenURLContext *context in URLContexts) {
        [HFCCheckout handleOpenURL:context.URL];
    }
}
```

### 发起支付

```objc
#import <HFCheckout/HFCCheckout.h>

- (void)payWithTokenId:(NSString *)tokenId {
    [HFCCheckout startPaymentWithTokenId:tokenId
                      fromViewController:self
                               onSuccess:^(NSDictionary * _Nullable result) {
        NSLog(@"支付成功: %@", result);
    }
                                 onError:^(NSString *code, NSString *message) {
        NSLog(@"支付错误 [%@]: %@", code, message);
    }
                                onCancel:^{
        NSLog(@"用户取消支付");
    }];
}
```

---

## 14. 常见问题

### Q: 运行报错 `Sandbox: rsync deny`

Xcode 15+ 默认开启了 User Script Sandboxing，在 Podfile 的 `post_install` 中关闭即可，详见 [2.3 安装](#23-安装)。

### Q: 启动应用黑屏/启动极慢

这是 LLDB 调试器的共享缓存问题，与 SDK 无关。解决方案：
1. 重启 iOS 设备
2. 清理 DerivedData：`rm -rf ~/Library/Developer/Xcode/DerivedData`
3. 临时验证：Product → Scheme → Edit Scheme → Run → Options → 取消勾选 "Debug executable"

### Q: 收银台显示了未引入的支付方式

确保使用最新版本的 xcframework。SDK 会在注册 Handler 时检查 `isAvailable`，未引入对应第三方 SDK 的支付方式不会被注册和显示。

### Q: 支付宝/微信回调无响应

1. 确认 `Info.plist` 中配置了正确的 URL Scheme
2. 确认 `AppDelegate` 和 `SceneDelegate` 中正确调用了 `[HFCCheckout handleOpenURL:]`
3. 确认 `configureAlipayScheme:` 和 `configureWeChatAppId:` 传入的参数正确

### Q: 只引入了 AlipaySDK，收银台还显示微信支付

确保使用最新编译的 xcframework。新版 SDK 在 `registerBuiltinHandlers` 中增加了 `isAvailable` 检查，未引入对应 SDK 的 Handler 不会被注册。
