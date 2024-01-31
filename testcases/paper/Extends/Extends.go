package Extender

type Extends struct {
    i int
    j int
}

func (e *Extends) A() int {
    return e.i
}

func (e *Extends) B() int {
    return e.j
}

type Extender struct {
    Extends
}

func (e *Extender) A() int {
    return e.j
}
