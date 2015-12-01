// always start filename.hpp with #define filename
/* include the file "filename" in another using this format
#ifndef filename
#include <filename.hpp>
#endif
*/
// use namespaces mesh variables fel hawa keda
/* example:
namespace views {
int lololeyy
}

use in another namespace:
views::lololeyy = 6;
*/
/* to keep constants manageable, don't use literals
unless they couldnt be different
example: "x = y * 360" >> dont need to "#define cycle 360"
but: "height = height + 5" >> should become "height = height + delta_height"
with "#define delta_height = 5" in the definitions file
*/
