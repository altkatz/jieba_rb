#include "segment.h"
#include <ruby/encoding.h>
#include <KeywordExtractor.hpp>

static rb_encoding* u8_enc;
VALUE cKeyword;
struct Keyword{
    CppJieba::KeywordExtractor * p;
};

static void keyword_free(void *p){
    delete ((Keyword*) p) -> p;
    delete (Keyword*)p;
}

static void init(VALUE self,
                 VALUE mode_rb_sym,
                 VALUE jieba_dict_rbs,
                 VALUE hmm_dict_rbs,
                 VALUE idf_rbs,
                 VALUE stop_words_rbs,
                 VALUE user_dict_rbs)
{
    Keyword * keyword = ALLOC(Keyword);
    LogDebug("kw_rb_alloc_done");
    VALUE ret_instance;
    ret_instance = Data_Wrap_Struct(cKeyword, NULL, keyword_free, keyword);
    rb_iv_set(self, "@_wrapper", ret_instance);
    LogDebug("kw_iv_set_done");
    Check_Type(jieba_dict_rbs, T_STRING);
    Check_Type(hmm_dict_rbs, T_STRING);
    Check_Type(user_dict_rbs, T_STRING);
    Check_Type(idf_rbs, T_STRING);
    Check_Type(stop_words_rbs, T_STRING);

    LogDebug("kw_checktype_done");
    std::string jieba_dict = StringValueCStr(jieba_dict_rbs);
    std::string hmm_dict = StringValueCStr(hmm_dict_rbs);
    std::string idf = StringValueCStr(idf_rbs);
    std::string stop_words = StringValueCStr(stop_words_rbs);
    std::string user_dict = StringValueCStr(user_dict_rbs);

    LogDebug("kw_convert_str_done");
    ID mode = SYM2ID(mode_rb_sym);
    if ( mode == rb_intern("tf_idf") )
    {
        keyword->p = new CppJieba::KeywordExtractor(jieba_dict, hmm_dict, idf, stop_words);
    }
    LogDebug("kw_cpp_constructor_done");
}

static VALUE extract(VALUE self, VALUE text_rbs, VALUE topN)
{
    LogDebug("begin_instance");
    Check_Type(text_rbs, T_STRING);
    std::string text = StringValueCStr(text_rbs);

    Check_Type(topN, T_FIXNUM);
    int top_n = NUM2INT(topN);

    self = rb_iv_get(self, "@_wrapper");
    Keyword * keyword;
    Data_Get_Struct(self, Keyword, keyword);

    LogDebug("done_instance_get_struct");
    std::vector<std::pair<std::string, double> > top_words;

    if (keyword->p->extract(text, top_words, top_n))
    {
        volatile VALUE arr = rb_ary_new();
        for(size_t i = 0; i < top_words.size(); i++)
        {
            volatile VALUE inner_arr = rb_ary_new();
            std::string & word = top_words[i].first;
            rb_ary_push(inner_arr, rb_enc_str_new(word.c_str(), word.length(), u8_enc));
            rb_ary_push(inner_arr, rb_float_new(top_words[i].second));

            rb_ary_push(arr, inner_arr);

        }
        LogDebug("done_instance");
        return arr;
    }
    else
    {
        return Qfalse;
    }
}

#define DEF(k,n,f,c) rb_define_method(k,n,RUBY_METHOD_FUNC(f),c)

extern "C" {
    void Init_keyword()
    {
        cKeyword = rb_define_class_under(mJieba, "Keyword", rb_cObject);
        u8_enc = rb_utf8_encoding();
        DEF(cKeyword, "_init", init, 6);
        DEF(cKeyword, "extract",extract,2);
    }

}
