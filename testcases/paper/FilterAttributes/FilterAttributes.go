package FilterAttributes

type ExternalClass struct {
    external int
}

var ExternalClassInstance ExternalClass

type FilterAttributes struct {
    i, j int
}

func (fa *FilterAttributes) A() int {
    fa.i = ExternalClassInstance.external
    return fa.i
}

func (fa *FilterAttributes) B() int {
    fa.j = ExternalClassInstance.external
    return fa.j
}
