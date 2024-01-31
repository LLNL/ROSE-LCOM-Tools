package FilterMethodCalls

type ExternalClass struct {
    external int
}

func (ec *ExternalClass) External() int {
    return 5
}

var ExternalClassInstance ExternalClass

type FilterMethodCalls struct {
    i, j int
}

func (fa *FilterMethodCalls) A() int {
    fa.i = ExternalClassInstance.External()
    return fa.i
}

func (fa *FilterMethodCalls) B() int {
    fa.j = ExternalClassInstance.External()
    return fa.j
}
