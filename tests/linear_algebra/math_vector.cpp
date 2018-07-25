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

// @todo Модифицирующий доступ к элементам по индексу
// @todo Реакция на выход индекса за границы диапазона
// @todo Операторы == и !=
// @todo Линейные операции: умножение на скаляр и сложение векторов
// @todo Конструктор без аргументов
// @todo Вычисление среднего
