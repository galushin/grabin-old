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

        /// @brief Тип итератора
        using iterator = typename Data::iterator;

        /// @brief Тип константного итератора
        using const_iterator = typename Data::const_iterator;

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
        @return <tt> *this </tt>
        */
        symmetric_matrix & operator+=(symmetric_matrix const & x)
        {
            checking_policy::check_equal_dimensions(*this, x);

            this->data_ += x.data_;

            return *this;
        }

        /** @brief Умножение матрицы на скаляр
        @param alpha скаляр, на который умножается матрица
        @post Уможает каждый элемент <tt> *this </tt> на @c alpha
        @return <tt> *this </tt>
        */
        symmetric_matrix & operator*=(T const & alpha)
        {
            this->data_ *= alpha;

            return *this;
        }

        /** @brief Деление матрицы на скаляр
        @param alpha скаляр, на который делится матрица
        @post Делит каждый элемент <tt> *this </tt> на @c alpha
        @return <tt> *this </tt>
        */
        symmetric_matrix & operator/=(T const & alpha)
        {
            this->data_ /= alpha;

            return *this;
        }

        // Итераторы
        //@{
        /** @brief Итератор начала последовательности элементов
        @return Итератор, задающий начало последовательности элементов
        */
        iterator begin()
        {
            return this->data_.begin();
        }

        const_iterator begin() const
        {
            return this->data_.begin();
        }
        //@}

        //@{
        /** @brief Итератор конца последовательности элементов
        @return Итератор, задающий конец последовательности элементов
        */
        iterator end()
        {
            return this->data_.end();
        }

        const_iterator end() const
        {
            return this->data_.end();
        }
        //@}

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

    /** @brief Умножение матрицы на скаляр справа
    @param x матрица
    @param alpha скаляр
    @return Матрица, элементы которой равны соответствующим элементам @c x, умноженным на @c alpha
    */
    template <class T1, class Check, class T2>
    auto operator*(symmetric_matrix<T1, Check> x, T2 const & alpha)
    -> symmetric_matrix<decltype(x(0, 0)*alpha), Check>
    {
        symmetric_matrix<decltype(x(0, 0)*alpha), Check> result(std::move(x));
        result *= alpha;
        return result;
    }

    /** @brief Умножение матрицы на скаляр слева
    @param x матрица
    @param alpha скаляр
    @return Матрица, элементы которой равны @c alpha, умноженному соответствующим элементам @c x
    */
    template <class T1, class T2, class Check>
    auto operator*(T1 const & alpha, symmetric_matrix<T2, Check> x)
    -> symmetric_matrix<decltype(alpha*x(0, 0)), Check>
    {
        for(auto & elem : x)
        {
            elem = alpha * elem;
        }

        return x;
    }

    /** @brief Оператор деления матрицы на скалря
    @param x матрицы
    @param alpha скаляр
    @post <tt> alpha != 0 </tt>
    @return Матрица, элементы которого равны соответствующим элементам @c x, делённым на @c a
    */
    template <class T1, class Check, class T2>
    auto operator/(symmetric_matrix<T1, Check> x, T2 const & alpha)
    -> symmetric_matrix<decltype(x(0,0) / alpha), Check>
    {
        auto result = symmetric_matrix<decltype(x(0,0)/alpha), Check>(std::move(x));
        result /= alpha;
        return result;
    }
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_LINEAR_ALGEBRA_SYMMETRIC_MATRIX_HPP_INCLUDED
