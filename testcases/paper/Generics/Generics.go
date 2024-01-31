package generics

type Generic[T any] struct {
   i, j, k, l T
}

func (c Generic[T]) Method1() {
   c.i = c.k
}
 
func (c Generic[T]) Method2() {
   c.j = c.l
}

// func main() {
//    var c Generic[int]
//    c.Method1()
//    c.Method2()
// }
