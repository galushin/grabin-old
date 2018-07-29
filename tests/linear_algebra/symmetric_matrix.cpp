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
