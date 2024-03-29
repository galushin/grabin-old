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

#include <grabin/statistics/mean.hpp>

#include <catch/catch.hpp>

TEST_CASE("mean of empty set and one element")
{
    grabin::mean_accumulator<double> acc;

    REQUIRE(acc.count() == 0);
    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(0.0, 1));

    auto const x0 = double{42};
    acc(x0);

    REQUIRE(acc.count() == 1);
    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(x0, 1));
}

TEST_CASE("mean of several identical elements")
{
    grabin::mean_accumulator<double> acc;

    auto const x0 = double{3.14};
    auto const N = 100;

    for(auto n = N; n > 0; -- n)
    {
        acc(x0);
    }

    REQUIRE(acc.count() == N);
    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(x0, 1));
}

TEST_CASE("mean of two elements")
{
    auto const x1 = double{1.0};
    auto const x2 = double{2.0};

    grabin::mean_accumulator<double> acc;
    acc(x1)(x2);

    REQUIRE(acc.count() == 2);
    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP((x1 + x2) / 2, 1));
}

TEST_CASE("mean of ints is double")
{
    auto const x1 = int{1};
    auto const x2 = int{2};

    grabin::mean_accumulator<int> acc;
    static_assert(std::is_same<decltype(acc.mean()), double const &>::value,
                  "mean of ints must be floating point");

    acc(x1)(x2);

    REQUIRE(acc.count() == 2);
    REQUIRE(acc.mean()*acc.count() == (x1 + x2));

    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(double(x1 + x2) / 2, 1));
}

TEST_CASE("mean of sequential integers")
{
    auto const x0 = 0;
    auto const N = 100;

    auto const xN = x0 + (N - 1);

    grabin::mean_accumulator<int> acc;
    for(auto x = x0; x <= xN; ++ x)
    {
        acc(x);
    }

    REQUIRE(acc.count() == N);

    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinULP(double(x0 + xN) / 2.0, 1));
}

TEST_CASE("mean of arithmetic progression")
{
    auto const x0 = double{-10};
    auto const N = 100;
    auto const d = double{0.45};
    auto const xN = x0 + d * (N - 1);

    grabin::mean_accumulator<double> acc;

    for(auto x = x0; x <= xN; x += d)
    {
        acc(x);
    }

    REQUIRE(acc.count() == N);

    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinAbs((x0 + xN) / 2, 1e-10));
}

TEST_CASE("mean of arithmetic progression - symmetric")
{
    auto const x0 = double{-10};
    auto const xN = -x0;
    auto const N = 101;
    auto const d = (xN - x0) / (N-1);

    grabin::mean_accumulator<double> acc;

    for(auto x = x0; x <= xN; x += d)
    {
        acc(x);
    }

    REQUIRE(acc.count() == N);

    REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinAbs(0.0, 1e-10));
}
