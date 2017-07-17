module JiebaRb
  class Segment
    VALID_MODES = %i[mix hmm mp query full].freeze

    def initialize(opts = nil)
      opts ||= {}
      unless opts[:mode].nil? || VALID_MODES.include?(opts[:mode])
        raise ArgumentError, "The mode is #{opts[:mode]}. It should be one of :mix :hmm :mp"
      end

      @mode = opts[:mode] || :mix
      @max_word_length = opts[:max_word_length] || 8
      @hmm = opts[:hmm] || true
    end

    def segment(str)
      JiebaRb.internal.segment(str, @mode, @max_word_length, @hmm)
    end
  end
end