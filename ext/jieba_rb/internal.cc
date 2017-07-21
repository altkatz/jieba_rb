#include <ruby.h>
#include <ruby/encoding.h>
#include "cppjieba/Jieba.hpp"

#define GET_CPPJIEBA(_data) jieba_cpp_data* _data; \
                            TypedData_Get_Struct(self, jieba_cpp_data, &jieba_cpp_type, _data)

typedef struct {
    cppjieba::Jieba* jieba;
} jieba_cpp_data;

// make compiler happy
typedef VALUE (ruby_method)(...);

static ID rb_sMp;
static ID rb_sMix;
static ID rb_sHmm;
static ID rb_sQuery;
static ID rb_sFull;
static rb_encoding* u8_enc;
VALUE rb_cJiebaRb_Internal;
extern "C" VALUE rb_mJiebaRb;

static void jieba_cpp_free(void* _this)
{
    jieba_cpp_data* data = static_cast<jieba_cpp_data*>(_this);
    delete data->jieba;
    data->jieba = nullptr;
}

static size_t jieba_cpp_memsize(const void* _)
{
    return sizeof(jieba_cpp_data);
}

static const rb_data_type_t jieba_cpp_type = {
    "jieba/internal",
    {NULL, jieba_cpp_free, jieba_cpp_memsize,},
    0, 0,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

extern "C" {

VALUE internal_alloc(VALUE self)
{
    jieba_cpp_data* data;
    return TypedData_Make_Struct(self, jieba_cpp_data, &jieba_cpp_type, data);
}

VALUE internal_initialize(VALUE self,
                          VALUE dict_path,
                          VALUE model_path,
                          VALUE user_dict_path,
                          VALUE idf_path,
                          VALUE stop_word_path)
{
    GET_CPPJIEBA(data);
    data->jieba = new cppjieba::Jieba(StringValueCStr(dict_path),
                                      StringValueCStr(model_path),
                                      StringValueCStr(user_dict_path),
                                      StringValueCStr(idf_path),
                                      StringValueCStr(stop_word_path));
}

VALUE internal_extract_keyword(VALUE self, VALUE text_rbs, VALUE topN)
{
    std::string text = StringValueCStr(text_rbs);
    int top_n = NUM2INT(topN);
    GET_CPPJIEBA(data);

    std::vector<std::pair<std::string, double> > top_words;

    data->jieba->extractor.Extract(text, top_words, top_n);
    VALUE arr = rb_ary_new2(top_words.size());
    for (auto iter = top_words.begin(); iter != top_words.end(); iter++) {
        VALUE inner_arr = rb_ary_new2(2);
        rb_ary_push(inner_arr, rb_enc_str_new(iter->first.c_str(), iter->first.length(), u8_enc));
        rb_ary_push(inner_arr, rb_float_new(iter->second));
        rb_ary_push(arr, inner_arr);
    }
    return arr;
}

static VALUE internal_segment(VALUE self, VALUE text_rbs, VALUE mode, VALUE max_length_, VALUE hmm_)
{
    std::string text = StringValueCStr(text_rbs);
    size_t max_length = NUM2UINT(max_length_);
    int hmm = (hmm_ == Qtrue ? 1 : 0);
    GET_CPPJIEBA(data);
    ID rb_sMode = SYM2ID(mode);
    std::vector<std::string> words;

    if (rb_sMode == rb_sMp) {
        data->jieba->CutSmall(text, words, max_length);
    } else if (rb_sMode == rb_sMix) {
        data->jieba->Cut(text, words, hmm);
    } else if (rb_sMode == rb_sHmm) {
        data->jieba->CutHMM(text, words);
    } else if (rb_sMode == rb_sQuery) {
        data->jieba->CutForSearch(text, words, hmm);
    } else if (rb_sMode == rb_sFull) {
        data->jieba->CutAll(text, words);
    }

    VALUE arr = rb_ary_new2(words.size());
    for (auto iter = words.begin(); iter != words.end(); iter++) {
        rb_ary_push(arr, rb_enc_str_new(iter->c_str(), iter->length(), u8_enc));
    }
    return arr;
}

static VALUE internal_segment_tag(VALUE self, VALUE text_rbs)
{
    std::string text = StringValueCStr(text_rbs);
    GET_CPPJIEBA(data);

    std::vector<std::pair<std::string, std::string>> words;
    data->jieba->Tag(text, words);

    VALUE result = rb_hash_new();
    for (auto iter = words.begin(); iter != words.end(); iter++) {
        rb_hash_aset(result,
            rb_enc_str_new(iter->first.c_str(), iter->first.length(), u8_enc),
            rb_enc_str_new(iter->second.c_str(), iter->second.length(), u8_enc));
    }
    return result;
}


void Init_internal()
{
    rb_sMp = rb_intern("mp");
    rb_sMix = rb_intern("mix");
    rb_sHmm = rb_intern("hmm");
    rb_sQuery = rb_intern("query");
    rb_sFull = rb_intern("full");
    u8_enc = rb_utf8_encoding();

    rb_cJiebaRb_Internal = rb_define_class_under(rb_mJiebaRb, "Internal", rb_cData);
    rb_define_alloc_func(rb_cJiebaRb_Internal, internal_alloc);
    rb_define_method(rb_cJiebaRb_Internal, "initialize", (ruby_method*) &internal_initialize, 5);
    rb_define_method(rb_cJiebaRb_Internal, "extract_keyword", (ruby_method*) &internal_extract_keyword, 2);
    rb_define_method(rb_cJiebaRb_Internal, "segment", (ruby_method*) &internal_segment, 4);
    rb_define_method(rb_cJiebaRb_Internal, "segment_tag", (ruby_method*) &internal_segment_tag, 1);
}

}