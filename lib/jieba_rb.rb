require "jieba_rb/version"
require "jieba"
module JiebaRb
  abs = File.expand_path File.dirname(__FILE__)
  EXT_BASE = "#{abs}/../ext/cppjieba/"
  DEFAULT_JIEBA_DICT = EXT_BASE + "dict/jieba.dict.utf8";
  DEFAULT_HMM_DICT = EXT_BASE + "dict/hmm_model.utf8";
  DEFAULT_USER_DICT = EXT_BASE + "dict/user.dict.utf8";

  class Segment
    private :_init
    def initialize opts = {}
      valid_modes = [:mix, :hmm, :mp]
      if mode = opts[:mode]
        raise "Mode must be one of :mix :hmm :mp" unless valid_modes.include? mode
      else
        mode = :mix #default
      end

      jieba_dict = opts[:jieba_dict] || DEFAULT_JIEBA_DICT
      hmm_dict = opts[:hmm_dict] || DEFAULT_HMM_DICT
      user_dict = opts[:user_dict] || ""
      user_dict = DEFAULT_USER_DICT if user_dict == :default

      _init mode, jieba_dict, hmm_dict, user_dict
    end
  end

  class Keyword
    DEFAULT_IDF = EXT_BASE + "dict/idf.utf8"
    DEFAULT_STOP_WORDS = EXT_BASE + "dict/stop_words.utf8"

    private :_init

    def initialize opts = {}
      valid_modes = [:tf_idf]
      if mode = opts[:mode]
        raise "Mode must be one of :tf_idf" unless valid_modes.include? mode
      else
        mode = :tf_idf #default
      end

      jieba_dict = opts[:jieba_dict] || DEFAULT_JIEBA_DICT
      hmm_dict = opts[:hmm_dict] || DEFAULT_HMM_DICT
      idf_path = opts[:idf] || DEFAULT_IDF
      stop_words_path = opts[:stop_words] || DEFAULT_STOP_WORDS

      user_dict = opts[:user_dict] || ""
      user_dict = DEFAULT_USER_DICT if user_dict == :default

      _init mode, jieba_dict, hmm_dict, idf_path, stop_words_path, user_dict
    end
  end

  class Tagging
    private :_init
    def initialize opts = {}
      jieba_dict = opts[:jieba_dict] || DEFAULT_JIEBA_DICT
      hmm_dict = opts[:hmm_dict] || DEFAULT_HMM_DICT
      user_dict = opts[:user_dict] || ""
      user_dict = DEFAULT_USER_DICT if user_dict == :default

      _init jieba_dict, hmm_dict, user_dict
    end
  end
end
