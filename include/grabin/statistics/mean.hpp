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

#ifndef Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED
#define Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED

/** @file grabin/statistics/mean.hpp
 @brief Вычисление выборочного среднего
*/

#include <type_traits>
#include <utility>

namespace grabin
{
inline namespace v0
{
    /** @brief Класс-характеристика для определения типа среднего значения
    @tparam T тип элементов
    @tparam N тип для представления количества элементов
    */
    template <class T, class N>
    struct average_type
    {
        /// @brief Тип среднего значения элементов
        using type = std::conditional_t<std::is_integral<T>::value, double, T>;
    };

    /** @brief Тип-синоним для типа среднего значения
    @tparam T тип элементов
    @tparam N тип для представления количества элементов
    */
    template <class T, class N>
    using average_type_t = typename average_type<T, N>::type;

    /** @brief Накопитель для вычисления выборочного среднего
    @tparam T тип элементов
    @tparam IntType тип для представления количества элементов
    */
    template <class T, class IntType = int>
    class mean_accumulator
    {
    public:
        /// @brief Тип количества элементов
        using counter_type = IntType;

        /// @brief Тип среднего
        using mean_type = average_type_t<T, IntType>;

        /** @brief Конструктор
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->mean() == T{0} </tt>
        */
        mean_accumulator() = default;

        /** @brief Конструктор с явным заданием "нулевого" элемента
        @param zero "нулевой" элемент
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->mean() == zero </tt>
        */
        mean_accumulator(T zero)
         : n_(0)
         , mean_(std::move(zero))
        {}

        /** @brief Обновление статистик с учётом нового элемента
        @param x новый элемент
        @return <tt> *this </tt>
        */
        mean_accumulator & operator()(T const & x)
        {
            ++ this->n_;

            this->mean_ += (x - this->mean_) / this->n_;

            return *this;
        }

        /** @brief Количество элементов
        @return Количество обработанных к настоящему моменту элементов
        */
        counter_type const & count() const
        {
            return this->n_;
        }

        /** @brief Среднее значение
        @return Накопленное к настоящему моменту среднее значение
        */
        mean_type const & mean() const
        {
            return this->mean_;
        }

    private:
        counter_type n_ = counter_type(0);
        mean_type mean_ = mean_type(0);
    };
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED
