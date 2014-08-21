#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <iterator>
#include <array>
#include <random>

class Card
{
public:
    explicit Card() = default;
    explicit Card(std::string, std::string, int);
        
    std::string name();
    std::string const& name() const;
private:
    std::string m_name, m_symbol;
    int m_number;
};

template<bool B, typename V = void>
using enable_if_it = typename std::enable_if<B, V>::type;

template<typename T, typename = void>
struct is_iterator
{
   static constexpr bool value = false;
};

template<typename T>
struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
{
   static constexpr bool value = true;
};

template<bool B, typename V = void>
using enable_if_t = typename std::enable_if<B, V>::type;

class Deck
{
private:
    template<typename It>
    class iterator;
        
    using container_type = std::vector<Card>;
    
    template<typename T>
    iterator<T> make_iterator(T it) const { return {it}; }
    
    container_type m_cards;
public:
    explicit Deck() = default;
    
    template<typename Iter, typename = enable_if_t<is_iterator<Iter>::value>>
    explicit Deck(Iter, Iter);
    
    template<typename... Args>
    explicit Deck(Args&&...);
    
    std::size_t size() const;
    
    template<typename Generator>
    void shuffle(Generator&& g);
    
    void shuffle();
    
    auto begin() -> decltype(make_iterator(std::begin(m_cards)));
    auto end()   -> decltype(make_iterator(std::end(m_cards)));
    
    auto begin() const -> decltype(make_iterator((m_cards).cend()));
    auto end()   const -> decltype(make_iterator((m_cards).cbegin()));
};

Card::Card(std::string name, std::string symbol, int number)
    : m_name(name)
    , m_symbol(symbol)
    , m_number(number)
{ }

std::string Card::name() { return m_name; }
std::string const& Card::name() const { return m_name; }

template<typename Iter, typename>
Deck::Deck(Iter beg, Iter end) : m_cards(beg, end) { }

template<typename... Args>
Deck::Deck(Args&&... args) : m_cards{ std::forward<Args>(args)... } { }

std::size_t Deck::size() const { return m_cards.size(); }

template<typename Generator>
void Deck::shuffle(Generator&& g)
{
    std::shuffle(std::begin(m_cards), std::end(m_cards), std::forward<Generator>(g));
}

void Deck::shuffle()
{
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(std::begin(m_cards), std::end(m_cards), g);
}

template<typename It>
class Deck::iterator
{
public:
    iterator(It it)
        : m_it(it)
    { }
    
    friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs.m_it != rhs.m_it; }
    friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs);        }
    
    iterator& operator++(int) { ++m_it; return *this; }
    iterator operator++() { auto c = *this; ++*this; return c; }
    
    typename It::value_type& operator*() { return *m_it; }
    typename It::value_type const& operator*() const { return *m_it; }
private:
    It m_it;
};

auto Deck::begin() -> decltype(make_iterator(m_cards.begin()))   { return make_iterator(std::begin(m_cards)); }
auto Deck::end() -> decltype(make_iterator(std::end(m_cards)))       { return make_iterator(std::end(m_cards));   }
auto Deck::begin() const -> decltype(make_iterator(m_cards.cbegin())) { return make_iterator(m_cards.begin()); }
auto Deck::end() const -> decltype(make_iterator(m_cards.cend()))     { return make_iterator(m_cards.end());   }

template<typename cT, typename traits = std::char_traits<cT>>
std::basic_ostream<cT, traits>& operator<<(std::basic_ostream<cT, traits>& os, const Card& c)
{
     return os << "Name: " << c.name();
}

template<typename cT, typename traits = std::char_traits<cT>>
std::basic_ostream<cT, traits>& operator<<(std::basic_ostream<cT, traits>& os, const Deck& d)
{
    for (const auto& x : d)
        os << x << std::endl;
    return os;
}

int main()
{
    Deck d(Card("x", "y", 5), Card("y", "f", 2));
    Deck b(std::move(d));
    std::cout << b;
}