#include <segment.h>
#include <ruby/encoding.h>
#include <MPSegment.hpp>
#include <HMMSegment.hpp>
#include <MixSegment.hpp>

// for rubinius
#ifndef rb_enc_fast_mbclen
#   define rb_enc_fast_mbclen rb_enc_mbclen
#endif

static rb_encoding* u8_enc;
static rb_encoding* bin_enc;
static inline VALUE unify_key(VALUE key) {
    rb_encoding* enc = rb_enc_get(key);
    if (enc != u8_enc && enc != bin_enc) {
        return rb_funcall(key, rb_intern("encode"), 1, rb_enc_from_encoding(u8_enc));
    } else {
        return key;
    }
}

struct SegWrapper{
    CppJieba::ISegment * segp;
};
static void seg_free(void *p){
    delete ((SegWrapper*) p) -> segp;
    delete (SegWrapper*)p;
}

static VALUE allocate(VALUE klass)
{
    SegWrapper* seg_wrapper = new SegWrapper();
    return Data_Wrap_Struct(klass, NULL, seg_free, seg_wrapper);
}

static void mix_init(VALUE self, VALUE jieba_dict_rbs, VALUE hmm_dict_rbs){
    SegWrapper* seg_wrapper;
    Data_Get_Struct(self, SegWrapper, seg_wrapper);
    Check_Type(jieba_dict_rbs, T_STRING);
    Check_Type(hmm_dict_rbs, T_STRING);
    std::string jieba_dict = StringValueCStr(jieba_dict_rbs);
    std::string hmm_dict = StringValueCStr(hmm_dict_rbs);
    seg_wrapper->segp = new CppJieba::MixSegment(jieba_dict, hmm_dict);

}
static VALUE seg_cut(VALUE self, VALUE text_rbs)
{
    Check_Type(text_rbs, T_STRING);
    std::string text = StringValueCStr(text_rbs);

    SegWrapper* seg_wrapper;
    Data_Get_Struct(self, SegWrapper, seg_wrapper);

    std::vector<std::string> words;
    seg_wrapper->segp->cut(text, words);

    volatile VALUE arr = rb_ary_new();
    for (std::vector<std::string>::const_iterator j = words.begin(); j != words.end(); j++)
    {

        rb_ary_push(arr, rb_enc_str_new((*j).c_str(), (*j).length(), u8_enc));

    }
    return arr;
}

#define DEF(k,n,f,c) rb_define_method(k,n,RUBY_METHOD_FUNC(f),c)

extern "C" {
    void Init_segment()
    {
        VALUE cSegment = rb_define_class_under(mJieba, "Segment", rb_cObject);
        u8_enc = rb_utf8_encoding();
        bin_enc = rb_ascii8bit_encoding();

        rb_define_alloc_func(cSegment, allocate);
        DEF(cSegment, "_mix_init",mix_init,2);
        DEF(cSegment, "cut",seg_cut,1);
        //DEF(cSegment, "_mix_init_user",mix_init_user,3);

    }

}
