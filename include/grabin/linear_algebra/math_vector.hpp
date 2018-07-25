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

#ifndef Z_GRABIN_LINEAR_ALGEBRA_MATH_VECTOR_HPP_INCLUDED
#define Z_GRABIN_LINEAR_ALGEBRA_MATH_VECTOR_HPP_INCLUDED

/** @file grabin/linear_algebra/math_vector.hpp
 @brief Векторы и связанные с ними функции
*/

#include <stdexcept>
#include <vector>

namespace grabin
{
inline namespace v0
{
    /// @brief Стратегия проверок и обработки ошибок для векторов
    class vector_policy_throws
    {
    public:
        /** @brief Проверка индекса
        @param x вектор
        @param index индекс
        @throw logic_error, если <tt> index < 0 || index >= x.dim() </tt>
        */
        template <class Vector>
        static void check_index(Vector const & x, typename Vector::dimension_type index)
        {
            if(index < 0 || index >= x.dim())
            {
                // @todo Более информативное сообщение
                throw std::logic_error("incorrect index");
            }
        }
    };

    // @todo Стратегия проверок через assert

    /** @brief Класс "математического вектора
    @tparam T тип элементов
    @tparam Checking стратегия проверок и обработки ошибок
    @todo Определить требования к типам-параметрам
    */
    template <class T, class Checking = vector_policy_throws>
    class math_vector
    {
        using Data = std::vector<T>;
    public:
        // Типы
        /// @brief Стратегия проверки и обработки ошибок
        using checking_policy = Checking;

        /// @brief Тип размерности и индексов
        using dimension_type = typename Data::difference_type;

        // Создание, копирование, уничтожение
        /** @brief Создаёт вектор с элементами из списка инициализации
        @param init список элементов
        @post <tt> this->dim() == init.size() </tt>
        @post Для любого @c i из интервала <tt> [0; this->dim()) </tt> выполняется
        <tt> (*this)[i] == *(init.begin() + i) </tt>
        */
        math_vector(std::initializer_list<T> init)
         : data_(std::move(init))
        {}

        // Размер
        /** @brief Размерность
        @return Текущая размерность данного вектора
        */
        dimension_type dim() const
        {
            return this->data_.size();
        }

        // Доступ к элементам
        //@{
        /** @brief Доступ к элементу по индексу
        @param index индекс элемента
        @pre <tt> 0 <= index && index < this->dim() </tt>
        @return Ссылка на элемент с индексом @c index
        */
        T const & operator[](dimension_type index) const
        {
            checking_policy::check_index(*this, index);

            return this->data_[index];
        }

        T & operator[](dimension_type index);
        //@}

    private:
        Data data_;
    };
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_LINEAR_ALGEBRA_MATH_VECTOR_HPP_INCLUDED
