require "jieba_rb/version"
require "jieba"
module JiebaRb
  class Segment
    abs = File.expand_path File.dirname(__FILE__)
    EXT_BASE = "#{abs}/../ext/cppjieba/"
    JIEBA_DICT_FILE = EXT_BASE + "dict/jieba.dict.utf8";
    HMM_DICT_FILE = EXT_BASE + "dict/hmm_model.utf8";
    USER_DICT_FILE = EXT_BASE + "dict/user.dict.utf8";


#    private :_hmm_init
 #   private :_mp_init
    private :_mix_init
    def initialize opts = {}
      type = opts[:type] || "mix"
      jieba_dict = opts[:jieba_dict] || JIEBA_DICT_FILE
      hmm_dict = opts[:hmm_dict] || HMM_DICT_FILE
      user_dict = opts[:user_dict]
      #USER_DICT_FILE
      self.send ("_#{type}_init"), jieba_dict, hmm_dict#, user_dict
    end
  end
end
