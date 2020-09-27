#include <assert.h>
#include "../indexing/term.hpp"

// Prototypes.
void test_get_str();
void test_stop_words();
void test_tokenize();

int main()
{
    test_get_str();
    test_stop_words();
    test_tokenize();

    return 0;
}

// Tests getting term string.
void test_get_str()
{
    scam::indexing::term t("TestTerm");
    assert(t.get_str().compare("testterm") == 0);
}

// Tests words for being stop words.
void test_stop_words()
{
    scam::indexing::term t1("cat"), t2("can"), t3("she");

    assert(!t1.is_stop_word());
    assert(t2.is_stop_word());
    assert(t3.is_stop_word());
}

// Tests tokenizer.
void test_tokenize()
{
    scam::indexing::term t1("Test    "), t2("Test."), t3("    .Test.   "), t4("Test,  "), t5(",!.,?[].test,!.,?[].");

    assert(t1.tokenize().get_str().compare("test") == 0);
    assert(t2.tokenize().get_str().compare("test") == 0);
    assert(t3.tokenize().get_str().compare("test") == 0);
    assert(t4.tokenize().get_str().compare("test") == 0);
    assert(t5.tokenize().get_str().compare("test") == 0);
}