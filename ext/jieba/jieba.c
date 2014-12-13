#include <jieba.h>
VALUE mJieba;

void Init_jieba()
{
    mJieba = rb_define_module("JiebaRb");

    Init_segment();
}
