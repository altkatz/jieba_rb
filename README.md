# JiebaRb

[![Gem Version](https://badge.fury.io/rb/jieba_rb.svg)](http://badge.fury.io/rb/jieba_rb)

[![Build Status](https://travis-ci.org/altkatz/jieba_rb.png?branch=master)](https://travis-ci.org/altkatz/jieba_rb)


Ruby extension for [Cppjieba](https://github.com/aszxqw/cppjieba)

## Installation

Add this line to your application's Gemfile:

    gem 'jieba_rb'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install jieba_rb

## Word segment Usage

Mix Segment mode (HMM with Max Prob, default):

	require 'jieba_rb'
    seg = JiebaRb::Segment.new  # equivalent to "JiebaRb::Segment.new mode: :mix"
    words = seg.cut "令狐冲是云计算行业的专家"
    # 令狐冲 是 云 计算 行业 的 专家

Mix Segment mode with user-defined dictionary:

    seg = JiebaRb::Segment.new mode: :mix, user_dict: "ext/cppjieba/dict/user.dict.utf8"
    words = seg.cut "令狐冲是云计算行业的专家"
    # 令狐冲 是 云计算 行业 的 专家

HMM or Max probability (mp) Segment mode:

    seg = JiebaRb::Segment.new mode: :hmm # or  mode: :mp
    words = seg.cut "令狐冲是云计算行业的专家"

## Word tagging Usage

Default tagging:

	require 'jieba_rb'
    tagging = JiebaRb::Tagging.new
    pairs = tagging.tag "我是蓝翔技工拖拉机学院手扶拖拉机专业的。"
    # [{"我"=>"r"}, {"是"=>"v"}, {"蓝翔"=>"x"}, {"技工"=>"n"}, {"拖拉机"=>"n"}, {"学院"=>"n"}, {"手扶拖拉机"=>"n"}, {"专业"=>"n"}, {"的"=>"uj"}, {"。"=>"x"}]

Tagging with user-defined dictionary:

	require 'jieba_rb'
    tagging = JiebaRb::Tagging.new user_dict: :default
    pairs = tagging.tag "我是蓝翔技工拖拉机学院手扶拖拉机专业的。"
    # [{"我"=>"r"}, {"是"=>"v"}, {"蓝翔"=>"nz"}, {"技工"=>"n"}, {"拖拉机"=>"n"}, {"学院"=>"n"}, {"手扶拖拉机"=>"n"}, {"专业"=>"n"}, {"的"=>"uj"}, {"。"=>"x"}]

## Keyword Extractor Usage

* only support TF-IDF currently

```
    keyword = JiebaRb::Keyword.new
    keywords_weights = keyword.extract "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。", 5

                  [
                  ["CEO", 11.739204307083542],
                  ["升职", 10.8561552143],
                  ["加薪", 10.642581114],
                  ["手扶拖拉机", 10.0088573539],
                  ["巅峰", 9.49395840471]
                  ]
```



## Contributing

1. Fork it ( http://github.com/<my-github-username>/jieba_rb/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
