#include "segment.h"
#include <ruby/encoding.h>
#include <KeywordExtractor.hpp>

static rb_encoding* u8_enc;

struct Keyword{
    CppJieba::KeywordExtractor * p;
};

static void keyword_free(void *p){
    delete ((Keyword*) p) -> p;
    delete (Keyword*)p;
}

static VALUE allocate(VALUE klass)
{
    Keyword * keyword = new Keyword();
    return Data_Wrap_Struct(klass, NULL, keyword_free, keyword);
}

static void init(VALUE self,
                  VALUE mode_rb_sym,
                  VALUE jieba_dict_rbs,
                  VALUE hmm_dict_rbs,
                  VALUE idf_rbs,
                  VALUE stop_words_rbs,
                  VALUE user_dict_rbs)
{
    Keyword * keyword;
    Data_Get_Struct(self, Keyword, keyword);

    Check_Type(jieba_dict_rbs, T_STRING);
    Check_Type(hmm_dict_rbs, T_STRING);
    Check_Type(user_dict_rbs, T_STRING);
    Check_Type(idf_rbs, T_STRING);
    Check_Type(stop_words_rbs, T_STRING);

    std::string jieba_dict = StringValueCStr(jieba_dict_rbs);
    std::string hmm_dict = StringValueCStr(hmm_dict_rbs);
    std::string idf = StringValueCStr(idf_rbs);
    std::string stop_words = StringValueCStr(stop_words_rbs);
    std::string user_dict = StringValueCStr(user_dict_rbs);

    ID mode = SYM2ID(mode_rb_sym);
    if ( mode == rb_intern("tf_idf") )
    {
        keyword->p = new CppJieba::KeywordExtractor(jieba_dict, hmm_dict, idf, stop_words);
    }
}

static VALUE extract(VALUE self, VALUE text_rbs, VALUE topN)
{
    Check_Type(text_rbs, T_STRING);
    std::string text = StringValueCStr(text_rbs);

    Check_Type(topN, T_FIXNUM);
    int top_n = NUM2INT(topN);

    Keyword * keyword;
    Data_Get_Struct(self, Keyword, keyword);

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
        VALUE cKeyword = rb_define_class_under(mJieba, "Keyword", rb_cObject);
        u8_enc = rb_utf8_encoding();
        rb_define_alloc_func(cKeyword, allocate);
        DEF(cKeyword, "_init", init, 6);
        DEF(cKeyword, "extract",extract,2);
    }

}
