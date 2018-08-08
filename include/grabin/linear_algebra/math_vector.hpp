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

#include <sstream>
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
                std::ostringstream os;
                os << "Incorrect index = " << index << ", dimension = " << x.dim();
                throw std::logic_error(os.str());
            }
        }

        /** @brief Проверка индекса
        @param x, y вектора
        @throw logic_error, если <tt> x.dim() != y.dim() </tt>
        */
        template <class Vector>
        static void check_equal_dimensions(Vector const & x, Vector const & y)
        {
            if(x.dim() != y.dim())
            {
                throw std::logic_error("incompatible dimensions");
            }
        }

        /** @brief Проверка того, что делитель не равен нулю
        @param x делитель
        @throw logic_error, если <tt> x != T{0} </tt>
        */
        template <class Scalar>
        static void check_divisor_is_not_zero(Scalar const & x)
        {
            if(x == Scalar{0})
            {
                throw std::logic_error("Division by zero");
            }
        }
    };

    /** @brief Класс "математического вектора
    @tparam T тип элементов
    @tparam Checking стратегия проверок и обработки ошибок
    */
    template <class T, class Checking = vector_policy_throws>
    class math_vector
    {
        using Data = std::vector<T>;
    public:
        // Типы
        /// @brief Тип элементов
        using value_type = T;

        /// @brief Стратегия проверки и обработки ошибок
        using checking_policy = Checking;

        /// @brief Тип размерности и индексов
        using dimension_type = typename Data::difference_type;

        /// @brief Тип итератора
        using iterator = typename Data::iterator;

        /// @brief Тип константного итератора
        using const_iterator = typename Data::const_iterator;

        // Создание, копирование, уничтожение
        /// @brief Конструктор без параметров запрещён
        math_vector() = delete;

        /** @brief Создаёт нулевой вектор заданной размерности
        @param n размерность
        @post <tt> this->dim() == n </tt>
        @post Для любого @c i из интервала <tt> [0; this->dim()) </tt> выполняется
        <tt> (*this)[i] == T(0) </tt>
        */
        explicit math_vector(dimension_type n)
         : data_(n, T(0))
        {}

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

        T & operator[](dimension_type index)
        {
            return const_cast<T&>(static_cast<math_vector const&>(*this)[index]);
        }
        //@}

        // Операторы составного присваивания
        /** @brief Умножение на скаляр
        @param a числовой множитель
        @return <tt> *this </tt>
        @post Умножает каждый элемент <tt> *this </tt> на @c a
        */
        math_vector & operator*=(T const & a)
        {
            for(auto & x : *this)
            {
                x *= a;
            }

            return *this;
        }

        /** @brief Деление на скаляр
        @param a числовой множитель
        @return <tt> *this </tt>
        @post Делит каждый элемент <tt> *this </tt> на @c a
        */
        math_vector & operator/=(T const & a)
        {
            checking_policy::check_divisor_is_not_zero(a);

            for(auto & x : *this)
            {
                x /= a;
            }

            return *this;
        }

        /** @brief Прибавление вектора
        @param Прибавляемый вектор
        @pre <tt> this->dim() == x.dim() </tt>
        @post Прибавляет к каждому элементу <tt> *this </tt> соответсвующий элемент @c x
        @return *this
        */
        math_vector & operator+=(math_vector const & x)
        {
            checking_policy::check_equal_dimensions(*this, x);

            for(auto i = 0*x.dim(); i != x.dim(); ++ i)
            {
                (*this)[i] += x[i];
            }

            return *this;
        }

        /** @brief Вычитание вектора
        @param Вычитаемый вектор
        @pre <tt> this->dim() == x.dim() </tt>
        @post Вычитает из каждого элемента <tt> *this </tt> соответсвующий элемент @c x
        @return *this
        */
        math_vector & operator-=(math_vector const & x)
        {
            checking_policy::check_equal_dimensions(*this, x);

            for(auto i = 0*x.dim(); i != x.dim(); ++ i)
            {
                (*this)[i] -= x[i];
            }

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
        Data data_;
    };

    /** @brief Оператор "равно"
    @param x, y аргументы
    @return @c true, если равны размерности векторов и все их соответствующие элементы, иначе
    --- @b false.
    */
    template <class T, class Check>
    bool operator==(math_vector<T, Check> const & x, math_vector<T, Check> const & y)
    {
        return std::equal(x.begin(), x.end(), y.begin(), y.end());
    }

    /** @brief Оператор "не равно"
    @param x, y аргументы
    @return <tt> !(x == y) </tt>
    */
    template <class T, class Check>
    bool operator!=(math_vector<T, Check> const & x, math_vector<T, Check> const & y)
    {
        return !(x == y);
    }

    /** @brief Оператор умножения вектора на скаляр
    @param x вектор
    @param a скаляр
    @return Вектор, элементы которого имеют вид <tt> x[i] * a </tt>, где
    <tt> 0 <= i && i < x.dim() </tt>
    */
    template <class T1, class Check, class T2>
    auto operator*(math_vector<T1, Check> x, T2 const & a)
    -> math_vector<decltype(x[0]*a)>
    {
        auto result = math_vector<decltype(x[0]*a), Check>(std::move(x));
        result *= a;
        return result;
    }

    /** @brief Оператор умножения вектора на скаляр
    @param x вектор
    @param a скаляр
    @return Вектор, элементы которого имеют вид <tt> a * x[i] </tt>, где
    <tt> 0 <= i && i < x.dim() </tt>
    */
    template <class T, class Check>
    math_vector<T, Check>
    operator*(T const & a, math_vector<T, Check> x)
    {
        for(auto & elem : x)
        {
            elem = a * elem;
        }

        return x;
    }

    /** @brief Оператор сложения векторов
    @param x, y аргументы
    @pre <tt> x.dim() == y.dim() </tt>
    @return Вектор, элементы которого равны сумме соответствующих элементов векторов @c x и @c y
    */
    template <class T, class Check>
    math_vector<T, Check>
    operator+(math_vector<T, Check> x, math_vector<T, Check> const & y)
    {
        x += y;
        return x;
    }

    /** @brief Оператор деления вектора на скалря
    @param x вектор
    @param a скаляр
    @post <tt> a != 0 </tt>
    @return Вектор, элементы которого равны соответствующим элементам @c x, делённым на @c a
    */
    template <class T1, class Check, class T2>
    auto operator/(math_vector<T1, Check> x, T2 const & a)
    -> math_vector<decltype(x[0] / a), Check>
    {
        auto result = math_vector<decltype(x[0] / a), Check>(std::move(x));
        result /= a;
        return result;
    }

    /** @brief Оператор разности векторов
    @param x, y аргументы
    @pre <tt> x.dim() == y.dim() </tt>
    @return Вектор, элементы которого равны разности соответствующих элементов векторов @c x и @c y
    */
    template <class T, class Check>
    math_vector<T, Check>
    operator-(math_vector<T, Check> x, math_vector<T, Check> const & y)
    {
        x -= y;
        return x;
    }
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_LINEAR_ALGEBRA_MATH_VECTOR_HPP_INCLUDED
