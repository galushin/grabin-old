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

#ifndef Z_GRABIN_LINEAR_ALGEBRA_OUTER_PRODUCT_HPP_INCLUDED
#define Z_GRABIN_LINEAR_ALGEBRA_OUTER_PRODUCT_HPP_INCLUDED

/** @file grabin/linear_algebra/outer_product.hpp
 @brief Внешнее произведение векторов
*/

#include <grabin/linear_algebra/symmetric_matrix.hpp>

namespace grabin
{
inline namespace v0
{
    /** @brief Внешний "квадрат" -- внешнее произведения вектора самого на себя
    @param x вектор
    @return Симметричная матрица @c A такая, что <tt> A(i, j) == x[i] * x[j] </tt>
    для любых <tt> 0 <= i, j < x.dim() </tt>
    */
    template <class Vector>
    symmetric_matrix<typename Vector::value_type, typename Vector::checking_policy>
    outer_square(Vector const & x)
    {
        auto const n = x.dim();
        symmetric_matrix<typename Vector::value_type, typename Vector::checking_policy> result(n);

        for(auto i = 0*n; i < n; ++ i)
        for(auto j = 0*i; j <= i; ++ j)
        {
            result(i, j) = x[i] * x[j];
        }

        return result;
    }

    /** @brief Внешний "квадрат" (внешнее произведения вектора самого на себя) для арифметических
    типов (то есть для целочисленных типов и типов с плавающей точкой)
    @param x число
    @return x*x;
    */
    template <class T>
    std::enable_if_t<std::is_arithmetic<T>::value, T>
    outer_square(T const & x)
    {
        return x*x;
    }

    /** @brief Стандартная тензорная алгебра
    @tparam T тип элементов векторного пространства
    */
    template <class T>
    struct default_tensor_algebra
    {
    public:
        /** @brief Тензорный "квадрат" -- тензорное произведение на самого себя
        @param x вектор
        @return <tt> outer_square(x) </tt>, где outer_square может быть найдена с помощью ADL
        */
        static auto outer_square_impl(T const & x)
        {
            using ::grabin::outer_square;
            return outer_square(x);
        }

        /// @brief Тип тензорного произведения
        using tensor_product_type = decltype(default_tensor_algebra::outer_square_impl(std::declval<T>()));
    };
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_LINEAR_ALGEBRA_OUTER_PRODUCT_HPP_INCLUDED
