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

#include <grabin/statistics/variance.hpp>

#include <catch/catch.hpp>

TEST_CASE("variance of empty set and singular element")
{
    grabin::variance_accumulator<double> acc;

    REQUIRE(acc.count() == 0);
    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(0.0, 1));
    REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinAbs(0.0, 1e-10));

    auto const x0 = double{42};
    acc(x0);

    REQUIRE(acc.count() == 1);
    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(x0, 1));
    REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinAbs(0.0, 1e-10));
}

TEST_CASE("variance of several identical elements")
{
    grabin::variance_accumulator<double> acc;

    auto const x0 = double{3.14};
    auto const N = 100;

    for(auto n = N; n > 0; -- n)
    {
        acc(x0);
    }

    REQUIRE(acc.count() == N);
    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(x0, 1));
    REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinULP(0.0, 1));
    REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinULP(0.0, 1));
}

TEST_CASE("variance of two different elements")
{
    auto const x1 = double{1.0};
    auto const x2 = double{2.0};
    auto const m = (x1 + x2) / 2;
    auto const s = std::abs(x1 - x2) / 2;

    grabin::variance_accumulator<double> acc;
    acc(x1)(x2);

    REQUIRE(acc.count() == 2);
    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(m, 1));
    REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinULP(s*s, 1));
    REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinULP(s, 1));
}

TEST_CASE("variance of sequential integers")
{
    auto const N = 100;

    auto const s2 = (N*N - 1) / 12.0;
    auto const s = std::sqrt(s2);

    grabin::variance_accumulator<int> acc;

    for(auto n = 1; n <= N; ++ n)
    {
        auto const x = (n - 1);
        acc(x);
    }

    REQUIRE(acc.count() == N);

    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(double(N-1) / 2.0, 1));
    REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinAbs(s2, 1e-10));
    REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinAbs(s, 1e-10));
}

TEST_CASE("variance of arithmetic progression")
{
    auto const x0 = double{-10};
    auto const N = 100;
    auto const d = double{0.45};
    auto const xN = x0 + d * (N - 1);

    auto const s2 = (N*N - 1) / 12.0 * d*d;
    auto const s = std::sqrt(s2);

    grabin::variance_accumulator<double> acc;

    for(auto x = x0; x <= xN; x += d)
    {
        acc(x);
    }

    REQUIRE(acc.count() == N);

    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinAbs((x0 + xN) / 2, 1e-10));
    REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinAbs(s2, 1e-10));
    REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinAbs(s, 1e-10));
}
