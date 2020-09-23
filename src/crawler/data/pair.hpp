namespace scam::crawler
{
    template <typename T1, typename T2>
    struct pair
    {
        T1 first;
        T2 second;

        pair(const T1& first, const T2& second)
            : first(first), second(second)
        {}

        pair<T1, T2> operator=(const pair<T1, T2>& p)
        {
            this->first = p.first;
            this->second = p.second;
            return *this;
        }
    };
}