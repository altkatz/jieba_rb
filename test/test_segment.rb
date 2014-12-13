# coding: utf-8
require 'minitest/autorun'
require 'jieba_rb'
class JiebaTest < Minitest::Test
  def test_mix_segment
    seg = JiebaRb::Segment.new
    words = seg.cut "我来到南京市长江大桥"
    assert_equal %w(我 来到 南京市 长江大桥), words
  end
end
