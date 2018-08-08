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

#ifndef Z_GRABIN_STATISTICS_VARIANCE_HPP_INCLUDED
#define Z_GRABIN_STATISTICS_VARIANCE_HPP_INCLUDED

/** @file grabin/statistics/mean.hpp
 @brief Вычисление выборочной дисперсии
*/

#include <grabin/statistics/mean.hpp>
#include <grabin/linear_algebra/outer_product.hpp>

#include <cmath>

namespace grabin
{
inline namespace v0
{
    /** @brief Накопитель для вычисления дисперсии
    @tparam T тип элементов
    @tparam N тип для представления количества элементов
    @tparam Tensor_algebra используемая тензорная алгебра
    */
    template <class T, class IntType = int,
              template <class> class Tensor_algebra = default_tensor_algebra>
    class variance_accumulator
    {
        using Mean_acc = mean_accumulator<T, IntType>;

    public:
        /// @brief Тип количества элементов
        using counter_type = typename Mean_acc::counter_type;

        /// @brief Тип среднего
        using mean_type = typename Mean_acc::mean_type;

        /// @brief Тип тензорной алгебры
        using tensor_algebra = Tensor_algebra<mean_type>;

        /// @brief Тип дисперсии
        using variance_type = average_type_t<typename tensor_algebra::tensor_product_type, IntType>;

        /** @brief Конструктор
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->mean() == T(0) </tt>
        @post <tt> this->variance() == T(0) </tt>
        */
        variance_accumulator() = default;

        /** @brief Конструктор
        @param zero "нулевой" элемент
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->mean() == zero </tt>
        @post <tt> this->variance() == zero * zero </tt>, где * обозначает тензорное произведение
        */
        variance_accumulator(mean_type zero)
         : S_(tensor_algebra::outer_square_impl(zero))
         , mean_acc_(std::move(zero))
        {}

        /** @brief Обновление статистик с учётом нового элемента
        @param x новый элемент
        @return <tt> *this </tt>
        */
        variance_accumulator & operator()(T const & x)
        {
            this->S_ += this->square_impl(x - this->mean()) * this->count() / (this->count() + 1);

            this->mean_acc_(x);

            return *this;
        }

        /** @brief Количество элементов
        @return Количество обработанных к настоящему моменту элементов
        */
        counter_type const & count() const
        {
            return this->mean_acc_.count();
        }

        /** @brief Среднее значение
        @return Накопленное к настоящему моменту среднее значение
        */
        mean_type const & mean() const
        {
            return this->mean_acc_.mean();
        }

        /** @brief Дисперсия
        @return Накопленное к настоящему моменту значение дисперсии
        */
        variance_type variance() const
        {
            if(this->count() == 0)
            {
                return this->S_;
            }
            else
            {
                return this->S_ / this->count();
            }
        }

        /** @brief Среднеквадратическое отклонение
        @return <tt> sqrt(this->variance()) </tt>
        */
        variance_type standard_deviation() const
        {
            using std::sqrt;
            return sqrt(this->variance());
        }

    private:
        static variance_type square_impl(mean_type const & x)
        {
            return tensor_algebra::outer_square_impl(x);
        }

        variance_type S_ = variance_type(0.0);
        Mean_acc mean_acc_;
    };
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_STATISTICS_VARIANCE_HPP_INCLUDED
