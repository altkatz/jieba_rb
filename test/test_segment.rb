# coding: utf-8
require 'minitest/autorun'
require 'jieba_rb'
class JiebaTest < Minitest::Test
  def test_mix_segment
    seg = JiebaRb::Segment.new
    words = seg.cut "我来到南京市长江大桥"
    assert_equal %w(我 来到 南京市 长江大桥), words

    words = seg.cut "令狐冲是云计算行业的专家"
    assert_equal %w(令狐冲 是 云 计算 行业 的 专家), words
  end

  def test_mix_segment_with_user_dict
    seg = JiebaRb::Segment.new user_dict: :default
    words = seg.cut "令狐冲是云计算行业的专家"
    assert_equal %w(令狐冲 是 云计算 行业 的 专家), words
  end

  def test_hmm_segment
    seg = JiebaRb::Segment.new mode: :hmm
    words = seg.cut "令狐冲是云计算行业的专家"
    assert_equal %w(令狐冲 是 云计算 行业 的 专家), words
  end

  def test_max_prob_segment
    seg = JiebaRb::Segment.new mode: :mp
    words = seg.cut "令狐冲是云计算行业的专家"
    assert_equal %w(令狐冲 是 云 计算 行业 的 专家), words
  end

end
