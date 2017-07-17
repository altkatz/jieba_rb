# coding: utf-8
require 'minitest/autorun'
require 'jieba_rb'

class JiebaTest < Minitest::Test
  def test_keywords
    results = JiebaRb.extract_keyword "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。", 5

    assert_equal [["CEO",
                   11.739204307083542],
                  ["升职", 10.8561552143],
                  ["加薪", 10.642581114],
                  ["手扶拖拉机", 10.0088573539],
                  ["巅峰", 9.49395840471]], results

  end
end
