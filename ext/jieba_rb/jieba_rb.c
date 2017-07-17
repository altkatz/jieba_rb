#include <ruby.h>

VALUE rb_mJiebaRb;

void Init_jieba_rb()
{
    rb_mJiebaRb = rb_define_module("JiebaRb");

    Init_internal();
}
