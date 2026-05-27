Pod::Spec.new do |s|
  s.name             = 'HFCheckout'
  s.version          = '1.0.0'
  s.summary          = '汇付天下收银台SDK，支持微信、支付宝、云闪付等支付方式按需引入'
  s.description      = <<-DESC
HFCheckout 是汇付天下收银台SDK，支持通过 CocoaPods subspec 按需引入微信支付、支付宝、云闪付等支付SDK。

主要特性：
- 支持通过 subspec 按需引入支付SDK，避免不必要的依赖
- pod 'HFCheckout' 仅引入核心收银台，不含第三方支付SDK
- pod 'HFCheckout/WechatPaySDK' 引入微信支付
- pod 'HFCheckout/AlipaySDK' 引入支付宝（含花呗分期）
- pod 'HFCheckout/UnionPaySDK' 引入云闪付
- pod 'HFCheckout/All' 聚合引入全部支付SDK
- 简洁的收银台UI
- 自动注册已引入的支付Handler
- 支持自定义支付方式扩展
                       DESC

  s.homepage         = 'https://github.com/huifurepo/hfpx-checkout-ios'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { '张体宾' => 'tibin.zhang@huifu.com' }
  s.source           = { :git => 'git@github.com:huifurepo/hfpx-checkout-ios.git', :tag => s.version.to_s }

  s.ios.deployment_target = '12.0'
  s.requires_arc = true

  s.frameworks = 'UIKit', 'Foundation', 'SystemConfiguration', 'CoreTelephony', 'QuartzCore', 'CoreText', 'CoreGraphics', 'CoreMotion', 'Security', 'WebKit', 'Network'
  s.libraries = 'c++', 'z', 'sqlite3'

  s.default_subspecs = 'Core'

  s.subspec 'Core' do |core|
    core.vendored_frameworks = 'HFCheckout/HFCheckout.xcframework', 'HFCheckout/Frameworks/GMObjC/*.framework'
    core.resource_bundles = {
      'HFCheckout' => ['HFCheckout/Assets/**/*']
    }
  end

  s.subspec 'WechatPaySDK' do |wechat|
    wechat.dependency 'HFCheckout/Core'
    wechat.dependency 'WechatOpenSDK-XCFramework'
    wechat.pod_target_xcconfig = {
      'OTHER_LDFLAGS' => '-ObjC'
    }
  end

  s.subspec 'AlipaySDK' do |alipay|
    alipay.dependency 'HFCheckout/Core'
    alipay.dependency 'AlipaySDK-iOS'
    alipay.pod_target_xcconfig = {
      'OTHER_LDFLAGS' => '-ObjC'
    }
  end

  s.subspec 'UnionPaySDK' do |unionpay|
    unionpay.source_files = 'HFCheckout/Frameworks/UMSPosPayOnly/include/*.h'
    unionpay.public_header_files = 'HFCheckout/Frameworks/UMSPosPayOnly/include/*.h'
    unionpay.dependency 'HFCheckout/Core'
    unionpay.vendored_frameworks = 'HFCheckout/Frameworks/UPPaymentControl/UPPaymentControlMini.framework'
    unionpay.vendored_libraries = 'HFCheckout/Frameworks/UMSPosPayOnly/libUMSPosPayOnly.a'
    unionpay.pod_target_xcconfig = {
      'OTHER_LDFLAGS' => '-ObjC'
    }
  end

  s.subspec 'All' do |all|
    all.dependency 'HFCheckout/WechatPaySDK'
    all.dependency 'HFCheckout/AlipaySDK'
    all.dependency 'HFCheckout/UnionPaySDK'
  end
end
