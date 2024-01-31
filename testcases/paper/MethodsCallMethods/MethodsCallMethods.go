package MethodsCallMethods

type MethodsCallMethods struct {
    i int
    j int
}

func (m *MethodsCallMethods) A() int {
    return m.i
}

func (m *MethodsCallMethods) B() int {
    m.A()
    return m.j
}
