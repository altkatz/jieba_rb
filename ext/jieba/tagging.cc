#include "tagging.h"
#include <ruby/encoding.h>
#include <PosTagger.hpp>

static rb_encoding* u8_enc;
VALUE cTagging;
struct Tagging{
    CppJieba::PosTagger * p;
};

static void tagger_free(void *p){
    delete ((Tagging*) p) -> p;
    delete (Tagging*)p;
}

static void init(VALUE self,
                 VALUE jieba_dict_rbs,
                 VALUE hmm_dict_rbs,
                 VALUE user_dict_rbs)
{
    Tagging * tagging = new Tagging();
    VALUE ret_instance;
    ret_instance = Data_Wrap_Struct(cTagging, NULL, tagger_free, tagging);
    rb_iv_set(self, "@_wrapper", ret_instance);
    Check_Type(jieba_dict_rbs, T_STRING);
    Check_Type(hmm_dict_rbs, T_STRING);
    Check_Type(user_dict_rbs, T_STRING);

    std::string jieba_dict = StringValueCStr(jieba_dict_rbs);
    std::string hmm_dict = StringValueCStr(hmm_dict_rbs);
    std::string user_dict = StringValueCStr(user_dict_rbs);

    tagging->p = new CppJieba::PosTagger(jieba_dict, hmm_dict, user_dict);
}

static VALUE tag(VALUE self, VALUE text_rbs)
{
    Check_Type(text_rbs, T_STRING);
    std::string text = StringValueCStr(text_rbs);
    self = rb_iv_get(self, "@_wrapper");
    Tagging *tagging;
    Data_Get_Struct(self, Tagging, tagging);

    std::vector<std::pair<std::string, std::string>> pairs;
    tagging->p->tag(text, pairs);

    volatile VALUE arr = rb_ary_new();
    for (std::vector<std::pair<std::string, std::string>>::const_iterator j = pairs.begin(); j != pairs.end(); j++)
    {
        VALUE pair = rb_hash_new();
        rb_hash_aset(pair, rb_enc_str_new(std::get<0>(*j).c_str(), std::get<0>(*j).length(), u8_enc), rb_enc_str_new(std::get<1>(*j).c_str(), std::get<1>(*j).length(), u8_enc));
        rb_ary_push(arr, pair);

    }
    return arr;
}

#define DEF(k,n,f,c) rb_define_method(k,n,RUBY_METHOD_FUNC(f),c)

extern "C" {
    void Init_tagging()
    {
        cTagging = rb_define_class_under(mJieba, "Tagging", rb_cObject);
        u8_enc = rb_utf8_encoding();
        DEF(cTagging, "_init",init,3);
        DEF(cTagging, "tag",tag,1);
    }

}
