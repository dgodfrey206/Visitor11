#include <iostream>
#include <memory>
#include <string>

// The Visitor Pattern

template<class T, class U>
std::unique_ptr<T> make_unique(U&& u)
{
    return std::unique_ptr<T>(new T{std::forward<U>(u)});
}

template<class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T{std::forward<Args>(args)...});
}

class Liquor;
class Tobacco;
class Necessity;

class Visitor
{
public:
    virtual ~Visitor() = default;
    virtual double visit(std::unique_ptr<Liquor> liquor) = 0;
    virtual double visit(std::unique_ptr<Tobacco> tobaccoItem) = 0;
    virtual double visit(std::unique_ptr<Necessity> necessityItem) = 0;
};

class TaxVisitor : public Visitor
{
public:
    double visit(std::unique_ptr<Liquor> liquorItem);
    double visit(std::unique_ptr<Tobacco> tobaccoItem);
    double visit(std::unique_ptr<Necessity> necessityItem);
};

class TaxHolidayVisitor : public Visitor
{
public:
    double visit(std::unique_ptr<Liquor> liquor);
    double visit(std::unique_ptr<Tobacco> tobaccoItem);
    double visit(std::unique_ptr<Necessity> necessityItem);
};

class Visitable
{
    virtual double accept(Visitor& visitor) = 0;
    ~Visitable() = default;
};

class Liquor : public Visitable
{
public:
    Liquor(double item) : price(item) { }

    double getPrice() { return price; }

    double accept(Visitor& visitor)
    {
        return visitor.visit(make_unique<Liquor>(*this));
    }
private:
    double price;
};

class Tobacco : public Visitable
{
public:
    Tobacco(double item) : price(item) { }

    double getPrice() { return price; }

    double accept(Visitor& visitor)
    {
        return visitor.visit(make_unique<Tobacco>(*this));
    }
private:
    double price;
};

class Necessity : public Visitable
{
public:
    Necessity(double item) : price(item) { }

    double getPrice() { return price; }

    double accept(Visitor& visitor)
    {
        return visitor.visit(make_unique<Necessity>(*this));
    }
private:
    double price;
};

double TaxVisitor::visit(std::unique_ptr<Liquor> liquorItem)
{
    std::cout << "Liquor item: Price with Tax\n";
    return (liquorItem->getPrice() * .10) + liquorItem->getPrice();
}

double TaxVisitor::visit(std::unique_ptr<Tobacco> tobaccoItem)
{

    std::cout << "Tobacco item: Price with Tax\n";
    return (tobaccoItem->getPrice() * .32) + tobaccoItem->getPrice();
}

double TaxVisitor::visit(std::unique_ptr<Necessity> necessityItem)
{

    std::cout << "Necessity item: Price with Tax\n";
    return (necessityItem->getPrice() * 0) + necessityItem->getPrice();
}

double TaxHolidayVisitor::visit(std::unique_ptr<Liquor> liquorItem)
{
    std::cout << "Liquor item: Price with Holiday Tax\n";
    return (liquorItem->getPrice() * .5) + liquorItem->getPrice();
}

double TaxHolidayVisitor::visit(std::unique_ptr<Tobacco> tobaccoItem)
{

    std::cout << "Tobacco item: Price with Holiday Tax\n";
    return (tobaccoItem->getPrice() * .4) + tobaccoItem->getPrice();
}

double TaxHolidayVisitor::visit(std::unique_ptr<Necessity> necessityItem)
{

    std::cout << "Necessity item: Price with Holiday Tax\n";
    return (necessityItem->getPrice() * 0) + necessityItem->getPrice();
}

int main()
{
    TaxVisitor tax;
    TaxHolidayVisitor holiday_tax;

    Liquor beer(3.50);
    Tobacco cigar(10.00);
    Necessity food(53.25);

    std::cout << "Normal tax:\n";
    std::cout << beer.accept(tax) << '\n';
    std::cout << cigar.accept(tax) << '\n';
    std::cout << food.accept(tax) << std::endl;

    std::cout << "\nHoliday tax:\n";
    std::cout << beer.accept(holiday_tax) << '\n';
    std::cout << cigar.accept(holiday_tax) << '\n';
    std::cout << food.accept(holiday_tax) << std::endl;
}