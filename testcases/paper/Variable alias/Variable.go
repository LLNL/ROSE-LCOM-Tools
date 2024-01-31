package Variable

type Variable struct {
    i,j int
    k * int
}

func (c Variable) method() {
    c.i = 1
}

func (c Variable) method2() {
    c.k = &c.i
    c.j = *v.k
}

// Variable aliases don't count: LCOM4 = 2
// Variable aliases count:       LCOM4 = 1