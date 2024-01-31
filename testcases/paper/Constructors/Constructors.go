package Constructors

type Constructors struct {
    i, j int
}

func (c Constructors) method() {
    c.j = 2
}

func NewConstructors() *Constructors {
    return &Constructors{i: 1}
}

// Constructors don't count: LCOM4 = 1 (outputs nothing)
// Constructors count:       LCOM4 = 2