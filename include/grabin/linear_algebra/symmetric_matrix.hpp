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

#ifndef Z_GRABIN_LINEAR_ALGEBRA_SYMMETRIC_MATRIX_HPP_INCLUDED
#define Z_GRABIN_LINEAR_ALGEBRA_SYMMETRIC_MATRIX_HPP_INCLUDED

/** @file grabin/linear_algebra/symmetric_matrix.hpp
 @brief Класс симметричной матрицы
*/

#include <grabin/linear_algebra/math_vector.hpp>

namespace grabin
{
inline namespace v0
{
    /** @brief Симметричная матрица
    @tparam T тип элементов
    @tparam Checking стратегия проверок и обработки ошибок
    */
    template <class T, class Checking = vector_policy_throws>
    class symmetric_matrix
    {
        using Data = math_vector<T>;
    public:
        // Типы
        /// @brief Стратегия проверки и обработки ошибок
        using checking_policy = Checking;

        /// @brief Тип для представления размерности
        using dimension_type = typename Data::dimension_type;

        // Создание, копирование, уничтожение
        /** @brief Конструктор, создающий нулевую матрицу
        @param n размерность
        @post <tt> this->dim() == n </tt>
        @post <tt> (*this)(i, j) == 0 </tt> для любых <tt> 0 <= i, j < n </tt>
        */
        explicit symmetric_matrix(dimension_type n)
         : dim_(n)
         , data_(n*(n+1)/2)
        {}

        // Размер и ёмкость
        /** @brief Размерность матрицы
        @return Размерность матрицы
        */
        dimension_type dim() const
        {
            return this->dim_;
        }

        // Доступ к элементам
        //@{
        /** @brief Доступ к элементам матрицы
        @param row номер строки
        @param col номер стоблца
        @return Ссылка на элемент матрицы, находящий в строке @c row и столблце @c col
        */
        T const & operator()(dimension_type row, dimension_type col) const
        {
            if(col >= row)
            {
                using std::swap;
                swap(col, row);
            }

            return data_[row*(row+1)/2 + col];
        }

        T & operator()(dimension_type row, dimension_type col)
        {
            return const_cast<T&>(static_cast<symmetric_matrix const &>(*this)(row, col));
        }
        //@}

        // Линейные операции
        /** @brief Прибавление матрицы
        @param x прибавляемая матрица
        @pre <tt> this->dim() == x.dim() </tt>
        @post Прибавляет к каждому элементу <tt> *this </tt> соответсвующий элемент @c x
        @return <tt> *this </tt.
        */
        symmetric_matrix & operator+=(symmetric_matrix const & x)
        {
            checking_policy::check_equal_dimensions(*this, x);

            this->data_ += x.data_;

            return *this;
        }

    private:
        dimension_type dim_;
        Data data_;
    };

    /** @brief Оператор сложения матриц
    @param x, y аргументы
    @pre <tt> x.dim() == y.dim() </tt>
    @return Матрица, элементы которой равны сумме соответствующих элементов матриц @c x и @c y
    */
    template <class T, class Check>
    symmetric_matrix<T, Check>
    operator+(symmetric_matrix<T, Check> x, symmetric_matrix<T, Check> const & y)
    {
        x += y;
        return x;
    }
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_LINEAR_ALGEBRA_SYMMETRIC_MATRIX_HPP_INCLUDED
