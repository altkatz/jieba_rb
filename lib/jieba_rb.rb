require "jieba_rb/version"
require "jieba"
module JiebaRb
  class Segment
    abs = File.expand_path File.dirname(__FILE__)
    EXT_BASE = "#{abs}/../ext/cppjieba/"
    JIEBA_DICT_FILE = EXT_BASE + "dict/jieba.dict.utf8";
    HMM_DICT_FILE = EXT_BASE + "dict/hmm_model.utf8";
    USER_DICT_FILE = EXT_BASE + "dict/user.dict.utf8";

    private :_init
    def initialize opts = {}
      valid_seg_types = [:mix, :hmm, :mp]
      if type = opts[:type]
        raise "Type must be one of :mix :hmm :mp" unless valid_seg_types.include? type
      else
        type = :mix #default
      end

      jieba_dict = opts[:jieba_dict] || JIEBA_DICT_FILE
      hmm_dict = opts[:hmm_dict] || HMM_DICT_FILE
      user_dict = opts[:user_dict] || ""
      user_dict = USER_DICT_FILE if user_dict == :default

      _init type, jieba_dict, hmm_dict, user_dict
    end
  end
end
