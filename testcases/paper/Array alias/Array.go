package Array

type Array struct {
    i [50]int
}

func (c Array) method1() {
    c.i[1] = 1
}

func (c Array) method2() {
    c.i[2] = 2
}

func (c Array) method3() {
    c.i[3] = 3
}

// Uses "slices"
func (c Array) aliasMethod() {
    var j []int = c.i[0:4]
    j[0] = 3
}


// Alias doesn't count: LCOM4 = 2 or 4
// Alias counts:        LCOM4 = 1 or 3 (outputs nothing)