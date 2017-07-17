require 'jieba_rb/jieba_rb'
require 'jieba_rb/version'
require 'jieba_rb/segment'

module JiebaRb
  EXT_BASE = File.join(File.dirname(__FILE__), '..', 'ext', 'cppjieba', 'dict')
  DICT_PATH = File.join(EXT_BASE, 'jieba.dict.utf8')
  HMM_DICT_PATH = File.join(EXT_BASE, 'hmm_model.utf8')
  USER_DICT = File.join(EXT_BASE, 'user.dict.utf8')
  IDF_PATH = File.join(EXT_BASE, 'idf.utf8')
  STOP_WORD_PATH = File.join(EXT_BASE, 'stop_words.utf8')

  def self.extract_keyword(str, top_n)
    internal.extract_keyword(str, top_n)
  end

  def self.segment(str, opts = nil)
    JiebaRb::Segment.new(opts).segment(str)
  end

  def self.segment_tag(str)
    internal.segment_tag(str)
  end

  class << self
    def internal
      @backend ||= JiebaRb::Internal.new(DICT_PATH,
                                         HMM_DICT_PATH,
                                         USER_DICT,
                                         IDF_PATH,
                                         STOP_WORD_PATH)
    end
  end
end
