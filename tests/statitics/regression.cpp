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

#include <grabin/statistics/regression.hpp>

#include <catch/catch.hpp>

TEST_CASE("linear regression for empty set and singular element")
{
    grabin::linear_regression_accumulator<double, double> acc;

    REQUIRE(acc.count() == 0);
    REQUIRE_THAT(acc.intercept(), Catch::Matchers::WithinULP(0.0, 1));
    REQUIRE_THAT(acc.effects(), Catch::Matchers::WithinULP(0.0, 1));

    auto const a = -2.3;
    auto const b = 3.14;
    auto const x0 = 1.25;
    auto const y0 = a*x0 + b;

    acc(x0, y0);

    REQUIRE(acc.count() == 1);

    auto const a1 = acc.effects();
    auto const b1 = acc.intercept();

    REQUIRE_THAT(a1 * x0 + b1, Catch::Matchers::WithinAbs(y0, 1e-10));
}

TEST_CASE("linear regression for two points")
{
    auto const a = -2.3;
    auto const b = 3.14;

    auto const x0 = 1.25;
    auto const y0 = a*x0 + b;

    auto const x1 = 1.75;
    auto const y1 = a*x1 + b;

    grabin::linear_regression_accumulator<double, double> acc;
    acc(x0, y0)(x1, y1);

    REQUIRE(acc.count() == 2);

    auto const a1 = acc.effects();
    auto const b1 = acc.intercept();

    CHECK_THAT(a1 * x0 + b1, Catch::Matchers::WithinAbs(y0, 1e-10));
    CHECK_THAT(a1 * x1 + b1, Catch::Matchers::WithinAbs(y1, 1e-10));

    CHECK_THAT(a1, Catch::Matchers::WithinAbs(a, 1e-10));
    CHECK_THAT(b1, Catch::Matchers::WithinAbs(b, 1e-10));
}

TEST_CASE("linear regression for sample with no noise")
{
    auto const a = -2.3;
    auto const b = 3.14;

    auto const x0 = 1.25;
    auto const N = 100;
    auto const d = 0.15;

    grabin::linear_regression_accumulator<double, double> acc;

    for(auto n = 0; n < N; ++ n)
    {
        auto const x = x0 + d * n;
        auto const y = a * x + b;

        acc(x, y);
    }

    REQUIRE(acc.count() == N);

    auto const a1 = acc.effects();
    auto const b1 = acc.intercept();

    CHECK_THAT(a1, Catch::Matchers::WithinAbs(a, 1e-10));
    CHECK_THAT(b1, Catch::Matchers::WithinAbs(b, 1e-10));

    for(auto n = 0; n < N; ++ n)
    {
        auto const x = x0 + d * n;
        auto const y = a * x + b;

        CHECK_THAT(a1*x + b1, Catch::Matchers::WithinAbs(y, 1e-10));
    }
}

TEST_CASE("linear regression for sample of identical elements")
{
    auto const a = -2.3;
    auto const b = 3.14;

    auto const x0 = 1.25;
    auto const N = 100;

    grabin::linear_regression_accumulator<double, double> acc;

    for(auto n = 0; n < N; ++ n)
    {
        auto const y = a * x0 + b;

        acc(x0, y);
    }

    REQUIRE(acc.count() == N);

    auto const a1 = acc.effects();
    auto const b1 = acc.intercept();

    CHECK_THAT(a1*x0 + b1, Catch::Matchers::WithinAbs(a*x0 + b, 1e-10));
}
