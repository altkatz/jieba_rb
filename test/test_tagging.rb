# coding: utf-8
require 'minitest/autorun'
require 'jieba_rb'
class JiebaTest < Minitest::Test
  def test_tagging
    tagging = JiebaRb::Tagging.new
    pairs = tagging.tag "我是蓝翔技工拖拉机学院手扶拖拉机专业的。"
    assert_equal [{"我"=>"r"}, {"是"=>"v"}, {"蓝翔"=>"x"}, {"技工"=>"n"}, {"拖拉机"=>"n"}, {"学院"=>"n"}, {"手扶拖拉机"=>"n"}, {"专业"=>"n"}, {"的"=>"uj"}, {"。"=>"x"}], pairs
  end

  def test_tagging_with_user_dict
    tagging = JiebaRb::Tagging.new user_dict: :default
    pairs = tagging.tag "我是蓝翔技工拖拉机学院手扶拖拉机专业的。"
    assert_equal [{"我"=>"r"}, {"是"=>"v"}, {"蓝翔"=>"nz"}, {"技工"=>"n"}, {"拖拉机"=>"n"}, {"学院"=>"n"}, {"手扶拖拉机"=>"n"}, {"专业"=>"n"}, {"的"=>"uj"}, {"。"=>"x"}], pairs
  end

end
