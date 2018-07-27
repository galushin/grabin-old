/**
   Этот файл — часть библиотеки Grabin.

   Grabin - свободное программное обеспечение: вы можете перераспространять его и/или изменять ее на
   условиях Стандартной общественной лицензии GNU либо версии 3, либо (по вашему выбору) любой более
   поздней версии.

   Grabin распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без
   неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в
   Стандартной общественной лицензии GNU.

   Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этим программным
   обеспечением. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#include <grabin/linear_algebra/math_vector.hpp>

#include <catch/catch.hpp>

TEST_CASE("math_vector ctor ititializer list")
{
    using Element = int;

    std::initializer_list<Element> const args{3, 1, 4, 1, 5};

    grabin::math_vector<Element> const x{args};

    REQUIRE(x.dim() == args.size());

    for(auto i = 0 * x.dim(); i < x.dim(); ++ i)
    {
        CHECK(x[i] == args.begin()[i]);
    }
}

TEST_CASE("math_vector throws if index is out of range")
{
    grabin::math_vector<int> const x{1, 2, 3, 4};

    CHECK_THROWS_AS(x[-1], std::logic_error);
    CHECK_THROWS_AS(x[x.dim()], std::logic_error);
    CHECK_THROWS_AS(x[x.dim() + 5], std::logic_error);
}

TEST_CASE("math_vector mutable index access")
{
    grabin::math_vector<int> x{1, 2, 3, 4};
    auto const x_old = x;
    auto const a = - 13;

    REQUIRE(x[0] != a);

    x[0] = a;

    CHECK(a == x[0]);

    CHECK_THROWS_AS(x[-1], std::logic_error);
    CHECK_THROWS_AS(x[x.dim()], std::logic_error);
    CHECK_THROWS_AS(x[x.dim() + 5], std::logic_error);
}

TEST_CASE("math_vector equal operator : same size")
{
    grabin::math_vector<int> x{1, 2, 3, 4};
    auto const x_old = x;

    CHECK(x_old == x);
    CHECK(!(x_old != x));

    auto const a = - 13;

    REQUIRE(x[0] != a);

    x[0] = a;

    CHECK(x != x_old);
    CHECK(!(x == x_old));
}

TEST_CASE("math_vector equal operator : different size")
{
    grabin::math_vector<int> const x{1, 2, 3, 4};
    grabin::math_vector<int> const y{1, 2, 3};

    REQUIRE(x.dim() != y.dim());

    CHECK(x != y);
    CHECK(y != x);
    CHECK(!(x == y));
}

TEST_CASE("math_vector : multiply assign")
{
    grabin::math_vector<int> x{1, 2, 3, 4};
    auto const x_old = x;

    auto const a = -3;

    grabin::math_vector<int> & r = (x *= a);

    static_assert(std::is_same<decltype(x *= a), decltype((x))>::value,
                  "multiplies assign must return *this");
    REQUIRE(&r == &x);

    REQUIRE(x.dim() == x_old.dim());

    for(auto i = 0*x.dim(); i < x.dim(); ++ i)
    {
        CHECK(x[i] == x_old[i] * a);
    }

}

TEST_CASE("math_vector : multiply")
{
    grabin::math_vector<int> const x_old{1, 2, 3, 4};
    auto const a = -3;

    auto const x1 = x_old * a;
    auto const x2 = a * x_old;

    REQUIRE(x1.dim() == x_old.dim());
    REQUIRE(x2.dim() == x_old.dim());

    for(auto i = 0*x1.dim(); i < x1.dim(); ++ i)
    {
        CHECK(x1[i] == x_old[i] * a);
    }

    CHECK(x1 == x2);
}

TEST_CASE("math_vector: plus_assign")
{
    grabin::math_vector<int> x{1, 2, 3, 4};
    auto const x_old = x;
    grabin::math_vector<int> const y{1, -2, 2, 3};

    REQUIRE(x.dim() == y.dim());

    grabin::math_vector<int> & r = (x += y);

    static_assert(std::is_same<decltype(x += y), decltype((x))>::value,
                  "plus assign must return *this");
    REQUIRE(&r == &x);

    REQUIRE(x.dim() == x_old.dim());

    for(auto i = 0*x.dim(); i < x.dim(); ++ i)
    {
        CHECK(x[i] == x_old[i] + y[i]);
    }
}

TEST_CASE("math_vector: plus")
{
    grabin::math_vector<int> const x{1, 2, 3, 4};
    grabin::math_vector<int> const y{1, -2, 2, 3};

    REQUIRE(x.dim() == y.dim());

    auto const z = x + y;

    REQUIRE(z.dim() == x.dim());

    for(auto i = 0*x.dim(); i < x.dim(); ++ i)
    {
        CHECK(z[i] == x[i] + y[i]);
    }
}

TEST_CASE("math_vector: plus assign throws on different dimenstions")
{
    grabin::math_vector<int> x{1, 2, 3, 4};
    grabin::math_vector<int> const y{1, 2, 3};

    REQUIRE(x.dim() != y.dim());

    REQUIRE_THROWS_AS(x += y, std::logic_error);
}

TEST_CASE("math_vector: plus throws on different dimenstions")
{
    grabin::math_vector<int> const x{1, 2, 3, 4};
    grabin::math_vector<int> const y{1, 2, 3};

    REQUIRE(x.dim() != y.dim());

    REQUIRE_THROWS_AS(x + y, std::logic_error);
}

// @todo Вычисление среднего
