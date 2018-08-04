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

#include <grabin/linear_algebra/outer_product.hpp>
#include <grabin/linear_algebra/symmetric_matrix.hpp>

#include <catch/catch.hpp>

TEST_CASE("symmetric_matrix : zero matrix")
{
    auto const n = 5;

    grabin::symmetric_matrix<int> const zero(n);

    REQUIRE(zero.dim() == n);

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        REQUIRE(zero(i, j) == 0);
    }

    for(auto const & x : zero)
    {
        REQUIRE(x == 0);
    }
}

TEST_CASE("math_vector : outer_square")
{
    grabin::math_vector<int> const x{2, -3, 4};

    auto const C = grabin::outer_square(x);

    REQUIRE(C.dim() == x.dim());
    auto const n = C.dim();

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        CHECK(C(i, j) == x[i]*x[j]);
    }
}

TEST_CASE("symmetric_matrix : plus assign")
{
    grabin::math_vector<int> const x1{2, -3, 4};
    grabin::math_vector<int> const x2{2, -3, 4};

    auto C = grabin::outer_square(x1);
    C += grabin::outer_square(x1);

    REQUIRE(C.dim() == x1.dim());
    REQUIRE(C.dim() == x2.dim());
    auto const n = C.dim();

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        CHECK(C(i, j) == (x1[i]*x1[j] + x2[i]*x2[j]));
    }
}

TEST_CASE("symmetric_matrix : plus")
{
    grabin::math_vector<int> const x1{2, -3, 4};
    grabin::math_vector<int> const x2{2, -3, 4};

    auto const C = grabin::outer_square(x1) + grabin::outer_square(x1);

    REQUIRE(C.dim() == x1.dim());
    REQUIRE(C.dim() == x2.dim());
    auto const n = C.dim();

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        CHECK(C(i, j) == (x1[i]*x1[j] + x2[i]*x2[j]));
    }
}

TEST_CASE("math_vector : multiply assign by scalar")
{
    grabin::math_vector<int> const x{2, -3, 4};
    auto const alpha = 3;

    auto C = grabin::outer_square(x);
    auto & r = (C *= alpha);

    REQUIRE(&r == &C);

    static_assert(std::is_same<decltype(C *= alpha), decltype((C))>::value,
                  "multiplies assign must return *this");

    REQUIRE(C.dim() == x.dim());
    auto const n = C.dim();

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        CHECK(C(i, j) == alpha*x[i]*x[j]);
    }
}

TEST_CASE("math_vector : left multiply by scalar")
{
    grabin::math_vector<int> const x{2, -3, 4};
    auto const alpha = 3;

    auto const C = grabin::outer_square(x);
    auto const D = C * alpha;

    REQUIRE(D.dim() == x.dim());
    auto const n = D.dim();

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        CHECK(D(i, j) == alpha*C(i, j));
    }
}

TEST_CASE("math_vector : right multiply by scalar")
{
    grabin::math_vector<int> const x{2, -3, 4};
    auto const alpha = 3;

    auto const C = grabin::outer_square(x);
    auto const D = alpha * C;

    REQUIRE(D.dim() == x.dim());
    auto const n = D.dim();

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        CHECK(D(i, j) == alpha*C(i, j));
    }
}

TEST_CASE("math_vector : divide assign by scalar")
{
    grabin::math_vector<int> const x{2, -3, 4};
    auto const alpha = 3;

    auto C = grabin::outer_square(x);
    auto & r = (C /= alpha);

    REQUIRE(&r == &C);

    static_assert(std::is_same<decltype(C /= alpha), decltype((C))>::value,
                  "multiplies assign must return *this");

    REQUIRE(C.dim() == x.dim());
    auto const n = C.dim();

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        CHECK(C(i, j) == x[i]*x[j]/alpha);
    }
}

TEST_CASE("math_vector : divide by scalar")
{
    grabin::math_vector<int> const x{2, -3, 4};
    auto const alpha = 3;

    auto const C = grabin::outer_square(x);
    auto const D = C / alpha;

    REQUIRE(D.dim() == x.dim());
    auto const n = D.dim();

    for(auto i = 0*n; i < n; ++ i)
    for(auto j = 0*n; j < n; ++ j)
    {
        CHECK(D(i, j) == C(i, j)/alpha);
    }
}
