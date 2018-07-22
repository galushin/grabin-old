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

#include <cmath>

namespace grabin
{
inline namespace v0
{
    /** @brief Накопитель для вычисления дисперсии
    @tparam T тип элементов
    @tparam N тип для представления количества элементов
    */
    template <class T, class IntType = int>
    class variance_accumulator
    {
        using Mean_acc = mean_accumulator<T, IntType>;

    public:
        /// @brief Тип количества элементов
        using counter_type = typename Mean_acc::counter_type;

        /// @brief Тип среднего
        using mean_type = typename Mean_acc::mean_type;

        /// @brief Тип дисперсии
        using variance_type = mean_type;

        /** @brief Конструктор
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->mean() == T{0} </tt>
        @post <tt> this->variance() == T{0} </tt>
        @post <tt> this->standard_deviation() == T{0} </tt>
        */
        variance_accumulator() = default;

        /** @brief Обновление статистик с учётом нового элемента
        @param x новый элемент
        @return <tt> *this </tt>
        */
        variance_accumulator & operator()(T const & x)
        {
            auto const m_old = this->mean();

            this->mean_acc_(x);

            this->S_ += (x - this->mean()) * (x - m_old);

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
        Mean_acc mean_acc_;
        variance_type S_ = variance_type{0.0};
    };
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_STATISTICS_VARIANCE_HPP_INCLUDED
