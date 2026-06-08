Pod::Spec.new do |s|
  s.name             = 'HFCheckout'
  s.version          = '1.0.2'
  s.summary          = 'HFCheckout'
  s.description      = <<-DESC
HFCheckout SDK
                       DESC

  s.homepage         = 'https://github.com/huifurepo/hfpx-checkout-ios'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'zhangtibin' => 'tibin.zhang@huifu.com' }
  s.source           = { :git => 'https://github.com/huifurepo/hfpx-checkout-ios.git', :tag => s.version.to_s }

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
    wechat.pod_target_xcconfig = { 'OTHER_LDFLAGS' => '-ObjC' }
  end

  s.subspec 'AlipaySDK' do |alipay|
    alipay.dependency 'HFCheckout/Core'
    alipay.dependency 'AlipaySDK-iOS'
    alipay.pod_target_xcconfig = { 'OTHER_LDFLAGS' => '-ObjC' }
  end

  s.subspec 'UnionPaySDK' do |unionpay|
    unionpay.dependency 'HFCheckout/Core'
    unionpay.vendored_frameworks = 'HFCheckout/Frameworks/UPPaymentControl/UPPaymentControlMini.framework'
    unionpay.vendored_libraries = 'HFCheckout/Frameworks/UMSPosPayOnly/libUMSPosPayOnly.a'
    unionpay.pod_target_xcconfig = { 'OTHER_LDFLAGS' => '-ObjC' }
  end

  s.subspec 'All' do |all|
    all.dependency 'HFCheckout/WechatPaySDK'
    all.dependency 'HFCheckout/AlipaySDK'
    all.dependency 'HFCheckout/UnionPaySDK'
  end
end
