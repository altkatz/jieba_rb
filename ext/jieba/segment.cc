#include "segment.h"
#include <ruby/encoding.h>
#include <MPSegment.hpp>
#include <HMMSegment.hpp>
#include <MixSegment.hpp>

static rb_encoding* u8_enc;
VALUE cSegment;
struct SegWrapper{
    CppJieba::ISegment * segp;
};
static void seg_free(void *p){
    delete ((SegWrapper*) p) -> segp;
    delete (SegWrapper*)p;
}

static void seg_init(VALUE self,
                     VALUE type_rb_sym,
                     VALUE jieba_dict_rbs,
                     VALUE hmm_dict_rbs,
                     VALUE user_dict_rbs)
{
    SegWrapper* seg_wrapper = new SegWrapper();
    VALUE ret_instance;
    ret_instance = Data_Wrap_Struct(cSegment, NULL, seg_free, seg_wrapper);
    rb_iv_set(self, "@_wrapper", ret_instance);
    Check_Type(jieba_dict_rbs, T_STRING);
    Check_Type(hmm_dict_rbs, T_STRING);
    Check_Type(user_dict_rbs, T_STRING);

    std::string jieba_dict = StringValueCStr(jieba_dict_rbs);
    std::string hmm_dict = StringValueCStr(hmm_dict_rbs);
    std::string user_dict = StringValueCStr(user_dict_rbs);

    ID type = SYM2ID(type_rb_sym);
    if ( type == rb_intern("mix") )
    {
        seg_wrapper->segp = new CppJieba::MixSegment(jieba_dict, hmm_dict, user_dict);
    }
    else if ( type == rb_intern("hmm") )
    {
        seg_wrapper->segp = new CppJieba::HMMSegment(hmm_dict);
    }
    else if ( type == rb_intern("mp"))
    {
        seg_wrapper->segp = new CppJieba::MPSegment(jieba_dict);
    }
}

static VALUE seg_cut(VALUE self, VALUE text_rbs)
{
    Check_Type(text_rbs, T_STRING);
    std::string text = StringValueCStr(text_rbs);
    self = rb_iv_get(self, "@_wrapper");
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
        cSegment = rb_define_class_under(mJieba, "Segment", rb_cObject);
        u8_enc = rb_utf8_encoding();
        DEF(cSegment, "_init",seg_init,4);
        DEF(cSegment, "cut",seg_cut,1);
    }

}
