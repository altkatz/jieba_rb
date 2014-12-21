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

Mix Segment (HMM with Max Prob, default):

	require 'jieba_rb'
    seg = JiebaRb::Segment.new type: :mix
    words = seg.cut "令狐冲是云计算行业的专家"
    # 令狐冲 是 云 计算 行业 的 专家

Mix Segment with user-defined dictionary:

    seg = JiebaRb::Segment.new type: :mix, user_dict: "ext/cppjieba/dict/user.dict.utf8"
    words = seg.cut "令狐冲是云计算行业的专家"
    # 令狐冲 是 云计算 行业 的 专家

HMM or Max probability (mp) Segment: 

    seg = JiebaRb::Segment.new type: :hmm # or  type: :mp 
    words = seg.cut "令狐冲是云计算行业的专家"


## Contributing

1. Fork it ( http://github.com/<my-github-username>/jieba_rb/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
