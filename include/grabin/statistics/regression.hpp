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

#ifndef Z_GRABIN_STATISTICS_REGRESSION_HPP_INCLUDED
#define Z_GRABIN_STATISTICS_REGRESSION_HPP_INCLUDED

/** @file grabin/statistics/regression.hpp
 @brief Средства для определения уравнений регрессии
*/

#include <grabin/statistics/variance.hpp>

namespace grabin
{
inline namespace v0
{
    /** @brief Накопитель для вычисления коэффициентов линейной регрессии
    @tparam Input тип входной переменной
    @tparam Output тип выходной переменной
    @tparam IntType тип для представления объёма выборки
    */
    template <class Input, class Output = Input, class IntType = int>
    class linear_regression_accumulator
    {
        using Input_acc = grabin::variance_accumulator<Input, IntType>;
        using Output_acc = grabin::mean_accumulator<Output, IntType>;
        using covariance_type = decltype(std::declval<Input>() * std::declval<Output>());

    public:
        /// @brief Тип для представления количества элементов выборки
        using counter_type = typename Input_acc::counter_type;

        /// @brief Тип постоянного слагаемого
        using intercept_type = Output;

        /// @brief Тип для хранения коэффициентов
        using effect_type = Input;

        /** @brief Конструктор
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->intercept() == Output{0} </tt>
        @post <tt> this->effects() == Input{0} </tt>
        */
        linear_regression_accumulator() = default;

        /** @brief Обновление статистик с учётом нового элемента
        @param x новое значение входной переменной
        @param y соответствующее значение выходной переменной
        @return <tt> *this </tt>
        */
        linear_regression_accumulator &
        operator()(Input const & x, Output const & y)
        {
            auto const my_old = this->y_stat_.mean();
            this->x_stat_(x);
            this->y_stat_(y);

            this->cov_ += (x - this->x_stat_.mean()) * (y - my_old);

            return *this;
        }

        /** @brief Количество элементов
        @return Количество обработанных к настоящему моменту элементов
        */
        counter_type const & count() const
        {
            return this->x_stat_.count();
        }

        /** @brief Постоянное слагаемое
        @return Накопленное к настоящему моменту постоянное слагаемое
        */
        intercept_type intercept() const
        {
            return this->y_stat_.mean() - this->effects() * this->x_stat_.mean();
        }

        /** @brief Коэффициенты
        @return Накопленные к настоящему моменту значения коэффициентов
        */
        effect_type effects() const
        {
            if(this->count() < 2)
            {
                return {0.0};
            }
            else if(this->x_stat_.variance() > 0)
            {
                return (this->cov_ / this->count()) / this->x_stat_.variance();
            }
            else
            {
                return {0.0};
            }
        }

    private:
        Input_acc x_stat_;
        Output_acc y_stat_;
        covariance_type cov_ = covariance_type{0.0};
    };
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_STATISTICS_REGRESSION_HPP_INCLUDED
