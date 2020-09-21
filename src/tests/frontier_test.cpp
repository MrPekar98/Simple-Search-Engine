#include <assert.h>
#include "../crawler/data/frontier.hpp"

// Prototypes.
void test_empty_simple();
void test_get_next_simple();
void test_size_simple();
void test_empty_advanced();
void test_get_next_advanced();
void test_size_advanced();

int main()
{
    test_empty_simple();
    test_get_next_simple();
    test_size_simple();
    test_empty_advanced();
    test_get_next_advanced();
    test_size_advanced();

    return 0;
}

// Tests URL exists in simple frontier.
void test_empty_simple()
{
    scam::crawler::frontier f;
    
    assert(f.empty());
    f.add_url("URL");
    assert(!f.empty());
}

// Tests retrieving of URLs in simple frontier.
void test_get_next_simple()
{
    scam::crawler::frontier f;
    f.add_url("URL1");
    f.add_url("URL2");

    assert(f.get_next().compare("URL1") == 0);
    assert(f.get_next().compare("URL2") == 0);
}

// Tests size of simple frontier.
void test_size_simple()
{
    scam::crawler::frontier f;
    f.add_url("URL1");
    f.add_url("URL2");
    
    assert(f.size() == 2);
}

// Tests empty() function in advanced frontier.
void test_empty_advanced()
{
    scam::crawler::mercator m(3, 3);

    assert(m.empty());
    m.add_url("URL", 0);
    assert(!m.empty());
}

// Tests getting next URL in advanced frontier.
void test_get_next_advanced()
{
    scam::crawler::mercator m(3, 3);
    m.add_url("URL1", 0);
    m.add_url("URL2", 0);
    m.add_url("URL3", 2);

    assert(m.get_next().compare("URL1") == 0);
    assert(m.get_next().compare("URL3") == 0);
    assert(m.get_next().compare("URL2") == 0);
}

// Tests size of advanced frontier.
void test_size_advanced()
{
    scam::crawler::mercator m(3, 3);
    m.add_url("URL1", 0);
    m.add_url("URL2", 0);
    m.add_url("URL3", 2);

    assert(m.size() == 3);
    m.get_next();
    assert(m.size() == 2);
}